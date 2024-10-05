import numpy as np
import mnist

NUM_TIMESTEPS = 79

mnist.datasets_url = "https://storage.googleapis.com/cvdf-datasets/mnist/"
test_labels = mnist.test_labels()
test_images = mnist.test_images()

# Flatten images and convert intensity to time
test_images = np.reshape(test_images, (test_images.shape[0], -1))

padded_size = int(np.ceil(test_images.shape[1] / 32)) * 32

spikes = []
for t in test_images:
    times = np.round(20.0 * np.log(t / (t - 51))).astype(int)
    times_in_range = (t > 51) & (times < 79)

    spike_event_histogram = np.zeros((NUM_TIMESTEPS, test_images.shape[1]), dtype=bool)
    spike_event_histogram[times[times_in_range], times_in_range] = 1
    spike_event_histogram = np.pad(spike_event_histogram, ((0, 0), (0, (padded_size - test_images.shape[1]))))
    spikes.append(np.packbits(spike_event_histogram, axis=1, bitorder="little"))

spikes = np.stack(spikes)
spikes.tofile("mnist_spikes.bin")
#test_times_int = np.round(test_times).astype(np.int16)
#test_times_int.tofile("mnist_times.bin")

test_labels_int = np.round(test_labels).astype(np.int16)
test_labels_int.tofile("mnist_labels.bin")