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


def calc_nrmse(target, result):
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
                     num_v_frac_bits, num_a_frac_bits, pal, title_prefix, 
                     twin_share=None, v_ylim=None, a_ylim=None):
    v_scale = 1.0 / (2 ** num_v_frac_bits)
    a_scale = 1.0 / (2 ** num_a_frac_bits)
    a_axis  = axis.twinx()
    
    # Plot voltage and standard deviation from hardware
    axis.plot(timesteps, data_average[:,0] * v_scale, color=pal[0], linewidth=0.5)
    axis.fill_between(timesteps, (data_average[:,0] - data_std[:,0]) * v_scale, 
                      (data_average[:,0] + data_std[:,0]) * v_scale,
                      alpha=0.5, color=pal[0])

    a_axis.plot(timesteps, data_average[:,1] * a_scale, color=pal[1], linewidth=0.5)
    a_axis.fill_between(timesteps, (data_average[:,1] - data_std[:,1]) * a_scale, 
                       (data_average[:,1] + data_std[:,1]) * a_scale,
                       alpha=0.5, color=pal[1])
    
    # Plot floating point reference
    axis.plot(timesteps, v_rec, linestyle="--", color=pal[0], linewidth=0.5)
    a_axis.plot(timesteps, a_rec, linestyle="--", color=pal[1], linewidth=0.5)

    # Calculate RMSE
    v_rmse = calc_nrmse(v_rec, data_average[:,0] * v_scale)
    a_rmse = calc_nrmse(a_rec, data_average[:,1] * a_scale)
    print(f"{title_prefix} (V RMSE={v_rmse}, A RMSE={a_rmse})")
    
    if v_ylim is not None:
        axis.set_ylim(v_ylim)

    if a_ylim is not None:
        a_axis.set_ylim(a_ylim)

    if twin_share is not None:
        a_axis.sharey(twin_share)

    return a_axis
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


fig, axes = plt.subplots(2, 2, sharex="col", sharey="row", squeeze=False, 
                         figsize=(plot_settings.double_column_width, 2.0))
pal = sns.color_palette()

timesteps = np.arange(len(v_rec))

print("Stochastic rounding:")
axes[0,0].set_title("A", loc="left")
a_00 = plot_fixed_point(axes[0,0], timesteps, v_rec, a_rec, data_12_8_average, data_12_8_std, 
                        V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RZ")

axes[0,1].set_title("B", loc="left")
a_01 = plot_fixed_point(axes[0,1], timesteps, v_rec, a_rec, data_12_8_rs_average, data_12_8_rs_std, 
                        V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RS",
                        twin_share=a_00)

print("Saturation:")
axes[1,0].set_title("C", loc="left")
a_10 = plot_fixed_point(axes[1,0], timesteps, v_bad_rec, a_bad_rec, data_bad_12_8_rs_average, data_bad_12_8_rs_std, 
                        V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RS")

axes[1,1].set_title("D", loc="left")
a_11 = plot_fixed_point(axes[1,1], timesteps, v_bad_rec, a_bad_rec, data_bad_12_8_sat_rs_average, data_bad_12_8_sat_rs_std, 
                        V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RS + Saturation",
                        twin_share=a_10)

# Hide crud in middle of axes
a_00.get_yaxis().set_visible(False)
a_10.get_yaxis().set_visible(False)

# Override limits to hide clipped region
axes[1,0].set_ylim((0, 2.0))
a_10.set_ylim((0, 75))

# Label axes
a_01.set_ylabel("A")
a_11.set_ylabel("A")
for i in range(2):
    axes[i, 0].set_ylabel("V")
    axes[1, i].set_xlabel("Time [ms]")

fig.tight_layout(pad=0)
fig.savefig("alif_rounding.pdf")
plt.show()
