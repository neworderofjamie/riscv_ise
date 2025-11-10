import numpy as np
import matplotlib.pyplot as plt

from argparse import ArgumentParser
from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model,
                    NeuronUpdateProcess, Parameter, ProcessGroup,
                    RoundingMode, Runtime, Shape,
                    Variable)
from pyfenn.models import ExpLUTBroadcast, RNGInit

from pyfenn import disassemble, init_logging
from pyfenn.utils import (ceil_divide, copy_and_push, 
                          generate_exp_lut_and_push, get_array_view,
                          seed_and_push, zero_and_push)


parser = ArgumentParser("Adaptive exponential neuron")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
parser.add_argument("--num-timesteps", type=int, default=4000, help="Number of timesteps to sample for")
args = parser.parse_args()

fixed_point = 12

class AdExp:
    def __init__(self, shape, num_timesteps: int, tau_m: float, tau_w: float, 
                 r: float, e_l: float, delta_t: float, v_thresh: float, 
                 v_spike: float, v_reset: float, a: float, b: float, 
                 i_offset: float, dt: float = 0.1, fixed_point: int = 12, name: str = ""):
        self.shape = shape
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"
        frac_dtype = "s0_15_sat_t"
        self.v = Variable(self.shape, dtype, num_timesteps + 1, name=f"{name}_V")
        self.w = Variable(self.shape, dtype, num_timesteps + 1, name=f"{name}_W")
        self.out_spikes = EventContainer(self.shape, num_timesteps + 1)

        self.process = NeuronUpdateProcess(
            f"""
            // If voltage is above artificial spike height
            if(V >= vSpike) {{
               V = vReset;
            }}
            // Calculate RK4 terms
            {{
                const {dtype} v1 = {self._dv('V', 'W')};
                const {dtype} w1 = {self._dw('V', 'W')};
                {dtype} tmpV = V + (halfDT * v1);
                {dtype} tmpW = W + (halfDT * w1);
                const {dtype} v2 = {self._dv('tmpV', 'tmpW')};
                const {dtype} w2 = {self._dw('tmpV', 'tmpW')};
                tmpV = V + (halfDT * v2);
                tmpW = W + (halfDT * w2);
                const {dtype} v3 = {self._dv('tmpV', 'tmpW')};
                const {dtype} w3 = {self._dw('tmpV', 'tmpW')};
                tmpV = V + (dt * v3);
                tmpW = W + (dt * w3);
                const {dtype} v4 = {self._dv('tmpV', 'tmpW')};
                const {dtype} w4 = {self._dw('tmpV', 'tmpW')};
                // Update V
                V += sixthDT * (v1 + (2.0h{fixed_point} * (v2 + v3)) + v4);
                // If we're not above peak, update w
                // **NOTE** it's not safe to do this at peak as w may well be huge
                if(V <= -0.4h{fixed_point}) {{
                   W += sixthDT * (w1 + (2.0h{fixed_point} * (w2 + w3)) + w4);
                }}
            }}
            
            if(V > -0.4h{fixed_point}) {{
                // **NOTE** we reset v to arbitrary plotting peak rather than to actual reset voltage
                V = vSpike;
                W += b;
                Spike();
            }}
            """,
            {"dt": Parameter(dt, dtype),
             "halfDT": Parameter(dt / 2.0, frac_dtype),
             "sixthDT": Parameter(dt / 6.0, frac_dtype),
             "tauMRecip": Parameter(1.0 / tau_m, frac_dtype),
             "R": Parameter(r, dtype),
             "eL": Parameter(e_l, dtype),
             "deltaT": Parameter(delta_t, dtype),
             "deltaTRecip": Parameter(1.0 / delta_t, "s6_9_sat_t"),
             "vThresh": Parameter(v_thresh, dtype),
             "vSpike": Parameter(v_spike, dtype),
             "vReset": Parameter(v_reset, dtype),
             "tauWRecip": Parameter(1.0 / tau_w, frac_dtype),
             "a": Parameter(a, dtype),
             "b": Parameter(b, dtype),
             "iOffset": Parameter(i_offset, dtype)}, 
            {"V": self.v, "W": self.w},
            {"Spike": self.out_spikes},
            name)
    
    def _dv(self, v: str, w: str):
        return f"tauMRecip * (-(({v}) - eL) + (deltaT @ exp_3((({v}) - vThresh) * deltaTRecip)) + R * (iOffset - ({w})))"

    def _dw(self, v: str, w: str):
        return f"tauWRecip * ((a * ({v} - eL)) - {w})"


init_logging()


# Model
c = 281.0 / 1000.0
gL = 30.0 / 1000.0
v_scale = 0.01
w_scale = 10.0
ad_exp = AdExp(32, args.num_timesteps, tau_m=(c / gL), tau_w=144.0,
               r=((1.0 / gL) * (v_scale / w_scale)), e_l=(-70.6 * v_scale),
               delta_t=(2.0 * v_scale), v_thresh=(-50.4 * v_scale),
               v_spike=(10.0 * v_scale), v_reset=(-70.6 * v_scale), 
               a=((4.0 / 1000.0) / (v_scale / w_scale)), b=(0.0805 * w_scale),
               i_offset=(700.0 * (w_scale / 1000.0)), dt=0.1, fixed_point=fixed_point,
               name="ad_exp")

rng_init = RNGInit()
lut_broadcast = ExpLUTBroadcast()

# Group processes
init_processes = ProcessGroup([rng_init.process, lut_broadcast.process])
neuron_processes = ProcessGroup([ad_exp.process])

# Create backend
backend_params = {"keep_params_in_registers": False,
                  "rounding_mode": RoundingMode.STOCHASTIC}
backend = BackendFeNNHW(**backend_params) if args.device else BackendFeNNSim(**backend_params)

# Create model
model = Model([init_processes, neuron_processes], backend)

# Generate init and sim code
init_code = backend.generate_kernel([init_processes], model)
code = backend.generate_simulation_kernel([neuron_processes], [], [],
                                          args.num_timesteps, model)

# Disassemble if required
if args.disassemble:
    print("Init:")
    for i, c in enumerate(init_code):
        print(f"{i * 4} : {disassemble(c)}")

    print("Simulation:")
    for i, c in enumerate(code):
        print(f"{i * 4} : {disassemble(c)}")

# Create runtime
runtime = Runtime(model, backend)

# Allocate memory for model
runtime.allocate()

# Zero i and initialise V
zero_and_push(ad_exp.w, runtime)
fixed_point_one = (2 ** fixed_point)
v_array, v_view = get_array_view(runtime, ad_exp.v, np.int16)
v_view[:] = np.round(-70.6 * v_scale * fixed_point_one).astype(np.int16)
v_array.push_to_device()

# Initialise exp LUT and RNG
generate_exp_lut_and_push(lut_broadcast.lut, runtime)
seed_and_push(rng_init.seed, runtime)

# Set init instructions
print("Initialising")
runtime.set_instructions(init_code)

# Initialise
runtime.run()

# Set sim instructions
print("Simulating")
runtime.set_instructions(code)

# Simulate
runtime.run()

# Pull neuron voltage and adaptation variables
w_array, w_view = get_array_view(runtime, ad_exp.w,
                                 np.int16)
v_array.pull_from_device()
w_array.pull_from_device()

v_view = np.reshape(v_view, (-1, 32))
w_view = np.reshape(w_view, (-1, 32))

timesteps = np.arange(0.0, (args.num_timesteps * 0.1) + 0.1, 0.1)

# Create plot
figure, axes = plt.subplots(2, sharex=True)

# Plot voltages
axes[0].set_title("Voltage")
axes[0].plot(timesteps, v_view[:,0] / fixed_point_one)

axes[1].set_title("Adaption current")
axes[1].plot(timesteps, w_view[:,0] / fixed_point_one)
axes[1].set_xlabel("Time [ms]")

# Show plot
plt.show()