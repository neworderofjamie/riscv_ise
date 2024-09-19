import numpy as np
import matplotlib.pyplot as plt

def load_data(filename, desired_shape):
    data = np.loadtxt(filename, delimiter=",")
    assert desired_shape[0] == data.shape[0]
    return (np.average(data, axis=1), 
            np.std(data, axis=1))

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
axes[0,0].errorbar(timesteps, data_10_average / (2 ** 10), yerr=data_10_std / (2 ** 10))
axes[0,0].plot(timesteps, v_rec, linestyle="--")

# Apply fixed point scale and compare
axes[1,0].set_title("11 fractional bits, RTZ")
axes[1,0].errorbar(timesteps, data_11_average / (2 ** 11), yerr=data_11_std / (2 ** 11))
axes[1,0].plot(timesteps, v_rec, linestyle="--")

# Apply fixed point scale and compare
axes[2,0].set_title("10 fractional bits, RTN")
axes[2,0].errorbar(timesteps, data_10_rn_average / (2 ** 10), yerr=data_10_rn_std / (2 ** 10))
axes[2,0].plot(timesteps, v_rec, linestyle="--")

# Apply fixed point scale and compare
axes[3,0].set_title("10 fractional bits, RTS")
axes[3,0].errorbar(timesteps, data_10_rs_average / (2 ** 10), yerr=data_10_rs_std / (2 ** 10))
axes[3,0].plot(timesteps, v_rec, linestyle="--")


plt.show()
