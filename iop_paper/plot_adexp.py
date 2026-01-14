import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import plot_settings

from adexp_params import param_sets

genn_data = np.load("adexp_genn.npz")
fenn_data = np.load("adexp_fenn.npz")

fig, axes = plt.subplots(2, 3, sharex="col", sharey="row", figsize=(plot_settings.double_column_width, 3.0))

timesteps = np.arange(0.0, 2000 * 0.1, 0.1)

order = ["tonic", "adaptation", "initial burst",
         "transient spiking", "regular bursting", "delayed accelerating"]
    
fixed_point_one = 2 ** 11
for i, (a, o) in enumerate(zip(axes.flatten(), order)):
    letter = chr(ord("A") + i)
    print(f"{letter} = {o}")
    
    a.set_title(letter, loc="left")
    
    fenn_rn_actor = a.plot(timesteps, fenn_data[f"{o}_v_rn"][:,1] / (fixed_point_one * 0.01))[0]
    fenn_rs_actor = a.plot(timesteps, fenn_data[f"{o}_v_rs"][:,1] / (fixed_point_one * 0.01))[0]
    genn_actor = a.plot(timesteps, genn_data[f"{o}_v"][:,0], color="gray", linestyle="--")[0]
    
    a.xaxis.grid(False)
    sns.despine(ax=a)

for i in range(axes.shape[0]):
    axes[i,0].set_ylabel("V [mV]")
for j in range(axes.shape[1]):
    axes[-1, j].set_xlabel("Time [ms]")

axes[0,0].set_xlim((0, 100))
fig.legend([fenn_rn_actor, fenn_rs_actor, genn_actor], ["FeNN (RN)", "FeNN (RS)", "GeNN reference"], 
           ncol=3, loc="lower center", frameon=False)

fig.tight_layout(pad=0, rect=[0.0, 0.1, 1.0, 1.0])
fig.savefig("adexp.pdf")
plt.show()
