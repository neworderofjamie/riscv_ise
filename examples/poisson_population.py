import numpy as np
import mnist

from argparse import ArgumentParser
from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model, 
                    PerformanceCounter, ProcessGroup, Runtime, Shape)
from pyfenn.models import Linear, Memset
from models import LI, LI_SIMPLE, LIF, Bernoulli, LIF_STDP

from pyfenn import disassemble, init_logging
from pyfenn.utils import (get_array_view, get_latency_spikes, copy_and_push,
                          read_perf_counter, zero_and_push, quantise)
from tqdm.auto import tqdm

# One trial can consist of several timesteps, determining how many
# times the processes are run
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

num_fixed_point_bits=6
# Model
# input = Bernoulli(shape=10, prob_spike=.5,
#                  record_timesteps= 1, fixed_point= num_fixed_point_bits, name= "")

input = EventContainer(Shape(input_shape), num_timesteps)
# output = LI_SIMPLE(output_shape, tau_m=.0001, num_timesteps=2, fixed_point=num_fixed_point_bits, name="output")
output = LIF_STDP(output_shape, alpha=.01, v_thresh=5, v_reset=0, record_timesteps=1, fixed_point=num_fixed_point_bits, dt=1, name="output")
# input_output = Linear(input.out_spikes, output.i, "s9_6_sat_t", name="input_output")
input_output = Linear(input, output.i, "s9_6_sat_t", name="input_output")

# Note that memset will clear the variable at the end of the trial!!
# v_zero = Memset(output.v)

# Group processes
# neuron_update_processes = ProcessGroup([input.process, output.process], PerformanceCounter() if args.time else None)
neuron_update_processes = ProcessGroup([output.process], PerformanceCounter() if args.time else None)
synapse_update_processes = ProcessGroup([input_output.process], PerformanceCounter() if args.time else None)
# zero_processes = ProcessGroup([v_zero.process], PerformanceCounter() if args.time else None)

# Create backend
backend = BackendFeNNHW() if args.device else BackendFeNNSim()

# Create model
model = Model([neuron_update_processes, synapse_update_processes],
              backend)

# Generate sim code
code = backend.generate_simulation_kernel([synapse_update_processes, neuron_update_processes],  # Update synapses and then neurons every timestep
                                          [], [],
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
# input_output.weight variable is an 80 element array where every 8*ith element
# corresponds to the i-th synapse (e.g., index 16 is the third synapse). Each 
# element corresponds to a fixed point int with 6 fractional bits, so 32 is .5,
# 64 is 1, 128 is 2.
weights = np.zeros(input_shape*8,dtype='uint64')*64
weights[0]=2**6
weights[8]=2**6
copy_and_push(weights, input_output.weight, runtime)
# I could instead do 
# copy_and_push(np.ones(input_shape*64,dtype='uint8'), input_output.weight, runtime)

# Zero remaining state
zero_and_push(output.v, runtime)
zero_and_push(output.i, runtime)

if args.time:
    zero_and_push(neuron_update_processes.performance_counter, runtime)
    zero_and_push(synapse_update_processes.performance_counter, runtime)
    zero_and_push(zero_processes.performance_counter, runtime)

# Set instructions
runtime.set_instructions(code)

# Initialize variables to hold input spike and output voltages
input_spike_array, input_spike_view = get_array_view(runtime, input, np.uint32)
output_v_array, output_v_view = get_array_view(runtime, output.v, np.int16)
output_spike_array, output_spike_view = get_array_view(runtime, output.out_spikes, np.int16)


for i in range(10):
    # Change the input spike array and push to device
    # Use a 32 bit unsigned int to determine which of the 32 potential neurons have spiked 
    # e.g., input_spike_code = 5 means first and third neuron spiked (101)
    input_spike_code = 3
    input_spike_view[:] = np.array([input_spike_code],dtype=np.uint32)
    input_spike_array.push_to_device()

    # run the model
    runtime.run()

    # Copy input spike array and output voltage from device
    input_spike_array.pull_from_device()
    output_v_array.pull_from_device()
    output_spike_array.pull_from_device()
    print("Input spikes: ", input_spike_view)
    print("Output voltages: ", output_v_view[0]/(2**num_fixed_point_bits))
    print("Output spikes: ", output_spike_view[0])
    print()


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
