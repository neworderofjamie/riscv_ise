import numpy as np
from tonic.datasets import SHD

dataset = SHD(save_to="data", train=False)

# Loop through dataset
spikes = []
labels = []
timestep_range = np.arange(1171)
padded_size = int(np.ceil(700 / 32)) * 32
neuron_range = np.arange(padded_size + 1)
for events, label in dataset:
    # Build histogram
    spike_event_histogram = np.histogram2d(events["t"] / 1000.0, events["x"], (timestep_range, neuron_range))[0]
    spike_event_histogram = np.minimum(spike_event_histogram, 1).astype(bool)
    spikes.append(np.packbits(spike_event_histogram, axis=1, bitorder="little"))
    labels.append(label)

# Stack spikes together and convert labels to numpy
spikes = np.stack(spikes)
labels = np.asarray(labels, dtype=np.int16)

# Write to binary files
spikes.tofile("shd_spikes.bin")
labels.tofile("shd_labels.bin")
