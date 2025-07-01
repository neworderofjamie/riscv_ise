import numpy as np

from numbers import Number
from pyfenn import Runtime
from typing import Sequence, Union

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

def copy_and_push(data: np.ndarray, state, runtime: Runtime):
    # Get array and view
    array, view = get_array_view(runtime, state, data.dtype)

    # Copy data to array host pointer
    view[:] = data

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


def build_sparse_connectivity(row_ind: Sequence[np.ndarray], weight: Number,
                              sparse_connectivity_bits: int) -> np.ndarray:
    num_pre = len(row_ind)

    # Determine which lane each postsynaptic index belongs in
    row_lane = [r % 32 for r in row_ind]

    # Sort rows of indices by their lane
    row_ind_sorted = [i[np.argsort(l)] for i, l in zip(row_ind, row_lane)]

    # Count how many connections each lane needs to process in each row
    row_conns_per_lane = [np.bincount(l) for l in row_lane]

    # Determine maximum number of vectors
    num_vectors = max(np.amax(c) for c in row_conns_per_lane)

    # Calculate cumulative sum of bin count to determine where to split per-bank
    row_conn_lane_sections = [np.cumsum(c) for c in row_conns_per_lane]

    # Convert row indices into addresses
    row_data_sorted = [((r // 32) * 2).astype(np.int16) for r in row_ind_sorted]

    # Check largest address fits without sparse connectivity bits
    max_address = max(np.amax(r) for r in row_data_sorted)
    if max_address >= 2**sparse_connectivity_bits:
        raise RuntimeError("Not enough bits to represent connectivity")

    # Check weight fits within remaining bits
    weight_bits = 15 - sparse_connectivity_bits
    max_weight = (2**weight_bits) - 1
    min_weight = -max_weight - 1
    if weight < min_weight or weight > max_weight:
        raise RuntimeError("Not enough bits for weight")

    # Combine weight and indices
    row_data_sorted = [r | (weight << sparse_connectivity_bits)
                       for r in row_data_sorted]

    padded_rows = []
    for d, s in zip(row_data_sorted, row_conn_lane_sections):
        # Split, pad list of connections with  
        # **NOTE** we only care about which L.L.M. address of target in bytes
        conn_id_banked = np.transpose(np.vstack([np.pad(a, (0, num_vectors - len(a)), 
                                                        constant_values=-2)
                                                for a in np.split(d, s[:-1])]))
        conn_id_banked = np.pad(conn_id_banked, ((0, 0), (0, 32 - conn_id_banked.shape[1])),
                                constant_values=-2)
        
        padded_rows.append(np.reshape(conn_id_banked, 32 * num_vectors))
    
    return np.vstack(padded_rows)

def generate_fixed_prob(num_pre: int, num_post: int,
                        prob: float) -> Sequence[np.ndarray]:
    # Loop through presynaptic neurons
    rows = []
    for i in range(num_pre):
        # Make num_post bernoulli trials
        row_mask = np.random.choice([True, False], num_post,
                                    p=[prob, 1.0 - prob])
        # Add indices of 1s to row
        rows.append(np.where(row_mask)[0])
    return rows
