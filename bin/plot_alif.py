import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

import plot_settings

def load_data(filename, desired_shape):
    data = np.loadtxt(filename, delimiter=",")
    assert desired_shape[0] == data.shape[0]
    data = np.reshape(data, (data.shape[0], -1, 2))
    return (np.average(data, axis=1), 
            np.std(data, axis=1))


def calc_rmse(target, result):
    return np.sqrt(np.mean((result - target)**2))

def plot_fixed_point(axis, timesteps, v_rec, a_rec, data_average, data_std, 
                     num_v_frac_bits, num_a_frac_bits, pal, title_prefix):
    v_scale = 1.0 / (2 ** num_v_frac_bits)
    a_scale = 1.0 / (2 ** num_a_frac_bits)
    a_axis  = axis.twinx()
    
    # Plot voltage and standard deviation from hardware
    axis.plot(timesteps, data_average[:,0] * v_scale, color=pal[0])
    axis.fill_between(timesteps, (data_average[:,0] - data_std[:,0]) * v_scale, 
                      (data_average[:,0] + data_std[:,0]) * v_scale,
                      alpha=0.5, color=pal[0])

    a_axis.plot(timesteps, data_average[:,1] * a_scale, color=pal[1])
    a_axis.fill_between(timesteps, (data_average[:,1] - data_std[:,1]) * a_scale, 
                       (data_average[:,1] + data_std[:,1]) * a_scale,
                       alpha=0.5, color=pal[1])
    
    # Plot floating point reference
    axis.plot(timesteps, v_rec, linestyle="--", color=pal[0])
    a_axis.plot(timesteps, a_rec, linestyle="--", color=pal[1])

    # Calculate RMSE
    v_rmse = calc_rmse(v_rec, data_average[:,0] * v_scale)
    a_rmse = calc_rmse(a_rec, data_average[:,1] * a_scale)
    
    axis.set_title(f"{title_prefix} (V RMSE={v_rmse}, A RMSE={a_rmse})")
    axis.set_ylabel("V")
    a_axis.set_ylabel("A")
    
    axis.set_ylim((0, 1.5))
    a_axis.set_ylim((0, 40.0))


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

data_9_9_average, data_9_9_std = load_data("out_alif_9_9.txt", poisson.shape)
data_12_9_average, data_12_9_std = load_data("out_alif_12_9.txt", poisson.shape)
data_12_9_rn_average, data_12_9_rn_std = load_data("out_alif_12_9_rn.txt", poisson.shape)
data_12_9_rs_average, data_12_9_rs_std = load_data("out_alif_12_9_rs.txt", poisson.shape)
#data_14_10_average, data_14_10_std = load_data("out_alif_14_10.txt", poisson.shape)
#data_14_10_sat_average, data_14_10_sat_std = load_data("out_alif_14_10_sat.txt", poisson.shape)

# Simulate
v = 0
a = 0
v_rec = []
a_rec = []
for p in poisson:
    v *= ALPHA
    a *= RHO
    v += (p * WEIGHT)

    if v >= (V_THRESH + (BETA * a)):
        v -= V_THRESH
        a += 1.0
    v_rec.append(v)
    a_rec.append(a)

timesteps = np.arange(len(poisson))

fig, axes = plt.subplots(4, sharex=True, squeeze=False)

pal = sns.color_palette()


plot_fixed_point(axes[0,0], timesteps, v_rec, a_rec, data_9_9_average, data_9_9_std, 
                 9, 9, pal, "9, 9 fractional bits, RZ")

plot_fixed_point(axes[1,0], timesteps, v_rec, a_rec, data_12_9_average, data_12_9_std, 
                 12, 9, pal, "12, 9 fractional bits, RZ")

plot_fixed_point(axes[2,0], timesteps, v_rec, a_rec, data_12_9_rn_average, data_12_9_rn_std, 
                 12, 9, pal, "12, 9 fractional bits, RN")
plot_fixed_point(axes[3,0], timesteps, v_rec, a_rec, data_12_9_rs_average, data_12_9_rs_std, 
                 12, 9, pal, "12, 9 fractional bits, RS")


# Apply fixed point scale and compare
#plot_fixed_point(axes[2,0], timesteps, v_rec, a_rec, data_10_rn_average, data_10_rn_std, 10, pal,
#                 "10 fractional bits, RTN")

# Apply fixed point scale and compare
#plot_fixed_point(axes[3,0], timesteps, v_rec, a_rec, data_10_rs_average, data_10_rs_std, 10, pal,
#                 "10 fractional bits, RS")
axes[3,0].set_xlabel("Time [ms]")

plt.show()
