import numpy as np

from pyfenn import (BackendFeNNSim, EventContainer, Model,
                    NeuronUpdateProcess, NumericValue, Parameter,
                    ProcessGroup, Runtime, Shape, UnresolvedType, Variable)
from models import RNGInit

from pyfenn import disassemble, init_logging
from utils import get_array_view, seed_and_push, zero_and_push

class Poisson:
    def __init__(self, shape, rate: float):
        self.shape = Shape(shape)
        self.num_spikes = Variable(self.shape, UnresolvedType("int16_t"))
        
        self.process = NeuronUpdateProcess(
            """
            s1_14_t p = 1.0h14;
            NumSpikes = 0;
            do
            {
                NumSpikes++;
                p *= fennrand();
            } while (p > ExpMinusLambda);
            """,
            {"ExpMinusLambda": Parameter(NumericValue(np.exp(-(rate / 1000))),
                                         UnresolvedType("s1_14_t"))},
            {"NumSpikes": self.num_spikes},
            {})


num_samples = 100
shape = [32]
rate = 5000.0
disassemble_code = True

init_logging()

# Model
rng_init = RNGInit()
poisson = Poisson(shape, rate)

# Group processes
init_processes = ProcessGroup([rng_init.process])
update_processes = ProcessGroup([poisson.process])

# Create model
model = Model([init_processes, update_processes])

# Create backend and use to generate sim code
backend = BackendFeNNSim()
init_code = backend.generate_kernel(init_processes, model)

code = backend.generate_kernel(update_processes, model)

# Disassemble if required
if disassemble_code:
    print("Init code:")
    for i, c in enumerate(init_code):
        print(f"{i * 4} : {disassemble(c)}")

    print("Sim code:")
    for i, c in enumerate(code):
        print(f"{i * 4} : {disassemble(c)}")

# Create runtime
runtime = Runtime(model, backend)

# Allocate memory for model
runtime.allocate()

# Get array and view
seed_and_push(rng_init.seed, runtime)
zero_and_push(poisson.num_spikes, runtime)

# Set init instructions and run
runtime.set_instructions(init_code)
runtime.run()

# Set simulation instruction
runtime.set_instructions(code)

num_spikes_array, num_spikes_view = get_array_view(runtime, poisson.num_spikes,
                                                   np.int16)
                                                       
for i in range(num_samples):
    runtime.run()
    
    # Copy output V sum from device
    num_spikes_array.pull_from_device();
    print(num_spikes_view)

