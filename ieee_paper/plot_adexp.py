import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import plot_settings

from adexp_params import param_sets

genn_data = np.load("adexp_genn.npz")
fenn_data = np.load("adexp_fenn.npz")

fig, axes = plt.subplots(2, 3, sharex="col", sharey="row", figsize=(plot_settings.double_column_width, 3.0))

timesteps = np.arange(0.0, 2000 * 0.1, 0.1)

fixed_point_one = 2 ** 11
for i, (a, n) in enumerate(zip(axes.flatten(), param_sets.keys())):
    letter = chr(ord("A") + i)
    print(f"{letter} = {n}")
    
    a.set_title(letter, loc="left")
    
    genn_actor = a.plot(timesteps, fenn_data[f"{n}_v"][:,1] / (fixed_point_one * 0.01))[0]
    fenn_actor = a.plot(timesteps, genn_data[f"{n}_v"][:,0], linestyle="--", color=genn_actor.get_color())[0]
    
    a.xaxis.grid(False)
    sns.despine(ax=a)

for i in range(axes.shape[0]):
    axes[i,0].set_ylabel("V [mV]")
for j in range(axes.shape[1]):
    axes[-1, j].set_xlabel("Time [ms]")

fig.legend([fenn_actor, genn_actor], ["FeNN", "GeNN reference"], 
           ncol=2, loc="lower center", frameon=False)

fig.tight_layout(pad=0, rect=[0.0, 0.1, 1.0, 1.0])

plt.show()
