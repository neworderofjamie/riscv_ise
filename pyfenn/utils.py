import numpy as np

from pyfenn import Runtime

def get_array_view(runtime: Runtime, state, dtype):
    array = runtime.get_array(state)
    view = np.asarray(array.host_view).view(dtype)
    assert not view.flags["OWNDATA"]
    return array, view

# Divide two integers, rounding up i.e. effectively taking ceil
def ceil_divide(numerator, denominator):
    return (numerator + denominator - 1) // denominator

def load_and_push(filename: str, state, runtime: Runtime):
    # Load data from file
    data = np.fromfile(filename, dtype=np.uint8)

    # Get array and view
    array, view = get_array_view(runtime, state, np.uint8)
    assert array.host_view.nbytes == data.nbytes

    # Copy data to array host pointer
    view[:] = data

    # Push to device
    array.push_to_device()

def zero_and_push(state, runtime: Runtime):
    # Get array and view
    array, view = get_array_view(runtime, state, np.uint8)

    # Zero
    # **HACK** assigning to the slice causes bus errors with DMA buffer
    for i in range(len(view)):
        view[i] = 0
    #view[:] = 0

    # Push to device
    array.push_to_device()

def read_perf_counter(perf_counter, runtime: Runtime):
    # Get array and view
    array, view = get_array_view(runtime, perf_counter, np.uint64)

    # Pull
    array.pull_from_device()
    
    return view[0], view[1]

def seed_and_push(state, runtime: Runtime):
    # Get array and view
    array, view = get_array_view(runtime, state, np.int16)

    # Zero
    int16_info = np.iinfo(np.int16)
    view[:] = np.random.randint(int16_info.min, int16_info.max, 64, dtype=np.int16)

    # Push to device
    array.push_to_device()

def get_latency_spikes(images, tau=20.0, num_timesteps=79, threshold=51):
    # Flatten images and convert intensity to time
    images = np.reshape(images, (images.shape[0], -1))

    padded_size = int(np.ceil(images.shape[1] / 32)) * 32

    spikes = []
    for i in images:
        times = np.round(tau * np.log(i / (i - threshold))).astype(int)
        times_in_range = (i > threshold) & (times < num_timesteps)

        spike_event_histogram = np.zeros((num_timesteps, images.shape[1]), dtype=bool)
        spike_event_histogram[times[times_in_range], times_in_range] = 1
        spike_event_histogram = np.pad(spike_event_histogram, ((0, 0), (0, (padded_size - images.shape[1]))))
        spikes.append(np.packbits(spike_event_histogram, axis=1, bitorder="little").flatten())

    # Stack spikes and view as uint32
    return np.stack(spikes).view(np.uint32)
