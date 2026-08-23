import logging
import numpy as np

from numbers import Number
from pyfenn._frontend import IAppender, PlogSeverity, Runtime
from typing import Optional, Sequence, Tuple, Union

from platform import system

RaggedArray = Sequence[np.ndarray]
SplitRaggedArray = Sequence[Sequence[np.ndarray]]

class PythonLogAppender(IAppender):
    _log_level_map = {PlogSeverity.FATAL: logging.FATAL,
                      PlogSeverity.ERROR: logging.ERROR,
                      PlogSeverity.WARNING: logging.WARNING,
                      PlogSeverity.INFO: logging.INFO,
                      PlogSeverity.DEBUG: logging.DEBUG,
                      PlogSeverity.VERBOSE: logging.DEBUG}

    def __init__(self):
        # **NOTE** direct __init__ constructor should 
        # be called, and super() should not be use
        IAppender.__init__(self)

        # Create Python logger
        self.logger = logging.getLogger("FeNN")
        
    def write(self, record):
        # Translate severity
        level = PythonLogAppender._log_level_map.get(record.severity,
                                                     logging.INFO)

        # Create Python logging record and handle
        record = self.logger.makeRecord(name=self.logger.name, level=level,
                                        fn=record.file, lno=record.line,
                                        msg=record.message, args=[],
                                        exc_info=None, func=record.func)
        self.logger.handle(record)
    
    
def is_kria() -> bool:
    # If we're on Linux (otherwise, uname is not available)
    if system() == "Linux":
        from os import uname
        return uname().release.endswith("xilinx-zynqmp")
    else:
        return False

def get_views(runtime: Runtime, state):
    # Get views
    return [np.array(a, copy=False)
            for a in runtime.get_arrays(state)]

# Divide two integers, rounding up i.e. effectively taking ceil
def ceil_divide(numerator, denominator):
    return (numerator + denominator - 1) // denominator

def pad(numerator, denominator):
    return denominator * ceil_divide(numerator, denominator)

def quantise(data, fractional_bits: int, percentile: float = 99.0):
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

    # Scale, round and convert to int16
    fp_one = 2.0 ** fractional_bits

    # Scale by fixed point, round, convert to int and flatten
    return np.round(data * fp_one).astype(np.int16).flatten()

def zero_and_push(state, runtime: Runtime):
    # Get array views
    views = get_views(runtime, state)

    # Zero
    # **HACK** assigning to the slice causes bus errors with DMA buffer
    for v in views:
        for i in range(len(v)):
            v[i] = 0
    #view[:] = 0

    # Push to device
    runtime.push_state_to_device(state)

def copy_and_push(data: np.ndarray, state, runtime: Runtime):
    # Get array and view
    views = get_views(runtime, state)

    # Get split dimension
    split_dimension = runtime.get_split_dimension(state)

    # Determine overall shape
    overall_shape = list(views[0].shape)
    if split_dimension is not None:
        split_dimension = len(views[0].shape) - 1 - split_dimension
        overall_shape[split_dimension] = sum(v.shape[split_dimension]
                                             for v in views)
        explicit_inds_slice = split_dimension * (slice(None),) 

    # Reshape data to this shape
    data = np.reshape(data, overall_shape)

    # Copy data to array host pointer
    split_dim_start = 0
    for v in views:
        assert(v.dtype == data.dtype)
        if split_dimension is not None:
            v[:] = data[explicit_inds_slice 
                        + (slice(split_dim_start, 
                                 split_dim_start + v.shape[split_dimension]),)]
            split_dim_start += v.shape[split_dimension]
        # Otherwise, copy all data into view
        else:
            v[:] = data

    # Push to device
    runtime.push_state_to_device(state)

def load_and_push(filename: str, state, runtime: Runtime):
    # Get views
    views = get_views(runtime, state)
    assert len(views) == 1

    # Load data with correct datatype
    data = np.fromfile(filename, dtype=views[0].dtype)
    data = np.reshape(data, views[0].shape)

    # Copy data to array host pointer
    for v in views:
        v[:] = data
   
    # Push to device
    runtime.push_state_to_device(state)

def load_quantise_and_push(filename: str, fractional_bits: int,
                           state, runtime: Runtime, percentile: float = 99.0):
    # Load data from file and quantise
    data = quantise(np.load(filename), fractional_bits, percentile)

    # Copy and push
    copy_and_push(data, state, runtime)

def pull_spikes(num_timesteps: int, state, runtime: Runtime):
    runtime.pull_state_from_device(state)
    spike_views = get_views(runtime, state)
    
    spike_views = [np.reshape(v.view(np.uint8), (num_timesteps, -1)) for v in spike_views]

    return [np.where(np.unpackbits(v, axis=1, bitorder="little"))
            for v in spike_views]


def read_perf_counter(perf_counter, runtime: Runtime):
    assert False
    # Get array and view
    array, view = get_array_view(runtime, perf_counter, np.uint64)

    # Pull
    array.pull_from_device()
    
    return view[0], view[1]

def seed_and_push(state, runtime: Runtime):
    # Get array and view
    views = get_views(runtime, state)

    # Generate different seeds for each view
    int16_info = np.iinfo(np.int16)
    for v in views:
        v[:] = np.random.randint(int16_info.min, int16_info.max, 64, dtype=np.int16)

    # Push to device
    runtime.push_state_to_device(state)

def generate_exp_lut_and_push(state, runtime: Runtime):
    views = get_views(runtime, state, np.int16)
    
    num_bits = 15
    table_bits = (num_bits - 3) // 2
    lut_size = (1 << table_bits) + 1

    log2 = np.log(2.0)
    exp_max = 0.5 * log2
    step = (2.0 * exp_max) / (2 ** table_bits)

    # Generate LUT and copy into all views
    lut = np.round(np.exp(np.arange(-exp_max, exp_max + step, step)) * (2 ** 14)).astype(np.int16)
    for v in views:
        v[:lut_size] = lut
    
    # Push to device
    runtime.push_state_to_device(state)

def build_spike_array(timesteps, neuron_ids):
    # Check timesteps and neuron ids can be 16-bit encoded
    assert np.all(neuron_ids < (1 << 15))
    assert np.all(timesteps < (1 << 15))

    # Order by time
    order = np.argsort(timesteps)
    timesteps = timesteps[order]
    neuron_ids = neuron_ids[order]
    
    # Count number of spikes in each timestep and use to split neuron ids
    num_spikes_per_time = np.cumsum(np.bincount(timesteps))
    timestep_spikes = np.split(neuron_ids, num_spikes_per_time)
    
    # Concatenate timestamps onto each non-empty group of neuron ids
    timestep_spikes = [np.concatenate(((t | 1 << 15,), n))
                       for t, n in enumerate(timestep_spikes)
                       if len(n) > 0]

    # Rejoin into single array
    timestep_spikes = np.concatenate(timestep_spikes)

    # We need a minimum of a single padding value at the end of each 
    # spike array and total length to be a multiple of 2
    padded_length = pad(1 + len(timestep_spikes), 2)
    
    # Pad 
    timestep_spikes = np.pad(timestep_spikes, 
                             (2, padded_length - len(timestep_spikes)), 
                             constant_values=(0, 0xFFFF))

    # Convert to uint16 and return
    return timestep_spikes.astype(np.uint16)


def convert_tonic_spikes(events: np.ndarray, ordering: Sequence[str],
                         shape: Tuple, time_scale=1.0 / 1000.0,
                         dt: float = 1.0, max_time: Optional[float] = None,
                         histogram_thresh: Optional[int] = 1):
    # Calculate cumulative sum of each neuron's spike count
    num_neurons = np.prod(shape) 

    # Check dataset datatype includes time and polarity
    if "t" not in ordering or "p" not in ordering:
        raise RuntimeError("Only tonic datasets with time (t) and "
                           "polarity (p) in ordering are supported")

    # If sensor has single polarity
    if shape[2] == 1:
        # If sensor is 2D, flatten x and y into event IDs
        if ("x" in ordering) and ("y" in ordering):
            spike_event_ids = events["x"] + (events["y"] * shape[0])
        # Otherwise, if it's 1D, simply use X
        elif "x" in ordering:
            spike_event_ids = events["x"]
        else:
            raise RuntimeError("Only 1D and 2D sensors supported")
    # Otherwise
    else:
        # If sensor is 2D, flatten x, y and p into event IDs
        if ("x" in ordering) and ("y" in ordering):
            spike_event_ids = (events["p"] +
                               (events["x"] * shape[2]) + 
                               (events["y"] * shape[0] * shape[2]))
        # Otherwise, if it's 1D, flatten x and p into event IDs
        elif "x" in ordering:
            spike_event_ids = events["p"] + (events["x"] * shape[2])
        else:
            raise RuntimeError("Only 1D and 2D sensors supported")
    
    scaled_t = events["t"] * time_scale
    
    # Build ranges for neuron ids and timesteps
    neuron_range = np.arange(num_neurons + 1)
    max_time = max_time or np.amax(scaled_t) + dt
    timestep_range = np.arange(0.0, max_time, dt)

    # Compute histogram
    spike_event_hist = np.histogram2d(spike_event_ids, scaled_t,
                                      (neuron_range, timestep_range))[0]

    # Find indices of bins where there are enough events
    thresh_id, thresh_t = np.where(spike_event_hist >= histogram_thresh)
    
    # Build spike arrays and return
    return build_spike_array(thresh_t, thresh_id)
                     
def get_latency_spikes(images, tau=20.0, num_timesteps=79, threshold=51):
    # Flatten images and convert intensity to time
    images = np.reshape(images, (images.shape[0], -1))

    spikes = []
    for i in images:
        times = np.round(tau * np.log(i / (i - threshold))).astype(int)
        
        # Get IDs of neurons which should spike
        neuron_ids = np.where((i > threshold) & (times < num_timesteps))[0]
        
        # Build spike array and add to list
        spikes.append(build_spike_array(times[neuron_ids], neuron_ids))

    # Calculate maximum spike array length
    max_spike_array_length = max(len(s) for s in spikes)
    
    return spikes, max_spike_array_length

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

def build_sparse_connectivity(row_ind: SplitRaggedArray, 
                              weight: Union[Number, SplitRaggedArray],
                              sparse_connectivity_bits: int) -> np.ndarray:
    # Check number of presynaptic neurons is the same across all splits
    num_pre = [len(r) for r in row_ind]
    assert all(num_pre[0] == n for n in num_pre)
    
    # Determine which lane each postsynaptic index belongs in
    row_lane = [[r % 32 for r in core_row_ind]
                for core_row_ind in row_ind]

    # Get order of each row
    row_order = [[np.argsort(l) for l in core_row_lane]
                  for core_row_lane in row_lane]
                      
    # Use this to order indices
    row_ind_sorted = [[i[o] for i, o in zip(core_row_ind, core_row_order)]
                      for core_row_ind, core_row_order in zip(row_ind, row_order)]

    # Count how many connections each lane needs to process in each row
    row_conns_per_lane = [[np.bincount(l) for l in core_row_lane]
                          for core_row_lane in row_lane]

    # Determine maximum number of vectors
    num_vectors = max(np.amax(c) for core_row_conns_per_lane in row_conns_per_lane
                      for c in core_row_conns_per_lane if len(c) > 0)

    # Calculate cumulative sum of bin count to determine where to split per-bank
    row_conn_lane_ind = [[np.cumsum(c) for c in core_row_conns_per_lane]
                         for core_row_conns_per_lane in row_conns_per_lane]

    # Convert row indices into addresses
    row_data_sorted = [[((r // 32) * 2).astype(np.int16) 
                        for r in core_row_ind_sorted]
                        for core_row_ind_sorted in row_ind_sorted]

    # Check largest address fits without sparse connectivity bits
    max_address = max(np.amax(r) for core_row_data_sorted in row_data_sorted
                      for r in core_row_data_sorted if len(r) > 0)
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
        row_data_sorted = [[r | (weight << sparse_connectivity_bits)
                            for r in core_row_data_sorted]
                            for core_row_data_sorted in row_data_sorted]
    else:
        # Check weights and row indices are provided for same core count
        assert len(weight) == len(row_ind)
       
        # Loop through core row indices and weights
        for core_row_ind, core_weight in zip(row_ind, weight):
            # Check they have the same number of rows
            assert len(core_weight) == len(core_row_ind)
        
            # Check each row limits
            for i, w in zip(core_row_ind, core_weight):
                assert len(i) == len(w)
                if len(w) > 0 and (np.amin(w) < min_weight or np.amax(w) > max_weight):
                    raise RuntimeError("Not enough bits for weight")
        
        # Sort weights into same order as indices
        weight_sorted = [[w[o] for w, o in zip(core_weight, core_row_order)]
                          for core_weight, core_row_order in zip(weights, row_ind)]

        # Combine weight and indices
        row_data_sorted = [[r | (w << sparse_connectivity_bits)
                           for r, w in zip(core_row_data_sorted, core_weight_sorted)]
                           for core_row_data_sorted, core_weight_sorted in zip(row_data_sorted, weight_sorted)]
    
    # Loop through cores
    padded_rows = []
    for core_row_data_sorted, core_row_conn_lane_ind in zip(row_data_sorted, row_conn_lane_ind):
        # Loop through rows
        core_padded_rows = []
        for i, l in zip(core_row_data_sorted, core_row_conn_lane_ind):
            # Split, pad list of connections with  
            # **NOTE** we only care about which L.L.M. address of target in bytes
            conn_id_banked = np.transpose(np.vstack([np.pad(a, (0, num_vectors - len(a)), 
                                                            constant_values=-2)
                                                    for a in np.split(i, l[:-1])]))
            conn_id_banked = np.pad(conn_id_banked, ((0, 0), (0, 32 - conn_id_banked.shape[1])),
                                    constant_values=0)
            
            core_padded_rows.append(np.reshape(conn_id_banked, 32 * num_vectors))
        padded_rows.append(np.vstack(core_padded_rows).astype(np.int16))
    
    return padded_rows

def split_sparse_connectivity(inds: RaggedArray, 
                              right_edges: Sequence[int],
                              *args) -> SplitRaggedArray:
    # Count splits
    num_splits = len(right_edges) + 1
    
    # Create list of lists to hold split indices
    split_inds = [[] for _ in range(num_splits)]
    
    # Create list of list of lists for additional split arrays
    split_arrays = [[[] for _ in range(num_splits)]
                    for _ in range(len(args))]

    # Loop through rows and other ragged arrays
    for r in zip(inds, *args):
        # Split inds based on right edge
        # **NOTE** we want to copy 
        splits = np.searchsorted(r[0], right_edges)
        split_row_inds = np.split(r[0], splits)
        assert len(split_row_inds) == num_splits

        # Split each array the same way
        split_row_arrays = [np.split(a, splits) for a in r[1:]]
        assert all(len(s) == num_splits for s in split_row_arrays)

        # Loop through split rows
        for i, splits in enumerate(zip(split_row_inds, *split_row_arrays)):
            # Subtract right edge of previous split 
            # i.e. left edge from indices
            split_row_ind = splits[0].copy()
            if i > 0:
                split_row_ind -= right_edges[i - 1]

            # Add indices to list
            split_inds[i].append(split_row_ind)
            
            # Loop through split arrays and add splits to list
            for j, a in enumerate(split_arrays):
                a[i].append(splits[j + 1])
    
    # Return tuple of split arrays
    return (split_inds if len(split_arrays) == 0 
            else (split_inds, *split_arrays))
    
def generate_fixed_prob(num_pre: int, num_post: int,
                        prob: float) -> RaggedArray:
    # Loop through presynaptic neurons
    rows = []
    for i in range(num_pre):
        # Make num_post bernoulli trials
        row_mask = np.random.choice([True, False], num_post,
                                    p=[prob, 1.0 - prob])
        # Add indices of 1s to row
        rows.append(np.where(row_mask)[0])
    return rows
