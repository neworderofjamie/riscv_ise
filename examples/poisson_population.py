import numpy as np
import mnist

from argparse import ArgumentParser
from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model, 
                    PerformanceCounter, ProcessGroup, Runtime, Shape)
from pyfenn.models import Linear, LinearWithSTDP, Memset, RNGInit
from models import LI, LIF, Bernoulli, Linear_LIF_STDP

from pyfenn import disassemble, init_logging, RNGInitProcess
from pyfenn.utils import (get_array_view, get_latency_spikes, copy_and_push,
                          read_perf_counter, zero_and_push, quantise, seed_and_push)
from tqdm.auto import tqdm
import matplotlib.pyplot as plt

# One trial can consist of several timesteps, determining how many
# times the processes are run
num_timesteps = 1
extra_input_shape = 10

primary_input_shape = 1
output_shape = 1
num_trials = 1500
# Pretend as if 1000 trials correspond to 1 second
trials_per_second = 1000

extra_input_output_shape = (extra_input_shape, output_shape)

parser = ArgumentParser("Poisson population")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--time", action="store_true", help="Record detailed timings using performance counters")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
args = parser.parse_args()


init_logging()

num_int_bits=6
num_frac_bits = 15-num_int_bits

# Scaling Parameters
v_theta = 1
x_max = 1
j_c = 1

# Neural Parameters
lamb = .01
theta_v = .8 
v_reset = 0
prob_spike_primary = .07
prob_spike_extra = .1 # This is equal to 100Hz because 1000 samples/per second

# Calcium Parameters
tau_c =  60
theta_low_up=3
theta_low_down=3
theta_high_up=13
theta_high_down=4

# Synaptic Parameters
theta_x=.5
a=.1
b=.1
alpha=.0035
beta=.0035


# Input layer
j_plus = 1
j_minus = 0


# weird things happen when prob_spike can't be expressed as a fraction with denom=64
primary_input = Bernoulli(Shape(primary_input_shape),prob_spike=prob_spike_primary,record_timesteps=1,name="primary_input")
extra_input = Bernoulli(Shape(extra_input_shape),prob_spike=prob_spike_extra, record_timesteps=1,name="extra_input")


output = Linear_LIF_STDP(output_shape, lamb=lamb, tau_c=tau_c, j_c=j_c,v_thresh=v_theta, v_reset=v_reset, 
                         fixed_point=num_frac_bits,record_timesteps=1, dt=1, name="output")

extra_input_output = Linear(extra_input.out_spikes, output.i, f"s{num_int_bits}_{num_frac_bits}_sat_t", name="extra_input_output")

primary_input_output = LinearWithSTDP(primary_input.out_spikes, primary_input.time_since_last_spike, output.i, 
                                      output.v, output.c,f"s{num_int_bits}_{num_frac_bits}_sat_t", theta_x=theta_x,
                                      a=a, b=b, alpha=alpha, beta=beta,j_plus=j_plus, j_minus=j_minus, theta_v=theta_v, 
                                      theta_low_up=theta_low_up, theta_low_down=theta_low_down,
                                      theta_high_up=theta_high_up, theta_high_down=theta_high_down,
                                      x_max=x_max, name="primary_input_output")



# Group processes
neuron_update_processes = ProcessGroup([extra_input.process, primary_input.process, output.process], PerformanceCounter() if args.time else None,name="neuron_update_processes_group")
synapse_update_processes = ProcessGroup([extra_input_output.process, primary_input_output.process], PerformanceCounter() if args.time else None,name="synapse_update_processes_group")
# synapse_update_processes = ProcessGroup([extra_input_output.process], PerformanceCounter() if args.time else None,name="synapse_update_processes_group")

# zero_processes = ProcessGroup([v_zero.process], PerformanceCounter() if args.time else None)

# Initial processes
rng_init = RNGInit()
init_processes = ProcessGroup([rng_init.process],name="init_processes_group")

# Create backend
backend = BackendFeNNSim()

# Create model
model = Model([init_processes, neuron_update_processes, synapse_update_processes],
              backend)

# Generate sim code
code = backend.generate_simulation_kernel([synapse_update_processes,neuron_update_processes],  # Update synapses and then neurons every timestep
                                          [init_processes], [],
                                          num_timesteps, model)

# Disassemble if required
if args.disassemble:
    for i, c in enumerate(code):
        print(f"PC:{i * 4}   Inst:{c}  Disassembled:{disassemble(c)}")

# Create runtime
runtime = Runtime(model, backend)

# Allocate memory for model
runtime.allocate()

# Load weights
extra_input_weights = np.ones(32*10,dtype='int16') * np.round(.06 * (1 << num_frac_bits)).astype(np.int16)
copy_and_push(extra_input_weights, extra_input_output.weight, runtime)

# Here we set the synaptic variable X in the Fusi paper
primary_input_x = np.ones(32,dtype='int16') * np.round(0 * (1 << num_frac_bits)).astype(np.int16)
copy_and_push(primary_input_x, primary_input_output.x, runtime)

# Initialize calcium variable at 2
output_calcium =  np.ones(32,dtype='int16') * np.round(2.5 * (1 << num_frac_bits)).astype(np.int16)
copy_and_push(output_calcium, output.c, runtime)


# Zero remaining state
zero_and_push(output.v, runtime)
zero_and_push(output.i, runtime)
# zero_and_push(output.c, runtime)
zero_and_push(primary_input.time_since_last_spike, runtime)
zero_and_push(primary_input.next_time_since_last_spike, runtime)
zero_and_push(primary_input.out_spikes, runtime)
zero_and_push(extra_input.time_since_last_spike, runtime)
zero_and_push(extra_input.next_time_since_last_spike, runtime)
zero_and_push(extra_input.out_spikes, runtime)

if args.time:
    zero_and_push(neuron_update_processes.performance_counter, runtime)
    zero_and_push(synapse_update_processes.performance_counter, runtime)
    zero_and_push(zero_processes.performance_counter, runtime)

# Set instructions
runtime.set_instructions(code)

# Initialize variables to hold extra_input spike and output voltages
primary_input_spike_array, primary_input_spike_view = get_array_view(runtime, primary_input.out_spikes, np.uint32)
extra_input_spike_array, extra_input_spike_view = get_array_view(runtime, extra_input.out_spikes, np.uint32)
output_v_array, output_v_view = get_array_view(runtime, output.v, np.int16)
output_spike_array, output_spike_view = get_array_view(runtime, output.out_spikes, np.uint32)
output_c_array, output_c_view = get_array_view(runtime, output.c, np.int16)
primary_x_array, primary_x_view = get_array_view(runtime, primary_input_output.x,np.int16)

neural_activity = [[0] * 6 for i in range(num_trials)]

for i in range(num_trials):
    # Load the RNG seed
    seed_and_push(rng_init.seed, runtime)
    # run the model
    runtime.run()

    # Copy extra_input spike array and output voltage from device
    extra_input_spike_array.pull_from_device()
    primary_input_spike_array.pull_from_device()
    output_v_array.pull_from_device()
    output_c_array.pull_from_device()
    output_spike_array.pull_from_device()
    primary_x_array.pull_from_device()

    neural_activity[i][0] = extra_input_spike_view[0]
    neural_activity[i][1] = output_v_view[0]/(2**num_frac_bits)
    neural_activity[i][2] = output_spike_view[0]
    neural_activity[i][3] = primary_input_spike_view[0]
    neural_activity[i][4] = output_c_view[0]/(2**num_frac_bits)
    neural_activity[i][5] = primary_x_view[0]/(2**num_frac_bits)


extra_presyn_spikes = [neural_act[0] for neural_act in neural_activity]
postsyn_voltages = [neural_act[1] for neural_act in neural_activity]
postsyn_spikes = [neural_act[2] for neural_act in neural_activity]
primary_presyn_spikes = [neural_act[3] for neural_act in neural_activity]
postsyn_calcium = [neural_act[4] for neural_act in neural_activity]
primary_x = [neural_act[5] for neural_act in neural_activity]

postsyn_spike_rate = np.sum(postsyn_spikes) / (num_trials/trials_per_second)
primary_spike_rate = np.sum(primary_presyn_spikes) / (num_trials/trials_per_second)

fig, axes = plt.subplots(4, sharex=True, figsize=(12, 5))
fig.tight_layout(pad=2.0)

# plot presyn spikes
presyn_spike_times = np.where(np.array(primary_presyn_spikes) == 1)[0]
for s in presyn_spike_times:
    axes[0].set_xlim((0,num_trials))
    axes[0].axvline(s)
axes[0].title.set_text("Presynaptic spikes")

# plot X
axes[1].plot(primary_x)
axes[1].title.set_text("Synaptic internal variable X(t)")
for i in [theta_x, x_max]:
    axes[1].axhline(i, linestyle="--", color="black", linewidth=0.5)


# plot postsyn V
axes[2].title.set_text('Postsynaptic voltage V(t) (Spike rate: ' + str(postsyn_spike_rate) + " Hz)")
axes[2].plot(postsyn_voltages)
for i in [theta_v, v_theta]:
    axes[2].axhline(i, linestyle="--", color="black", linewidth=0.5)
postsyn_spike_times = np.where(np.array(postsyn_spikes) == 1)[0]
for s in postsyn_spike_times:
    axes[2].axvline(s, color="red", linewidth=0.5)

# plot C
axes[3].plot(postsyn_calcium)
axes[3].title.set_text("Calcium variable C(t)")
for i in [theta_low_up, theta_low_down, theta_high_up, theta_high_down]:
    axes[3].axhline(i, linestyle="--", color="black", linewidth=0.5)

plt.show()

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
