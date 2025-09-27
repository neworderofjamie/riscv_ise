import numpy as np
import mnist

from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model, ProcessGroup,
                    Runtime)
from pyfenn.models import Linear, Memset, RNGInit
from models import ALIF, LI
from tonic.datasets import DVSGesture
from tonic.transforms import Compose, Downsample

from pyfenn import disassemble, init_logging
from pyfenn.utils import (build_sparse_connectivity, ceil_divide,
                          get_array_view, load_quantise_and_push,
                          quantise, seed_and_push, zero_and_push)
from tqdm.auto import tqdm

def build_sparse_conn(checkpoint_stem: str, num_pre: int,
                      num_sparse_connectivity_bits: int,
                      **quantise_kwargs):
    # Load pre and postsynaptic indices
    pre_ind = np.load(f"{checkpoint_stem}-pre_ind.npy")
    post_ind = np.load(f"{checkpoint_stem}-post_ind.npy")

    # Load and quantise weights
    weights = quantise(np.load(f"{checkpoint_stem}-g.npy"), **quantise_kwargs)
    
    # Loop through presynaptic neurons
    row_weights = []
    row_ind = []
    for i in range(num_pre):
        mask = (pre_ind == i)
        
        row_weights.append(weights[mask])
        row_ind.append(post_ind[mask])
    
    return build_sparse_connectivity(row_ind, row_weights, num_sparse_connectivity_bits)

dt = 5.0
num_timesteps = 300
input_shape = 32 * 32
hidden_shape = 512
output_shape = 11
input_hidden_shape = (input_shape, hidden_shape)
hidden_hidden_shape = (hidden_shape, hidden_shape)
hidden_output_shape = (hidden_shape, output_shape)
num_sparse_connectivity_bits = 5
device = False
record = False
disassemble_code = False

# Load and preprocess DVS-gesture
transform = Compose([Downsample(spatial_factor=0.25)])
dataset = DVSGesture(save_to="data", train=False, transform=transform)
sensor_size = (32, 32, 2)

# Loop through dataset
dvs_gesture_spikes = []
dvs_gesture_labels = []
timestep_range = np.arange(0, (num_timesteps + 1) * dt, dt)
neuron_range = np.arange((ceil_divide(input_shape, 32) * 32) + 1)
max_len = 0
for events, label in dataset:
    # Build histogram
    # **TODO** x and y
    spike_event_histogram = np.histogram2d(events["t"] / 1000.0, events["x"], (timestep_range, neuron_range))[0]
    spike_event_histogram = np.minimum(spike_event_histogram, 1).astype(bool)
    spike_event_bits = np.packbits(spike_event_histogram, axis=1, bitorder="little")
    dvs_gesture_spikes.append(spike_event_bits.view(np.uint32).flatten())
    dvs_gesture_labels.append(label)

# Build connectivity
in_hid_conn = build_sparse_conn("checkpoints_0_False_0_False_512_100_dvs_gesture_1_1234_False_0.01_512_True_alif_0.05_0.1_0.0_5.0_1500.0_full/99-Conn_Pop0_Pop2",
                                input_shape, num_sparse_connectivity_bits, fractional_bits=7)
hid_hid_conn = build_sparse_conn("checkpoints_0_False_0_False_512_100_dvs_gesture_1_1234_False_0.01_512_True_alif_0.05_0.1_0.0_5.0_1500.0_full/99-Conn_Pop2_Pop2",
                                 hidden_shape, num_sparse_connectivity_bits, fractional_bits=7)
print(in_hid_conn.shape)
print(hid_hid_conn.shape)
init_logging()

# Input spikes
input_spikes = EventContainer(input_shape, num_timesteps)

# Model
rng_init = RNGInit()
hidden = ALIF(hidden_shape, 20.0, 2000.0, 4, 0.6, 0.0174,
              record, 7, dt=dt, name="hidden")
output = LI(output_shape, 20.0, num_timesteps, 11, dt=dt, name="output")

input_hidden = Linear(input_spikes, hidden.i, "s8_7_sat_t", max_row_length=in_hid_conn.shape[1],
                      num_sparse_connectivity_bits=num_sparse_connectivity_bits, name="input_hidden")
hidden_hidden = Linear(hidden.out_spikes, hidden.i, "s8_7_sat_t", max_row_length=hid_hid_conn.shape[1],
                       num_sparse_connectivity_bits=num_sparse_connectivity_bits, name="hidden_hidden")
hidden_output = Linear(hidden.out_spikes, output.i, "s4_11_sat_t", name="hidden_output")

output_zero = Memset(output.v_avg)

# Group processes
init_processes = ProcessGroup([rng_init.process])
neuron_update_processes = ProcessGroup([hidden.process, output.process])
synapse_update_processes = ProcessGroup([input_hidden.process, hidden_hidden.process, 
                                         hidden_output.process])
zero_processes = ProcessGroup([output_zero.process])

# Create backend
backend = BackendFeNNHW() if device else BackendFeNNSim()

# Create model
model = Model([init_processes, neuron_update_processes, synapse_update_processes, zero_processes],
              backend)

# Generate init and sim code
init_code = backend.generate_kernel([init_processes], model)
code = backend.generate_simulation_kernel([synapse_update_processes, neuron_update_processes],  # Update synapses and then neurons every timestep
                                          [zero_processes], [],
                                          num_timesteps, model)

# Disassemble if required
if disassemble_code:
    for i, c in enumerate(code):
        print(f"{i * 4} : {disassemble(c)}")

# Create runtime
runtime = Runtime(model, backend)

# Allocate memory for model
runtime.allocate()

# Copy connectivity
copy_and_push(in_hid_conn.flatten(), input_hidden.weight, runtime)
copy_and_push(hid_hid_conn.flatten(), hidden_hidden.weight, runtime)

# Load output weights and biases, quantise and push to FeNN
load_quantise_and_push("checkpoints_0_False_0_False_512_100_dvs_gesture_1_1234_False_0.01_512_True_alif_0.05_0.1_0.0_5.0_1500.0_full/99-Conn_Pop2_Pop1-g.npy", 11, hidden_output.weight,
                       runtime, hidden_shape[0], True, percentile=100.0)
load_quantise_and_push("checkpoints_0_False_0_False_512_100_dvs_gesture_1_1234_False_0.01_512_True_alif_0.05_0.1_0.0_5.0_1500.0_full/99-Pop1-Bias.npy", 11, output.bias, runtime, 
                       1, True, percentile=100.0)

# Zero remaining state
zero_and_push(hidden.v, runtime)
zero_and_push(hidden.a, runtime)
zero_and_push(hidden.i, runtime)
zero_and_push(hidden.refrac_time, runtime)
zero_and_push(output.v, runtime)
zero_and_push(output.i, runtime)
zero_and_push(output.v_avg, runtime)

# Get array and view
seed_and_push(rng_init.seed, runtime)

# Set init instructions and run
runtime.set_instructions(init_code)
runtime.run()

# Set instructions
runtime.set_instructions(code)

# Loop through examples
input_spike_array, input_spike_view = get_array_view(runtime, input_spikes,
                                                     np.uint32)
hidden_spike_array = runtime.get_array(hidden.out_spikes)
output_v_avg_array, output_v_avg_view = get_array_view(runtime, output.v_avg, np.int16)

num_correct = 0
for spikes, label in tqdm(zip(dvs_gesture_spikes, dvs_gesture_labels),
                          total=len(dvs_gesture_labels)):
    # Copy data to array host pointe
    input_spike_view[:] = spikes
    input_spike_array.push_to_device();

    # Classify
    runtime.run()

    # Copy output V sum from device
    output_v_avg_array.pull_from_device();

    # Determine if output is correct
    classification = np.argmax(output_v_avg_view)
    if classification == label:
        num_correct += 1

print(f"{num_correct} / {len(shd_labels)} correct {100.0 * (num_correct / len(shd_labels))}%")
