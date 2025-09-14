import numpy as np
import mnist

from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model,
                    NeuronUpdateProcess, PlogSeverity, ProcessGroup,
                    Parameter, PerformanceCounter, RoundingMode, Runtime, Shape,
                     Variable)
from pyfenn.models import Linear, Memset

from tonic.datasets import SHD

from pyfenn import disassemble, init_logging
from pyfenn.utils import (build_delay_weights, ceil_divide, copy_and_push, 
                          get_array_view, load_quantise_and_push, pull_spikes,
                          read_perf_counter, quantise, zero_and_push)
from tqdm.auto import tqdm

class LIF:
    def __init__(self, shape, tau_m: float, tau_syn: float, v_thresh: float,
                 fixed_point: int = 5, spike_record_timesteps: int = 1,
                 name: str = ""):
        self.shape = Shape(shape)
        decay_dtype = "s0_15_sat_t"
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"

        self.v = Variable(self.shape, dtype, name=f"{name}_v")
        self.i = Variable(self.shape, dtype, name=f"{name}_i")
        self.out_spikes = EventContainer(self.shape, spike_record_timesteps)
        self.process = NeuronUpdateProcess(
            f"""
            // Synapse
            s{15 - fixed_point}_{fixed_point}_sat_t inSyn = I * IScale;
            I *= Beta;

            // Neuron
            V = (Alpha * V) + (OneMinusAlpha * inSyn);
            
            if(V >= VThresh) {{
               Spike();
               V = 0.0h{fixed_point};
            }}
            """,
            {"Alpha": Parameter(np.exp(-1.0 / tau_m), decay_dtype),
             "OneMinusAlpha": Parameter(1.0 - np.exp(-1.0 / tau_m), decay_dtype),
             "Beta": Parameter(np.exp(-1.0 / tau_syn), decay_dtype),
             "IScale": Parameter(tau_syn * (1.0 - np.exp(-1.0 / tau_syn)), decay_dtype),
             "VThresh": Parameter(v_thresh, dtype)},
            {"V": self.v, "I": self.i},
            {"Spike": self.out_spikes},
            name)

class LI:
    def __init__(self, shape, tau_m: float, tau_syn: float, num_timesteps: int,
                 fixed_point: int = 5, name: str = ""):
        self.shape = Shape(shape)
        decay_dtype = "s0_15_sat_t"
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"

        self.v = Variable(self.shape, dtype, name=f"{name}_v")
        self.v_avg = Variable(self.shape, dtype, name=f"{name}_v_avg")
        self.i = Variable(self.shape, dtype, name=f"{name}_i")
        self.process = NeuronUpdateProcess(
            f"""
            // Synapse
            s{15 - fixed_point}_{fixed_point}_sat_t inSyn = I * IScale;
            I *= Beta;

            // Neuron
            V = (Alpha * V) + (OneMinusAlpha * inSyn);
            VAvg += (VAvgScale * V);
            """,
            {"Alpha": Parameter(np.exp(-1.0 / tau_m), decay_dtype), 
             "OneMinusAlpha": Parameter(1.0 - np.exp(-1.0 / tau_m), decay_dtype),
             "Beta": Parameter(np.exp(-1.0 / tau_syn), decay_dtype),
             "IScale": Parameter(tau_syn * (1.0 - np.exp(-1.0 / tau_syn)), decay_dtype),
             "VAvgScale": Parameter(1.0 / (num_timesteps / 2), decay_dtype)},
            {"V": self.v, "VAvg": self.v_avg, "I": self.i},
            {}, name)
        
num_timesteps = 1170
input_shape = [700]
hidden_shape = [256]
output_shape = [20]
input_hidden_shape = [input_shape[0], hidden_shape[0]]
hidden_hidden_shape = [hidden_shape[0], hidden_shape[0]]
hidden_output_shape = [hidden_shape[0], output_shape[0]]
device = False
record = False
time = True
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

init_logging(PlogSeverity.INFO)

# Input spikes
input_spikes = EventContainer(Shape(input_shape), num_timesteps)

# Model
hidden = LIF(hidden_shape, 20.0, 5.0, 1.0,
             8, 1, name="hidden")
output = LI(output_shape, 20.0, 5.0, num_timesteps, 8, name="output")

input_hidden = Linear(input_spikes, hidden.i, "s7_8_sat_t", name="input_hidden")
hidden_hidden = Linear(hidden.out_spikes, hidden.i, "s7_8_sat_t", name="hidden_hidden")
hidden_output = Linear(hidden.out_spikes, output.i, "s7_8_sat_t", name="hidden_output")

# Zero remaining state
hidden_i_zero = Memset(hidden.i)
hidden_v_zero = Memset(hidden.v)
output_i_zero = Memset(output.i)
output_v_zero = Memset(output.v)
output_v_avg_zero = Memset(output.v_avg)

# Group processes
neuron_update_processes = ProcessGroup([hidden.process, output.process],
                                       PerformanceCounter() if time else None)
synapse_update_processes = ProcessGroup([input_hidden.process, hidden_hidden.process, 
                                         hidden_output.process],
                                        PerformanceCounter() if time else None)
reset_processes = ProcessGroup([hidden_i_zero.process, hidden_v_zero.process,
                                output_i_zero.process, output_v_zero.process,
                                output_v_avg_zero.process],
                               PerformanceCounter() if time else None)

# Create backend
backend_params = {"rounding_mode": RoundingMode.STOCHASTIC}
backend = BackendFeNNHW(**backend_params) if device else BackendFeNNSim(**backend_params)

# Create model
model = Model([neuron_update_processes, synapse_update_processes, reset_processes],
              backend)

# Generate code
sim_code = backend.generate_simulation_kernel([synapse_update_processes, neuron_update_processes],  # Update synapses and then neurons every timestep
                                              [reset_processes], [],
                                              num_timesteps, model)

# Disassemble if required
if disassemble_code:
    print("Simulation:")
    for i, c in enumerate(sim_code):
        print(f"{i * 4} : {disassemble(c)}")

# Create runtime
runtime = Runtime(model, backend)

# Allocate memory for model
runtime.allocate()


# Load and quantise output weights
load_quantise_and_push("0/108-Conn_Pop0_Pop1-g.npy",    #+- 1
                       8, input_hidden.weight, runtime, input_shape[0], percentile=100.0)
load_quantise_and_push("0/108-Conn_Pop1_Pop1-g.npy",    #+- 1
                       8, hidden_hidden.weight, runtime, hidden_shape[0], percentile=100.0)
load_quantise_and_push("0/108-Conn_Pop1_Pop2-g.npy",    #+-3
                       8, hidden_output.weight, runtime, hidden_shape[0], True, percentile=100.0)

if time:
    zero_and_push(neuron_update_processes.performance_counter, runtime)
    zero_and_push(synapse_update_processes.performance_counter, runtime)
    zero_and_push(reset_processes.performance_counter, runtime)

# Set sim instructions
runtime.set_instructions(sim_code)

# Loop through examples
input_spike_array, input_spike_view = get_array_view(runtime, input_spikes,
                                                     np.uint32)
output_v_avg_array, output_v_avg_view = get_array_view(runtime, output.v_avg, np.int16)
num_correct = 0
for spikes, label in tqdm(zip(shd_spikes, shd_labels),
                          total=len(shd_labels)):
    # Copy data to array host pointe
    input_spike_view[:] = spikes
    input_spike_array.push_to_device()

    # Classify
    runtime.run()

    # Copy output V sum from device
    output_v_avg_array.pull_from_device()

    # Determine if output is correct
    classification = np.argmax(output_v_avg_view)
    if classification == label:
        num_correct += 1

print(f"{num_correct} / {len(shd_labels)} correct {100.0 * (num_correct / len(shd_labels))}%")

if time:
    neuron_update_cycles, neuron_update_instructions = read_perf_counter(
        neuron_update_processes.performance_counter, runtime)
    synapse_update_cycles, synapse_update_instructions = read_perf_counter(
        synapse_update_processes.performance_counter, runtime)
    reset_cycles, reset_instructions = read_perf_counter(
        reset_processes.performance_counter, runtime)
    print(f"Neuron update {neuron_update_cycles} cycles, {neuron_update_instructions} instruction ({neuron_update_instructions / neuron_update_cycles})")
    print(f"Synapse update {synapse_update_cycles} cycles, {synapse_update_instructions} instruction ({synapse_update_instructions / synapse_update_cycles})")
    print(f"Reset {reset_cycles} cycles, {reset_instructions} instruction ({reset_instructions / reset_cycles})")

