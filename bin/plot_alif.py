import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

import plot_settings

def load_data(filename, desired_length):
    data = np.loadtxt(filename, delimiter=",")
    assert data.shape[0] == desired_length
    data = np.reshape(data, (data.shape[0], -1, 2))
    return (np.average(data, axis=1), 
            np.std(data, axis=1))


def calc_rmse(target, result):
    scale = np.amax(target) - np.amin(target)
    return np.sqrt(np.mean((result - target)**2)) / scale

def simulate_ref(filename):
    TAU_A = 2000.0
    TAU_M = 20.0
    ALPHA = np.exp(-1 / TAU_M)
    RHO = np.exp(-1 / TAU_A)
    WEIGHT = 0.01
    BETA = 0.0174
    V_THRESH = 0.6
    
    # Load poisson input
    poisson = np.fromfile(filename, dtype=np.int16)

    # Throw away copies of poisson and check shape matches
    poisson = np.reshape(poisson, (-1, 32))
    poisson = poisson[:,0]

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
    
    return v_rec, a_rec


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
    
    #axis.set_ylim((0, 1.5))
    #a_axis.set_ylim((0, 40.0))


V_FP = 14
A_FP = 9

# Load input and generate reference simulation
v_rec, a_rec = simulate_ref("poisson_data.bin")
v_bad_rec, a_bad_rec = simulate_ref("poisson_data_bad.bin")
assert len(v_rec) == len(v_bad_rec)


# Load data
data_12_8_average, data_12_8_std = load_data(f"out_alif_{V_FP}_{A_FP}.txt", len(v_rec))
data_12_8_rs_average, data_12_8_rs_std = load_data(f"out_alif_{V_FP}_{A_FP}_rs.txt", len(v_rec))
data_bad_12_8_rs_average, data_bad_12_8_rs_std = load_data(f"out_alif_bad_{V_FP}_{A_FP}_rs.txt", len(v_rec))
data_bad_12_8_sat_rs_average, data_bad_12_8_sat_rs_std = load_data(f"out_alif_bad_{V_FP}_{A_FP}_sat_rs.txt", len(v_rec))


fig, axes = plt.subplots(2, 2, sharex="col", sharey="row", squeeze=False)
pal = sns.color_palette()

timesteps = np.arange(len(v_rec))

plot_fixed_point(axes[0,0], timesteps, v_rec, a_rec, data_12_8_average, data_12_8_std, 
                 V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RZ")

plot_fixed_point(axes[1,0], timesteps, v_rec, a_rec, data_12_8_rs_average, data_12_8_rs_std, 
                 V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RS")

plot_fixed_point(axes[0,1], timesteps, v_bad_rec, a_bad_rec, data_bad_12_8_rs_average, data_bad_12_8_rs_std, 
                 V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RS")

plot_fixed_point(axes[1,1], timesteps, v_bad_rec, a_bad_rec, data_bad_12_8_sat_rs_average, data_bad_12_8_sat_rs_std, 
                 V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RS + Saturation")


axes[1,0].set_xlabel("Time [ms]")

plt.show()
