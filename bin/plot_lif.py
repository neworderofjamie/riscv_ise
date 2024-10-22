import numpy as np
import matplotlib.pyplot as plt

voltages = np.loadtxt("lif_voltages.csv", delimiter=",")
spikes = np.loadtxt("lif_spikes.csv", delimiter=",")
spikes_sim = np.loadtxt("lif_spikes_sim.csv", delimiter=",")

FIXED_POINT = 13
scale = 2 ** FIXED_POINT
fig, axes = plt.subplots(2, sharex=True)

axes[0].scatter(spikes[:,0], spikes[:,1],marker="x", label="Device")
axes[0].scatter(spikes_sim[:,0], spikes_sim[:,1],marker="*",label="Simulator")
axes[0].set_ylabel("Neuron ID")
axes[0].set_ylim((0, 32))
for a in range(32):
    axes[1].plot((a*2) + (voltages[:,a] / scale))
axes[1].set_ylabel("Voltage")
axes[1].set_xlabel("Time [ms]")
axes[0].legend()
plt.show()
