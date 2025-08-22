import matplotlib.pyplot as plt
import numpy as np

from pygenn import GeNNModel

from pygenn import create_neuron_model

def dv(v: str, w: str):
    return f"tauMRecip * (-(({v}) - eL) + (deltaT * exp((({v}) - vThresh) * deltaTRecip)) + R * (i - ({w})))"

def dw(v: str, w: str):
    return f"tauWRecip * ((a * ({v} - eL)) - {w})"

adexp_model = create_neuron_model(
    "adexp",
    params=["tauMRecip",    # Reciprocal of membrane capacitance [pF]
            "R",            # Leak conductance [nS]
            "eL",           # Leak reversal potential [mV]
            "deltaT",       # Slope factor [mV]
            "deltaTRecip",  # Reciprocal of slope factor
            "vThresh",      # Threshold voltage [mV]
            "vSpike",       # Artificial spike height [mV]
            "vReset",       # Reset voltage [mV]
            "tauWRecip",    # Reciprocal of adaption time constant
            "a",            # Subthreshold adaption [nS]
            "b",            # Spike-triggered adaptation [nA]
            "iOffset"],     # Offset current
    vars=[("V", "scalar"), ("W", "scalar"),("v1", "scalar"), 
          ("w1", "scalar"), ("v2", "scalar"), ("w2", "scalar"), 
          ("v3", "scalar"), ("w3", "scalar"), ("v4", "scalar"), 
          ("w4", "scalar")],
    
    sim_code=
        f"""
        const scalar i = Isyn + iOffset;
        // If voltage is above artificial spike height
        if(V >= vSpike) {{
           V = vReset;
        }}
        // Calculate RK4 terms
        v1 = {dv('V', 'W')};
        w1 = {dw('V', 'W')};
        const scalar halfDT = dt * 0.5;
        scalar tmpV = V + (halfDT * v1);
        scalar tmpW = W + (halfDT * w1);
        v2 = {dv('tmpV', 'tmpW')};
        w2 = {dw('tmpV', 'tmpW')};
        tmpV = V + (halfDT * v2);
        tmpW = W + (halfDT * w2);
        v3 = {dv('tmpV', 'tmpW')};
        w3 = {dw('tmpV', 'tmpW')};
        tmpV = V + (dt * v3);
        tmpW = W + (dt * w3);
        v4 = {dv('tmpV', 'tmpW')};
        w4 = {dw('tmpV', 'tmpW')};
        // Update V
        const scalar sixthDT = dt * 0.166666667;
        V += sixthDT * (v1 + (2.0 * (v2 + v3)) + v4);
        // If we're not above peak, update w
        // **NOTE** it's not safe to do this at peak as wn may well be huge
        if(V <= -0.4) {{
           W += sixthDT * (w1 + (2.0 * (w2 + w3)) + w4);
        }}
        """,

    threshold_condition_code="V > -0.4",

    reset_code=
        """
        // **NOTE** we reset v to arbitrary plotting peak rather than to actual reset voltage
        V = vSpike;
        W += b;
        """)

c = 281.0 / 1000.0
gL = 30.0 / 1000.0
v_scale = 0.01
w_scale = 10.0
num_timesteps = 10000

# Parameters
adexp_params = {
    "R":            (1.0 / gL) * (v_scale / w_scale),
    "tauMRecip":    gL / c,
    "eL":           -70.6 * v_scale,
    "deltaT":       2.0 * v_scale,
    "deltaTRecip":  1.0 / (2.0 * v_scale),
    "vThresh":      -50.4 * v_scale,
    "vSpike":       10.0 * v_scale,
    "vReset":       -70.6 * v_scale,
    "tauWRecip":    1.0 / 144.0,
    "a":            (4.0 / 1000.0) / (v_scale / w_scale) ,
    "b":            0.0805 * w_scale,
    "iOffset":      700.0 * (w_scale / 1000.0)}
adexp_vars = {"V": -70.6 * v_scale, "W": 0.0, "v1": 0.0, "w1": 0.0, "v2": 0.0, "w2": 0.0, "v3": 0.0, "w3": 0.0, "v4": 0.0, "w4": 0.0}

print(adexp_params)



# Create model
model = GeNNModel("double", "adexp", backend="single_threaded_cpu")
model.dt = 0.1

# Add facilitation population with 1 spike source which fires a single spike in first timestep
adexp_pop = model.add_neuron_population("AdExp", 1, adexp_model, 
                                        adexp_params, adexp_vars)
adexp_pop.spike_recording_enabled = True

# Build and load model
model.build()
model.load(num_recording_timesteps=num_timesteps)

# Simulate, recording V and Trigger every timestep
adexp_v = []
adexp_w = []
adexp_v_grad = [[], [], [], []]
adexp_w_grad = [[], [], [], []]
while model.timestep < num_timesteps:
    model.step_time()
    adexp_pop.vars["V"].pull_from_device()
    adexp_pop.vars["W"].pull_from_device()
    adexp_v.append(adexp_pop.vars["V"].values)
    adexp_w.append(adexp_pop.vars["W"].values)
    
    for i in range(4):
        adexp_pop.vars[f"v{i + 1}"].pull_from_device()
        adexp_pop.vars[f"w{i + 1}"].pull_from_device()
        
        adexp_v_grad[i].append(adexp_pop.vars[f"v{i + 1}"].values)
        adexp_w_grad[i].append(adexp_pop.vars[f"w{i + 1}"].values)

model.pull_recording_buffers_from_device()
times, _ = adexp_pop.spike_recording_data[0]

# Stack recordings together
adexp_v = np.vstack(adexp_v)
adexp_w = np.vstack(adexp_w)

np.save("adexp_v_ref.npy", adexp_v)
np.save("adexp_w_ref.npy", adexp_w)
np.save("adexp_spike_times_ref.npy", times)

for i in range(4):
    adexp_v_grad[i] = np.vstack(adexp_v_grad[i])
    adexp_w_grad[i] = np.vstack(adexp_w_grad[i])

# Create plot
figure, axes = plt.subplots(5, sharex=True)

# Plot voltages
timesteps = np.arange(0, num_timesteps * 0.1, 0.1)
axes[0].set_title("Voltage")
axes[0].plot(timesteps, adexp_v)

axes[1].set_title("Adaption current")
axes[1].plot(timesteps, adexp_w)

axes[2].set_title("Voltage gradient")
axes[3].set_title("Adaption current gradient")
for i in range(4):
    axes[2].plot(timesteps, adexp_v_grad[i])
    axes[3].plot(timesteps, adexp_w_grad[i])

axes[4].set_title("Spikes")
axes[4].vlines(times, 0, 1)
# Show plot
plt.show()


