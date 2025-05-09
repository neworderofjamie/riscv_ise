import numpy as np
import mnist

from pyfenn import (BackendFeNNHW, BackendFeNNSim, Model,
                    ProcessGroup, Runtime)
from models import Copy

from pyfenn import disassemble, init_logging
from pyfenn.utils import get_array_view, load_and_push, zero_and_push

from pyfenn.nir_import import parse

NUM_TIMESTEPS = 100
DT = 1.0
DEVICE = False
DISASSEMBLE_CODE = True

init_logging()

# Load model
(in_proc, out_proc, neuron_proc_group, syn_proc_group, var_vals) =\
    parse("braille_noDelay_noBias_subtract.nir", NUM_TIMESTEPS)

# Create model
model = Model([neuron_proc_group, syn_proc_group])

# Create backend and use to generate sim code
backend = BackendFeNNHW() if DEVICE else BackendFeNNSim()
code = backend.generate_simulation_kernel([syn_proc_group, neuron_proc_group],
                                          [], NUM_TIMESTEPS, model)

# Disassemble if required
if DISASSEMBLE_CODE:
    for i, c in enumerate(code):
        print(f"{i * 4} : {disassemble(c)}")

# Create runtime
runtime = Runtime(model, backend)

# Allocate memory for model
runtime.allocate()

# Load weights
for var, vals in var_vals.items():
    # Get array and view
    array, view = get_array_view(runtime, var, np.uint8)
    assert array.host_view.nbytes == vals.nbytes

    # Copy data to array host pointer
    view[:] = vals

    # Push to device
    array.push_to_device()

assert False
# Zero remaining state
"""
zero_and_push(hidden.v, runtime)
zero_and_push(hidden.i, runtime)
zero_and_push(hidden.refrac_time, runtime)
zero_and_push(output.v, runtime)
zero_and_push(output.i, runtime)
zero_and_push(output.v_avg, runtime)
"""
# Set instructions
runtime.set_instructions(code)

# Loop through examples
input_spike_array, input_spike_view = get_array_view(runtime, 
                                                     in_proc.out_spikes,
                                                     np.uint32)
num_correct = 0
for i in range(num_examples):
    # Copy data to array host pointe
    input_spike_view[:] = mnist_spikes[i]
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
    if classification == mnist_labels[i]:
        num_correct += 1

    # Push ORIGINAL output back to device (zeroing)
    output_v_avg_array.push_to_device()

print(f"{num_correct} / {num_examples} correct {100.0 * (num_correct / num_examples)}%")
