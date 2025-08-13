import numpy as np
import matplotlib.pyplot as plt

from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model,
                    NeuronUpdateProcess, NumericValue, Parameter,
                    ProcessGroup, Runtime, Shape, UnresolvedType, Variable)
from models import ExpLUTBroadcast

from pyfenn import disassemble, init_logging
from pyfenn.utils import (ceil_divide, copy_and_push, 
                          generate_exp_lut_and_push, get_array_view,
                          zero_and_push)


device = False
disassemble_code = False
num_timesteps = 10000

class AdExp:
    def __init__(self, shape, num_timesteps: int, tau_m: float, tau_w: float, 
                 r: float, e_l: float, delta_t: float, v_thresh: float, 
                 v_spike: float, v_reset: float, a: float, b: float, 
                 i_offset: float, dt: float = 0.1, name: str = ""):
        self.shape = Shape(shape)
        dtype = UnresolvedType("s1_14_sat_t")
        self.v = Variable(self.shape, dtype, num_timesteps, name=f"{name}_V")
        self.w = Variable(self.shape, dtype, num_timesteps, name=f"{name}_W")
        self.out_spikes = EventContainer(self.shape, num_timesteps)

        self.process = NeuronUpdateProcess(
            f"""
            // If voltage is above artificial spike height
            if(V >= vSpike) {{
               V = vReset;
            }}
            // Calculate RK4 terms
            v1 = {self._dv('V', 'W')};
            w1 = {self._dw('V', 'W')};
            s1_14_sat_t tmpV = V + (halfDT * v1);
            s1_14_sat_t tmpW = W + (halfDT * w1);
            const s1_14_sat_t v2 = {self._dv('tmpV', 'tmpW')};
            const s1_14_sat_t w2 = {self._dw('tmpV', 'tmpW')};
            tmpV = V + (halfDT * v2);
            tmpW = W + (halfDT * w2);
            const s1_14_sat_t v3 = {self._dv('tmpV', 'tmpW')};
            const s1_14_sat_t w3 = {self._dw('tmpV', 'tmpW')};
            tmpV = V + (dt * v3);
            tmpW = W + (dt * w3);
            const s1_14_sat_t v4 = {self._dv('tmpV', 'tmpW')};
            const s1_14_sat_t w4 = {self._dw('tmpV', 'tmpW')};
            // Update V
            V += sixthDT * (v1 + (2.0 * (v2 + v3)) + v4);
            // If we're not above peak, update w
            // **NOTE** it's not safe to do this at peak as wn may well be huge
            if(V <= -0.4) {{
               W += sixthDT * (w1 + (2.0 * (w2 + w3)) + w4);
            }}
            
            if(V > -0.4) {{
                // **NOTE** we reset v to arbitrary plotting peak rather than to actual reset voltage
                V = vSpike;
                W += b;
                Spike();
            }}
            """,
            {"dt": Parameter(NumericValue(dt), dtype),
             "halfDT": Parameter(NumericValue(dt * 0.5), dtype),
             "sixthDT": Parameter(NumericValue(dt / 6.0), dtype),
             "tauMRecip": Parameter(NumericValue(1.0 / tau_m), dtype),
             "R": Parameter(NumericValue(r), dtype),
             "eL": Parameter(NumericValue(e_l), dtype),
             "deltaT": Parameter(NumericValue(delta_t), dtype),
             "deltaTRecip": Parameter(NumericValue(1.0 / delta_t), dtype),
             "vThresh": Parameter(NumericValue(v_thresh), dtype),
             "vSpike": Parameter(NumericValue(v_spike), dtype),
             "vReset": Parameter(NumericValue(v_reset), dtype),
             "tauWRecip": Parameter(NumericValue(1.0 / tau_w), dtype),
             "a": Parameter(NumericValue(a), dtype),
             "b": Parameter(NumericValue(b), dtype),
             "iOffset": Parameter(NumericValue(i_offset), dtype)}, 
            {"V": self.v, "W": self.w},
            {"Spike": self.out_spikes},
            name)
    
    def _dv(self, v: str, w: str):
        return f"tauMRecip * (-(({v}) - eL) + (deltaT * exp((({v}) - vThresh) * deltaTRecip)) + R * (iOffset - ({w})))"

    def _dw(self, v: str, w: str):
        return f"tauWRecip * ((a * ({v} - eL)) - {w})"


init_logging()


# Model
c = 281.0 / 1000.0
gL = 30.0 / 1000.0
v_scale = 0.01
ad_exp = AdExp([32], num_timesteps, tau_m=(c / gL), tau_w=144.0,
               r=((1.0 / gL) * v_scale), e_l=(-70.6 * v_scale),
               delta_t=(2.0 * v_scale), v_thresh=(-50.4 * v_scale),
               v_spike=(10.0 * v_scale), v_reset=(-70.6 * v_scale), 
               a=((4.0 / 1000.0) / v_scale), b=0.0805, 
               i_offset=(700.0 / 1000.0), dt=0.1, name="ad_exp")
lut_broadcast = ExpLUTBroadcast()

# Group processes
init_processes = ProcessGroup([lut_broadcast.process])
neuron_processes = ProcessGroup([ad_exp.process])

# Create backend
backend = BackendFeNNHW() if device else BackendFeNNSim()

# Create model
model = Model([init_processes, neuron_processes], backend)

# Generate init and sim code
init_code = backend.generate_kernel([init_processes], model)
code = backend.generate_simulation_kernel([neuron_processes], [],
                                          num_timesteps, model)

# Disassemble if required
# Disassemble if required
if disassemble_code:
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
v_array, v_view = get_array_view(runtime, ad_exp.v, np.int16)
v_view[:] = np.round(-70.6 * v_scale * (2**14)).astype(np.int16)
v_array.push_to_device()

# Initialise exp LUT
generate_exp_lut_and_push(lut_broadcast.lut, runtime)

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

print(w_view.shape, v_view.shape)
"""
# Create plot
figure, axes = plt.subplots(2)

# Plot voltages
axes[0].set_title("Voltage")
axes[0].plot(adexp_v)

axes[1].set_title("Adaption current")
axes[1].plot(adexp_w)

# Show plot
plt.show()
"""