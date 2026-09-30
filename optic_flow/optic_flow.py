import cv2
import numpy as np
import pyfenn.fenn_backend as backend

from argparse import ArgumentParser
from enum import IntEnum
from pyfenn.models import Downsample2D, SparseLinear, Memset
from pyfenn.utils import PythonLogAppender

from pyfenn.utils import (build_sparse_connectivity, build_spike_array,
                          copy_and_push, get_views, pull_spikes,
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
        self.i = backend.Variable(self.shape, "s2_13_sat_t", name=f"{name}_I")
        channel = backend.EventChannel(self.shape, name=f"{name}_out_spikes")
        self.spike_sink = channel.sink
        self.out_spikes = channel.source
        self.process = backend.NeuronUpdateProcess(
            f"""
            s5_10_sat_t inSyn;
            {{
                inSyn = (I * {syn_scale}h10);
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

OUTPUT_SCALE = 12
FLOW_PERSISTENCE = 0.995
OUTPUT_VECTOR_SCALE = 2.0

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

# If we are running on device
if args.device:
    event_camera = backend.GenX320()
    event_input = event_camera.source
# Otherwise
else:
    # Load raw courtyard data
    # **NOTE** prophesee compressed hdf5 load is incredible annoying to setup!
    courtyard_data = np.load("courtyard.npy")

    # First couple of ms contain crap so throw away
    lose_early_mask = (courtyard_data["t"] > 1000)
    courtyard_data = courtyard_data[lose_early_mask]

    # Only keep on events
    courtyard_data = courtyard_data[courtyard_data["p"]]
    
    # Determine which frame each event is destined for
    event_frame = courtyard_data["t"] // (33 * 1000)

    # Count events per-frame and use to split event data into frames
    events_per_frame = np.bincount(event_frame)
    events_per_frame = np.cumsum(events_per_frame)
    timestep_events = np.split(courtyard_data, events_per_frame)
    
    # **YUCK** remove final empty frame
    assert(len(timestep_events[-1]) == 0)
    timestep_events = timestep_events[:-1]

    # **YUCK** truncate events
    timestep_events = [e[:min(len(e), 20000)] for e in timestep_events]

    print(timestep_events[0][:10])
    
    # Turn each frame into a spike array
    # **NOTE** we use POT shape to build flat spike indices
    courtyard_frames = [build_spike_array((e["t"] // 1000) - (33 * i), 
                                          np.ravel_multi_index((e["x"], e["y"]), (512, 512)))
                        for i, e in enumerate(timestep_events)]
    
    print(courtyard_frames[0][:10])
    # Count maximum events per frame and build event source buffer
    max_events_per_frame = max(len(f) for f in courtyard_frames)
    print(f"Max events per frame: {max_events_per_frame}")
    event_input = backend.EventSourceBuffer((320, 320), max_events_per_frame, name="input_events")


# Neurons
macro_pixel_pop = CUBALIF(backend, (MACRO_PIXEL_SIZE * MACRO_PIXEL_SIZE,), 
                          tau_m=20.0, tau_syn=5.0, v_thresh=10.0, name="MacroPixel")

detector_pop = CUBALIFIE(backend, (DETECTOR_SIZE * DETECTOR_SIZE * len(Detector),), 
                         tau_m=20.0, tau_syn_exc=25.0, tau_syn_inh=50.0, 
                         v_thresh=10.0, num_timesteps=NUM_TIMESTEPS_PER_FRAME, name="Detector")

# Synapses
downsample_pop = Downsample2D(backend, event_input, macro_pixel_pop.i, 0.8, name="EventCamMacroPixel")
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

# Copy connectivity
copy_and_push(macro_pixel_detector_exc_conn[0], macro_pixel_detector_exc_pop.weight, runtime)
copy_and_push(macro_pixel_detector_inh_conn[0], macro_pixel_detector_inh_pop.weight, runtime)


# Initialise membrane voltages
# **TODO** use init kernel
zero_and_push(macro_pixel_pop.v, runtime)
zero_and_push(macro_pixel_pop.i, runtime)
zero_and_push(detector_pop.v, runtime)

zero_and_push(detector_pop.spike_sink, runtime)

# Initialise
print("Initialising")
runtime.run(init_kernel)

cv2.namedWindow("Output")

output = np.zeros((DETECTOR_SIZE, DETECTOR_SIZE, 2))
output_image = np.zeros((DETECTOR_SIZE * OUTPUT_SCALE, DETECTOR_SIZE * OUTPUT_SCALE, 3),
                        dtype=np.uint8)

output_start_x, output_start_y = np.meshgrid(np.arange(DETECTOR_SIZE), np.arange(DETECTOR_SIZE))
output_start_x *= OUTPUT_SCALE
output_start_y *= OUTPUT_SCALE

assert not args.device

input_spike_views = get_views(runtime, event_input)

# **TEMP** run 10 frames
for f in courtyard_frames:
    # Copy data to array host pointer
    input_spike_views[0][:len(f)] = f
    runtime.push_state_to_device(event_input)

    # Simulate
    runtime.run(sim_kernel)

    # Get detector spikes
    # **TODO** asynchronous loop
    detector_spikes = pull_spikes(NUM_TIMESTEPS_PER_FRAME + 1, detector_pop.spike_sink, runtime)
    detector_spike_ids = detector_spikes[0][1]
    print(len(detector_spike_ids))

    # Unravel to get x, y and detector
    detector_spike_ids = np.unravel_index(detector_spike_ids, (DETECTOR_SIZE, DETECTOR_SIZE, len(Detector)))

    # Split detector channel into horizontal/vertical and polarity
    output_channel = (detector_spike_ids[2] & 0x2) >> 1
    output_polarity = (2 * (detector_spike_ids[2] & 0x1)) - 1

    # Make vectorised update
    output[detector_spike_ids[0], detector_spike_ids[1], output_channel] += output_polarity
    output *= FLOW_PERSISTENCE

    # Scale output to render size
    scaled_output = np.round(output * OUTPUT_VECTOR_SCALE).astype(int)

    # Draw optic flow arrows
    for i in range(DETECTOR_SIZE):
        for j in range(DETECTOR_SIZE):
            cv2.line(output_image, (output_start_x[i,j], output_start_y[i,j]),
                     (output_start_x[i,j] + scaled_output[i,j,0], output_start_y[i,j] + scaled_output[i, j, 1]),
                     (255, 255, 255))
    cv2.imshow("Output", output_image)
    cv2.waitKey(1)
