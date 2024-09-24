import numpy as np
import matplotlib.pyplot as plt

input_spikes = np.loadtxt("shd_input_spikes.csv", delimiter=",")
hidden_spikes = np.loadtxt("shd_hidden_spikes.csv", delimiter=",")
hidden_v = np.loadtxt("shd_hidden_v.csv", delimiter=",")

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

var_fig, var_axes = plt.subplots(16, 16, sharex="col", sharey="row")
ref_hidden_v = np.load("hidden_v.npy")
print(np.amin(ref_hidden_v), np.amax(ref_hidden_v))
for i, a in enumerate(var_axes.flatten()):
    a.plot(ref_hidden_v[:,i], alpha=0.5)
    a.plot(hidden_v[:,i] / 256.0, alpha=0.5)
plt.show()
