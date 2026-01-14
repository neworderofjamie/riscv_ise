
import matplotlib.pyplot as plt
import numpy as np
import os
import seaborn as sns
import torch

from matplotlib.patches import Rectangle

import plot_settings

test_spikes, _ = torch.load(os.path.join(os.pardir, "examples", "ds_test.pt"),
                            weights_only=False).tensors
example_test_spikes = np.where(test_spikes[0])

fig, axes = plt.subplots(1, 2, figsize=(plot_settings.double_column_width, 3.0))

axes[0].scatter(example_test_spikes[0], example_test_spikes[1], s=1)
axes[0].set_xlabel("Timestep")
axes[0].set_ylabel("Neuron")
axes[0].set_title("A", loc="left")

pal = sns.color_palette()

platform = ["Nengo", "Norse", "Rockpool", "snnTorch", "Spyx", "SpiNNaker2", "Xylo", "FeNN"]
accuracy = [78.6, 93.57, 71.4, 92.1, 92.12, 93.57, 85.71, 92.86]
colours = ([pal[0]] * 4) + ([pal[1]] * 3) + [pal[2]]

bar_y = np.arange(len(platform))
axes[1].barh(bar_y, accuracy, color=colours)
axes[1].set_xticks(np.linspace(0, 100, 5))
axes[1].set_yticks(bar_y, platform)
axes[1].set_ylabel("Platform")
axes[1].set_xlabel("Test accuracy [%]")
axes[1].set_title("B", loc="left")

# Remove junk
axes[0].xaxis.grid(False)
axes[0].yaxis.grid(False)
axes[1].yaxis.grid(False)
sns.despine(ax=axes[0])
sns.despine(ax=axes[1])

fig.legend([Rectangle(color=pal[i], width=10, height=10, xy=(0,0)) 
            for i in range(3)], ["Software", "Hardware", "Ours"],
            ncol=3, loc="lower center", frameon=False, bbox_to_anchor=(0.75, 0.0))
fig.tight_layout(pad=0, rect=[0.0, 0.08, 1.0, 1.0])
fig.savefig("nir.pdf")
plt.show()
