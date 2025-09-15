import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import plot_settings

from pandas import read_csv

def get_peak_true_msops(df):
    return (df["Num SOPS"] / df["Simulation time [s]"]).max() / 1e6

def get_peak_msops(df, clock=166e6):
    return ((df["Num SOPS"] / df["Num event processing cycles"]) * clock).max() / 1e6

def plot_best_fit(x, y, degree, ax, colour):
    poly = np.poly1d(np.polyfit(x, y, degree))
    fit = poly(x)
    ax.plot(x, fit, color=colour, linestyle="--", alpha=0.5)

# Load data
df = read_csv("va_benchmark_True_perf.csv")

# Calculate some columns
df["Num inhibitory neurons"] = df["Num excitatory neurons"] // 4
df["Num neurons"] = df["Num inhibitory neurons"] + df["Num excitatory neurons"]
df["Average excitatory rate [spikes/s]"] = df["Num excitatory spikes"] / df["Num excitatory neurons"]
df["Average inhibitory rate [spikes/s]"] = df["Num inhibitory spikes"] / df["Num inhibitory neurons"]
df["Num SOPS"] = (df["Num excitatory spikes"] * (df["EE stride"] + df["EI stride"])) + (df["Num inhibitory spikes"] * (df["IE stride"] + df["II stride"]))

# Split DF into dense and sparse
sparse_df = df[df["Dense connectivity"] == False]
dense_df = df[df["Dense connectivity"] == True]

# Split these into URAM and DRAM
dense_uram_df = dense_df[dense_df["Using DRAM for weights"] == False]
dense_dram_df = dense_df[dense_df["Using DRAM for weights"] == True]
sparse_uram_df = sparse_df[sparse_df["Using DRAM for weights"] == False]
sparse_dram_df = sparse_df[sparse_df["Using DRAM for weights"] == True]

print(f"True dense URAM MSOPS/S = {get_peak_true_msops(dense_uram_df)}")
print(f"True dense DRAM MSOPS/S = {get_peak_true_msops(dense_dram_df)}")
print(f"True sparse URAM MSOPS/S = {get_peak_true_msops(sparse_uram_df)}")
print(f"True sparse DRAM MSOPS/S = {get_peak_true_msops(sparse_dram_df)}")

print(f"Dense URAM MSOPS/S = {get_peak_msops(dense_uram_df)}")
print(f"Dense DRAM MSOPS/S = {get_peak_msops(dense_dram_df)}")
print(f"Sparse URAM MSOPS/S = {get_peak_msops(sparse_uram_df)}")
print(f"Sparse DRAM MSOPS/S = {get_peak_msops(sparse_dram_df)}")


fig, axes = plt.subplots(1, 3, figsize=(plot_settings.double_column_width, 2.5))

# Plot neuron cycles and lines of best fit
dense_dram_actor = axes[0].scatter(dense_dram_df["Num neurons"], dense_dram_df["Num neuron update cycles"] / 1e6)
sparse_dram_actor = axes[0].scatter(sparse_dram_df["Num neurons"], sparse_dram_df["Num neuron update cycles"] / 1e6)
plot_best_fit(dense_dram_df["Num neurons"], dense_dram_df["Num neuron update cycles"] / 1e6, 1, axes[0], dense_dram_actor.get_facecolor())
plot_best_fit(sparse_dram_df["Num neurons"], sparse_dram_df["Num neuron update cycles"] / 1e6, 1, axes[0], sparse_dram_actor.get_facecolor())
axes[0].set_xlabel("Number of neurons")
axes[0].set_ylabel("Millions of neuron update cycles")
axes[0].xaxis.grid(False)
sns.despine(ax=axes[0])

# Plot synapse cycles and lines of best fit
axes[1].scatter(dense_dram_df["Num SOPS"] / 1e6, dense_dram_df["Num event processing cycles"] / 1e6, color=dense_dram_actor.get_facecolor())
axes[1].scatter(sparse_dram_df["Num SOPS"] / 1e6, sparse_dram_df["Num event processing cycles"] / 1e6, color=sparse_dram_actor.get_facecolor())
plot_best_fit(dense_dram_df["Num SOPS"] / 1e6, dense_dram_df["Num event processing cycles"] / 1e6, 1, axes[1], dense_dram_actor.get_facecolor())
plot_best_fit(sparse_dram_df["Num SOPS"] / 1e6, sparse_dram_df["Num event processing cycles"] / 1e6, 1, axes[1], sparse_dram_actor.get_facecolor())
axes[1].set_xlabel("Millions of synaptic operations")
axes[1].set_ylabel("Millions of event processing cycles")
axes[1].xaxis.grid(False)
sns.despine(ax=axes[1])

    
# Plot simulation time
axes[2].scatter(dense_dram_df["Num neurons"], dense_dram_df["Simulation time [s]"], color=dense_dram_actor.get_facecolor())
axes[2].scatter(sparse_dram_df["Num neurons"], sparse_dram_df["Simulation time [s]"], color=sparse_dram_actor.get_facecolor())
plot_best_fit(dense_dram_df["Num neurons"], dense_dram_df["Simulation time [s]"], 2, axes[2], dense_dram_actor.get_facecolor())
plot_best_fit(sparse_dram_df["Num neurons"], sparse_dram_df["Simulation time [s]"], 2, axes[2], sparse_dram_actor.get_facecolor())
axes[2].set_xlabel("Number of neurons")
axes[2].set_ylabel("Simulation time [s]")
axes[2].xaxis.grid(False)
sns.despine(ax=axes[2])

fig.legend([dense_dram_actor, sparse_dram_actor], 
           ["Dense", "Sparse"], 
           ncol=2, loc="lower center", frameon=False)

fig.tight_layout(pad=0, rect=[0.0, 0.1, 1.0, 1.0])

plt.show()
