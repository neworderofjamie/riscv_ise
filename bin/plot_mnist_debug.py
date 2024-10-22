import numpy as np
import matplotlib.pyplot as plt


times = [19, 6, 8, 8, 38, 5, 4, 4, 4, 4, 5, 6, 6, 6, 6, 6, 6, 6, 6, 7, 29, 12, 25, 12,
        8, 5, 4, 5, 4, 4, 4, 5, 5, 4, 4, 9, 30, 29, 29, 29, 40, 5, 4, 13, 19, 5, 6, 5,
        4, 19, 10, 4, 5, 40, 5, 4, 35, 10, 4, 6, 6, 5, 42, 10, 4, 7, 23, 5, 5, 45, 5, 4,
        7, 6, 4, 5, 4, 4, 22, 5, 4, 12, 10, 4, 4, 36, 5, 4, 4, 11, 4, 4, 5, 11, 4, 6]
ids = [202, 203, 204, 205, 206, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
       243, 244, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273,
       292, 294, 295, 296, 297, 299, 300, 301, 326, 327, 328, 354, 355, 356, 381, 382, 383, 408,
       409, 410, 411, 436, 437, 438, 464, 465, 466, 491, 492, 493, 518, 519, 520, 521, 546, 547,
       548, 573, 574, 575, 601, 602, 603, 628, 629, 630, 655, 656, 657, 682, 683, 684, 685, 710,
       711, 712, 713, 738, 739, 740]

sim_spikes = np.loadtxt("mnist_spikes_sim.csv",delimiter=",")
device_spikes = np.loadtxt("mnist_spikes_device.csv",delimiter=",")

sim_v = np.loadtxt("mnist_v_sim.csv", delimiter=",")
device_v = np.loadtxt("mnist_v_device.csv", delimiter=",")

fig, axes = plt.subplots(3, sharex=True)
axes[0].scatter(times, ids, s=1)
axes[1].scatter(sim_spikes[:,0], sim_spikes[:,1], marker="x", alpha=0.5, label="sim")
axes[1].scatter(device_spikes[:,0], device_spikes[:,1], marker="*", alpha=0.5, label="device")
axes[1].legend()

fig, axes = plt.subplots(1, 2, sharey=True)
for i in range(128):
    axes[0].plot((sim_v[:,i] / (2 ** 5)) + (i * 5))
    axes[1].plot((device_v[:,i] / (2 ** 5)) + (i * 5))

plt.show()
