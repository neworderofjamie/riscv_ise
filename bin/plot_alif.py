import matplotlib.gridspec as gs
import matplotlib.pyplot as plt
import numpy as np
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


def plot_trace(axis, timesteps, v_rec, a_rec, pal, linestyle, v_ylim=None, a_ylim=None):
    a_axis  = axis.twinx()

    # Plot floating point reference
    axis.plot(timesteps, v_rec, linestyle=linestyle, color=pal[0], linewidth=0.5)
    a_axis.plot(timesteps, a_rec, linestyle=linestyle, color=pal[1], linewidth=0.5)

    if v_ylim is not None:
        axis.set_ylim(v_ylim)

    if a_ylim is not None:
        a_axis.set_ylim(a_ylim)

    sns.despine(ax=axis)
    
    axis.xaxis.grid(False)
    axis.yaxis.grid(False)
    a_axis.yaxis.grid(False)

    return a_axis

def plot_fixed_point(axis, timesteps, v_rec, a_rec, data_average, data_std, 
                     num_v_frac_bits, num_a_frac_bits, pal, title_prefix, 
                     twin_share=None, v_ylim=None, a_ylim=None, x_lim=None):
    v_scale = 1.0 / (2 ** num_v_frac_bits)
    a_scale = 1.0 / (2 ** num_a_frac_bits)
    a_axis  = plot_trace(axis, timesteps, v_rec, a_rec, pal, "--", v_ylim, a_ylim)
    
    # Plot voltage and standard deviation from hardware
    axis.plot(timesteps, data_average[:,0] * v_scale, color=pal[0], linewidth=0.5)
    axis.fill_between(timesteps, (data_average[:,0] - data_std[:,0]) * v_scale, 
                      (data_average[:,0] + data_std[:,0]) * v_scale,
                      alpha=0.5, color=pal[0])

    a_axis.plot(timesteps, data_average[:,1] * a_scale, color=pal[1], linewidth=0.5)
    a_axis.fill_between(timesteps, (data_average[:,1] - data_std[:,1]) * a_scale, 
                       (data_average[:,1] + data_std[:,1]) * a_scale,
                       alpha=0.5, color=pal[1])
    
    # Calculate RMSE
    v_rmse = calc_nrmse(v_rec, data_average[:,0] * v_scale)
    a_rmse = calc_nrmse(a_rec, data_average[:,1] * a_scale)
    print(f"{title_prefix} (V RMSE={v_rmse}, A RMSE={a_rmse})")
    
    if twin_share is not None:
        a_axis.sharey(twin_share)

    if x_lim is not None:
        axis.set_xlim(x_lim)

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

# Create figure
fig = plt.figure(figsize=(plot_settings.double_column_width, 2.0), frameon=False)

# Create outer gridspec dividing plot area into 2 rows
gs_two_row = gs.GridSpec(2, 1, height_ratios=[1 / 3, 2 / 3])

# Create sub-grid spec with two columns for example overviews
gs_overview = gs.GridSpecFromSubplotSpec(1, 2, subplot_spec=gs_two_row[0,0], wspace=0.15)
example_1_overview_axis = plt.Subplot(fig, gs_overview[0,0])
example_2_overview_axis = plt.Subplot(fig, gs_overview[0,1])
fig.add_subplot(example_1_overview_axis)
fig.add_subplot(example_2_overview_axis)

# Create sub-grid specs for four zoom ins for each example
gs_zoom_in = gs.GridSpecFromSubplotSpec(2, 3, subplot_spec=gs_two_row[1,0], hspace=0.1, wspace=0.25)

# Add axes
example_1_t0_rz = plt.Subplot(fig, gs_zoom_in[0,0])
example_1_t0_rs = plt.Subplot(fig, gs_zoom_in[1,0], sharex=example_1_t0_rz)
example_1_t1_rz = plt.Subplot(fig, gs_zoom_in[0,1])
example_1_t1_rs = plt.Subplot(fig, gs_zoom_in[1,1], sharex=example_1_t1_rz)
example_2_t0 = plt.Subplot(fig, gs_zoom_in[0,2])
example_2_t0_sat = plt.Subplot(fig, gs_zoom_in[1,2], sharex=example_2_t0)

fig.add_subplot(example_1_t0_rz)
fig.add_subplot(example_1_t0_rs)
fig.add_subplot(example_1_t1_rz)
fig.add_subplot(example_1_t1_rs)
fig.add_subplot(example_2_t0)
fig.add_subplot(example_2_t0_sat)
fig.add_subplot(example_2_t0)
fig.add_subplot(example_2_t0_sat)

pal = sns.color_palette()

timesteps = np.arange(len(v_rec))

# Plot floating point references
#example_1_overview_axis.set_title("A", loc="left")
#example_2_overview_axis.set_title("B", loc="left")
plot_trace(example_1_overview_axis, timesteps, v_rec, a_rec, pal, "-")
example_1_overview_axis_a = plot_trace(example_2_overview_axis, timesteps, v_bad_rec, a_bad_rec, pal, "-")

# Plot two zoom ins of example 1
example_1_t0_xlim = (1000, 1250)
example_1_t1_xlim = (4000, 4250)
#example_1_t0_rz.set_title("C", loc="left")
#example_1_t1_rz.set_title("D", loc="left")
#example_1_t0_rs.set_title("F", loc="left")
#example_1_t1_rs.set_title("G", loc="left")
plot_fixed_point(example_1_t0_rz, timesteps, v_rec, a_rec, data_12_8_average, data_12_8_std, 
                 V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RZ", x_lim=example_1_t0_xlim)
plot_fixed_point(example_1_t0_rs, timesteps, v_rec, a_rec, data_12_8_rs_average, data_12_8_rs_std, 
                 V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RZ", x_lim=example_1_t0_xlim)
plot_fixed_point(example_1_t1_rz, timesteps, v_rec, a_rec, data_12_8_average, data_12_8_std, 
                 V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RZ", x_lim=example_1_t1_xlim)
plot_fixed_point(example_1_t1_rs, timesteps, v_rec, a_rec, data_12_8_rs_average, data_12_8_rs_std, 
                 V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RZ", x_lim=example_1_t1_xlim)

# Plot zoom in of example 2
example_2_t1_xlim = (4250, 4500)
#example_2_t0.set_title("E", loc="left")
#example_2_t0_sat.set_title("H", loc="left")
example_2_t0_a = plot_fixed_point(example_2_t0, timesteps, v_bad_rec, a_bad_rec, data_bad_12_8_rs_average, data_bad_12_8_rs_std, 
                                  V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RZ", x_lim=example_2_t1_xlim)
example_2_t0_sat_a = plot_fixed_point(example_2_t0_sat, timesteps, v_bad_rec, a_bad_rec, data_bad_12_8_sat_rs_average, data_bad_12_8_sat_rs_std, 
                                      V_FP, A_FP, pal, f"{V_FP}, {A_FP} fractional bits, RZ", x_lim=example_2_t1_xlim)

example_1_t0_rs.set_xlabel("Time [ms]")
example_1_t1_rs.set_xlabel("Time [ms]")
example_2_t0_sat.set_xlabel("Time [ms]")
example_1_overview_axis.set_ylabel("V")
example_1_t0_rz.set_ylabel("V")
example_1_t0_rs.set_ylabel("V")
example_1_overview_axis_a.set_ylabel("A")
example_2_t0_a.set_ylabel("A")
example_2_t0_sat_a.set_ylabel("A")

plt.setp(example_1_t0_rz.get_xticklabels(), visible=False)
plt.setp(example_1_t1_rz.get_xticklabels(), visible=False)
plt.setp(example_2_t0.get_xticklabels(), visible=False)

# Highlight areas
example_1_overview_axis.axvspan(example_1_t0_xlim[0], example_1_t0_xlim[1], alpha=0.5, color=pal[2])
example_1_overview_axis.axvspan(example_1_t1_xlim[0], example_1_t1_xlim[1], alpha=0.5, color=pal[3])
example_2_overview_axis.axvspan(example_2_t1_xlim[0], example_2_t1_xlim[1], alpha=0.5, color=pal[4])

fig.tight_layout(pad=0)
fig.savefig("alif_rounding.pdf")
plt.show()
