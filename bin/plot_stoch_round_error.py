import numpy as np
import matplotlib.pyplot as plt

SCALE = 2 ** 15

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

fig, axes = plt.subplots(3, sharex=True)
axes[0].set_title("Round-to-Zero")
axes[0].axvline(0.0, linestyle="--", color="black")
axes[0].hist(rz_error.flatten() * SCALE, bins=20, density=True)
axes[0].set_ylim((0, 1.0))

axes[1].set_title("Round-to-Nearest")
axes[1].axvline(0.0, linestyle="--", color="black")
axes[1].hist(rn_error.flatten() * SCALE, bins=20, density=True)
axes[1].set_ylim((0, 1.0))

axes[2].set_title("Stochastic round")
axes[2].axvline(0.0, linestyle="--", color="black")
axes[2].hist(rs_error.flatten() * SCALE, bins=20, density=True)
axes[2].set_ylim((0, 1.0))
plt.show()
