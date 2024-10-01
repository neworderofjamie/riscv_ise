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

fig, axes = plt.subplots(1, 3, figsize=(plot_settings.double_column_width, 2.0))
axes[0].pie([num_scalar_alu, num_scalar_mem, num_scalar_jump, num_vector_alu, num_vector_mem, num_other],
             labels=["Scalar ALU", "Scalar Memory", "Scalar Control", "Vector ALU", "Vector Memory", "Other"])

bar_ticks = np.arange(4)
axes[2].bar(bar_ticks, [jetson_cpu_seconds_per_timestep * 1E6, jetson_gpu_seconds_per_timestep * 1E6,
                            16.79, fenn_seconds_per_timestep * 1E6])
axes[2].set_ylabel("Time per timestep [us]")
axes[2].set_xticks(bar_ticks, ["Jetson\nCPU", "Jetson\nGPU", "Loihi*", "FeNN"])
sns.despine(ax=axes[2])

fig.tight_layout(pad=0)
plt.show()