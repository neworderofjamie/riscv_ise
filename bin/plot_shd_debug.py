import matplotlib.pyplot as plt
import numpy as np

def plot_var(stem, num_rows=4, num_cols=4):
    var_sim = np.loadtxt(f"shd_{stem}_sim.csv", delimiter=",")
    var_device = np.loadtxt(f"shd_{stem}_device.csv", delimiter=",")

    var_fig, var_axes = plt.subplots(num_rows, num_cols, sharex="col", sharey="row")
    var_fig.suptitle(stem)

    for i, a in enumerate(var_axes.flatten()):
        a.plot(var_device[:,120 + i], alpha=0.5, label="Device")
        a.plot(var_sim[:,120 + i],  alpha=0.5, label="Simulation")
    a.legend()
"""
spike_sim = np.loadtxt("shd_spikes_sim.csv", delimiter=",")
spike_device = np.loadtxt("shd_spikes_device.csv", delimiter=",")

input_spike_sim = np.loadtxt("shd_input_spikes_sim.csv", delimiter=",")
input_spike_device = np.loadtxt("shd_input_spikes_device.csv", delimiter=",")



spike_fig, spike_axis = plt.subplots()
spike_axis.scatter(spike_sim[:,0], spike_sim[:,1], marker="x", label="Simulation")
spike_axis.scatter(spike_device[:,0], spike_device[:,1], marker="*", label="Device")
spike_axis.legend()

"""
#plot_var("isyn", 4, 4)
plot_var("v", 4, 4)

plt.show()
