import numpy as np
import matplotlib.pyplot as plt

spikes = np.loadtxt("mnist_spikes_sim.csv",delimiter=",")
fig, axis = plt.subplots()
axis.scatter(spikes[:,0], spikes[:,1], s=1)
plt.show()
