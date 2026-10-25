import numpy as np
import pyfenn.fenn_backend as backend

from argparse import ArgumentParser
from pyfenn.models import DenseLinear, Memset, SparseLinear
from pyfenn.utils import PythonLogAppender

from tonic.datasets import DVSGesture
from tonic.transforms import Compose, CropTime, Denoise, Downsample

from pyfenn.utils import (build_sparse_connectivity, build_spike_array,
                          ceil_divide, copy_and_push, copy_split_and_push,
                          get_views, split_sparse_connectivity, quantise,
                          zero_and_push)
from tqdm.auto import tqdm, trange

class LIF:
    def __init__(self, backend, shape, alpha: float, v_thresh: float,
                 v_fixed_point: int = 5, i_fixed_point: int = 5, 
                 tau_ref: int = 9, name: str = ""):
        self.shape = shape
        v_dtype = f"s{15 - v_fixed_point}_{v_fixed_point}_sat_t"
        i_dtype = f"s{15 - i_fixed_point}_{i_fixed_point}_sat_t"
        
        self.v = backend.Variable(self.shape, v_dtype, name=f"{name}_v")
        self.i = backend.Variable(self.shape, i_dtype, name=f"{name}_i")
        self.tau_ref = backend.Variable(self.shape, "int16_t", name=f"{name}_tau_ref")
        self.refrac_counter = backend.Variable(self.shape, "int16_t", name=f"{name}_refrac_counter")

        channel = backend.EventChannel(self.shape, name=f"{name}_out_spikes")
        self.out_spikes = channel.source
        self.process = backend.NeuronUpdateProcess(
            f"""
            // Neuron
            V = ({alpha}h15 * V) + (I * 1.0h{v_fixed_point});
            I = 0.0h{i_fixed_point};

            I = I;
            I = I;
            I = I;
            I = I;
            I = I;
            I = I;
            I = I;

            if (RefracCounter > 0) {{
                RefracCounter--;
            }}

            if(V >= {v_thresh}h{v_fixed_point} && RefracCounter <= 0) {{
               Spike();
               V -= {v_thresh}h{v_fixed_point};
               RefracCounter = {tau_ref};
            }}
            """,
            {"V": self.v, "I": self.i, "RefracCounter": self.refrac_counter},
            {"Spike": channel.sink},
            name=name)

class LIFOutput:
    def __init__(self, backend, shape, alpha: float, v_thresh: float, 
                 v_fixed_point: int = 5, i_fixed_point: int = 5, 
                 tau_ref: int = 9, name: str = ""):
        self.shape = shape
        v_dtype = f"s{15 - v_fixed_point}_{v_fixed_point}_sat_t"
        i_dtype = f"s{15 - i_fixed_point}_{i_fixed_point}_sat_t"

        self.v = backend.Variable(self.shape, v_dtype, name=f"{name}_v")
        self.i = backend.Variable(self.shape, i_dtype, name=f"{name}_i")
        self.refrac_counter = backend.Variable(self.shape, "int16_t", name=f"{name}_refrac_counter")
        self.num_spikes = backend.Variable(self.shape, "int16_sat_t", name=f"{name}_spike_count")

        self.process = backend.NeuronUpdateProcess(
            f"""
            // Neuron
            V = ({alpha}h15 * V) + (I * 1.0h{v_fixed_point});
            I = 0.0h{i_fixed_point};

            I = I;
            I = I;
            I = I;
            I = I;
            I = I;
            I = I;
            I = I;

            if (RefracCounter > 0) {{
                RefracCounter--;
            }}

            if(V >= {v_thresh}h{v_fixed_point} && RefracCounter <= 0) {{
               NumSpikes++;
               V -= {v_thresh}h{v_fixed_point};
               RefracCounter = {tau_ref};
            }}
            """,
            {"V": self.v, "I": self.i, "NumSpikes": self.num_spikes, 
             "RefracCounter": self.refrac_counter},
            {},
            name=name)

def get_sat_type(num_fractional_bits):
    return f"s{15 - num_fractional_bits}_{num_fractional_bits}_sat_t"

def remap_width_height_channel(inds, width, height, num_channels):
    # Unravel indices into their y, x, c components
    ind_y, ind_x, ind_c = np.unravel_index(inds, (height, width, num_channels))
    
    # Ravel them back in x, y, c order
    return np.ravel_multi_index((ind_x, ind_y, ind_c), (width, height, num_channels))

def remap_channel_height_width(inds, width, height, num_channels):
    # Unravel indices into their y, x, c components
    ind_y, ind_x, ind_c = np.unravel_index(inds, (height, width, num_channels))
    
    # Ravel them back in c, y, x order
    return np.ravel_multi_index((ind_c, ind_y, ind_x), (num_channels, height, width))

def shuffle_channel_height_width(weights, pre_width, pre_height, pre_channels, num_post):
    # Reshape weights into (pre_height, pre_width, pre_channels, num_post) shape
    weights = np.reshape(weights, (pre_height, pre_width, pre_channels, num_post))
    
    # Re-order presynaptic axes
    weights = np.moveaxis(weights, (0, 1, 2, 3), (1, 2, 0, 3))
    
    # Reshape back into (num_pre, num_post) shape
    return np.reshape(weights, (pre_height * pre_width * pre_channels, num_post))

def build_sparse_conn(conn, backend, num_pre: int, num_post: int, 
                      num_cores: int, num_sparse_connectivity_bits: int,
                      **quantise_kwargs):
    # Load pre and postsynaptic indices
    pre_ind, post_ind, weights = conn
    pre_ind = pre_ind.astype(int)
    post_ind = post_ind.astype(int)

    # Load and quantise weights
    weights = quantise(weights, **quantise_kwargs)
    assert weights.shape == pre_ind.shape
    assert weights.shape == post_ind.shape
    assert np.amin(pre_ind) >= 0
    assert np.amax(pre_ind) < num_pre

    # Loop through presynaptic neurons
    row_weights = []
    row_ind = []
    for i in range(num_pre):
        mask = (pre_ind == i)

        order = np.argsort(post_ind[mask])
        row_weights.append(weights[mask][order])
        row_ind.append(post_ind[mask][order])

    # Use backend helper to determine how postsynaptic population will be split
    post_neurons_per_core = [backend.get_split_dimension ((num_post,), c, 0, num_cores, 32)
                             for c in range(num_cores)]

    # Calculate edges
    # **TODO** this really could go in split
    splits = np.cumsum(post_neurons_per_core)[:-1]

    # Split connectivity between cores
    row_ind, row_weights = split_sparse_connectivity(row_ind, splits, row_weights)

    # Build connectivity
    return build_sparse_connectivity(row_ind, row_weights, num_sparse_connectivity_bits)

parser = ArgumentParser("DVS gesture classifier using LIF neurons")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--time", action="store_true", help="Record detailed timings using performance counters")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
parser.add_argument("--num-cores", type=int, default=1, help="Number of cores to split ")
args = parser.parse_args()

# Shapes of neurons at end of each layer
layer_shapes = [(32, 32, 2),
                (15, 15, 16),   # Hidden 1
                (15, 15, 16),   # Hidden 2
                (7, 7, 8),      # Hidden 3
                (256,),         # Hidden 4
                (64,)]          # Output **HACK** expand 

# Use backend helper to determine how postsynaptic population will be split
layer_neurons_per_core = [[backend.get_split_dimension((np.prod(l),), c, 0, args.num_cores, 32)
                           for c in range(args.num_cores)]
                          for l in layer_shapes]

# Number of sparse connectivity bits each layer requires
print(layer_neurons_per_core)
layer_sparse_connectivity_bits = [int(np.ceil(np.log2(ceil_divide(max(n), 32)))) + 1
                                  for n in layer_neurons_per_core[1:4]]
print(layer_sparse_connectivity_bits)
# Fractional bits neurons at end of each layer use for V and I 
layer_weight_fractional_bits = [6, 6, 6, 7, 9]

layer_v_fractional_bits = [4, 6, 6, 6, 9]

# Load weights
hidden1_conn = np.load("hidden1.npy")
hidden2_conn = np.load("hidden2.npy")
hidden3_conn = np.load("hidden3.npy")
hidden4_conn = np.load("hidden4.npy")
output_conn = np.reshape(np.load("output.npy"), (256, 11))
output_conn = np.pad(output_conn, ((0, 0), (0, 64 - 11)))

# Input neurons need to be arranged in width, height, channel order
hidden1_conn[0] = remap_width_height_channel(hidden1_conn[0].astype(int), 32, 32, 2)

# Hidden 1 neurons need to be arranged in width, height, channel order
hidden1_conn[1] = remap_width_height_channel(hidden1_conn[1].astype(int), 15, 15, 16)
hidden2_conn[0] = remap_width_height_channel(hidden2_conn[0].astype(int), 15, 15, 16)

# Hidden 2 neurons need to be arranged in channel, height, width order
hidden2_conn[1] = remap_channel_height_width(hidden2_conn[1].astype(int), 15, 15, 16)
hidden3_conn[0] = remap_channel_height_width(hidden3_conn[0].astype(int), 15, 15, 16)

# Hidden 3 neurons need to be arranged in channel, height, width order
hidden3_conn[1] = remap_channel_height_width(hidden3_conn[1].astype(int), 7, 7, 8)
hidden4_conn = shuffle_channel_height_width(hidden4_conn, 7, 7, 8, 256)

raw_sparse_conn = [hidden1_conn, hidden2_conn, hidden3_conn]

# Build sparse weights
sparse_weights = [build_sparse_conn(r, backend, num_pre=np.prod(n_pre), 
                                    num_post=np.prod(n_post), num_cores=args.num_cores,
                                    num_sparse_connectivity_bits=n, 
                                    fractional_bits=f, percentile=100.0)
                  for i, (n_pre, n_post, n, f, r) in enumerate(zip(layer_shapes[:3], layer_shapes[1:4], 
                                                                   layer_sparse_connectivity_bits, 
                                                                   layer_weight_fractional_bits, raw_sparse_conn))]

if args.num_cores == 1:
    input_size = np.prod(layer_shapes[0])
elif args.num_cores == 2:
    # **YUCK** figure out where the INPUT should be split to match the postsynaptic split
    last_pre_core_0 = next((len(sparse_weights[0][0]) -1 - i) 
                           for i, r in enumerate(reversed(sparse_weights[0][0]))
                           if np.any(r != 0))
    first_pre_core_1 = next(i for i, r in enumerate(sparse_weights[0][1])
                            if np.any(r != 0))
    print(f"Last input neuron on core 0 = {last_pre_core_0}")
    print(f"First input neuron on core 1 = {first_pre_core_1}")
    # Slice up sparse weights
    sparse_weights[0][0] = sparse_weights[0][0][:last_pre_core_0 + 1]
    sparse_weights[0][1] = sparse_weights[0][1][first_pre_core_1:]

    # **YUCK** pad to maximum number of rows
    input_size = max(sparse_weights[0][0].shape[0], 
                     sparse_weights[0][1].shape[0])
    sparse_weights[0][0] = np.pad(sparse_weights[0][0], 
                                  ((0, input_size - sparse_weights[0][0].shape[0]), (0, 0)))
    sparse_weights[0][1] = np.pad(sparse_weights[0][1], 
                                  ((0, input_size - sparse_weights[0][1].shape[0]), (0, 0)))
    print(f"Input size: {input_size}")
else:
    assert False

# Load DVS gesture, denoising and cropping time
dt = 0.1
sensor_size = (32, 32, 2)
chunk_timesteps = int(120 / dt)
dataset = DVSGesture(save_to="./data", train=False, 
                     transform=Compose([Denoise(filter_time=10000),
                                        Downsample(spatial_factor=0.25)]))

# Loop through dataset
input_spikes = []
input_labels = []


max_spike_array_length = 0
#for events, label in tqdm(dataset, "Preprocessing dataset"):
for i in range(5):
    events, label = dataset[i]

    # Convert times to 1ms timesteps and check they are sorted
    timestep_scale = int(1000 * dt)
    timesteps = np.floor(events["t"] / timestep_scale).astype(int)
    assert np.all(timesteps[:-1] <= timesteps[1:])

    # Split events into chunks
    chunk_bins = np.arange(0, timesteps[-1] + chunk_timesteps, chunk_timesteps)
    events_per_chunk = np.histogram(timesteps, chunk_bins)[0]
    events_per_chunk = np.cumsum(events_per_chunk)
    split_events = np.split(events, events_per_chunk)

    # Loop through chunks of events
    chunk_spikes = []
    for i, e in enumerate(split_events):
        # Make all chunks start at 0
        shifted = e
        shifted["t"] -= i * (chunk_timesteps * timestep_scale)
        assert np.all(shifted["t"] >= 0.0)

        # Flatten x, y and p into GenX320-ordered event IDs
        spike_event_ids = (shifted["p"] +
                           (shifted["y"] * sensor_size[2]) + 
                           (shifted["x"] * sensor_size[1] * sensor_size[2]))
        event_timesteps = np.floor(shifted["t"] / 1000.0).astype(int)
        if args.num_cores == 1:
            spike_array = build_spike_array(event_timesteps, spike_event_ids)
            max_spike_array_length = max(max_spike_array_length, len(spike_array))
            chunk_spikes.append((spike_array,))
        elif args.num_cores == 2:
            # Build array from spike event IDs which need to be send to core 0
            core0_mask = (spike_event_ids <= last_pre_core_0)
            core0_spike_array = build_spike_array(event_timesteps[core0_mask], spike_event_ids[core0_mask])
            
            # Build array from spike event IDs which need to be send to core 1
            core1_mask = (spike_event_ids >= first_pre_core_1)
            core1_spike_array = build_spike_array(event_timesteps[core1_mask], 
                                                  spike_event_ids[core1_mask] - first_pre_core_1)
            max_spike_array_length = max(max_spike_array_length, len(core0_spike_array), len(core1_spike_array))
            chunk_spikes.append((core0_spike_array, core1_spike_array))
        else:
            assert False

    print(f"{len(chunk_spikes)} chunks in example")
    input_spikes.append(chunk_spikes)
    input_labels.append(label)

print(f"Max spike array length: {max_spike_array_length}")
log_appender = backend.ConsoleAppender()#PythonLogAppender()
backend.init_logging(log_appender, backend.PlogSeverity.INFO)

# Input spikes
input_spike_buffer = backend.EventSourceBuffer((input_size,), max_spike_array_length)

# Populations of LIF neurons
beta = 0.93**(dt / 1.0)
populations = [LIF(backend, (np.prod(s),), beta, 1.0, v_f, i_f, name=f"hidden{i + 1}")
               for i, (s, v_f, i_f) in enumerate(zip(layer_shapes[1:-1], 
                                                     layer_v_fractional_bits, 
                                                     layer_weight_fractional_bits))]
populations.append(LIFOutput(backend, (np.prod(layer_shapes[-1]),), beta, 1.0,
                   layer_v_fractional_bits[-1], layer_weight_fractional_bits[-1], name="output"))

# Connect input spikes to first layer
input_hidden1 = SparseLinear(backend, input_spike_buffer, populations[0].i,
                             get_sat_type(layer_weight_fractional_bits[0]), max_row_length=sum(c.shape[1] for c in sparse_weights[0]),
                             num_sparse_connectivity_bits=layer_sparse_connectivity_bits[0], name="input_hidden1")

# Add sparse connectivity
hidden1_hidden2 = SparseLinear(backend, populations[0].out_spikes, populations[1].i,
                               get_sat_type(layer_weight_fractional_bits[1]), max_row_length=sum(c.shape[1] for c in sparse_weights[1]),
                               num_sparse_connectivity_bits=layer_sparse_connectivity_bits[1], name="hidden1_hidden2")

hidden2_hidden3 = SparseLinear(backend, populations[1].out_spikes, populations[2].i,
                               get_sat_type(layer_weight_fractional_bits[2]), max_row_length=sum(c.shape[1] for c in sparse_weights[2]),
                               num_sparse_connectivity_bits=layer_sparse_connectivity_bits[2], name="hidden2_hidden3")

hidden3_hidden4 = DenseLinear(backend, populations[2].out_spikes, populations[3].i,
                              get_sat_type(layer_weight_fractional_bits[3]), name="hidden3_hidden4")

hidden4_output = DenseLinear(backend, populations[3].out_spikes, populations[4].i,
                             get_sat_type(layer_weight_fractional_bits[4]), name="hidden4_hidden5")



# Group processes
perf_counters = ["num_instructions", "num_clock_cycles"] if args.time else []
neuron_update_processes = backend.ProcessGroup([p.process for p in populations], perf_counters)
synapse_update_processes = backend.ProcessGroup([input_hidden1.process, hidden1_hidden2.process, 
                                                 hidden2_hidden3.process, hidden3_hidden4.process,
                                                 hidden4_output.process], perf_counters)
# Create kernel
kernel = backend.SimulationLoopKernel(
    chunk_timesteps, [synapse_update_processes, neuron_update_processes],
    [], [])

# Create runtime
runtime_params = {"neuron_update_rounding_mode": backend.RoundingMode.NEAREST,
                  "use_dram_for_weights": True, "dma_buffer_size": 64 * 1024 * 1024}
runtime = (backend.RuntimeHW([kernel], args.num_cores, **runtime_params) if args.device 
           else backend.RuntimeSim([kernel], args.num_cores, **runtime_params))


# Disassemble if required
if args.disassemble:
    print("Simulation:")
    code = runtime.get_kernel_code(kernel)
    for i, c in enumerate(code):
        print(f"{i * 4} : {backend.disassemble(c)}")

# Allocate memory for model
runtime.allocate()

# Copy connectivity
copy_and_push(np.concatenate(sparse_weights[0], axis=1), input_hidden1.weight, runtime)
copy_and_push(np.concatenate(sparse_weights[1], axis=1), hidden1_hidden2.weight, runtime)
copy_and_push(np.concatenate(sparse_weights[2], axis=1), hidden2_hidden3.weight, runtime)

# Load and quantise output weights
copy_and_push(quantise(hidden4_conn, layer_weight_fractional_bits[3], percentile=100.0),
              hidden3_hidden4.weight, runtime)

copy_and_push(quantise(output_conn, layer_weight_fractional_bits[4], percentile=100.0),
                       hidden4_output.weight, runtime)

# Loop through examples
input_spike_views = get_views(runtime, input_spike_buffer)

output_num_spikes_views = get_views(runtime, populations[-1].num_spikes)

# Loop through input gestures
num_correct = 0
time_simulated = 0
for spike_chunks, label in zip(input_spikes, input_labels):
    # Manually zero voltages
    for p in populations:
        zero_and_push(p.v, runtime)

    # Zero num spikes
    zero_and_push(populations[-1].num_spikes, runtime)

    # Loop through spike chunks
    for i, arrays in enumerate(spike_chunks):
        print(f"chunk {i}/{len(spike_chunks)}")
        #copy_split_and_push(arrays, input_spike_buffer, runtime)
        for a, v in zip(arrays, input_spike_views):
            v[:len(a)] = a
        runtime.push_state_to_device(input_spike_buffer)

        # Classify
        runtime.run(kernel)
        time_simulated += chunk_timesteps

    # Copy output V sum from device
    runtime.pull_state_from_device(populations[-1].num_spikes)

    # Determine if output is correct
    print(output_num_spikes_views[0], label)
    classification = np.argmax(output_num_spikes_views[0][:11])
    if classification == label:
        num_correct += 1

if not args.device:
    saturation_count = np.asarray(runtime.saturation_count[0])
    non_zero_saturation_count = np.where(saturation_count > 0)
    for i in non_zero_saturation_count[0]:
        print(f"{saturation_count[i]} saturations occur at {i * 4}")

    print(f"Number of router buffer overflows {runtime.slave_buffer_overflow}")

print(f"Simulated {time_simulated} timesteps")
print(f"{num_correct} / {len(input_labels)} correct {100.0 * (num_correct / len(input_labels))}%")

if args.time:
    neuron_update_cycles = runtime.read_performance_counter(neuron_update_processes, 'num_clock_cycles')
    neuron_update_instructions = runtime.read_performance_counter(neuron_update_processes, 'num_instructions')

    synapse_update_cycles = runtime.read_performance_counter(synapse_update_processes, 'num_clock_cycles')
    synapse_update_instructions = runtime.read_performance_counter(synapse_update_processes, 'num_instructions')

    print(f"Neuron update {neuron_update_cycles} cycles, {neuron_update_instructions} instruction ({neuron_update_instructions[0] / neuron_update_cycles[0]})")
    print(f"Synapse update {synapse_update_cycles} cycles, {synapse_update_instructions} instruction ({synapse_update_instructions[0] / synapse_update_cycles[0]})")
