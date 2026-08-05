import numpy as np
import mnist

from argparse import ArgumentParser
import pyfenn.fenn_backend as backend

from pyfenn.fenn_backend import (EventSourceBuffer, PlogSeverity, 
                                 ProcessGroup, RuntimeHW, RuntimeSim, 
                                 SimulationLoopKernel)
from pyfenn.models import DenseLinear, Memset
from pyfenn.utils import PythonLogAppender
from models import LI, LIF

from pyfenn.fenn_backend import disassemble, init_logging
from pyfenn.utils import (get_views, get_latency_spikes, load_and_push,
                          read_perf_counter, zero_and_push)
from tqdm.auto import tqdm

num_timesteps = 79
input_shape = 28 * 28
hidden_shape = 128
output_shape = 10
input_hidden_shape = (input_shape, hidden_shape)
hidden_output_shape = (hidden_shape, output_shape)

parser = ArgumentParser("MNIST classifier")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--time", action="store_true", help="Record detailed timings using performance counters")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
args = parser.parse_args()

# Load and preprocess MNIST
mnist.datasets_url = "https://storage.googleapis.com/cvdf-datasets/mnist/"
mnist_spikes, max_spikes_per_image = get_latency_spikes(mnist.test_images())
mnist_labels = mnist.test_labels().astype(np.int16)

log_appender = PythonLogAppender()
init_logging(log_appender, PlogSeverity.DEBUG)

# Input spikes
input_spikes = backend.EventSourceBuffer(input_shape, num_timesteps)

# Model
hidden = LIF(backend, hidden_shape, 20.0, 5, 0.61, 1, 5, name="hidden")
output = LI(backend, output_shape, 20.0, num_timesteps, 6, name="output")
input_hidden = DenseLinear(backend, input_spikes, hidden.i, "s10_5_sat_t", name="input_hidden")
hidden_output = DenseLinear(backend, hidden.out_spikes, output.i, "s9_6_sat_t", name="hidden_output")

avg_zero = Memset(backend, output.v_avg)

# Group processes
neuron_update_processes = ProcessGroup([hidden.process, output.process])
synapse_update_processes = ProcessGroup([input_hidden.process, hidden_output.process])
zero_processes = ProcessGroup([avg_zero.process])

kernel = SimulationLoopKernel(
    num_timesteps, [synapse_update_processes, neuron_update_processes],
    [zero_processes], [])

# Create backend
runtime = RuntimeHW([kernel], 1) if args.device else RuntimeSim([kernel], 1)

# Disassemble if required
if args.disassemble:
    code = runtime.get_kernel_code(kernel)
    for i, c in enumerate(code):
        print(f"{i * 4} : {disassemble(c)}")

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

if args.time:
    zero_and_push(neuron_update_processes.performance_counter, runtime)
    zero_and_push(synapse_update_processes.performance_counter, runtime)
    zero_and_push(zero_processes.performance_counter, runtime)

# Loop through examples
input_spike_views = get_views(runtime, input_spikes, np.uint16)
assert len(input_spike_views) == 1

output_v_avg_views = get_views(runtime, output.v_avg, np.int16)
assert len(output_v_avg_views) == 1
num_correct = 0
for i in tqdm(range(len(mnist_labels))):
    # Copy data to array host pointe
    input_spike_views[0][:] = mnist_spikes[i]
    runtime.push_state_to_device(input_spikes)

    # Classify
    runtime.run(kernel)

    # Copy output V sum from device
    runtime.pull_state_from_device(output.v_avg)

    # Determine if output is correct
    classification = np.argmax(output_v_avg_views[0])
    if classification == mnist_labels[i]:
        num_correct += 1


print(f"{num_correct} / {len(mnist_labels)} correct {100.0 * (num_correct / len(mnist_labels))}%")

if args.time:
    neuron_update_cycles, neuron_update_instructions = read_perf_counter(
        neuron_update_processes.performance_counter, runtime)
    synapse_update_cycles, synapse_update_instructions = read_perf_counter(
        synapse_update_processes.performance_counter, runtime)
    zero_cycles, zero_instructions = read_perf_counter(
        zero_processes.performance_counter, runtime)
    
    print(f"Neuron update {neuron_update_cycles} cycles, {neuron_update_instructions} instruction ({neuron_update_instructions / neuron_update_cycles})")
    print(f"Synapse update {synapse_update_cycles} cycles, {synapse_update_instructions} instruction ({synapse_update_instructions / synapse_update_cycles})")
    print(f"Zero {zero_cycles} cycles, {zero_instructions} instruction ({zero_instructions / zero_cycles})")
