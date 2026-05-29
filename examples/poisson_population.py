import numpy as np
import mnist

from argparse import ArgumentParser
from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model, 
                    PerformanceCounter, ProcessGroup, Runtime, Shape)
from pyfenn.models import Linear, Memset
from models import LI, LIF, Bernoulli

from pyfenn import disassemble, init_logging
from pyfenn.utils import (get_array_view, get_latency_spikes, copy_and_push,
                          read_perf_counter, zero_and_push, quantise)
from tqdm.auto import tqdm

num_timesteps = 1
input_shape = 10
output_shape = 1
input_output_shape = (input_shape, output_shape)

parser = ArgumentParser("Poisson population")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--time", action="store_true", help="Record detailed timings using performance counters")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
args = parser.parse_args()


init_logging()


# Model
input = Bernoulli(shape=10, prob_spike=.5,
                 record_timesteps= 1, fixed_point= 9, name= "")
output = LI(output_shape, .001, num_timesteps, 6, name="output")
input_output = Linear(input.out_spikes, output.i, "s9_6_sat_t", name="input_output")

avg_zero = Memset(output.v_avg)

# Group processes
neuron_update_processes = ProcessGroup([input.process, output.process], PerformanceCounter() if args.time else None)
synapse_update_processes = ProcessGroup([input_output.process], PerformanceCounter() if args.time else None)
zero_processes = ProcessGroup([avg_zero.process], PerformanceCounter() if args.time else None)

# Create backend
backend = BackendFeNNHW() if args.device else BackendFeNNSim()

# Create model
model = Model([neuron_update_processes, synapse_update_processes, zero_processes],
              backend)

# Generate sim code
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

# Load weights
# print(np.asarray(runtime.get_array(input_output.weight).host_view).shape)
# for some reason the input_output.weight variable is a 640 element uint8 array, 
# so the data we push to it must be the same?
copy_and_push(np.ones(input_shape*8,dtype='uint64')*2, input_output.weight, runtime)
# I could instead do 
# copy_and_push(np.ones(input_shape*64,dtype='uint8'), input_output.weight, runtime)

# Zero remaining state
zero_and_push(output.v, runtime)
zero_and_push(output.i, runtime)
zero_and_push(output.v_avg, runtime)

if args.time:
    zero_and_push(neuron_update_processes.performance_counter, runtime)
    zero_and_push(synapse_update_processes.performance_counter, runtime)
    zero_and_push(zero_processes.performance_counter, runtime)

# Set instructions
runtime.set_instructions(code)

# Run the model
runtime.run()
runtime.run()
runtime.run()

# Initialize variables to hold input spike and output voltages
input_spike_array, input_spike_view = get_array_view(runtime, input.out_spikes,
                                                     np.uint32)
output_v_avg_array, output_v_avg_view = get_array_view(runtime, output.v_avg, np.int16)


# Copy input spike array and output voltage from device
input_spike_array.pull_from_device()
output_v_avg_array.pull_from_device()
print(input_spike_view)
print(output_v_avg_view)


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
