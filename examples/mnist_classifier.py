import numpy as np

from pyfenn import (BackendFeNNSim, EventContainer, EventPropagationProcess,
                    Model, NeuronUpdateProcess, NumericValue, Parameter,
                    ProcessGroup, Runtime, Shape, UnresolvedType, Variable)

# Divide two integers, rounding up i.e. effectively taking ceil
def ceil_divide(numerator, denominator):
    return (numerator + denominator - 1) // denominator

def load_and_push(filename: str, state, runtime: Runtime):
    # Load data from file
    data = np.fromfile(filename, dtype=np.uint8)
    
    # Get array
    array = runtime.get_array(state)
    
    # Get dtype view of host memoryview
    assert array.host_view.nbytes == data.nbytes
    view = np.asarray(array.host_view).view(np.uint8)
    assert not view.flags["OWNDATA"]

    # Copy data to array host pointer
    view[:] = data

    # Push to device
    array.push_to_device()

def zero_and_push(state, runtime: Runtime):
    # Get array
    array = runtime.get_array(state)
 
    # Zero
    view = np.asarray(array.host_view).view(np.uint8)
    assert not view.flags["OWNDATA"]
    view[:] = 0

    # Push to device
    array.push_to_device()

num_examples = 10000
num_timesteps = 79
input_shape = Shape([28 * 28])
hidden_shape = Shape([128])
output_shape = Shape([10])
input_hidden_shape = Shape([28 * 28, 128])
hidden_output_shape = Shape([128, 10])
device = False
record = False

# **YUCK** implementation detail
num_input_spike_words = ceil_divide(28 * 28, 32)
num_input_spike_array_words = num_input_spike_words * num_timesteps
    
# Input spikes
input_spikes = EventContainer(input_shape, num_timesteps)

# Hidden neurons
hidden_v = Variable(hidden_shape, UnresolvedType("s10_5_sat_t"))
hidden_i = Variable(hidden_shape, UnresolvedType("s10_5_sat_t"))
hidden_refrac_time = Variable(hidden_shape, UnresolvedType("int16_t"))
hidden_spikes = EventContainer(hidden_shape, num_timesteps if record else 1)
hidden = NeuronUpdateProcess(
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
    {"Alpha": Parameter(NumericValue(np.exp(-1.0 / 20.0)), UnresolvedType("s10_5_sat_t")),
     "VThresh": Parameter(NumericValue(0.61), UnresolvedType("s10_5_sat_t")),
     "TauRefrac": Parameter(NumericValue(5), UnresolvedType("int16_t"))},
    {"V": hidden_v, "I": hidden_i, "RefracTime": hidden_refrac_time},
    {"Spike": hidden_spikes})

# Output neurons
output_v = Variable(output_shape, UnresolvedType("s9_6_sat_t"))
output_i = Variable(output_shape, UnresolvedType("s9_6_sat_t"))
output_v_avg = Variable(output_shape, UnresolvedType("s9_6_sat_t"))
output_bias = Variable(output_shape, UnresolvedType("s9_6_sat_t"))
output = NeuronUpdateProcess(
    """
    V = (Alpha * V) + I + Bias;
    I = 0.0h6;
    VAvg += (VAvgScale * V);
    """,
    {"Alpha": Parameter(NumericValue(np.exp(-1.0 / 20.0)), UnresolvedType("s9_6_sat_t")), 
     "VAvgScale": Parameter(NumericValue(1.0 / (num_timesteps / 2)), UnresolvedType("s9_6_sat_t"))},
    {"V": output_v, "VAvg": output_v_avg, "I": output_i, "Bias": output_bias})

# Input->Hidden event propagation
input_hidden_weight = Variable(input_hidden_shape, UnresolvedType("s10_5_sat_t"))
input_hidden = EventPropagationProcess(input_spikes, input_hidden_weight, hidden_i)

# Hidden->Output event propagation
hidden_output_weight = Variable(hidden_output_shape, UnresolvedType("s9_6_sat_t"))
hidden_output = EventPropagationProcess(hidden_spikes, hidden_output_weight, output_i)

# Group processes
neuron_update_processes = ProcessGroup([hidden, output])
synapse_update_processes = ProcessGroup([input_hidden, hidden_output])

# Create model
model = Model([neuron_update_processes, synapse_update_processes])

# Create backend and use to generate sim code
# **YUCK** simulate flag
backend = BackendFeNNSim()
code = backend.generate_simulation_kernel(synapse_update_processes, neuron_update_processes, 
                                          num_timesteps, True, model)

# **TODO** disassembler

# Create runtime
runtime = Runtime(model, backend)

# Set instructions
runtime.set_instructions(code)

# Allocate memory for model
runtime.allocate()

# Load weights
load_and_push("mnist_in_hid.bin", input_hidden_weight, runtime)
load_and_push("mnist_hid_out.bin", hidden_output_weight, runtime)
load_and_push("mnist_bias.bin", output_bias, runtime)

# Zero remaining state
zero_and_push(hidden_v, runtime)
zero_and_push(hidden_i, runtime)
zero_and_push(hidden_refrac_time, runtime)
zero_and_push(output_v, runtime)
zero_and_push(output_i, runtime)
zero_and_push(output_v_avg, runtime)

# Load data
mnist_spikes = np.fromfile("mnist_spikes.bin", dtype=np.uint32)
mnist_labels = np.fromfile("mnist_labels.bin", dtype=np.int16)

# Loop through examples
input_spike_array = runtime.get_array(input_spikes)
input_spike_view = np.asarray(input_spike_array.host_view).view(np.uint32)
assert not input_spike_view.flags["OWNDATA"]
hidden_spike_array = runtime.get_array(hidden_spikes)
output_v_avg_array = runtime.get_array(output_v_avg)
output_v_avg_view = np.asarray(output_v_avg_array.host_view).view(np.int16)
assert not output_v_avg_view.flags["OWNDATA"]
num_correct = 0
for i in range(num_examples):
    # Copy data to array host pointe
    start_word = i * num_input_spike_array_words
    input_spike_view[:] = mnist_spikes[start_word: start_word + num_input_spike_array_words]
    input_spike_array.push_to_device();

    # Classify
    print("meep")
    runtime.run()
    print("floop")
    # If we're recording, write input and hidden spikes to file
    #if record:
    #    recordSpikes("mnist_input_spikes_" + std::to_string(i) + ".csv", inputSpikeArray,
    #                 inputShape.getNumNeurons(), numTimesteps);
    #    recordSpikes("mnist_hidden_spikes_" + std::to_string(i) + ".csv", hiddenSpikeArray,
    #                 hiddenShape.getNumNeurons(), numTimesteps);

    # Copy output V sum from device
    output_v_avg_array.pull_from_device();
    print("Roop")
    # Determine if output is correct
    classification = np.argmax(output_v_avg_view)
    if classification == mnist_labels[i]:
        numCorrect += 1

    # Zero output and push
    output_v_avg_view[:] = 0
    output_v_avg_array.push_to_device()

print(f"{num_correct} / {num_examples} correct {100.0 * (num_correct / num_examples)}%")
