import numpy as np
import matplotlib.pyplot as plt
alif_sim = np.loadtxt("out_aliftest_14_9.txt", delimiter=",")
alif_device = np.loadtxt("out_aliftest_14_9.txt", delimiter=",")

fig, axes = plt.subplots(2, sharex=True)
axes[0].plot(alif_sim[:, 0], label="Sim")
axes[0].plot(alif_device[:, 0], label="Device")
axes[0].legend()

axes[1].plot(alif_sim[:, 1], label="Sim")
axes[1].plot(alif_device[:, 1], label="Device")
axes[1].legend()

plt.show()
