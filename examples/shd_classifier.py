import numpy as np
import mnist

from argparse import ArgumentParser
from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model, ProcessGroup,
                    Runtime)
from pyfenn.models import Linear, Memset, RNGInit
from models import ALIF, LI
from tonic.datasets import SHD

from pyfenn import disassemble, init_logging
from pyfenn.utils import (ceil_divide, get_array_view, load_quantise_and_push,
                          seed_and_push, zero_and_push)
from tqdm.auto import tqdm

num_timesteps = 1170
input_shape = 700
hidden_shape = 256
output_shape = 20
input_hidden_shape = (input_shape, hidden_shape)
hidden_hidden_shape = (hidden_shape, hidden_shape)
hidden_output_shape = (hidden_shape, output_shape)

parser = ArgumentParser("SHD classifier using ALIF neurons")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
args = parser.parse_args()


# Load and preprocess SHD
dataset = SHD(save_to="data", train=False)

# Loop through dataset
shd_spikes = []
shd_labels = []
timestep_range = np.arange(num_timesteps + 1)
neuron_range = np.arange((ceil_divide(input_shape, 32) * 32) + 1)
for events, label in tqdm(dataset, "Preprocessing dataset"):
    # Build histogram
    spike_event_histogram = np.histogram2d(events["t"] / 1000.0, events["x"], (timestep_range, neuron_range))[0]
    spike_event_histogram = np.minimum(spike_event_histogram, 1).astype(bool)
    spike_event_bits = np.packbits(spike_event_histogram, axis=1, bitorder="little")
    shd_spikes.append(spike_event_bits.view(np.uint32).flatten())
    shd_labels.append(label)

init_logging()

# Input spikes
input_spikes = EventContainer(input_shape, num_timesteps)

# Model
rng_init = RNGInit()
hidden = ALIF(hidden_shape, 20.0, 2000.0, 4, 0.6, 0.0174,
              1, 7, name="hidden")
output = LI(output_shape, 20.0, num_timesteps, 11, name="output")

input_hidden = Linear(input_spikes, hidden.i, "s8_7_sat_t", name="input_hidden")
hidden_hidden = Linear(hidden.out_spikes, hidden.i, "s8_7_sat_t", name="hidden_hidden")
hidden_output = Linear(hidden.out_spikes, output.i, "s4_11_sat_t", name="hidden_output")

output_zero = Memset(output.v_avg)

# Group processes
init_processes = ProcessGroup([rng_init.process])
neuron_update_processes = ProcessGroup([hidden.process, output.process])
synapse_update_processes = ProcessGroup([input_hidden.process, hidden_hidden.process, 
                                         hidden_output.process])
zero_processes = ProcessGroup([output_zero.process])

# Create backend
backend = BackendFeNNHW() if args.device else BackendFeNNSim()

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
    for i, c in enumerate(code):
        print(f"{i * 4} : {disassemble(c)}")

# Create runtime
runtime = Runtime(model, backend)

# Allocate memory for model
runtime.allocate()

# Load weights, quantise and pusn to FeNN
load_quantise_and_push("99-Conn_Pop0_Pop1-g.npy", 7, 
                       input_hidden.weight, runtime, percentile=100.0)
load_quantise_and_push("99-Conn_Pop1_Pop1-g.npy", 7,
                       hidden_hidden.weight, runtime, percentile=100.0)
load_quantise_and_push("99-Conn_Pop1_Pop2-g.npy", 11, hidden_output.weight,
                       runtime, hidden_shape, True, percentile=100.0)
load_quantise_and_push("99-Pop2-Bias.npy", 11, output.bias, runtime, 
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
for spikes, label in tqdm(zip(shd_spikes, shd_labels),
                          total=len(shd_labels), desc="Simulating"):
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
