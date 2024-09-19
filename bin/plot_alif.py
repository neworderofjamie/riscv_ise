import numpy as np
import matplotlib.pyplot as plt

def load_data(filename, desired_shape):
    data = np.loadtxt(filename, delimiter=",")
    assert desired_shape[0] == data.shape[0]
    return (np.average(data, axis=1), 
            np.std(data, axis=1))

def plot_fixed_point(axis, timesteps, v_rec, data_average, data_std, num_frac_bits):
    scale = 1.0 / (2 ** num_frac_bits)
    axis.errorbar(timesteps, data_average * scale, yerr=data_std * scale)
    axis.plot(timesteps, v_rec, linestyle="--")
    axis.set_ylabel("Membrane voltage")

TAU_A = 2000.0
TAU_M = 20.0
ALPHA = np.exp(-1 / TAU_M)
RHO = np.exp(-1 / TAU_A)
WEIGHT = 0.01
BETA = 0.0174
V_THRESH = 0.6

# Load poisson input
poisson = np.fromfile("poisson_data.bin", dtype=np.int16)

# Throw away copies of poisson and check shape matches
poisson = np.reshape(poisson, (-1, 32))
poisson = poisson[:,0]

data_10_average, data_10_std = load_data("out_alif_10.txt", poisson.shape)
data_11_average, data_11_std = load_data("out_alif_11.txt", poisson.shape)
data_10_rn_average, data_10_rn_std = load_data("out_alif_10_rn.txt", poisson.shape)
data_10_rs_average, data_10_rs_std = load_data("out_alif_10_rs.txt", poisson.shape)

# Simulate
v = 0
a = 0
v_rec = []
a_rec = []
for p in poisson:
    v *= ALPHA
    a *= RHO
    v += (p * WEIGHT)

    if v > (V_THRESH + (BETA * a)):
        v -= V_THRESH
        a += 1.0
    v_rec.append(v)
    a_rec.append(a)

timesteps = np.arange(len(poisson))

fig, axes = plt.subplots(4, sharex=True, squeeze=False)

# Apply fixed point scale and compare
axes[0,0].set_title("10 fractional bits, RTZ")
plot_fixed_point(axes[0,0], timesteps, v_rec, data_10_average, data_10_std, 10)

# Apply fixed point scale and compare
axes[1,0].set_title("11 fractional bits, RTZ")
plot_fixed_point(axes[1,0], timesteps, v_rec, data_11_average, data_11_std, 10)

# Apply fixed point scale and compare
axes[2,0].set_title("10 fractional bits, RTN")
plot_fixed_point(axes[2,0], timesteps, v_rec, data_10_rn_average, data_10_rn_std, 10)

# Apply fixed point scale and compare
axes[3,0].set_title("10 fractional bits, RTS")
plot_fixed_point(axes[3,0], timesteps, v_rec, data_10_rs_average, data_10_rs_std, 10)
axes[3,0].set_xlabel("Time [ms]")

plt.show()
