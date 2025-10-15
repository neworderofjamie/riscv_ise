import numpy as np

from numbers import Number
from pyfenn import Runtime
from typing import Sequence, Union

from platform import system

def is_kria() -> bool:
    # If we're on Linux (otherwise, uname is not available)
    if system() == "Linux":
        from os import uname
        return uname().release.endswith("xilinx-zynqmp")
    else:
        return False

def get_array_view(runtime: Runtime, state, dtype, shape=None):
    array = runtime.get_array(state)
    view = np.asarray(array.host_view).view(dtype)
    if shape is not None:
        view = np.reshape(view, shape)
    assert not view.flags["OWNDATA"]
    return array, view

# Divide two integers, rounding up i.e. effectively taking ceil
def ceil_divide(numerator, denominator):
    return (numerator + denominator - 1) // denominator

def quantise(data, fractional_bits: int, num_pre: int = None,
             pad_post: bool = False, percentile: float = 99.0):
    # Split data into positive and negative
    positive_mask = (data > 0)
    positive_data = data[positive_mask]
    negative_data = data[np.logical_not(positive_mask)]

    # Calculate desired percentile
    positive_perc = np.percentile(positive_data, percentile)
    negative_perc = np.percentile(-negative_data, percentile)

    # Calculate the largest of these and clip
    max_val = max(positive_perc, negative_perc)
    data = np.clip(data, -max_val, max_val)

    if pad_post:
        assert num_pre is not None
        
        data = np.reshape(data, (num_pre, -1))
        pad_num_post = ceil_divide(data.shape[1], 32) * 32
        data = np.pad(data, ((0, 0), (0, pad_num_post - data.shape[1])))  

    # Scale, round and convert to int16
    fp_one = 2.0 ** fractional_bits

    # Scale by fixed point, round, convert to int and flatten
    return np.round(data * fp_one).astype(np.int16).flatten()

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

def copy_and_push(data: np.ndarray, state, runtime: Runtime, shape=None):
    # Get array and view
    array, view = get_array_view(runtime, state, data.dtype, shape)

    # Copy data to array host pointer
    view[:] = data

    # Push to device
    array.push_to_device()

def load_and_push(filename: str, state, runtime: Runtime):
    copy_and_push(np.fromfile(filename, dtype=np.uint8), state, runtime)

def load_quantise_and_push(filename: str, fractional_bits: int,
                           state, runtime: Runtime, num_pre: int = None,
                           pad_post: bool = False, percentile: float = 99.0):
    # Load data from file and quantise
    data = quantise(np.load(filename), fractional_bits, 
                    num_pre, pad_post, percentile)

    # Copy and push
    copy_and_push(data, state, runtime)

def pull_spikes(num_timesteps: int, state, runtime: Runtime):
    spike_array, spike_view = get_array_view(runtime,
                                             state,
                                             np.uint8)
    spike_array.pull_from_device()
    spike_view = np.reshape(spike_view, (num_timesteps, -1))

    return np.where(np.unpackbits(spike_view, axis=1, bitorder="little"))

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

def generate_exp_lut_and_push(state, runtime: Runtime):
    array, view = get_array_view(runtime, state, np.int16)
    
    num_bits = 15
    table_bits = (num_bits - 3) // 2
    lut_size = (1 << table_bits) + 1

    log2 = np.log(2.0)
    exp_max = 0.5 * log2
    step = (2.0 * exp_max) / (2 ** table_bits)

    # Generate LUT and copy into view
    lut = np.round(np.exp(np.arange(-exp_max, exp_max + step, step)) * (2 ** 14)).astype(np.int16)
    view[:lut_size] = lut
    
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

def build_delay_weights(weights: np.ndarray, delays: np.ndarray,
                        delay_bits: int) -> np.ndarray:
    assert weights.shape == delays.shape
    
    # Check largest delay fits within delay connectivity bits
    addresses = delays * 2
    max_address = np.amax(addresses)
    if max_address >= 2**delay_bits:
        raise RuntimeError("Not enough bits to represent delays")

    # Check weight fits within remaining bits
    weight_bits = 16 - delay_bits
    max_weight = (2**weight_bits) - 1
    min_weight = -max_weight - 1
    if np.amin(weights) < min_weight or np.amax(weights) > max_weight:
        raise RuntimeError("Not enough bits for weight")

    # Combine weight and indices
    return (addresses | (weights << delay_bits)).astype(np.int16)

def build_sparse_connectivity(row_ind: Sequence[np.ndarray], 
                              weight: Union[Number, Sequence[np.ndarray]],
                              sparse_connectivity_bits: int) -> np.ndarray:
    # Determine which lane each postsynaptic index belongs in
    row_lane = [r % 32 for r in row_ind]
    
    # Get ordering of rows
    row_order = [np.argsort(l) for l in row_lane]

    # Sort rows of indices by their lane
    row_ind_sorted = [i[o] for i, o in zip(row_ind, row_order)]

    # Count how many connections each lane needs to process in each row
    row_conns_per_lane = [np.bincount(l) for l in row_lane]

    # Determine maximum number of vectors
    num_vectors = max(np.amax(c) for c in row_conns_per_lane
                      if len(c) > 0)

    # Calculate cumulative sum of bin count to determine where to split per-bank
    row_conn_lane_sections = [np.cumsum(c) for c in row_conns_per_lane]

    # Convert row indices into addresses
    row_data_sorted = [((r // 32) * 2).astype(np.int16) for r in row_ind_sorted]

    # Check largest address fits without sparse connectivity bits
    max_address = max(np.amax(r) for r in row_data_sorted
                      if len(r) > 0)
    if max_address >= 2**sparse_connectivity_bits:
        raise RuntimeError("Not enough bits to represent connectivity")
    
    # Check weight fits within remaining bits
    weight_bits = 15 - sparse_connectivity_bits
    max_weight = (2**weight_bits) - 1
    min_weight = -max_weight - 1
    if isinstance(weight, Number):
        if weight < min_weight or weight > max_weight:
            raise RuntimeError("Not enough bits for weight")
        
        # Combine weight and indices
        row_data_sorted = [r | (weight << sparse_connectivity_bits)
                           for r in row_data_sorted]
    else:
        # Check each row limits
        for w in weight:
            if len(w) > 0 and (np.amin(w) < min_weight or np.amax(w) > max_weight):
                raise RuntimeError("Not enough bits for weight")
        
        # Sort weights into same order as indices
        weight_sorted = [w[o] for w, o in zip(weight, row_order)]

        # Combine weight and indices
        assert len(row_data_sorted) == len(weight)
        row_data_sorted = [r | (w << sparse_connectivity_bits)
                           for r, w in zip(row_data_sorted, weight)]

    padded_rows = []
    for d, s in zip(row_data_sorted, row_conn_lane_sections):
        # Split, pad list of connections with  
        # **NOTE** we only care about which L.L.M. address of target in bytes
        conn_id_banked = np.transpose(np.vstack([np.pad(a, (0, num_vectors - len(a)), 
                                                        constant_values=0)
                                                for a in np.split(d, s[:-1])]))
        conn_id_banked = np.pad(conn_id_banked, ((0, 0), (0, 32 - conn_id_banked.shape[1])),
                                constant_values=0)
        
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
