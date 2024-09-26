import numpy as np
import matplotlib.pyplot as plt

def plot_var_grid(var, ref_var, fixed_point, title, num_row, num_col):
    fig, axes = plt.subplots(num_row, num_col, sharex="col", sharey="row", squeeze=False)
    fig.suptitle(title)
    for i, a in enumerate(axes.flatten()):
        a.set_title(str(i), loc="left")
        ref_actor = a.plot(ref_var[:,i], alpha=0.5)[0]
        actor = a.plot(var[:,i] / (2 ** fixed_point), alpha=0.5)[0]
        #a.axhline(2 ** (15 - fixed_point), color="gray", linestyle="--")
        #a.axhline(-(2 ** (15 - fixed_point)), color="gray", linestyle="--")
    
    fig.legend([actor, ref_actor], ["FeNN", "GeNN"], ncol=2, loc="lower center")
    #fig.tight_layout(pad=0)

# Load GeNN data
ref_input_spikes_times = np.load("input_spikes_times.npz")
ref_input_spikes_ids = np.load("input_spikes_ids.npz")
ref_hidden_spikes_times = np.load("hidden_spikes_times.npz")
ref_hidden_spikes_ids = np.load("hidden_spikes_ids.npz")
ref_hidden_v = np.load("hidden_v.npy")
ref_hidden_a = np.load("hidden_a.npy")
ref_out_v = np.load("out_v.npy")
ref_out_v_sum = np.load("out_v_sum.npy") / 1170

for i in range(4):
    # Load FeNN data
    input_spikes = np.loadtxt(f"shd_input_spikes_{i}.csv", delimiter=",")
    hidden_spikes = np.loadtxt(f"shd_hidden_spikes_{i}.csv", delimiter=",")
    hidden_v = np.loadtxt(f"shd_hidden_v_{i}.csv", delimiter=",")
    hidden_a = np.loadtxt(f"shd_hidden_a_{i}.csv", delimiter=",")
    output_v = np.loadtxt(f"shd_output_v_{i}.csv", delimiter=",")
    output_v_sum = np.loadtxt(f"shd_output_v_sum_{i}.csv", delimiter=",")


    spike_fig, spike_axes = plt.subplots(2, 2, sharex="col", sharey="row")
    spike_fig.suptitle(f"Example {i}")
    spike_axes[0,0].set_title(f"FeNN ({len(hidden_spikes[:,0])} hidden spikes)")
    spike_axes[0,1].set_title(f"GeNN ({len(ref_hidden_spikes_ids[f'arr_{i}'])} hidden spikes)")
    spike_axes[0,0].set_ylabel("Input neuron")
    spike_axes[0,1].set_ylabel("Input neuron")
    spike_axes[0,0].scatter(input_spikes[:,0], input_spikes[:,1], s=1)
    spike_axes[0,1].scatter(ref_input_spikes_times[f"arr_{i}"], 
                            ref_input_spikes_ids[f"arr_{i}"], s=1)

    spike_axes[1,0].set_ylabel("Hidden neuron")
    spike_axes[1,1].set_ylabel("Hidden neuron")
    spike_axes[1,0].scatter(hidden_spikes[:,0], hidden_spikes[:,1], s=1)
    spike_axes[1,1].scatter(ref_hidden_spikes_times[f"arr_{i}"],
                            ref_hidden_spikes_ids[f"arr_{i}"], s=1)

    spike_axes[1,0].set_xlim((0, 1170))
    spike_axes[1,1].set_xlim((0, 1170))
    spike_axes[1,0].set_xlabel("Time [ms]")
    spike_axes[1,1].set_xlabel("Time [ms]")

    #plot_var_grid(hidden_v, ref_hidden_v[i], 7, f"Example{i}: Hidden V", 8, 32)
    #plot_var_grid(hidden_a, ref_hidden_a[i], 7, f"Example{i}: Hidden A", 8, 32)
    #plot_var_grid(output_v_sum, ref_out_v_sum[i], 11, f"Example{i}: Output VSum", 5, 4)
    #plot_var_grid(output_v, ref_out_v[i], 11, f"Example{i}: Output V", 5, 4)

plt.show()
