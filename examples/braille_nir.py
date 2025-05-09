import numpy as np
import mnist
import torch

from pyfenn import (BackendFeNNHW, BackendFeNNSim, Model,
                    ProcessGroup, Runtime)
from models import Copy

from pyfenn import disassemble, init_logging
from pyfenn.utils import get_array_view, load_and_push, zero_and_push

from time import perf_counter
from pyfenn.nir_import import parse

DT = 1e-4
DEVICE = False
DISASSEMBLE_CODE = False

init_logging()

# Load test set tensor
test_spikes, test_labels = torch.load("ds_test.pt", weights_only=False).tensors

# Pack 1s and 0s into bits
test_spikes = [np.packbits(test_spikes[i].to(int), axis=1, bitorder="little") 
               for i in range(test_spikes.shape[0])]

# Pad to 32-bit and view as uint32
test_spikes = [np.pad(s, ((0, 0), (0, 2))).view(np.uint32)
               for s in test_spikes]

num_timesteps = test_spikes[0].shape[0]
print(f"{num_timesteps} timesteps")

# Load model
(in_proc, out_proc, neuron_proc_group, syn_proc_group, var_vals, neuron_proc) =\
    parse("braille_noDelay_noBias_subtract.nir", num_timesteps, DT)

# Create model
model = Model([neuron_proc_group, syn_proc_group])

# Create backend and use to generate sim code
backend = BackendFeNNHW() if DEVICE else BackendFeNNSim()
code = backend.generate_simulation_kernel([syn_proc_group, neuron_proc_group],
                                          [], num_timesteps, model)

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
    array, view = get_array_view(runtime, var, np.int16)
    assert array.host_view.nbytes == vals.nbytes

    # Copy data to array host pointer
    view[:] = vals

    # Push to device
    array.push_to_device()

# Zero remaining state
zero_and_push(neuron_proc["lif1.lif"].v, runtime)
zero_and_push(neuron_proc["lif1.lif"].i, runtime)
zero_and_push(neuron_proc["lif2"].v, runtime)
zero_and_push(neuron_proc["lif2"].i, runtime)

# Set instructions
runtime.set_instructions(code)

# Loop through examples
input_spike_array, input_spike_view = get_array_view(runtime, 
                                                     in_proc.out_spikes,
                                                     np.uint32)
output_spike_array, output_spike_view = get_array_view(runtime,
                                                       out_proc.out_spikes,
                                                       np.uint8)
output_spike_view = np.reshape(output_spike_view, (num_timesteps, -1))
num_correct = 0
time = 0
for i, (spikes, label) in enumerate(zip(test_spikes, test_labels)):
    # Copy data to array host pointer
    assert input_spike_view.nbytes == spikes.nbytes
    input_spike_view[:] = spikes.flatten()
    input_spike_array.push_to_device();

    # Classify
    start = perf_counter()
    runtime.run()
    time += (perf_counter() - start)

    # Copy output V sum from device
    output_spike_array.pull_from_device()
    
    # Unpack spikes
    output_spikes = np.unpackbits(output_spike_view, axis=1, bitorder="little")[:,:7]
    classification = np.argmax(np.sum(output_spikes, axis=0))

    if classification == label:
        num_correct += 1

print(f"{num_correct} / {len(test_labels)} correct {100.0 * (num_correct / len(test_labels))}%")
print(f"{time / len(test_labels)}s/classification")
