import numpy as np
import mnist

from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model, ProcessGroup,
                    Runtime, Shape)
from models import ALIF, Copy, LI, Linear, RNGInit
from tonic.datasets import SHD

from pyfenn import disassemble, init_logging
from pyfenn.utils import (ceil_divide, get_array_view, load_and_push, 
                          seed_and_push, zero_and_push)
from tqdm.auto import tqdm

num_timesteps = 1024
input_shape = [700]
hidden_shape = [256]
output_shape = [20]
input_hidden_shape = [input_shape[0], hidden_shape[0]]
hidden_hidden_shape = [hidden_shape[0], hidden_shape[0]]
hidden_output_shape = [hidden_shape[0], output_shape[0]]
device = False
record = False
disassemble_code = False

# Load and preprocess SHD
dataset = SHD(save_to="data", train=False)

# Loop through dataset
shd_spikes = []
shd_labels = []
timestep_range = np.arange(num_timesteps + 1)
neuron_range = np.arange((ceil_divide(input_shape[0], 32) * 32) + 1)
for events, label in dataset:
    # Build histogram
    spike_event_histogram = np.histogram2d(events["t"] / 1000.0, events["x"], (timestep_range, neuron_range))[0]
    spike_event_histogram = np.minimum(spike_event_histogram, 1).astype(bool)
    spike_event_bits = np.packbits(spike_event_histogram, axis=1, bitorder="little")
    shd_spikes.append(spike_event_bits.view(np.uint32).flatten())
    shd_labels.append(label)

init_logging()

# Input spikes
input_spikes = EventContainer(Shape(input_shape), num_timesteps)

# Model
rng_init = RNGInit()
hidden = ALIF(hidden_shape, 20.0, 2000.0, 4, 0.6, 0.0174,
              record, 7, name="hidden")
output = LI(output_shape, 20.0, num_timesteps, 11, name="output")

input_hidden = Linear(input_spikes, hidden.i, "s8_7_sat_t", name="input_hidden")
hidden_hidden = Linear(hidden.out_spikes, hidden.i, "s8_7_sat_t", name="hidden_hidden")
hidden_output = Linear(hidden.out_spikes, output.i, "s4_11_sat_t", name="hidden_output")

output_copy = Copy(output.v_avg)

# Group processes
init_processes = ProcessGroup([rng_init.process])
neuron_update_processes = ProcessGroup([hidden.process, output.process])
synapse_update_processes = ProcessGroup([input_hidden.process, hidden_hidden.process, 
                                         hidden_output.process])
copy_processes = ProcessGroup([output_copy.process])

# Create model
model = Model([init_processes, neuron_update_processes, synapse_update_processes, copy_processes])

# Create backend and use to generate sim code
backend = BackendFeNNHW() if device else BackendFeNNSim()
init_code = backend.generate_kernel([init_processes], model)
code = backend.generate_simulation_kernel([synapse_update_processes, neuron_update_processes],  # Update synapses and then neurons every timestep
                                          [copy_processes],
                                          num_timesteps, model)

# Disassemble if required
if disassemble_code:
    for i, c in enumerate(code):
        print(f"{i * 4} : {disassemble(c)}")

# Create runtime
runtime = Runtime(model, backend)

# Allocate memory for model
runtime.allocate()

# Load weights
load_and_push("99-Conn_Pop0_Pop1-g.bin", input_hidden.weight, runtime)
load_and_push("99-Conn_Pop1_Pop1-g.bin", hidden_hidden.weight, runtime)
load_and_push("99-Conn_Pop1_Pop2-g.bin", hidden_output.weight, runtime)
load_and_push("99-Pop2-Bias.bin", output.bias, runtime)

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

output_v_avg_array, _ = get_array_view(runtime, output.v_avg, np.int16)
output_v_avg_copy_array, output_v_avg_copy_view = get_array_view(runtime, output_copy.target,
                                                                 np.int16)
num_correct = 0
for spikes, label in tqdm(zip(shd_spikes, shd_labels),
                          total=len(shd_labels)):
    # Copy data to array host pointe
    input_spike_view[:] = spikes
    input_spike_array.push_to_device();

    # Classify
    runtime.run()

    # If we're recording, write input and hidden spikes to file
    #if record:
    #    recordSpikes("mnist_input_spikes_" + std::to_string(i) + ".csv", inputSpikeArray,
    #                 inputShape.getNumNeurons(), numTimesteps);
    #    recordSpikes("mnist_hidden_spikes_" + std::to_string(i) + ".csv", hiddenSpikeArray,
    #                 hiddenShape.getNumNeurons(), numTimesteps);

    # Copy output V sum from device
    output_v_avg_copy_array.pull_from_device();

    # Determine if output is correct
    classification = np.argmax(output_v_avg_copy_view)
    if classification == label:
        num_correct += 1

    # Push ORIGINAL output back to device (zeroing)
    output_v_avg_array.push_to_device()

print(f"{num_correct} / {len(shd_labels)} correct {100.0 * (num_correct / len(shd_labels))}%")
