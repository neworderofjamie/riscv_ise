import numpy as np

import nest
import nest.voltage_trace

from adexp_params import param_sets

def simulate_nest(params, num_timesteps=2000):
    nest.ResetKernel()

    ###############################################################################
    # First we make sure that the resolution of the simulation is 0.1 ms. This is
    # important, since the slop of the action potential is very steep.

    nest.resolution = 0.1
    neuron = nest.Create("aeif_cond_alpha")

    ###############################################################################
    # `a` and `b` are parameters of the adex model. Their values come from the
    # publication.

    neuron.set(C_m=params["c"] * 1000.0,
               V_reset=params["v_r"],
               E_L=params["e_l"],
               g_L=params["g_l"] * 1000.0,
               I_e=params["i_offset"],
               a=params["a"], b=params["b"] * 1000.0,
               Delta_T=params["delta_t"], tau_w=params["tau_w"],
               V_th=params["v_t"], V_peak=10.0,
               V_m=params["e_l"])
    
    
    # Add multimeter to record V and W
    mm = nest.Create("multimeter", 1, {"record_from": ["V_m", "w"], "record_to": "memory"})
    nest.SetStatus(mm, {"interval": 0.1})
    nest.Connect(mm, neuron)
    
    # Add spike recorder
    sr  = nest.Create("spike_recorder", params={"record_to": "memory", "time_in_steps": True})
    nest.Connect(neuron, sr)
    
    # Simulate
    nest.Simulate((num_timesteps + 1) * 0.1)
    
    # **HACK** set fixed spike height
    v = mm.events["V_m"].copy()
    v[sr.events["times"] - 1] = 10.0

    # Stack recordings together
    return v, mm.events["w"], sr.events["times"] * 0.1

num_timesteps = 2000
print("Running NEST reference simulations")
genn_data = {}
for n, p in param_sets.items():
    v, w, spike_times = simulate_nest(p, num_timesteps)
    genn_data[f"{n}_v"] = v
    genn_data[f"{n}_w"] = w
    genn_data[f"{n}_spike_times"] = spike_times

np.savez("adexp_nest.npz", **genn_data)
