import numpy as np

from pyfenn import (BackendFeNNSim, EventContainer, EventPropagationProcess,
                    Model, NeuronUpdateProcess, NumericValue, Parameter,
                    ProcessGroup, Runtime, Shape, UnresolvedType, Variable)

from pyfenn import disassemble

def get_array_view(runtime: Runtime, state, dtype):
    array = runtime.get_array(state)
    view = np.asarray(array.host_view).view(dtype)
    assert not view.flags["OWNDATA"]
    return array, view

# Divide two integers, rounding up i.e. effectively taking ceil
def ceil_divide(numerator, denominator):
    return (numerator + denominator - 1) // denominator

def load_and_push(filename: str, state, runtime: Runtime):
    # Load data from file
    data = np.fromfile(filename, dtype=np.uint8)
    
    # Get array and view
    array, view = get_array_view(runtime, state, np.uint8)
    assert array.host_view.nbytes == data.nbytes

    # Copy data to array host pointer
    view[:] = data

    # Push to device
    array.push_to_device()

def zero_and_push(state, runtime: Runtime):
    # Get array and view
    array, view = get_array_view(runtime, state, np.uint8)
 
    # Zero
    view[:] = 0

    # Push to device
    array.push_to_device()

class LIF:
    def __init__(self, shape, tau_m: float, tau_refrac: int, v_thresh: float,
                 record_spikes: bool = False, dtype = "s10_5_sat_t"):
        self.shape = Shape(shape)
        dtype = UnresolvedType(dtype)
        self.v = Variable(self.shape, dtype)
        self.i = Variable(self.shape, dtype)
        self.refrac_time = Variable(self.shape, UnresolvedType("int16_t"))
        self.out_spikes = EventContainer(self.shape, 
                                         (num_timesteps if record_spikes
                                          else 1))
        self.process = NeuronUpdateProcess(
            """
            V = (Alpha * V) + I;
            I = 0.0h5;
            if (RefracTime > 0) {
               RefracTime -= 1;
            }
            else if(V >= VThresh) {
               Spike();
               V -= VThresh;
               RefracTime = TauRefrac;
            }
            """,
            {"Alpha": Parameter(NumericValue(np.exp(-1.0 / tau_m)), dtype),
             "VThresh": Parameter(NumericValue(v_thresh), dtype),
             "TauRefrac": Parameter(NumericValue(tau_refrac), UnresolvedType("int16_t"))},
            {"V": self.v, "I": self.i, "RefracTime": self.refrac_time},
            {"Spike": self.out_spikes})

class LI:
    def __init__(self, shape, tau_m: float, num_timesteps: int,
                 dtype: str = "s10_5_sat_t"):
        self.shape = Shape(shape)
        dtype = UnresolvedType(dtype)

        self.v = Variable(self.shape, dtype)
        self.i = Variable(self.shape, dtype)
        self.v_avg = Variable(self.shape, dtype)
        self.bias = Variable(self.shape, dtype)
        self.process = NeuronUpdateProcess(
            """
            V = (Alpha * V) + I + Bias;
            I = 0.0h6;
            VAvg += (VAvgScale * V);
            """,
            {"Alpha": Parameter(NumericValue(np.exp(-1.0 / tau_m)), dtype), 
             "VAvgScale": Parameter(NumericValue(1.0 / (num_timesteps / 2)), dtype)},
            {"V": self.v, "VAvg": self.v_avg, "I": self.i, "Bias": self.bias})

class Linear:
    def __init__(self, source_events: EventContainer, target_var: Variable,
                 weight_dtype: str):
        self.shape = Shape([source_events.shape.num_neurons,
                            target_var.shape.num_neurons])
        weight_dtype = UnresolvedType(weight_dtype)

        self.weight = Variable(self.shape, weight_dtype)
        self.process = EventPropagationProcess(source_events, self.weight,
                                               target_var)

num_examples = 10000
num_timesteps = 79
input_shape = [28 * 28]
hidden_shape = [128]
output_shape = [10]
input_hidden_shape = [28 * 28, 128]
hidden_output_shape = [128, 10]
device = False
record = False
disassemble = False

# **YUCK** implementation detail
num_input_spike_words = ceil_divide(28 * 28, 32)
num_input_spike_array_words = num_input_spike_words * num_timesteps
    
# Input spikes
input_spikes = EventContainer(Shape(input_shape), num_timesteps)

# Model
hidden = LIF(hidden_shape, 20.0, 5, 0.61, record, "s10_5_sat_t")
output = LI(output_shape, 20.0, num_timesteps, "s9_6_sat_t")
input_hidden = Linear(input_spikes, hidden.i, "s10_5_sat_t")
hidden_output = Linear(hidden.out_spikes, output.i, "s9_6_sat_t")

# Group processes
neuron_update_processes = ProcessGroup([hidden.process, output.process])
synapse_update_processes = ProcessGroup([input_hidden.process, hidden_output.process])

# Create model
model = Model([neuron_update_processes, synapse_update_processes])

# Create backend and use to generate sim code
# **YUCK** simulate flag
backend = BackendFeNNSim()
code = backend.generate_simulation_kernel(synapse_update_processes, neuron_update_processes, 
                                          num_timesteps, True, model)

# Disassemble if required
if disassemble:
    for i, c in enumerate(code):
        print(f"{i * 4} : {disassemble(c)}")

# Create runtime
runtime = Runtime(model, backend)

# Set instructions
runtime.set_instructions(code)

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

# Load data
mnist_spikes = np.fromfile("mnist_spikes.bin", dtype=np.uint32)
mnist_labels = np.fromfile("mnist_labels.bin", dtype=np.int16)

# Loop through examples
input_spike_array, input_spike_view = get_array_view(runtime, input_spikes,
                                                     np.uint32)
hidden_spike_array = runtime.get_array(hidden.out_spikes)

output_v_avg_array, output_v_avg_view = get_array_view(runtime, output.v_avg,
                                                       np.int16)
num_correct = 0
for i in range(num_examples):
    # Copy data to array host pointe
    start_word = i * num_input_spike_array_words
    input_spike_view[:] = mnist_spikes[start_word: start_word + num_input_spike_array_words]
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
    output_v_avg_array.pull_from_device();

    # Determine if output is correct
    classification = np.argmax(output_v_avg_view)
    if classification == mnist_labels[i]:
        num_correct += 1

    # Zero output and push
    output_v_avg_view[:] = 0
    output_v_avg_array.push_to_device()

print(f"{num_correct} / {num_examples} correct {100.0 * (num_correct / num_examples)}%")
