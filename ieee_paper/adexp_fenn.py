import numpy as np
from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model,
                    NeuronUpdateProcess, NumericValue, Parameter,
                    ProcessGroup, RoundingMode, Runtime, Shape,
                    UnresolvedType, Variable)
from pyfenn.models import ExpLUTBroadcast, RNGInit

from pyfenn import disassemble, init_logging
from pyfenn.utils import (generate_exp_lut_and_push, get_array_view,
                        seed_and_push, zero_and_push)

from adexp_params import param_sets

def simulate_fenn(params, device, num_timesteps=2000, disassemble_code=False, fixed_point=11):
    class AdExp:
        def __init__(self, shape, num_timesteps: int, tau_m: float, tau_w: float, 
                    r: float, e_l: float, delta_t: float, v_thresh: float, 
                    v_spike: float, v_reset: float, a: float, b: float, 
                    i_offset: float, dt: float = 0.1, fixed_point: int = 12, name: str = ""):
            self.shape = Shape(shape)
            type_str = f"s{15 - fixed_point}_{fixed_point}_sat_t"
            dtype = UnresolvedType(type_str)
            frac_dtype = UnresolvedType("s0_15_sat_t")
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
                    const {type_str} v1 = {self._dv('V', 'W')};
                    const {type_str} w1 = {self._dw('V', 'W')};
                    {type_str} tmpV = V + (halfDT * v1);
                    {type_str} tmpW = W + (halfDT * w1);
                    const {type_str} v2 = {self._dv('tmpV', 'tmpW')};
                    const {type_str} w2 = {self._dw('tmpV', 'tmpW')};
                    tmpV = V + (halfDT * v2);
                    tmpW = W + (halfDT * w2);
                    const {type_str} v3 = {self._dv('tmpV', 'tmpW')};
                    const {type_str} w3 = {self._dw('tmpV', 'tmpW')};
                    tmpV = V + (dt * v3);
                    tmpW = W + (dt * w3);
                    const {type_str} v4 = {self._dv('tmpV', 'tmpW')};
                    const {type_str} w4 = {self._dw('tmpV', 'tmpW')};
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
                {"dt": Parameter(NumericValue(dt), dtype),
                 "halfDT": Parameter(NumericValue(dt / 2.0), frac_dtype),
                 "sixthDT": Parameter(NumericValue(dt / 6.0), frac_dtype),
                 "tauMRecip": Parameter(NumericValue(1.0 / tau_m), frac_dtype),
                 "R": Parameter(NumericValue(r), dtype),
                 "eL": Parameter(NumericValue(e_l), dtype),
                 "deltaT": Parameter(NumericValue(delta_t), dtype),
                 "deltaTRecip": Parameter(NumericValue(1.0 / delta_t), UnresolvedType("s6_9_sat_t")),
                 "vThresh": Parameter(NumericValue(v_thresh), dtype),
                 "vSpike": Parameter(NumericValue(v_spike), dtype),
                 "vReset": Parameter(NumericValue(v_reset), dtype),
                 "tauWRecip": Parameter(NumericValue(1.0 / tau_w), frac_dtype),
                 "a": Parameter(NumericValue(a), dtype),
                 "b": Parameter(NumericValue(b), dtype),
                 "iOffset": Parameter(NumericValue(i_offset), dtype)}, 
                 {"V": self.v, "W": self.w},
                 {"Spike": self.out_spikes},
                 name)
        
        def _dv(self, v: str, w: str):
            return f"tauMRecip * (-(({v}) - eL) + (deltaT @ exp_3((({v}) - vThresh) * deltaTRecip)) + R * (iOffset - ({w})))"

        def _dw(self, v: str, w: str):
            return f"tauWRecip * ((a * ({v} - eL)) - {w})"


    init_logging()

    # Model
    v_scale = 0.01
    w_scale = 10.0

    ad_exp = AdExp([32], num_timesteps, tau_m=(params["c"] / params["g_l"]), tau_w=params["tau_w"],
                   r=((1.0 / params["g_l"]) * (v_scale / w_scale)), e_l=(params["e_l"] * v_scale),
                   delta_t=(params["delta_t"] * v_scale), v_thresh=(params["v_t"] * v_scale),
                   v_spike=(10.0 * v_scale), v_reset=(params["v_r"] * v_scale), 
                   a=((params["a"] / 1000.0) / (v_scale / w_scale)), b=(params["b"] * w_scale),
                   i_offset=(params["i_offset"] * (w_scale / 1000.0)), dt=0.1, fixed_point=fixed_point,
                   name="ad_exp")

    rng_init = RNGInit()
    lut_broadcast = ExpLUTBroadcast()

    # Group processes
    init_processes = ProcessGroup([rng_init.process, lut_broadcast.process])
    neuron_processes = ProcessGroup([ad_exp.process])

    # Create backend
    backend_params = {"keep_params_in_registers": False,
                      "rounding_mode": RoundingMode.STOCHASTIC}
    backend = BackendFeNNHW(**backend_params) if device else BackendFeNNSim(**backend_params)

    # Create model
    model = Model([init_processes, neuron_processes], backend)

    # Generate init and sim code
    init_code = backend.generate_kernel([init_processes], model)
    code = backend.generate_simulation_kernel([neuron_processes], [], [],
                                            num_timesteps, model)

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
    fixed_point_one = 2 ** fixed_point
    v_array, v_view = get_array_view(runtime, ad_exp.v, np.int16)
    v_view[:] = np.round(params["e_l"] * v_scale * fixed_point_one).astype(np.int16)
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

    # Get arrays and views corresponding to neuron state
    w_array, w_view = get_array_view(runtime, ad_exp.w, np.int16)
    spike_array, spike_view = get_array_view(runtime, ad_exp.out_spikes, np.uint8)
    spike_view = np.reshape(spike_view, (num_timesteps + 1, -1))

    # Reshape views into more useful shapes
    v_view = np.reshape(v_view, (-1, 32))
    w_view = np.reshape(w_view, (-1, 32))

    # Run
    runtime.run()

    # Copy data back to host
    v_array.pull_from_device()
    w_array.pull_from_device()
    spike_array.pull_from_device()

    # Add to lists
    v = np.copy(v_view[1:,:])
    w = np.copy(w_view[1:,:])
    
    spikes = np.unpackbits(spike_view, axis=1, bitorder="little")[1:,:]
    #spike_times = [np.where(spikes[:,i])[0] * 0.1 for i in range(32)]
    
    # Wrap state around and copy back to device
    v_view[0] = v_view[-1]
    w_view[0] = w_view[-1]
    v_array.push_to_device()
    w_array.push_to_device()
    
    return v, w, spikes

#
device = False
num_timesteps = 2000
disassemble_code = False
fixed_point = 11
print("Running FeNN simulations")
fenn_data = {}
for n, p in param_sets.items():
    v, w, spike_times = simulate_fenn(p, device, num_timesteps, disassemble_code, fixed_point) 
    fenn_data[f"{n}_v"] = v
    fenn_data[f"{n}_w"] = w
    fenn_data[f"{n}_spikes"] = spike_times

np.savez("adexp_fenn.npz", **fenn_data)
