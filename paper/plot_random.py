import numpy as np
import matplotlib.gridspec as gs
import matplotlib.pyplot as plt
import seaborn as sns

import plot_settings

from scipy.stats import poisson

SCALE = 2 ** 15


# Create figure
fig = plt.figure(figsize=(plot_settings.double_column_width, 2.0), frameon=False)

# Create outer gridspec dividing plot area into 2 columns
gs_two_col = gs.GridSpec(1, 2)

# Create axes in 1st columns
poisson_axis = plt.Subplot(fig, gs_two_col[0])
fig.add_subplot(poisson_axis)

# Create sub-grid spec in 2nd column
gs_round = gs.GridSpecFromSubplotSpec(3, 1, subplot_spec=gs_two_col[1])

# Add axes with sharex x within this
rtz_axis = plt.Subplot(fig, gs_round[0])
rtn_axis = plt.Subplot(fig, gs_round[1], sharex=rtz_axis)
rts_axis = plt.Subplot(fig, gs_round[2], sharex=rtz_axis)
fig.add_subplot(rtz_axis)
fig.add_subplot(rtn_axis)
fig.add_subplot(rts_axis)

# Plot poisson distribution
poisson_data = np.loadtxt("out_poisson_device.txt", dtype=int)
x = np.arange(np.amin(poisson_data), 1 + np.amax(poisson_data))
poisson_axis.hist(poisson_data, bins=x - 0.5, density=True, linewidth=0)
poisson_axis.plot(x, poisson.pmf(x, 5.12))
poisson_axis.set_xlabel("k")
poisson_axis.set_xticks(x)
poisson_axis.set_ylabel("P(x=k)")
poisson_axis.set_title("A", loc="left")
poisson_axis.xaxis.grid(False)
sns.despine(ax=poisson_axis)

# Load operands and reshape into pairs of vectors
operands = np.fromfile("stoch_rand_operands.bin", dtype=np.int16)
operands = np.reshape(operands, (-1, 2, 32))

# Convert to float
operands_float = operands / SCALE
assert np.all(np.abs(operands_float) <= 1.0)

# Load results and reshape into triples of vectors
results = np.fromfile("out_stoch_round.bin", dtype=np.int16)
results = np.reshape(results, (-1, 3 ,32))
results_float = results / SCALE

# Calculate floating point multiplication
results_correct = operands_float[:,0,:] * operands_float[:,1,:]

# Calculate errors between two rounding modes
rz_error = results_float[:,0,:] - results_correct
rn_error = results_float[:,1,:] - results_correct
rs_error = results_float[:,2,:] - results_correct

rtz_axis.set_title("B", loc="left")
rtz_axis.axvline(0.0, linestyle="--", color="black")
rtz_axis.hist(rz_error.flatten() * SCALE, bins=20, density=True, linewidth=0)
rtz_axis.set_ylim((0, 1.0))
sns.despine(ax=rtz_axis)
rtz_axis.xaxis.grid(False)
rtz_axis.yaxis.grid(False)

#rtn_axis.set_title("C", loc="left")
rtn_axis.axvline(0.0, linestyle="--", color="black")
rtn_axis.hist(rn_error.flatten() * SCALE, bins=20, density=True, linewidth=0)
rtn_axis.set_ylim((0, 1.0))
sns.despine(ax=rtn_axis)
rtn_axis.xaxis.grid(False)
rtn_axis.yaxis.grid(False)

#rts_axis.set_title("D", loc="left")
rts_axis.axvline(0.0, linestyle="--", color="black")
rts_axis.hist(rs_error.flatten() * SCALE, bins=20, density=True, linewidth=0)
rts_axis.set_ylim((0, 1.0))
sns.despine(ax=rts_axis)
rts_axis.xaxis.grid(False)
rts_axis.yaxis.grid(False)

fig.tight_layout(pad=0)
fig.savefig("random.pdf")
plt.show()