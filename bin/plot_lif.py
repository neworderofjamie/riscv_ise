import numpy as np
import matplotlib.pyplot as plt

voltages = np.loadtxt("lif_voltages.csv", delimiter=",")
spikes = np.loadtxt("lif_spikes.csv", delimiter=",")

FIXED_POINT = 13
scale = 2 ** FIXED_POINT
fig, axes = plt.subplots(2, sharex=True)

axes[0].scatter(spikes[:,0], spikes[:,1],s=1)
for a in range(32):
    axes[1].plot((a*2) + (voltages[:,a] / scale))
plt.show()
