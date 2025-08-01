import numpy as np
import mnist

from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model,
                    NeuronUpdateProcess, NumericValue, ProcessGroup, Parameter,
                    Runtime, Shape, UnresolvedType, Variable)
from models import Copy, LI, Linear, RNGInit
from tonic.datasets import SHD

from pyfenn import disassemble, init_logging
from pyfenn.utils import (build_delay_weights, ceil_divide, copy_and_push, 
                          get_array_view, load_quantise_and_push, quantise, 
                          seed_and_push, zero_and_push)
from tqdm.auto import tqdm

class LIF:
    def __init__(self, shape, tau_m: float, tau_syn: float, v_thresh: float,
                 fixed_point: int = 5, spike_record_timesteps: int = 1,
                 i_delay_timesteps: int = 1, name: str = ""):
        self.shape = Shape(shape)
        decay_dtype = UnresolvedType("s0_15_sat_t")
        dtype = UnresolvedType(f"s{15 - fixed_point}_{fixed_point}_sat_t")
        self.v = Variable(self.shape, dtype, name=f"{name}_v")
        self.i = Variable(self.shape, dtype, i_delay_timesteps, name=f"{name}_i")
        self.out_spikes = EventContainer(self.shape, spike_record_timesteps)
        self.process = NeuronUpdateProcess(
            f"""
            // Synapse
            s{15 - fixed_point}_{fixed_point}_sat_t inSyn = I * IScale;
            I *= Beta;

            // Neuron
            V = (Alpha * V) + inSyn;
            
            if(V >= VThresh) {{
               Spike();
               V = 0.0h{fixed_point};
            }}
            """,
            {"Alpha": Parameter(NumericValue(np.exp(-1.0 / tau_m)), decay_dtype),
             "Beta": Parameter(NumericValue(np.exp(-1.0 / tau_syn)), decay_dtype),
             "IScale": Parameter(NumericValue((1.0 - np.exp(-1.0 / tau_m)) * tau_syn * (1.0 - np.exp(-1.0 / tau_syn))), dtype),
             "VThresh": Parameter(NumericValue(v_thresh), dtype)},
            {"V": self.v, "I": self.i},
            {"Spike": self.out_spikes},
            name)

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
for i in range(100):
    events, label = dataset[i]
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
hidden = LIF(hidden_shape, 20.0, 5.0, 1.0,
             8, 1, 64, name="hidden")
output = LI(output_shape, 20.0, num_timesteps, 8, name="output")

input_hidden = Linear(input_spikes, hidden.i, "s7_8_sat_t", 
                      num_delay_bits=6, name="input_hidden")
hidden_hidden = Linear(hidden.out_spikes, hidden.i, "s7_8_sat_t", 
                       num_delay_bits=6, name="hidden_hidden")
hidden_output = Linear(hidden.out_spikes, output.i, "s7_8_sat_t", name="hidden_output")

output_copy = Copy(output.v_avg)

# Group processes
neuron_update_processes = ProcessGroup([hidden.process, output.process])
synapse_update_processes = ProcessGroup([input_hidden.process, hidden_hidden.process, 
                                         hidden_output.process])
copy_processes = ProcessGroup([output_copy.process])

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

# Load and round delays
in_hid_delays = np.round(np.load("checkpoints_6_1_256_62_1_0_1.0_1_5e-12/best-Conn_Pop0_Pop1-d.npy")).astype(np.uint16)
hid_hid_delays = np.round(np.load("checkpoints_6_1_256_62_1_0_1.0_1_5e-12/best-Conn_Pop1_Pop1-d.npy")).astype(np.uint16)

# Load and quantise weights
in_hid_weights = quantise(np.load("checkpoints_6_1_256_62_1_0_1.0_1_5e-12/best-Conn_Pop0_Pop1-g.npy"),
                          8, input_shape[0], True)
                          
hid_hid_weights = quantise(np.load("checkpoints_6_1_256_62_1_0_1.0_1_5e-12/best-Conn_Pop1_Pop1-g.npy"),
                           8, hidden_shape[0], True) 

# Combine weights and delays and push
copy_and_push(build_delay_weights(in_hid_weights, in_hid_delays, 6),
              input_hidden.weight, runtime)
copy_and_push(build_delay_weights(hid_hid_weights, hid_hid_delays, 6),
              hidden_hidden.weight, runtime)

# Load and quantise output weights
load_quantise_and_push("checkpoints_6_1_256_62_1_0_1.0_1_5e-12/best-Conn_Pop1_Pop2-g.npy",
                       8, hidden_output.weight, runtime, hidden_shape[0], True)

# Zero remaining state
zero_and_push(hidden.v, runtime)
zero_and_push(hidden.i, runtime)
zero_and_push(output.v, runtime)
zero_and_push(output.bias, runtime) # Optimise out
zero_and_push(output.v_avg, runtime)

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
for spikes, label in tqdm(zip(shd_spikes[:100], shd_labels),
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
