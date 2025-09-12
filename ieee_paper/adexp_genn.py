import numpy as np

from pygenn import GeNNModel

from pygenn import create_neuron_model

from adexp_params import param_sets

def simulate_genn(params, num_timesteps=2000):
    def dv(v: str, w: str):
        return f"(1.0 / c) * ((-gL * (({v}) - eL)) + (gL * deltaT * exp((({v}) - vThresh) / deltaT)) + i - ({w}))"

    def dw(v: str, w: str):
        return f"(1.0 / tauW) * ((a * ({v} - eL)) - {w})"

    adexp_model = create_neuron_model(
        "adexp",
        params=["c",        # Membrane capacitance [pF]
                "gL",       # Leak conductance [nS]
                "eL",       # Leak reversal potential [mV]
                "deltaT",   # Slope factor [mV]
                "vThresh",  # Threshold voltage [mV]
                "vSpike",   # Artificial spike height [mV]
                "vReset",   # Reset voltage [mV]
                "tauW",     # Adaption time constant
                "a",        # Subthreshold adaption [nS]
                "b",        # Spike-triggered adaptation [nA]
                "iOffset"], # Offset current
        vars=[("V", "scalar"), ("W", "scalar")],
        
        sim_code=
            f"""
            const scalar i = Isyn + iOffset;
            // If voltage is above artificial spike height
            if(V >= vSpike) {{
            V = vReset;
            }}
            // Calculate RK4 terms
            const scalar v1 = {dv('V', 'W')};
            const scalar w1 = {dw('V', 'W')};
            const scalar v2 = {dv('V + (dt * 0.5 * v1)', 'W + (dt * 0.5 * w1)')};
            const scalar w2 = {dw('V + (dt * 0.5 * v1)', 'W + (dt * 0.5 * w1)')};
            const scalar v3 = {dv('V + (dt * 0.5 * v2)', 'W + (dt * 0.5 * w2)')};
            const scalar w3 = {dw('V + (dt * 0.5 * v2)', 'W + (dt * 0.5 * w2)')};
            const scalar v4 = {dv('V + (dt * v3)', 'W + (dt * w3)')};
            const scalar w4 = {dw('V + (dt * v3)', 'W + (dt * w3)')};
            // Update V
            V += (dt / 6.0) * (v1 + (2.0 * (v2 + v3)) + v4);
            // If we're not above peak, update w
            // **NOTE** it's not safe to do this at peak as wn may well be huge
            if(V <= -40.0) {{
            W += (dt / 6.0) * (w1 + (2.0 * (w2 + w3)) + w4);
            }}
            """,

        threshold_condition_code="V > -40",

        reset_code=
            """
            // **NOTE** we reset v to arbitrary plotting peak rather than to actual reset voltage
            V = vSpike;
            W += b;
            """)

    # Parameters
    adexp_params = {
        "c":        params["c"] * 1000.0,
        "gL":       params["g_l"] * 1000.0,
        "eL":       params["e_l"],
        "deltaT":   params["delta_t"],
        "vThresh":  params["v_t"],
        "vSpike":   10.0,
        "vReset":   params["v_r"],
        "tauW":     params["tau_w"],
        "a":        params["a"],
        "b":        params["b"] * 1000.0,
        "iOffset":  params["i_offset"]}
    adexp_vars = {"V": params["e_l"], "W": 0.0}


    # Create model
    model = GeNNModel("float", "adexp", backend="single_threaded_cpu")
    model.dt = 0.1

    # Add facilitation population with 1 spike source which fires a single spike in first timestep
    adexp_pop = model.add_neuron_population("AdExp", 1, adexp_model, 
                                            adexp_params, adexp_vars)
    adexp_pop.spike_recording_enabled = True

    # Build and load model
    model.build()
    model.load(num_recording_timesteps=num_timesteps)

    # Simulate, recording V and Trigger every timestep
    v = []
    w = []
    while model.timestep < num_timesteps:
        model.step_time()
        adexp_pop.vars["V"].pull_from_device()
        adexp_pop.vars["W"].pull_from_device()
        v.append(adexp_pop.vars["V"].values)
        w.append(adexp_pop.vars["W"].values)
    
    # Download recording data
    model.pull_recording_buffers_from_device()

    # Get recording data
    spike_times, _ = adexp_pop.spike_recording_data[0]

    # Stack recordings together
    return np.vstack(v), np.vstack(w), spike_times

num_timesteps = 2000
print("Running GeNN reference simulations")
genn_data = {}
for n, p in param_sets.items():
    v, w, spike_times = simulate_genn(p, num_timesteps)
    genn_data[f"{n}_v"] = v
    genn_data[f"{n}_w"] = w
    genn_data[f"{n}_spike_times"] = spike_times

np.savez("adexp_genn.npz", **genn_data)
