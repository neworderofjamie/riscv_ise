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
    # Calculate log-latency from intensity
    times = np.round(20.0 * np.log(t / (t - 51))).astype(int)
    
    # Get IDs of neurons which should spike
    neuron_ids = np.where((t > 51) & (times < 79))[0]
    
    # Order by time
    times = times[neuron_ids]
    order = np.argsort(times)
    times = times[order]
    neuron_ids = neuron_ids[order]
    
    # Count number of spikes in each timestep and use this to split neuron ids
    num_spikes_per_time = np.cumsum(np.bincount(times))
    neuron_ids_per_time = np.split(neuron_ids, num_spikes_per_time)
    
    # Concatenate timestamps onto each non-empty group of neuron ids
    neuron_ids_per_time = [np.concatenate(((t | 1 << 15,), n))
                           for t, n in enumerate(neuron_ids_per_time)
                           if len(n) > 0]

    # Rejoin into single array
    neuron_ids_per_time = np.concatenate(neuron_ids_per_time)
    
    spikes.append(neuron_ids_per_time)

# Calculate maximum spikes per-image
max_spikes_per_image = max(len(s) for s in spikes)
print(f"Maximum spikes per-image = {max_spikes_per_image}")

# Pad all spike arrays to this length with uint16_max
spikes = [np.pad(s, (0, max_spikes_per_image - len(s)), constant_values=0xFFFF) for s in spikes]

# Concatenate and write to file
spikes = np.concatenate(spikes).astype(np.uint16)
spikes.tofile("mnist_spikes.bin")

test_labels_int = np.round(test_labels).astype(np.int16)
test_labels_int.tofile("mnist_labels.bin")