import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import plot_settings

from pandas import read_csv

def plot_best_fit(x, y, degree, ax, colour):
    poly = np.poly1d(np.polyfit(x, y, degree))
    fit = poly(x)
    ax.plot(x, fit, color=colour, linestyle="--", alpha=0.5)

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
df["Num SOPS"] = (df["Num excitatory spikes"] * (df["Num neurons"])) + (df["Num inhibitory spikes"] * (df["Num neurons"]))

# Extract 10% connectivity using DRAM rows for main scaling figure
dram_0_1_df = df[(df["Using DRAM for weights"] == True) & (df["Probability of connection"] == 0.1)]

# Split DF into dense and sparse
sparse_df = dram_0_1_df[dram_0_1_df["Dense connectivity"] == False]
dense_df = dram_0_1_df[dram_0_1_df["Dense connectivity"] == True]


# Extract performance of largest models
big_df = df[df["Num excitatory neurons"] == 12800]
big_df["True GSOPs"] = (big_df["Num SOPS"] / big_df["Simulation time [s]"]) / 1e9
big_df["Synapse GSOPs"] = ((big_df["Num SOPS"] / big_df["Num event processing cycles"]) * CLOCK_HZ) / 1e9
big_df["Theoretical GSOPs"] = big_df.apply(lambda r: (((CLOCK_HZ * 32) / (DENSE_CYCLES * 1e9)) if r["Dense connectivity"]
                                                      else ((CLOCK_HZ * 32) / (SPARSE_CYCLES * r["Probability of connection"] * 1e9))),
                                           axis="columns")

fig, axes = plt.subplots(1, 2, figsize=(plot_settings.double_column_width, 2.5))

# Plot neuron cycles and lines of best fit
#dense_dram_actor = axes[0].scatter(dense_dram_df["Num neurons"], dense_dram_df["Num neuron update cycles"] / 1e6)
#sparse_dram_actor = axes[0].scatter(sparse_dram_df["Num neurons"], sparse_dram_df["Num neuron update cycles"] / 1e6)
#plot_best_fit(dense_dram_df["Num neurons"], dense_dram_df["Num neuron update cycles"] / 1e6, 1, axes[0], dense_dram_actor.get_facecolor())
#plot_best_fit(sparse_dram_df["Num neurons"], sparse_dram_df["Num neuron update cycles"] / 1e6, 1, axes[0], sparse_dram_actor.get_facecolor())
#axes[0].set_xlabel("Number of neurons")
#axes[0].set_ylabel("Millions of neuron update cycles")
#axes[0].xaxis.grid(False)
#sns.despine(ax=axes[0])

# Plot synapse cycles and lines of best fit
#axes[1].scatter(dense_dram_df["Num SOPS"] / 1e6, dense_dram_df["Num event processing cycles"] / 1e6, color=dense_dram_actor.get_facecolor())
#axes[1].scatter(sparse_dram_df["Num SOPS"] / 1e6, sparse_dram_df["Num event processing cycles"] / 1e6, color=sparse_dram_actor.get_facecolor())
#plot_best_fit(dense_dram_df["Num SOPS"] / 1e6, dense_dram_df["Num event processing cycles"] / 1e6, 1, axes[1], dense_dram_actor.get_facecolor())
#plot_best_fit(sparse_dram_df["Num SOPS"] / 1e6, sparse_dram_df["Num event processing cycles"] / 1e6, 1, axes[1], sparse_dram_actor.get_facecolor())
#axes[1].set_xlabel("Millions of synaptic operations")
#axes[1].set_ylabel("Millions of event processing cycles")
#axes[1].xaxis.grid(False)
#sns.despine(ax=axes[1])

    
# Plot simulation time
dense_dram_actor = axes[1].scatter(dense_df["Num neurons"], dense_df["Simulation time [s]"])
sparse_dram_actor = axes[1].scatter(sparse_df["Num neurons"], sparse_df["Simulation time [s]"])
plot_best_fit(dense_df["Num neurons"], dense_df["Simulation time [s]"], 2, axes[1], dense_dram_actor.get_facecolor())
plot_best_fit(sparse_df["Num neurons"], sparse_df["Simulation time [s]"], 2, axes[1], sparse_dram_actor.get_facecolor())
axes[1].set_xlabel("Number of neurons")
axes[1].set_ylabel("Simulation time [s]")
axes[1].xaxis.grid(False)
sns.despine(ax=axes[1])

fig.legend([dense_dram_actor, sparse_dram_actor], 
           ["Dense", "Sparse"], 
           ncol=2, loc="lower center", frameon=False)

fig.tight_layout(pad=0, rect=[0.0, 0.1, 1.0, 1.0])

plt.show()
