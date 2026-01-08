import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import plot_settings

from matplotlib.ticker import ScalarFormatter

# Load data
fenn_data = np.load("exp_fenn.npz")

numpy_exp_x = np.exp(fenn_data["x"])

lsb = 2 ** -9
error = np.abs(numpy_exp_x - fenn_data["exp_x"])
monotonic = np.all(np.diff(fenn_data["exp_x"]) >= 0)
print(f"{100.0 * (np.sum(error < lsb) / len(error))}% differences less than LSB")
print(f"Approximation is monotonic: {monotonic}")

fig, axis = plt.subplots(figsize=(plot_settings.column_width, 3.0))
approx_actor = axis.plot(fenn_data["x"], fenn_data["exp_x"])[0]
numpy_actor = axis.plot(fenn_data["x"], numpy_exp_x)[0]

inset_axis = axis.inset_axes([0.2, 1.0 - 0.4, 0.4, 0.4])
inset_axis.plot(fenn_data["x"], fenn_data["exp_x"])[0]
inset_axis.plot(fenn_data["x"], numpy_exp_x)[0]

axis.set_xlabel("x")
axis.set_ylabel("exp(x)")

inset_axis.set_xlim((3.999, 4))
inset_axis.set_ylim((54.525, np.amax(numpy_exp_x)))
inset_axis.set_xticks([3.999, 4])
inset_axis.xaxis.set_major_formatter(ScalarFormatter(useOffset=False))
for a in [axis, inset_axis]:
    a.xaxis.grid(False)
    a.yaxis.grid(False)
    sns.despine(ax=a)

fig.legend([approx_actor, numpy_actor], ["FeNN", "Numpy"], 
           ncol=3, loc="lower center", frameon=False)

fig.tight_layout(pad=0, rect=[0.0, 0.08, 1.0, 1.0])
fig.savefig("exp.pdf")
plt.show()
