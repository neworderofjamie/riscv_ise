import numpy as np
import matplotlib.pyplot as plt

voltages = np.loadtxt("compiler_test_voltages.csv", delimiter=",")
spikes1 = np.loadtxt("compiler_test_hidden1_spikes.csv", delimiter=",")
spikes2 = np.loadtxt("compiler_test_hidden2_spikes.csv", delimiter=",")

FIXED_POINT = 13
scale = 2 ** FIXED_POINT
fig, axes = plt.subplots(2, sharex=True)

axes[0].scatter(spikes1[:,0], spikes1[:,1] * 2, marker="|")
axes[1].scatter(spikes2[:,0], spikes2[:,1] * 2, marker="|")
#axes[0].set_ylabel("Neuron ID")
#axes[0].set_ylim((0, 32))
for a in range(32):
    axes[0].plot((a*2) + (voltages[:,a] / scale))
for a in range(32):
    axes[1].plot((a*2) + (voltages[:,a + 32] / scale))
axes[0].set_ylabel("Voltage")
axes[1].set_ylabel("Voltage")
axes[1].set_xlabel("Time [ms]")
plt.show()
