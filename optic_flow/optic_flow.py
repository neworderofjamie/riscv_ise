import cv2
import numpy as np
import pyfenn.fenn_backend as backend

from argparse import ArgumentParser
from enum import IntEnum
from pyfenn.models import Downsample2D, SparseLinear, Memset
from pyfenn.utils import PythonLogAppender

from pyfenn.utils import (build_sparse_connectivity, ceil_divide,
                          copy_and_push, generate_fixed_prob, pull_spikes,
                          read_perf_counter, zero_and_push)

# Enumeration of the detectors in the output layer of model
class Detector(IntEnum):
    LEFT = 0
    RIGHT = 1
    UP = 2
    DOWN = 3

# Standard CUBA LIF model with single synaptic time constant
class CUBALIF:
    def __init__(self, backend, shape, tau_m: float, tau_syn: float,
                 v_thresh: float, name: str = ""):
        self.shape = shape
        dtype = "s5_10_sat_t"

        syn_scale = tau_syn * (1.0 - np.exp(-1.0 / tau_syn))
        beta = np.exp(-1.0 / tau_syn)
        alpha = np.exp(-1.0 / tau_m)

        self.v = backend.Variable(self.shape, dtype, name=f"{name}_V")
        self.i = backend.Variable(self.shape, "s14_1_sat_t", name=f"{name}_I")
        channel = backend.EventChannel(self.shape, name=f"{name}_out_spikes")
        self.spike_sink = channel.sink
        self.out_spikes = channel.source
        self.process = backend.NeuronUpdateProcess(
            f"""
            s5_10_sat_t inSyn;
            {{
                inSyn = (I @ {syn_scale}h10);
                I *= {beta}h15;
            }}
            
            // Update V
            const s5_10_sat_t VAlpha = {tau_m / 1.0}h10 * inSyn;
            V = VAlpha - ({alpha}h15 * (VAlpha - V));
            
            if(V >= {float(v_thresh)}h10) {{
               Spike();
               V = 0.0h10;
            }}
            """,
            {"V": self.v, "I": self.i},
            {"Spike": self.spike_sink},
            name=name)


# Standard CUBA LIF model with seperate inhibitory and excitatory synaptic time constants
class CUBALIFIE:
    def __init__(self, backend, shape, tau_m: float, tau_syn_exc: float, tau_syn_inh, 
                 v_thresh: float, num_timesteps: int = 1, name: str = ""):
        self.shape = shape
        dtype = "s5_10_sat_t"

        exc_scale = tau_syn_exc * (1.0 - np.exp(-1.0 / tau_syn_exc))
        inh_scale = tau_syn_inh * (1.0 - np.exp(-1.0 / tau_syn_inh))
        beta_exc = np.exp(-1.0 / tau_syn_exc)
        beta_inh = np.exp(-1.0 / tau_syn_inh)
        alpha = np.exp(-1.0 / tau_m)

        self.v = backend.Variable(self.shape, dtype, name=f"{name}_V")
        self.i_exc = backend.Variable(self.shape, "s14_1_sat_t", name=f"{name}_IExc")
        self.i_inh = backend.Variable(self.shape, "s14_1_sat_t", name=f"{name}_IInh")
        self.spike_sink = backend.EventSinkBuffer((num_timesteps + 1,) + self.shape, 
                                                  name=f"{name}_spike_sink")
        self.process = backend.NeuronUpdateProcess(
            f"""
            s5_10_sat_t inSyn;
            // Excitatory
            {{
                inSyn = (IExc @ {exc_scale}h10);
                IExc *= {beta_exc}h15;
            }}
            
            // Inhibitory
            {{
                inSyn += (IInh @ {inh_scale}h10);
                IInh *= {beta_inh}h15;
            }}
            
            // Update voltage
            const s5_10_sat_t VAlpha = {tau_m / 1.0}h10 * inSyn;
            V = VAlpha - ({alpha}h15 * (VAlpha - V));
            
            if(V >= {float(v_thresh)}h10) {{
               Spike();
               V = 0.0h10;
            }}
            """,
            {"V": self.v, "IExc": self.i_exc, "IInh": self.i_inh},
            {"Spike": backend.SlicedEventSink(self.spike_sink, True)},
            name=name)


# Size of (square) camera input 
INPUT_SIZE = 320

# Size of (square) downsampling kernel
KERNEL_SIZE = 8

# Size of (square) macro-pixel layer
MACRO_PIXEL_SIZE = INPUT_SIZE // KERNEL_SIZE

# Size of (square) detector layer 
DETECTOR_SIZE = MACRO_PIXEL_SIZE - 2

MACRO_PIXEL_DETECTOR_WEIGHT_FRACTIONAL_BITS = 1
MACRO_PIXEL_DETECTOR_EXC_WEIGHT = int(round(1.0 * 2**MACRO_PIXEL_DETECTOR_WEIGHT_FRACTIONAL_BITS))
MACRO_PIXEL_DETECTOR_INH_WEIGHT = int(round(-0.5 * 2**MACRO_PIXEL_DETECTOR_WEIGHT_FRACTIONAL_BITS))

# Number of timesteps to run between updating visualisation
NUM_TIMESTEPS_PER_FRAME = 33

# Loop through macro pixels
macro_pixel_detector_exc_inds = []
for yi in range(MACRO_PIXEL_SIZE):
    for xi in range(MACRO_PIXEL_SIZE):
        row_inds = []

        # If we're not in the border of the macro pixel layer
        if xi >= 1 and xi < (MACRO_PIXEL_SIZE - 1) and  yi >= 1 and yi < (MACRO_PIXEL_SIZE - 1):
            xj = (xi - 1) * len(Detector)
            yj = yi - 1

            # Add excitatory synapses to all detectors
            row_inds.append(xj + Detector.LEFT + (yj * DETECTOR_SIZE * len(Detector)))
            row_inds.append(xj + Detector.RIGHT + (yj * DETECTOR_SIZE * len(Detector)))
            row_inds.append(xj + Detector.UP + (yj * DETECTOR_SIZE * len(Detector)))
            row_inds.append(xj + Detector.DOWN + (yj * DETECTOR_SIZE * len(Detector)))

        # Add inhibitory connections
        # Create inhibitory connection to 'left' detector associated with macropixel one to right
        if xi < (MACRO_PIXEL_SIZE - 2) and yi >= 1 and yi < (MACRO_PIXEL_SIZE - 1):
            xj = (xi - 1 + 1) * len(Detector)
            yj = yi - 1
            row_inds.append(xj + Detector.LEFT + (yj * DETECTOR_SIZE * len(Detector)))
    
        # Create inhibitory connection to 'right' detector associated with macropixel one to right
        if xi >= 2 and yi >= 1 and yi < (MACRO_PIXEL_SIZE - 1):
            xj = (xi - 1 - 1) * len(Detector)
            yj = yi - 1
            row_inds.append(xj + Detector.RIGHT + (yj * DETECTOR_SIZE * len(Detector)))

        # Create inhibitory connection to 'up' detector associated with macropixel one below
        if xi >= 1 and xi < (MACRO_PIXEL_SIZE - 1) and yi < (MACRO_PIXEL_SIZE - 2):
            xj = (xi - 1) * len(Detector)
            yj = yi - 1 + 1
            row_inds.append(xj + Detector.UP + (yj * DETECTOR_SIZE * len(Detector)))

        # Create inhibitory connection to 'down' detector associated with macropixel one above
        if xi >= 1 and xi < (MACRO_PIXEL_SIZE - 1) and yi >= 2:
            xj = (xi - 1) * len(Detector)
            yj = yi - 1 - 1
            row_inds.append(xj + Detector.DOWN + (yj * DETECTOR_SIZE * len(Detector)))

        # Convert row lists to numpy and add to 
        macro_pixel_detector_exc_inds.append(np.asarray(row_inds, dtype=int))

# Loop through macro pixels
macro_pixel_detector_inh_inds = []
for yi in range(MACRO_PIXEL_SIZE):
    for xi in range(MACRO_PIXEL_SIZE):        
        # Create inhibitory connection to 'left' detector associated with macropixel one to right
        row_inds = []
        if xi < (MACRO_PIXEL_SIZE - 2) and yi >= 1 and yi < (MACRO_PIXEL_SIZE - 1):
            xj = (xi - 1 + 1) * len(Detector)
            yj = yi - 1
            row_inds.append(xj + Detector.LEFT + (yj * DETECTOR_SIZE * len(Detector)))
    
        # Create inhibitory connection to 'right' detector associated with macropixel one to right
        if xi >= 2 and yi >= 1 and yi < (MACRO_PIXEL_SIZE - 1):
            xj = (xi - 1 - 1) * len(Detector)
            yj = yi - 1
            row_inds.append(xj + Detector.RIGHT + (yj * DETECTOR_SIZE * len(Detector)))
    
        # Create inhibitory connection to 'up' detector associated with macropixel one below
        if xi >= 1 and xi < (MACRO_PIXEL_SIZE - 1) and yi < (MACRO_PIXEL_SIZE - 2):
            xj = (xi - 1) * len(Detector)
            yj = yi - 1 + 1
            row_inds.append(xj + Detector.UP + (yj * DETECTOR_SIZE * len(Detector)))
    
        # Create inhibitory connection to 'down' detector associated with macropixel one above
        if xi >= 1 and xi < (MACRO_PIXEL_SIZE - 1) and yi >= 2:
            xj = (xi - 1) * len(Detector)
            yj = yi - 1 - 1
            row_inds.append(xj + Detector.DOWN + (yj * DETECTOR_SIZE * len(Detector)))
    
        # Convert row lists to numpy and add to 
        macro_pixel_detector_inh_inds.append(np.asarray(row_inds, dtype=int))



num_sparse_connectivity_bits = 14 - MACRO_PIXEL_DETECTOR_WEIGHT_FRACTIONAL_BITS
macro_pixel_detector_exc_conn = build_sparse_connectivity([macro_pixel_detector_exc_inds], 
                                                          MACRO_PIXEL_DETECTOR_EXC_WEIGHT, 
                                                          num_sparse_connectivity_bits)
macro_pixel_detector_inh_conn = build_sparse_connectivity([macro_pixel_detector_inh_inds], 
                                                          MACRO_PIXEL_DETECTOR_INH_WEIGHT, 
                                                          num_sparse_connectivity_bits)

print(macro_pixel_detector_exc_conn[0].shape)
print(macro_pixel_detector_inh_conn[0].shape)

parser = ArgumentParser("Optic flow demo")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--time", action="store_true", help="Record detailed timings using performance counters")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
args = parser.parse_args()

log_appender = backend.ConsoleAppender()
backend.init_logging(log_appender, backend.PlogSeverity.DEBUG)

# Event camera
event_camera = backend.GenX320()

# Neurons
macro_pixel_pop = CUBALIF(backend, (MACRO_PIXEL_SIZE * MACRO_PIXEL_SIZE,), 
                          tau_m=20.0, tau_syn=5.0, v_thresh=10.0, name="MacroPixel")

detector_pop = CUBALIFIE(backend, (DETECTOR_SIZE * DETECTOR_SIZE * len(Detector),), 
                         tau_m=20.0, tau_syn_exc=25.0, tau_syn_inh=50.0, 
                         v_thresh=10.0, num_timesteps=NUM_TIMESTEPS_PER_FRAME, name="Detector")

# Synapses
downsample_pop = Downsample2D(backend, event_camera.source, macro_pixel_pop.i, 1.0, name="EventCamMacroPixel")
macro_pixel_detector_exc_pop = SparseLinear(backend, macro_pixel_pop.out_spikes, 
                                            detector_pop.i_exc, weight_dtype="s14_1_sat_t", max_row_length=macro_pixel_detector_exc_conn[0].shape[1],
                                            num_sparse_connectivity_bits=num_sparse_connectivity_bits, 
                                            name="MacroPixelDetectorExc")
macro_pixel_detector_inh_pop = SparseLinear(backend, macro_pixel_pop.out_spikes, 
                                            detector_pop.i_inh, weight_dtype="s14_1_sat_t", max_row_length=macro_pixel_detector_inh_conn[0].shape[1],
                                            num_sparse_connectivity_bits=num_sparse_connectivity_bits, 
                                            name="MacroPixelDetectorInh")

# Initialisation
detector_e_zero = Memset(backend, detector_pop.i_exc, name="zero_i_exc")
detector_i_zero = Memset(backend, detector_pop.i_inh, name="zero_i_inh")

# Group processes
i_zero_processes = backend.ProcessGroup([detector_e_zero.process, 
                                         detector_i_zero.process])
neuron_update_processes = backend.ProcessGroup([macro_pixel_pop.process, 
                                                detector_pop.process])
synapse_update_processes = backend.ProcessGroup([macro_pixel_detector_exc_pop.process,
                                                 macro_pixel_detector_inh_pop.process,
                                                 downsample_pop.process])

# Create init kernel
init_kernel = backend.SimpleKernel([i_zero_processes])

# Create simulation kernel
sim_kernel = backend.SimulationLoopKernel(
    NUM_TIMESTEPS_PER_FRAME, [synapse_update_processes, neuron_update_processes],
    [], [])
    
# Create runtime
runtime_params = {}
runtime = (backend.RuntimeHW([init_kernel, sim_kernel], 1, **runtime_params) if args.device 
           else backend.RuntimeSim([init_kernel, sim_kernel], 1, **runtime_params))


# Disassemble if required
if args.disassemble:
    print("Init:")
    code = runtime.get_kernel_code(init_kernel)
    for i, c in enumerate(code):
        print(f"{i * 4} : {backend.disassemble(c)}")

    print("Simulation:")
    code = runtime.get_kernel_code(sim_kernel)
    for i, c in enumerate(code):
        print(f"{i * 4} : {backend.disassemble(c)}")

# Allocate memory for model
runtime.allocate()