import matplotlib.gridspec as gs
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import plot_settings


NUM_TIMESTEPS = 1170
NUM_EXAMPLES = 2264

# Named sets of op codes
op_code_set_names = ["ALU", "Mem.", "Vec. ALU", "Vec. Mem.", "Other"]

def standard_op(funct1):
    return (funct1 << 2) | 3

def vector_op(funct1):
    return (funct1 << 2) | 2
    
op_code_map = {standard_op(4):  0,  # OP-IMM
               standard_op(12): 0,  # OP
               standard_op(0):  1,  # LOAD
               standard_op(8):  1,  # STORE
               vector_op(0):    2,  # VSOP
               vector_op(4):    3,  # VLOAD
               vector_op(5):    3}  # VSTORE

sect_addresses = [8, 232, 508, 1028, 1136]
sect_names = ["Input\nspikes", "Hidden\nspikes", "Hidden\nneurons", "Output\nneurons"]
assert len(sect_names) == (len(sect_addresses) - 1)

fenn_seconds = 29.04
jetson_gpu_seconds = 42.4 + 25.7 + 1.5
jetson_cpu_seconds = 75.68 + 35.81

fenn_seconds_per_timestep = fenn_seconds / (NUM_EXAMPLES * NUM_TIMESTEPS)
jetson_gpu_seconds_per_timestep = jetson_gpu_seconds / (NUM_EXAMPLES * NUM_TIMESTEPS)
jetson_cpu_seconds_per_timestep = jetson_cpu_seconds / (NUM_EXAMPLES * NUM_TIMESTEPS)

# Create figure
fig = plt.figure(figsize=(plot_settings.double_column_width, 2.0), frameon=False)

# Create outer gridspec dividing plot area into 3 columns
gs_three_col = gs.GridSpec(1, 3)

# Create axes in 1st and 3rd columns
bar_axis = plt.Subplot(fig, gs_three_col[1])
inst_axis = plt.Subplot(fig, gs_three_col[2])
fig.add_subplot(inst_axis)
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

# Stacked bar chart of different types of operation in different sections
heatmap = np.loadtxt("shd_heatmap.txt", dtype=int, delimiter=",")

sections = []
for start_addr, end_addr in zip(sect_addresses[0:-1], sect_addresses[1:]):
    sect_heatmap = heatmap[start_addr // 4:end_addr // 4,:]
    
    # Convert OP codes to class indices
    inst_class = np.asarray([op_code_map.get(o, 4) 
                             for o in sect_heatmap[:,0]])
    
    # Count instructions of each class in section
    sections.append([np.sum(sect_heatmap[inst_class == i,1]) / 1E9
                     for i, _ in enumerate(op_code_set_names)])

# Transpose sections
sections = list(zip(*sections))

bar_x = np.arange(len(sections[0]))
bottom = np.zeros(len(sections[0]))
actors = []
for s in sections:
    actors.append(inst_axis.bar(bar_x, s, width=0.8, 
                                linewidth=0, bottom=bottom))
    bottom += s

inst_axis.set_ylabel("Number of instructions [billion]")
inst_axis.set_title("C", loc="left")
inst_axis.set_xticks(bar_x, labels=sect_names)
inst_axis.xaxis.grid(False)
sns.despine(ax=inst_axis)

fig.legend(actors, op_code_set_names, ncol=3, loc="lower center", 
           bbox_to_anchor=(5/6, 0.0), frameon=False)

fig.tight_layout(pad=0, rect=[0.0, 0.175, 1.0, 1.0])
fig.savefig("shd_perf.pdf", dpi=400)
plt.show()
