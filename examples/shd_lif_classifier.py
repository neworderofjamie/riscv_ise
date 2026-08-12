import numpy as np
import mnist
import pyfenn.fenn_backend as backend

from argparse import ArgumentParser
from pyfenn.models import DenseLinear, Memset
from pyfenn.utils import PythonLogAppender

from tonic.datasets import SHD

from pyfenn.utils import (ceil_divide, copy_and_push, convert_tonic_spikes,
                          get_views, load_quantise_and_push, pull_spikes,
                          read_perf_counter, quantise, zero_and_push)
from tqdm.auto import tqdm

class LIF:
    def __init__(self, backend, shape, tau_m: float, tau_syn: float,
                 v_thresh: float, fixed_point: int = 5, 
                 spike_record_timesteps: int = 1, name: str = ""):
        self.shape = shape
        decay_dtype = "s0_15_sat_t"
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"
        
        i_scale = tau_syn * (1.0 - np.exp(-1.0 / tau_syn))
        alpha = np.exp(-1.0 / tau_m)
        beta = np.exp(-1.0 / tau_syn)
        self.v = backend.Variable(self.shape, dtype, name=f"{name}_v")
        self.i = backend.Variable(self.shape, dtype, name=f"{name}_i")
        self.out_spikes = backend.EventChannel(self.shape)
        self.process = backend.NeuronUpdateProcess(
            f"""
            // Synapse
            s{15 - fixed_point}_{fixed_point}_sat_t inSyn = I * {i_scale}h15;
            I *= {beta}h15;

            // Neuron
            V = ({alpha}h15 * V) + ({1.0 - alpha}h15 * inSyn);
            
            if(V >= {v_thresh}h{fixed_point}) {{
               Spike();
               V = 0.0h{fixed_point};
            }}
            """,
            {"V": self.v, "I": self.i},
            {"Spike": self.out_spikes},
            name=name)

class LI:
    def __init__(self, backend, shape, tau_m: float, tau_syn: float,
                 num_timesteps: int, fixed_point: int = 5, name: str = ""):
        self.shape = shape
        decay_dtype = "s0_15_sat_t"
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"
        
        i_scale = tau_syn * (1.0 - np.exp(-1.0 / tau_syn))
        v_avg_scale = 1.0 / (num_timesteps / 2)
        alpha = np.exp(-1.0 / tau_m)
        beta = np.exp(-1.0 / tau_syn)
        
        self.v = backend.Variable(self.shape, dtype, name=f"{name}_v")
        self.v_avg = backend.Variable(self.shape, dtype, name=f"{name}_v_avg")
        self.i = backend.Variable(self.shape, dtype, name=f"{name}_i")
        self.process = backend.NeuronUpdateProcess(
            f"""
            // Synapse
            s{15 - fixed_point}_{fixed_point}_sat_t inSyn = I * {i_scale}h15;
            I *= {beta}h15;

            // Neuron
            V = ({alpha}h15 * V) + ({1.0 - alpha}h15 * inSyn);
            VAvg += ({v_avg_scale}h15 * V);
            """,
            {"V": self.v, "VAvg": self.v_avg, "I": self.i}, {},
            name=name)
        
num_timesteps = 1170
input_shape = (700,)
hidden_shape = (256,)
output_shape = (20,)
input_hidden_shape = (input_shape, hidden_shape)
hidden_hidden_shape = (hidden_shape, hidden_shape)
hidden_output_shape = (hidden_shape, output_shape)

parser = ArgumentParser("SHD classifier using LIF neurons")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--time", action="store_true", help="Record detailed timings using performance counters")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
args = parser.parse_args()

# Load and preprocess SHD
dataset = SHD(save_to="data", train=False)

# Loop through dataset
shd_spikes = []
shd_labels = []
for events, label in tqdm(dataset, "Preprocessing dataset"):
    # Convert events into a spike array
    spike_array = convert_tonic_spikes(events, dataset.ordering,
                                       dataset.sensor_size, 
                                       max_time=num_timesteps)
    shd_spikes.append(spike_array)
    shd_labels.append(label)

# Calculate maximum spike array length
max_spike_array_length = max(len(s) for s in shd_spikes)

#log_appender = PythonLogAppender()
#backend.init_logging(log_appender, backend.PlogSeverity.DEBUG)

# Input spikes
input_spikes = backend.EventSourceBuffer(input_shape, max_spike_array_length)

# Model
hidden = LIF(backend, hidden_shape, 20.0, 5.0, 1.0,
             8, 1, name="hidden")
output = LI(backend, output_shape, 20.0, 5.0, num_timesteps, 8, name="output")

input_hidden = DenseLinear(backend, input_spikes, hidden.i, "s7_8_sat_t", name="input_hidden")
hidden_hidden = DenseLinear(backend, hidden.out_spikes, hidden.i, "s7_8_sat_t", name="hidden_hidden")
hidden_output = DenseLinear(backend, hidden.out_spikes, output.i, "s7_8_sat_t", name="hidden_output")

# Zero remaining state
hidden_i_zero = Memset(backend, hidden.i)
hidden_v_zero = Memset(backend, hidden.v)
output_i_zero = Memset(backend, output.i)
output_v_zero = Memset(backend, output.v)
output_v_avg_zero = Memset(backend, output.v_avg)

# Group processes
neuron_update_processes = backend.ProcessGroup([hidden.process, output.process])
synapse_update_processes = backend.ProcessGroup([input_hidden.process, hidden_hidden.process, 
                                                 hidden_output.process])
reset_processes = backend.ProcessGroup([hidden_i_zero.process, hidden_v_zero.process,
                                        output_i_zero.process, output_v_zero.process,
                                        output_v_avg_zero.process])

# Create kernel
kernel = backend.SimulationLoopKernel(
    num_timesteps, [synapse_update_processes, neuron_update_processes],
    [reset_processes], [])
    
# Create runtime
runtime_params = {"neuron_update_rounding_mode": backend.RoundingMode.STOCHASTIC}
runtime = (backend.RuntimeHW([kernel], 1, **runtime_params) if args.device 
           else backend.RuntimeSim([kernel], 1, **runtime_params))


# Disassemble if required
if args.disassemble:
    print("Simulation:")
    code = runtime.get_kernel_code(kernel)
    for i, c in enumerate(code):
        print(f"{i * 4} : {backend.disassemble(c)}")

# Allocate memory for model
runtime.allocate()

# Load and quantise output weights
load_quantise_and_push("0/108-Conn_Pop0_Pop1-g.npy",    #+- 1
                       8, input_hidden.weight, runtime, percentile=100.0)
load_quantise_and_push("0/108-Conn_Pop1_Pop1-g.npy",    #+- 1
                       8, hidden_hidden.weight, runtime, percentile=100.0)
load_quantise_and_push("0/108-Conn_Pop1_Pop2-g.npy",    #+-3
                       8, hidden_output.weight, runtime, percentile=100.0)

if args.time:
    zero_and_push(neuron_update_processes.performance_counter, runtime)
    zero_and_push(synapse_update_processes.performance_counter, runtime)
    zero_and_push(reset_processes.performance_counter, runtime)

# Loop through examples
input_spike_views = get_views(runtime, input_spikes)
assert len(input_spike_views) == 1

output_v_avg_views = get_views(runtime, output.v_avg)
assert len(output_v_avg_views) == 1

num_correct = 0
for spikes, label in tqdm(zip(shd_spikes, shd_labels),
                          total=len(shd_labels), desc="Simulating"):
    # Copy data to array host pointe
    input_spike_views[0][:len(spikes)] = spikes
    runtime.push_state_to_device(input_spikes)

    # Classify
    runtime.run(kernel)

    # Copy output V sum from device
    runtime.pull_state_from_device(output.v_avg)

    # Determine if output is correct
    classification = np.argmax(output_v_avg_views[0])
    if classification == label:
        num_correct += 1

print(f"{num_correct} / {len(shd_labels)} correct {100.0 * (num_correct / len(shd_labels))}%")

if args.time:
    neuron_update_cycles, neuron_update_instructions = read_perf_counter(
        neuron_update_processes.performance_counter, runtime)
    synapse_update_cycles, synapse_update_instructions = read_perf_counter(
        synapse_update_processes.performance_counter, runtime)
    reset_cycles, reset_instructions = read_perf_counter(
        reset_processes.performance_counter, runtime)
    print(f"Neuron update {neuron_update_cycles} cycles, {neuron_update_instructions} instruction ({neuron_update_instructions / neuron_update_cycles})")
    print(f"Synapse update {synapse_update_cycles} cycles, {synapse_update_instructions} instruction ({synapse_update_instructions / synapse_update_cycles})")
    print(f"Reset {reset_cycles} cycles, {reset_instructions} instruction ({reset_instructions / reset_cycles})")

