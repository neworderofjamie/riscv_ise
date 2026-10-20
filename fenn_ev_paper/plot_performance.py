from pandas import read_csv
import matplotlib.pyplot as plt
import numpy as np

data = read_csv("va_benchmark_sim.csv")

fig, axis = plt.subplots()

x = np.arange(len(data))
synapse_actor = axis.bar(x, data["1 core synapse cycles"], width=0.25)[0]
axis.bar(x + 0.25, data["2 core synapse cycles"], width=0.25, color=synapse_actor.get_facecolor())
axis.bar(x + 0.5, data["old synapse cycles"], width=0.25, color=synapse_actor.get_facecolor())
neuron_actor = axis.bar(x, data["1 core neuron cycles"], bottom=data["1 core synapse cycles"], width=0.25)[0]
axis.bar(x + 0.25, data["2 core neuron cycles"], bottom=data["2 core synapse cycles"], width=0.25, color=neuron_actor.get_facecolor())
axis.bar(x + 0.5, data["old neuron cycles"], bottom=data["old synapse cycles"], width=0.25, color=neuron_actor.get_facecolor())
plt.show()