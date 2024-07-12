import numpy as np
import matplotlib.pyplot as plt

input_spikes = np.loadtxt("input_spikes.csv", delimiter=",")
hidden_spikes = np.loadtxt("hidden_spikes.csv", delimiter=",")

fig, axes = plt.subplots(2, sharex=True)

axes[0].set_ylabel("Input neuron")
axes[0].scatter(input_spikes[:,0], input_spikes[:,1], s=1)

axes[1].set_ylabel("Hidden neuron")
axes[1].scatter(hidden_spikes[:,0], hidden_spikes[:,1], s=1)

axes[1].set_xlabel("Time [ms]")

plt.show()
