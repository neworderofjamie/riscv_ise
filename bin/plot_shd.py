import numpy as np
import matplotlib.pyplot as plt

def plot_var_grid(var, ref_var, fixed_point, title, num_row, num_col):
    fig, axes = plt.subplots(num_row, num_col, sharex="col", sharey="row", squeeze=False)
    fig.suptitle(title)
    for i, a in enumerate(axes.flatten()):
        a.set_title(str(i), loc="left")
        ref_actor = a.plot(ref_var[:,i], alpha=0.5)[0]
        actor = a.plot(var[:,i] / (2 ** fixed_point), alpha=0.5)[0]

    fig.legend([actor, ref_actor], ["FeNN", "GeNN"], ncol=2, loc="lower center")
    fig.tight_layout(pad=0)

input_spikes = np.loadtxt("shd_input_spikes.csv", delimiter=",")
hidden_spikes = np.loadtxt("shd_hidden_spikes.csv", delimiter=",")
hidden_v = np.loadtxt("shd_hidden_v.csv", delimiter=",")
hidden_a = np.loadtxt("shd_hidden_a.csv", delimiter=",")
output_v = np.loadtxt("shd_output_v.csv", delimiter=",")
output_v_sum = np.loadtxt("shd_output_v_sum.csv", delimiter=",")

ref_input_spikes_times = np.load("input_spikes_times.npy")
ref_input_spikes_ids = np.load("input_spikes_ids.npy")
ref_hidden_spikes_times = np.load("hidden_spikes_times.npy")
ref_hidden_spikes_ids = np.load("hidden_spikes_ids.npy")

spike_fig, spike_axes = plt.subplots(2, 2, sharex="col", sharey="row")

spike_axes[0,0].set_ylabel("Input neuron")
spike_axes[0,1].set_ylabel("Input neuron")
spike_axes[0,0].scatter(input_spikes[:,0], input_spikes[:,1], s=1)
spike_axes[0,1].scatter(ref_input_spikes_times, ref_input_spikes_ids, s=1)

spike_axes[1,0].set_ylabel("Hidden neuron")
spike_axes[1,1].set_ylabel("Hidden neuron")
spike_axes[1,0].scatter(hidden_spikes[:,0], hidden_spikes[:,1], s=1)
spike_axes[1,1].scatter(ref_hidden_spikes_times, ref_hidden_spikes_ids, s=1)

spike_axes[1,0].set_xlim((0, 1170))
spike_axes[1,1].set_xlim((0, 1170))
spike_axes[1,0].set_xlabel("Time [ms]")
spike_axes[1,1].set_xlabel("Time [ms]")

plot_var_grid(hidden_v, np.load("hidden_v.npy"), 9, "Hidden V", 8, 8)
plot_var_grid(hidden_a, np.load("hidden_a.npy"), 8, "Hidden A", 8, 8)
plot_var_grid(output_v_sum, np.load("out_v_sum.npy") / 1170, 11, "Output VSum", 5, 4)
plot_var_grid(output_v, np.load("out_v.npy"), 11, "Output V", 5, 4)

plt.show()
