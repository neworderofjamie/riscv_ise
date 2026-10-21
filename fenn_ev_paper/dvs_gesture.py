import numpy as np
import pyfenn.fenn_backend as backend

from argparse import ArgumentParser
from pyfenn.models import DenseLinear, Memset, SparseLinear
from pyfenn.utils import PythonLogAppender

from tonic.datasets import DVSGesture
from tonic.transforms import Compose, CropTime, Denoise, Downsample

from pyfenn.utils import (build_sparse_connectivity, ceil_divide, 
                          copy_and_push, convert_tonic_spikes, get_views,
                          load_quantise_and_push, pull_spikes,
                          quantise, zero_and_push)
from tqdm.auto import tqdm

class LIF:
    def __init__(self, backend, shape, alpha: float,
                 v_thresh: float, fixed_point: int = 5, 
                 tau_ref: int = 9, name: str = ""):
        self.shape = shape
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"
        
        self.v = backend.Variable(self.shape, dtype, name=f"{name}_v")
        self.i = backend.Variable(self.shape, dtype, name=f"{name}_i")
        self.tau_ref = backend.Variable(self.shape, "int16_t", name=f"{name}_tau_ref")
        self.refrac_counter = backend.Variable(self.shape, "int16_t", name=f"{name}_refrac_counter")

        channel = backend.EventChannel(self.shape, name=f"{name}_out_spikes")
        self.out_spikes = channel.source
        self.process = backend.NeuronUpdateProcess(
            f"""
            // Neuron
            V = ({alpha}h15 * V) + I;
            I = 0.0h{fixed_point};
            
            I = I;
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

            if(V >= {v_thresh}h{fixed_point} && RefracCounter <= 0) {{
               Spike();
               V -= {v_thresh}h{fixed_point};
               RefracCounter = {tau_ref};
            }}
            """,
            {"V": self.v, "I": self.i, "RefracCounter": self.refrac_counter},
            {"Spike": channel.sink},
            name=name)

class LIFOutput:
    def __init__(self, backend, shape, alpha: float,
                 v_thresh: float, fixed_point: int = 5, 
                 tau_ref: int = 9, name: str = ""):
        self.shape = shape
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"
        
        self.v = backend.Variable(self.shape, dtype, name=f"{name}_v")
        self.i = backend.Variable(self.shape, dtype, name=f"{name}_i")
        self.refrac_counter = backend.Variable(self.shape, "int16_t", name=f"{name}_refrac_counter")
        self.num_spikes = backend.Variable(self.shape, "int16_sat_t", name=f"{name}_spike_count")

        self.process = backend.NeuronUpdateProcess(
            f"""
            // Neuron
            V = ({alpha}h15 * V) + I;
            I = 0.0h{fixed_point};

            I = I;
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

            if(V >= {v_thresh}h{fixed_point} && RefracCounter <= 0) {{
               NumSpikes++;
               V -= {v_thresh}h{fixed_point};
               RefracCounter = {tau_ref};
            }}
            """,
            {"V": self.v, "I": self.i, "NumSpikes": self.num_spikes, 
             "RefracCounter": self.refrac_counter},
            {},
            name=name)

def get_sat_type(num_fractional_bits):
    return f"s{15 - num_fractional_bits}_{num_fractional_bits}_sat_t"

def build_sparse_conn(name: str, num_pre: int,
                      num_sparse_connectivity_bits: int,
                      **quantise_kwargs):
    # Load pre and postsynaptic indices
    pre_ind, post_ind, weights = np.load(name)
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

        row_weights.append(weights[mask])
        row_ind.append(post_ind[mask])

    return build_sparse_connectivity([row_ind], [row_weights], num_sparse_connectivity_bits)

# Shapes of neurons at end of each layer
layer_shapes = [(32, 32, 2),
                (15, 15, 16),   # Hidden 1
                (15, 15, 16),   # Hidden 2
                (7, 7, 8),      # Hidden 3
                (256,),         # Hidden 4
                (11,)]          # Output

# Number of sparse connectivity bits each layer requires
layer_sparse_connectivity_bits = [int(np.ceil(np.log2(ceil_divide(np.prod(s), 32)))) + 1
                                  for s in layer_shapes[1:4]]

# Fractional bits neurons at end of each layer use for V and I 
layer_fractional_bits = [7, 6, 7, 7, 12]

# Build sparse weights
sparse_weights = [build_sparse_conn(f"hidden{i + 1}.npy", num_pre=np.prod(s),
                                    num_sparse_connectivity_bits=n, 
                                    fractional_bits=f, percentile=100.0)
                  for i, (s, n, f) in enumerate(zip(layer_shapes[:3], layer_sparse_connectivity_bits, 
                                                    layer_fractional_bits))]

parser = ArgumentParser("DVS gesture classifier using LIF neurons")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--time", action="store_true", help="Record detailed timings using performance counters")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
args = parser.parse_args()

# Load DVS gesture, denoising and cropping time
dt = 0.1
chunk_timesteps = int(200 / dt)
sensor_size = (32, 32, 2)
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
        spike_array = convert_tonic_spikes(shifted, dataset.ordering, 
                                           sensor_size, histogram_thresh=None)
        max_spike_array_length = max(max_spike_array_length, len(spike_array))
        chunk_spikes.append(spike_array)

    input_spikes.append(chunk_spikes)
    input_labels.append(label)

print(max_spike_array_length)
log_appender = backend.ConsoleAppender()#PythonLogAppender()
backend.init_logging(log_appender, backend.PlogSeverity.DEBUG)

# Input spikes
input_spike_buffer = backend.EventSourceBuffer((np.prod(layer_shapes[0]),), max_spike_array_length)

# Populations of LIF neurons
beta = 0.93**(dt / 1.0)
populations = [LIF(backend, (np.prod(s),), beta, 1.0, f, name=f"hidden{i + 1}")
               for i, (s, f) in enumerate(zip(layer_shapes[1:-1], layer_fractional_bits))]
populations.append(LIFOutput(backend, (np.prod(layer_shapes[-1]),), beta, 1.0, 
                   layer_fractional_bits[-1], name="output"))

# Connect input spikes to first layer
input_hidden1 = SparseLinear(backend, input_spike_buffer, populations[0].i, 
                             get_sat_type(layer_fractional_bits[0]), max_row_length=sparse_weights[0][0].shape[1],
                             num_sparse_connectivity_bits=layer_sparse_connectivity_bits[0], name="input_hidden1")

# Add sparse connectivity
hidden1_hidden2 = SparseLinear(backend, populations[0].out_spikes, populations[1].i, 
                               get_sat_type(layer_fractional_bits[1]), max_row_length=sparse_weights[1][0].shape[1],
                               num_sparse_connectivity_bits=layer_sparse_connectivity_bits[1], name="hidden1_hidden2")

hidden2_hidden3 = SparseLinear(backend, populations[1].out_spikes, populations[2].i, 
                               get_sat_type(layer_fractional_bits[2]), max_row_length=sparse_weights[2][0].shape[1],
                               num_sparse_connectivity_bits=layer_sparse_connectivity_bits[2], name="hidden2_hidden3")
             
hidden3_hidden4 = DenseLinear(backend, populations[2].out_spikes, populations[3].i, 
                              get_sat_type(layer_fractional_bits[3]), name="hidden3_hidden4")

hidden4_output = DenseLinear(backend, populations[3].out_spikes, populations[4].i, 
                             get_sat_type(layer_fractional_bits[4]), name="hidden4_hidden5")



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
runtime = (backend.RuntimeHW([kernel], 1, **runtime_params) if args.device 
           else backend.RuntimeSim([kernel], 1, **runtime_params))


# Disassemble if required
if args.disassemble:
    print("Simulation:")
    code = runtime.get_kernel_code(kernel)
    for i, c in enumerate(code):
        print(f"{i * 4} : {backend.disassemble(c)}")

# Allocate memory for model
runtime.allocate()

# Copy connectivity
copy_and_push(sparse_weights[0][0], input_hidden1.weight, runtime)
copy_and_push(sparse_weights[1][0], hidden1_hidden2.weight, runtime)
copy_and_push(sparse_weights[2][0], hidden2_hidden3.weight, runtime)

# Load and quantise output weights
load_quantise_and_push("hidden4.npy", layer_fractional_bits[3], 
                       hidden3_hidden4.weight, runtime, percentile=100.0)
load_quantise_and_push("output.npy", layer_fractional_bits[4], 
                       hidden4_output.weight, runtime, percentile=100.0)

v_views = [get_views(runtime, p.v) for p in populations]

# Loop through examples
input_spike_views = get_views(runtime, input_spike_buffer)
assert len(input_spike_views) == 1

output_num_spikes_views = get_views(runtime, populations[-1].num_spikes)
assert len(output_num_spikes_views) == 1

# Loop through input gestures
num_correct = 0
time_simulated = 0
for spike_chunks, label in tqdm(zip(input_spikes, input_labels),
                                 total=len(input_labels), desc="Simulating"):
    # Manually zero voltages
    for p, v in zip(populations, v_views):
        v[0][:] = 0
        runtime.push_state_to_device(p.v)

    # Zero num spikes
    output_num_spikes_views[0][:] = 0
    runtime.push_state_to_device(populations[-1].num_spikes)

    # Loop through spike chunks
    for c in spike_chunks:
        # Copy data to array host pointe
        input_spike_views[0][:len(c)] = c
        runtime.push_state_to_device(input_spike_buffer)

        # Classify
        runtime.run(kernel)
        time_simulated += chunk_timesteps

    # Copy output V sum from device
    runtime.pull_state_from_device(populations[-1].num_spikes)

    # Determine if output is correct
    print(output_num_spikes_views[0])
    classification = np.argmax(output_num_spikes_views[0])
    if classification == label:
        num_correct += 1

saturation_count = np.asarray(runtime.saturation_count[0])
non_zero_saturation_count = np.where(saturation_count > 0)
for i in non_zero_saturation_count[0]:
    print(f"{saturation_count[i]} saturations occur at {i}")
    
print(f"Number of router buffer overflows {runtime.slave_buffer_overflow}")
print(f"Simulated {time_simulated} timesteps")
print(f"{num_correct} / {len(input_labels)} correct {100.0 * (num_correct / len(input_labels))}%")

if args.time:
    neuron_update_cycles = runtime.read_performance_counter(neuron_update_processes, 'num_clock_cycles')[0]
    neuron_update_instructions = runtime.read_performance_counter(neuron_update_processes, 'num_instructions')[0]

    synapse_update_cycles = runtime.read_performance_counter(synapse_update_processes, 'num_clock_cycles')[0]
    synapse_update_instructions = runtime.read_performance_counter(synapse_update_processes, 'num_instructions')[0]

    print(f"Neuron update {neuron_update_cycles} cycles, {neuron_update_instructions} instruction ({neuron_update_instructions / neuron_update_cycles})")
    print(f"Synapse update {synapse_update_cycles} cycles, {synapse_update_instructions} instruction ({synapse_update_instructions / synapse_update_cycles})")
