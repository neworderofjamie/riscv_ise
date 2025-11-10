import numpy as np
import mnist

from argparse import ArgumentParser
from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model, ProcessGroup,
                    RoundingMode, Runtime)
from pyfenn.models import Linear, Memset, RNGInit
from models import LIF, LI
from tonic.datasets import NMNIST
from tonic.transforms import Compose, Downsample

from pyfenn import disassemble, init_logging
from pyfenn.utils import (build_sparse_connectivity, ceil_divide,
                          copy_and_push, get_array_view,
                          load_quantise_and_push, quantise,
                          seed_and_push, zero_and_push)
from tqdm.auto import tqdm

def build_sparse_conn(checkpoint_stem: str, num_pre: int,
                      num_sparse_connectivity_bits: int,
                      **quantise_kwargs):
    # Load pre and postsynaptic indices
    pre_ind = np.load(f"{checkpoint_stem}-pre_ind.npy")
    post_ind = np.load(f"{checkpoint_stem}-post_ind.npy")

    # Load and quantise weights
    weights = quantise(np.load(f"{checkpoint_stem}-g.npy"), **quantise_kwargs)
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

    return build_sparse_connectivity(row_ind, row_weights, num_sparse_connectivity_bits)

dt = 1.0
num_timesteps = 300
input_shape = 34 * 34 * 2
hidden_shape = 512
output_shape = 10
input_hidden_shape = (input_shape, hidden_shape)
hidden_hidden_shape = (hidden_shape, hidden_shape)
hidden_output_shape = (hidden_shape, output_shape)
num_sparse_connectivity_bits = 5

parser = ArgumentParser("N-MNIST classifier with sparse connectivity")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
args = parser.parse_args()

# Load N-MNIST
dataset = NMNIST(save_to="data", train=False)
sensor_size = (34, 34, 2)

# Loop through dataset
n_mnist_spikes = []
n_mnist_labels = []
timestep_range = np.arange(0, (num_timesteps + 1) * dt, dt)
neuron_range = np.arange((ceil_divide(input_shape, 32) * 32) + 1)
for events, label in tqdm(dataset, "Preprocessing dataset"):
    # Build histogram
    neuron_id = (events["p"] + (events["x"] * sensor_size[2]) + 
                 (events["y"] * sensor_size[0] * sensor_size[2]))
    spike_event_histogram = np.histogram2d(events["t"] / 1000.0, neuron_id, 
                                           (timestep_range, neuron_range))[0]
    spike_event_histogram = np.minimum(spike_event_histogram, 1).astype(bool)
    spike_event_bits = np.packbits(spike_event_histogram, axis=1, bitorder="little")
    n_mnist_spikes.append(spike_event_bits.view(np.uint32).flatten())
    n_mnist_labels.append(label)

# Build connectivity
in_hid_conn = build_sparse_conn("n_mnist_checkpoints/98-Conn_Pop0_Pop2",
                                input_shape, num_sparse_connectivity_bits, fractional_bits=8,
                                percentile=100.0)
hid_hid_conn = build_sparse_conn("n_mnist_checkpoints/98-Conn_Pop2_Pop2",
                                 hidden_shape, num_sparse_connectivity_bits, fractional_bits=8,
                                 percentile=100.0)
init_logging()

# Input spikes
input_spikes = EventContainer(input_shape, num_timesteps)

# Model
rng_init = RNGInit()
hidden = LIF(hidden_shape, 20.0, 4, 0.61,
             1, 8, dt=dt, name="hidden")
output = LI(output_shape, 20.0, num_timesteps, 9, dt=dt, name="output")

input_hidden = Linear(input_spikes, hidden.i, "s7_8_sat_t", max_row_length=in_hid_conn.shape[1],
                      num_sparse_connectivity_bits=num_sparse_connectivity_bits, name="input_hidden")
hidden_hidden = Linear(hidden.out_spikes, hidden.i, "s7_8_sat_t", max_row_length=hid_hid_conn.shape[1],
                       num_sparse_connectivity_bits=num_sparse_connectivity_bits, name="hidden_hidden")
hidden_output = Linear(hidden.out_spikes, output.i, "s6_9_sat_t", name="hidden_output")

output_zero = Memset(output.v_avg)
hidden_zero = Memset(hidden.i)

# Group processes
init_processes = ProcessGroup([rng_init.process, hidden_zero.process])
neuron_update_processes = ProcessGroup([hidden.process, output.process])
synapse_update_processes = ProcessGroup([input_hidden.process, hidden_hidden.process, 
                                         hidden_output.process])
zero_processes = ProcessGroup([output_zero.process])

# Create backend
backend_kwargs = {"use_dram_for_weights": True, "rounding_mode": RoundingMode.STOCHASTIC, 
                  "dma_buffer_size": 2 * 1024 * 1024}
backend = (BackendFeNNHW(**backend_kwargs) 
           if args.device else BackendFeNNSim(**backend_kwargs))

# Create model
model = Model([init_processes, neuron_update_processes, synapse_update_processes, zero_processes],
              backend)

# Generate init and sim code
init_code = backend.generate_kernel([init_processes], model)
code = backend.generate_simulation_kernel([synapse_update_processes, neuron_update_processes],  # Update synapses and then neurons every timestep
                                          [zero_processes], [],
                                          num_timesteps, model)

# Disassemble if required
if args.disassemble:
    print("Init:")
    for i, c in enumerate(init_code):
        print(f"{i * 4} : {disassemble(c)}")

    print("Simulation:")
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
load_quantise_and_push("n_mnist_checkpoints/98-Conn_Pop2_Pop1-g.npy", 9, hidden_output.weight,
                       runtime, hidden_shape, True, percentile=100.0)
load_quantise_and_push("n_mnist_checkpoints/98-Pop1-Bias.npy", 9, output.bias, runtime, 
                       1, True, percentile=100.0)

# Zero remaining state
zero_and_push(hidden.v, runtime)
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
for spikes, label in tqdm(zip(n_mnist_spikes, n_mnist_labels),
                          total=len(n_mnist_labels), desc="Simulating"):
    # Copy data to array host pointe
    input_spike_view[:] = spikes
    input_spike_array.push_to_device()

    # Classify
    runtime.run()

    # Copy output V sum from device
    output_v_avg_array.pull_from_device()

    # Determine if output is correct
    classification = np.argmax(output_v_avg_view[:10])
    if classification == label:
        num_correct += 1

print(f"{num_correct} / {len(n_mnist_labels)} correct {100.0 * (num_correct / len(n_mnist_labels))}%")
