import numpy as np
import mnist

from argparse import ArgumentParser
from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model, 
                    PerformanceCounter, ProcessGroup, Runtime, Shape)
from pyfenn.models import Linear, Memset
from models import LI, LIF, Bernoulli, LIF_STDP

from pyfenn import disassemble, init_logging
from pyfenn.utils import (get_array_view, get_latency_spikes, copy_and_push,
                          read_perf_counter, zero_and_push, quantise)
from tqdm.auto import tqdm
import matplotlib.pyplot as plt

# One trial can consist of several timesteps, determining how many
# times the processes are run
num_timesteps = 1
input_shape = 10
output_shape = 1
num_trials = 1000
# Pretend as if 1000 trials correspond to 1 second
trials_per_second = 1000

input_output_shape = (input_shape, output_shape)

parser = ArgumentParser("Poisson population")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--time", action="store_true", help="Record detailed timings using performance counters")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
args = parser.parse_args()


init_logging()

num_fixed_point_bits=6
# Model
v_threshold = 1
postsyn_v_thresh_LTP = v_threshold*.8

# weird things happen when prob_spike can't be expressed as a fraction with denom=64
input = Bernoulli(Shape(input_shape),prob_spike=(2**4)/(2**6),record_timesteps=1,fixed_point=num_fixed_point_bits,name="input")
output = LIF_STDP(output_shape, alpha=.01, v_thresh=v_threshold, v_reset=0, record_timesteps=1, fixed_point=num_fixed_point_bits, dt=1, name="output")
input_output = Linear(input.out_spikes, output.i, "s9_6_sat_t", name="input_output")

# Note that memset will clear the variable at the end of the trial!!
# v_zero = Memset(output.v)

# Group processes
# neuron_update_processes = ProcessGroup([input.process, output.process], PerformanceCounter() if args.time else None)
neuron_update_processes = ProcessGroup([input.process, output.process], PerformanceCounter() if args.time else None)
synapse_update_processes = ProcessGroup([input_output.process], PerformanceCounter() if args.time else None)
# zero_processes = ProcessGroup([v_zero.process], PerformanceCounter() if args.time else None)

# Create backend
backend = BackendFeNNHW() if args.device else BackendFeNNSim()

# Create model
model = Model([neuron_update_processes, synapse_update_processes],
              backend)

# Generate sim code
code = backend.generate_simulation_kernel([synapse_update_processes,neuron_update_processes],  # Update synapses and then neurons every timestep
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
for idx in range(0,80,8):
    weights[idx]=2**1

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
input_spike_array, input_spike_view = get_array_view(runtime, input.out_spikes, np.uint32)
output_v_array, output_v_view = get_array_view(runtime, output.v, np.int16)
output_spike_array, output_spike_view = get_array_view(runtime, output.out_spikes, np.int16)

neural_activity = [[0] * 3 for i in range(num_trials)]

for i in range(num_trials):
    # run the model
    runtime.run()

    # Copy input spike array and output voltage from device
    input_spike_array.pull_from_device()
    output_v_array.pull_from_device()
    output_spike_array.pull_from_device()
    # print("Input spikes: ", input_spike_view)
    # print("Output voltages: ", output_v_view[0]/(2**num_fixed_point_bits))
    # print("Output spikes: ", output_spike_view[0])
    # print()
    neural_activity[i][0] = input_spike_view[0]
    neural_activity[i][1] = output_v_view[0]/(2**num_fixed_point_bits)
    neural_activity[i][2] = output_spike_view[0]

presyn_spikes = [neural_act[0] for neural_act in neural_activity]
postsyn_voltages = [neural_act[1] for neural_act in neural_activity]
postsyn_spikes = [neural_act[2] for neural_act in neural_activity]

postsyn_spike_rate = np.sum(postsyn_spikes) / (num_trials/trials_per_second)

fig, axes = plt.subplots(4, sharex=True, figsize=(12, 5))
fig.tight_layout(pad=2.0)

# plot postsyn V
axes[2].title.set_text('Postsynaptic voltage V(t) (Spike rate: ' + str(postsyn_spike_rate) + " Hz)")
axes[2].plot(postsyn_voltages)
axes[2].set_ylim((0,v_threshold*1.5))
axes[2].axhline(v_threshold, linestyle="--", color="black", linewidth=0.5)
axes[2].axhline(postsyn_v_thresh_LTP, linestyle="--", color="black", linewidth=0.5)
postsyn_spike_times = np.where(np.array(postsyn_spikes) == 1)[0]
for s in postsyn_spike_times:
    axes[2].axvline(s, color="red", linewidth=0.5)


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
