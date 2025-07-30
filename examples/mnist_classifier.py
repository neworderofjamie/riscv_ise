import numpy as np
import mnist

from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model, 
                    PerformanceCounter, ProcessGroup, Runtime, Shape)
from models import Copy, LI, LIF, Linear

from pyfenn import disassemble, init_logging
from pyfenn.utils import (get_array_view, get_latency_spikes, load_and_push,
                          read_perf_counter, zero_and_push)
from tqdm.auto import tqdm

num_timesteps = 79
input_shape = [28 * 28]
hidden_shape = [128]
output_shape = [10]
input_hidden_shape = [28 * 28, 128]
hidden_output_shape = [128, 10]
device = False
record = False
time = True
disassemble_code = False

# Load and preprocess MNIST
mnist.datasets_url = "https://storage.googleapis.com/cvdf-datasets/mnist/"
mnist_spikes = get_latency_spikes(mnist.test_images())
mnist_labels = mnist.test_labels().astype(np.int16)
mnist_spikes = np.reshape(mnist_spikes, (mnist_spikes.shape[0], num_timesteps, -1))
init_logging()

# Input spikes
input_spikes = EventContainer(Shape(input_shape), 128)

# Model
hidden = LIF(hidden_shape, 20.0, 5, 0.61, record, 5, "hidden")
output = LI(output_shape, 20.0, num_timesteps, 6, "output")
input_hidden = Linear(input_spikes, hidden.i, "s10_5_sat_t", name="input_hidden")
hidden_output = Linear(hidden.out_spikes, output.i, "s9_6_sat_t", name="hidden_output")

output_copy = Copy(output.v_avg)

# Group processes
neuron_update_processes = ProcessGroup([hidden.process, output.process], PerformanceCounter() if time else None)
synapse_update_processes = ProcessGroup([input_hidden.process, hidden_output.process], PerformanceCounter() if time else None)
copy_processes = ProcessGroup([output_copy.process], PerformanceCounter() if time else None)

# Create model
model = Model([neuron_update_processes, synapse_update_processes, copy_processes])

# Create backend and use to generate sim code
backend = BackendFeNNHW() if device else BackendFeNNSim()
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
load_and_push("mnist_in_hid.bin", input_hidden.weight, runtime)
load_and_push("mnist_hid_out.bin", hidden_output.weight, runtime)
load_and_push("mnist_bias.bin", output.bias, runtime)

# Zero remaining state
zero_and_push(hidden.v, runtime)
zero_and_push(hidden.i, runtime)
zero_and_push(hidden.refrac_time, runtime)
zero_and_push(output.v, runtime)
zero_and_push(output.i, runtime)
zero_and_push(output.v_avg, runtime)

if time:
    zero_and_push(neuron_update_processes.performance_counter, runtime)
    zero_and_push(synapse_update_processes.performance_counter, runtime)
    zero_and_push(copy_processes.performance_counter, runtime)

# Set instructions
runtime.set_instructions(code)

# Loop through examples
input_spike_array, input_spike_view = get_array_view(runtime, input_spikes,
                                                     np.uint32, (128, -1))

# Zero end of input spike buffer
input_spike_view[num_timesteps:,:] = 0

hidden_spike_array = runtime.get_array(hidden.out_spikes)

output_v_avg_array, _ = get_array_view(runtime, output.v_avg, np.int16)
output_v_avg_copy_array, output_v_avg_copy_view = get_array_view(runtime, output_copy.target,
                                                                 np.int16)
num_correct = 0
for i in tqdm(range(len(mnist_labels))):
    # Copy data to array host pointe
    input_spike_view[:num_timesteps,:] = mnist_spikes[i]
    input_spike_array.push_to_device()

    # Classify
    runtime.run()

    # If we're recording, write input and hidden spikes to file
    #if record:
    #    recordSpikes("mnist_input_spikes_" + std::to_string(i) + ".csv", inputSpikeArray,
    #                 inputShape.getNumNeurons(), numTimesteps);
    #    recordSpikes("mnist_hidden_spikes_" + std::to_string(i) + ".csv", hiddenSpikeArray,
    #                 hiddenShape.getNumNeurons(), numTimesteps);

    # Copy output V sum from device
    output_v_avg_copy_array.pull_from_device()

    # Determine if output is correct
    classification = np.argmax(output_v_avg_copy_view)
    if classification == mnist_labels[i]:
        num_correct += 1

    # Push ORIGINAL output back to device (zeroing)
    output_v_avg_array.push_to_device()

print(f"{num_correct} / {len(mnist_labels)} correct {100.0 * (num_correct / len(mnist_labels))}%")

if time:
    neuron_update_cycles, neuron_update_instructions = read_perf_counter(
        neuron_update_processes.performance_counter, runtime)
    synapse_update_cycles, synapse_update_instructions = read_perf_counter(
        synapse_update_processes.performance_counter, runtime)
    copy_cycles, copy_instructions = read_perf_counter(
        copy_processes.performance_counter, runtime)
    
    print(f"Neuron update {neuron_update_cycles} cycles, {neuron_update_instructions} instruction ({neuron_update_instructions / neuron_update_cycles})")
    print(f"Synapse update {synapse_update_cycles} cycles, {synapse_update_instructions} instruction ({synapse_update_instructions / synapse_update_cycles})")
    print(f"Copy {copy_cycles} cycles, {copy_instructions} instruction ({copy_instructions / copy_cycles})")
