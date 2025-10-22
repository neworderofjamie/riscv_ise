import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import plot_settings

from copy import copy
from pyfenn.utils import build_sparse_connectivity, generate_fixed_prob
from pandas import read_csv

def fit_poly(x, y, degree):
    return np.poly1d(np.polyfit(x, y, degree))

CLOCK_HZ = 166e6 # TODO update
DENSE_CYCLES = 4 
SPARSE_CYCLES = 7   # TODO update

# Load data
df = read_csv("va_benchmark_True_perf.csv")


# Calculate some columns
df["Num inhibitory neurons"] = df["Num excitatory neurons"] // 4
df["Num neurons"] = df["Num inhibitory neurons"] + df["Num excitatory neurons"]
df["Average excitatory rate [spikes/s]"] = df["Num excitatory spikes"] / df["Num excitatory neurons"]
df["Average inhibitory rate [spikes/s]"] = df["Num inhibitory spikes"] / df["Num inhibitory neurons"]
df["Num ESOPS"] = (df["Num excitatory spikes"] * (df["Num neurons"])) + (df["Num inhibitory spikes"] * (df["Num neurons"]))
df["Num SOPS"] = (df["Num excitatory spikes"] * (df["EE stride"] + df["EI stride"])) + (df["Num inhibitory spikes"] * (df["IE stride"] + df["II stride"]))

# Extract 10% connectivity using DRAM rows for main scaling figure
dram_0_1_df = df[(df["Using DRAM for weights"] == True) & (df["Probability of connection"] == 0.1)]

# Split DF into dense and sparse
sparse_df = dram_0_1_df[dram_0_1_df["Dense connectivity"] == False]
dense_df = dram_0_1_df[dram_0_1_df["Dense connectivity"] == True]

# Fit 1D polynomial to neuron and synapse cycles
neurons_poly = fit_poly(sparse_df["Num neurons"], sparse_df["Num neuron update cycles"], 1)
dense_synapse_poly = fit_poly(dense_df["Num SOPS"], dense_df["Num event processing cycles"], 1)
sparse_synapse_poly = fit_poly(sparse_df["Num SOPS"], sparse_df["Num event processing cycles"], 1)


# How many SOPs does GABAN example represent
gaban_num_neurons = 10000
gaban_connectivity = generate_fixed_prob(gaban_num_neurons, gaban_num_neurons, 0.1)
gaban_connectivity = build_sparse_connectivity(gaban_connectivity, 1, 10)
gaban_stride = gaban_connectivity.shape[1]
gaban_neuron_cycles = neurons_poly(gaban_num_neurons)
gaban_num_sops = 24.7 * gaban_num_neurons * gaban_stride
gaban_synapse_cycles = sparse_synapse_poly(gaban_num_sops)
gaban_time = (gaban_neuron_cycles + gaban_synapse_cycles) / CLOCK_HZ
print(f"Equivalent model to GABAN would simulate in {gaban_time}s (stride={gaban_stride})")

# How many SOPs does BlueVec example represent
bluevec_num_neurons = 64000
bluevec_neuron_cycles = neurons_poly(bluevec_num_neurons) * (13 / 5)
bluevec_num_sops = 10.0 * bluevec_num_neurons * 1000
bluevec_synapse_cycles = sparse_synapse_poly(bluevec_num_sops)
bluevec_time = (bluevec_neuron_cycles + bluevec_synapse_cycles) / CLOCK_HZ
print(f"Equivalent model to BlueVec would simulate in {bluevec_time}s)")


# Extract performance of largest models
big_df = df[df["Num excitatory neurons"] == 12800]
big_df["True GSOPs"] = (big_df["Num ESOPS"] / big_df["Simulation time [s]"]) / 1e9
big_df["Synapse GSOPs"] = ((big_df["Num ESOPS"] / big_df["Num event processing cycles"]) * CLOCK_HZ) / 1e9
big_df["Theoretical GSOPs"] = big_df.apply(lambda r: (((CLOCK_HZ * 32) / (DENSE_CYCLES * 1e9)) if r["Dense connectivity"]
                                                      else ((CLOCK_HZ * 32) / (SPARSE_CYCLES * r["Probability of connection"] * 1e9))),
                                           axis="columns")

big_df["xtick"] = big_df.apply(
    lambda r: ("Uncompressed" if r["Dense connectivity"] 
               else f"Compressed\n{(100.0 - (r['Probability of connection'] * 100)):.0f}%"),
    axis="columns")
                       
big_df = big_df[(big_df["Probability of connection"] != 0.25)
                | (big_df["Dense connectivity"] == False)]



fig, axes = plt.subplots(1, 3, figsize=(plot_settings.double_column_width, 2.5))

# Plot example spike raster
spikes = np.load("va_benchmark_spikes.npz")
inhibitory_actor = copy(axes[0].scatter(spikes["i_spike_times"], spikes["i_spike_ids"] + 2048, s=0.1))
excitatory_actor = copy(axes[0].scatter(spikes["e_spike_times"], spikes["e_spike_ids"], s=0.1))

axes[0].set_title("A", loc="left")
axes[0].set_xlabel("Time [ms]")
axes[0].set_ylabel("Neuron number")
sns.despine(ax=axes[0])
axes[0].xaxis.grid(False)
axes[0].yaxis.grid(False)

# Increase size of markers in spike actors
inhibitory_actor.set_sizes([10])
excitatory_actor.set_sizes([10])

fig.legend([inhibitory_actor, excitatory_actor], ["Inhibitory", "Excitatory"], 
           frameon=False, ncol=2, bbox_to_anchor=(1 / 6, 0.025), loc="lower center")
 
# Plot throughput
bar_x = np.arange(len(big_df))

# Plot simulation time
dense_dram_actor = axes[1].scatter(dense_df["Num neurons"], dense_df["Simulation time [s]"])
sparse_dram_actor = axes[1].scatter(sparse_df["Num neurons"], sparse_df["Simulation time [s]"])
axes[1].plot(dense_df["Num neurons"], 
             (neurons_poly(dense_df["Num neurons"]) 
              + dense_synapse_poly(dense_df["Num SOPS"])) / CLOCK_HZ, 
             color=dense_dram_actor.get_facecolor(), linestyle="--", alpha=0.5)
axes[1].plot(dense_df["Num neurons"], 
             (neurons_poly(sparse_df["Num neurons"]) 
              + sparse_synapse_poly(sparse_df["Num SOPS"])) / CLOCK_HZ, 
             color=sparse_dram_actor.get_facecolor(), linestyle="--", alpha=0.5)
axes[1].axhline(1.0, linestyle="--", color="gray")
axes[1].set_xlabel("Number of neurons")
axes[1].set_ylabel("Simulation time [s]")
axes[1].xaxis.grid(False)
axes[1].set_title("B", loc="left")
sns.despine(ax=axes[1])

fig.legend([dense_dram_actor, sparse_dram_actor], ["Uncompressed", "Compressed"], 
           frameon=False, ncol=2, bbox_to_anchor=(0.5, 0.025), loc="lower center")
 


true_actor = axes[2].bar(bar_x, big_df["True GSOPs"], width=0.2)
synapse_actor = axes[2].bar(bar_x + 0.2, big_df["Synapse GSOPs"], width=0.2)
theory_actor = axes[2].bar(bar_x + 0.4, big_df["Theoretical GSOPs"], width=0.2)

axes[2].set_xticks(bar_x + 0.2)
axes[2].set_xticklabels(big_df["xtick"])
#axes[2].set_xlabel("Connectivity")
axes[2].set_ylabel("Throughput (GSOP/s)")
axes[2].xaxis.grid(False)
axes[2].set_title("C", loc="left")
sns.despine(ax=axes[2])

fig.legend([true_actor, synapse_actor, theory_actor], ["Measured", "Measured synapses", "Theoretical"], 
           frameon=False, ncol=2, bbox_to_anchor=(1.0 - (1 / 6), 0.0), loc="lower center")

fig.tight_layout(pad=0, rect=[0.0, 0.125, 1.0, 1.0])
fig.savefig("va_benchmark.pdf")
plt.show()
