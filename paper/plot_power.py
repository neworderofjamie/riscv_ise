import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import plot_settings


# CSV filename, 'idle' power, sim time
data = [("power_fenn.csv", 8.6, 8.7, 36.6),
        ("power_jetson_gpu.csv", 7.0, 8.0, 77.05),
        ("power_jetson_cpu.csv", 7.0, 7.9, 119.29)]


fig, axes = plt.subplots(len(data), figsize=(plot_settings.column_width, 2.25), sharex=True, squeeze=False)


#total_synaptic_events = 938037605 * 10

idle_actor = None
init_actor = None
sim_actor = None

# Loop through devices
for i, (d, a) in enumerate(zip(data, axes[:,0])):
    # Load trace
    trace = np.loadtxt(d[0], skiprows=1, delimiter=",",
                       dtype={"names": ("time", "power", ), "formats": (float, float)})

    # Filter out clearly erroneous values
    valid = (trace["power"] < (d[1] * 5.0))
    time = trace["time"][valid]
    power = trace["power"][valid]
    
    # Find points power trace first crossed idle - assume first and last are experiment start and end times
    exp_start_index = np.where(power > d[1])[0][0]
    exp_end_index = np.where(power > d[2])[0][-1]
    exp_start_time = time[exp_start_index]
    exp_end_time = time[exp_end_index]

    sim_start_time = exp_end_time - d[3]
    sim_start_index = np.argmax(time > sim_start_time)

    # Make all times relative to experiment start
    time -= exp_start_time

    # Set title to device
    a.set_title(chr(i + ord("A")), loc="left")

    # Initial idle
    idle_actor = a.fill_between(time[:exp_start_index],
                                power[:exp_start_index])


    # Initialisation
    init_actor = a.fill_between(time[exp_start_index - 1:sim_start_index],
                                power[exp_start_index - 1:sim_start_index])


    # Simulation
    sim_actor = a.fill_between(time[sim_start_index - 1:exp_end_index],
                               power[sim_start_index - 1:exp_end_index])

    # Final idle
    a.fill_between(time[exp_end_index - 1:],
                   power[exp_end_index - 1:],
                   color=idle_actor.get_facecolor())

    # Calculate mean idle power
    idle_power = np.average(np.hstack((power[:exp_start_index], power[exp_end_index:])))

    # Calculate energy to solution
    energy_to_solution = np.trapz(power[exp_start_index:exp_end_index],
                                  time[exp_start_index:exp_end_index])

    # **TODO** should we subtract idle power
    sim_energy = np.trapz(power[sim_start_index:exp_end_index],
                          time[sim_start_index:exp_end_index])
    #energy_per_synaptic_event = sim_energy/ float(total_synaptic_events)

    print("%s:" % (d[0]))
    print("\tIdle power = %fW" % (idle_power))
    print("\tEnergy to solution = %fJ = %fkWh" % (energy_to_solution, energy_to_solution / 3600000.0))
    print("\tSimulation energy = %fJ = %fkWh" % (sim_energy, sim_energy / 3600000.0))
    #print("\tEnergy per synaptic event = %fuJ" % (energy_per_synaptic_event * 1E6))

    a.axvline(0.0, color="black", linestyle="--", linewidth=1.0)
    a.axvline(exp_end_time - exp_start_time, color="black", linestyle="--", linewidth=1.0)
    a.set_ylabel("Power [W]")
    a.xaxis.grid(False)
    #hidden_raster_axis.yaxis.grid(False)
    sns.despine(ax=a)

axes[-1,0].set_xlabel("Simulation time [s]")

fig.legend([idle_actor, init_actor, sim_actor],
           ["Idle", "Initialisation", "Simulation"],
           loc="lower center", ncol=3, frameon=False)
fig.tight_layout(pad=0.0, rect=(0.0, 0.125, 1.0, 1.0))
fig.savefig("power.pdf")
plt.show()

