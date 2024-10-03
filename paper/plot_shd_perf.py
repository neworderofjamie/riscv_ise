import matplotlib.gridspec as gs
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import plot_settings


NUM_TIMESTEPS = 1170
NUM_EXAMPLES = 2264

# Instruction counts
num_total = 4870831687
num_total_vector = 2048015638
num_scalar_jump = 671647121
num_scalar_mem = 219857040
num_scalar_alu = 1728445796
num_vector_mem = 1292095482
num_vector_alu = 549015134

num_other = (num_total - num_scalar_alu - num_scalar_mem - num_scalar_jump
             - num_vector_alu - num_vector_mem) 

fenn_seconds = 4870831687 / 200E6   # TODO
jetson_gpu_seconds = 42.4 + 25.7 + 1.5
jetson_cpu_seconds = 119    # TODO

fenn_seconds_per_timestep = fenn_seconds / (NUM_EXAMPLES * NUM_TIMESTEPS)
jetson_gpu_seconds_per_timestep = jetson_gpu_seconds / (NUM_EXAMPLES * NUM_TIMESTEPS)
jetson_cpu_seconds_per_timestep = jetson_cpu_seconds / (NUM_EXAMPLES * NUM_TIMESTEPS)

# Create figure
fig = plt.figure(figsize=(plot_settings.double_column_width, 2.0), frameon=False)

# Create outer gridspec dividing plot area into 3 columns
gs_three_col = gs.GridSpec(1, 3)

# Create axes in 1st and 3rd columns
bar_axis = plt.Subplot(fig, gs_three_col[1])
pie_axis = plt.Subplot(fig, gs_three_col[2])
fig.add_subplot(pie_axis)
fig.add_subplot(bar_axis)

# Create sub-grid spec in 2nd column
gs_raster = gs.GridSpecFromSubplotSpec(2, 1, subplot_spec=gs_three_col[0], hspace=0.1)

# Add axes with sharex x within this
input_raster_axis = plt.Subplot(fig, gs_raster[0])
hidden_raster_axis = plt.Subplot(fig, gs_raster[1], sharex=input_raster_axis)
fig.add_subplot(input_raster_axis)
fig.add_subplot(hidden_raster_axis)

# Spikes
input_spikes = np.loadtxt(f"shd_input_spikes_1.csv", delimiter=",")
hidden_spikes = np.loadtxt(f"shd_hidden_spikes_1.csv", delimiter=",")
input_raster_axis.scatter(input_spikes[:,0], input_spikes[:,1], s=0.1, rasterized=True)
hidden_raster_axis.scatter(hidden_spikes[:,0], hidden_spikes[:,1], s=0.1, rasterized=True)
input_raster_axis.set_ylabel("Input neuron")
hidden_raster_axis.set_ylabel("Hidden neuron")
hidden_raster_axis.set_xlabel("Time [ms]")
input_raster_axis.set_title("A", loc="left")
input_raster_axis.xaxis.grid(False)
input_raster_axis.yaxis.grid(False)
hidden_raster_axis.xaxis.grid(False)
hidden_raster_axis.yaxis.grid(False)
sns.despine(ax=input_raster_axis)
sns.despine(ax=hidden_raster_axis)

# Performance comparison
bar_ticks = np.arange(4)
bar_axis.bar(bar_ticks, [jetson_cpu_seconds_per_timestep * 1E6, jetson_gpu_seconds_per_timestep * 1E6,
                        16.79, fenn_seconds_per_timestep * 1E6])
bar_axis.set_ylabel("Time per timestep [µs]")
bar_axis.set_xticks(bar_ticks, ["Jetson\nCPU", "Jetson\nGPU", "Loihi", "FeNN"])
bar_axis.set_title("B", loc="left")
bar_axis.xaxis.grid(False)
sns.despine(ax=bar_axis)

# Pie chart of operations
pie_axis.pie([num_scalar_alu, num_scalar_mem, num_scalar_jump, num_vector_alu, num_vector_mem, num_other],
              labels=["Scalar ALU", "Scalar Memory", "Scalar Control", "Vector ALU", "Vector Memory", "Other"])
pie_axis.set_title("C", loc="left")


fig.tight_layout(pad=0)
fig.savefig("shd_perf.pdf", dpi=400)
plt.show()