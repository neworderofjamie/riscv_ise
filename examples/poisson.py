import numpy as np
import matplotlib.pyplot as plt

from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model,
                    NeuronUpdateProcess, NumericValue, Parameter,
                    ProcessGroup, Runtime, Shape, UnresolvedType, Variable)
from models import Copy, RNGInit

from pyfenn import disassemble, init_logging
from pyfenn.utils import get_array_view, seed_and_push, zero_and_push
from scipy.stats import poisson

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
                ++NumSpikes;
                p *= fennrand();
            } while (p > ExpMinusLambda);
            """,
            {"ExpMinusLambda": Parameter(NumericValue(np.exp(-(rate / 1000))),
                                         UnresolvedType("s1_14_t"))},
            {"NumSpikes": self.num_spikes},
            {})

device = False
num_samples = 100
shape = [32]
rate = 5000.0
disassemble_code = False

init_logging()

# Model
rng_init = RNGInit()
poisson_process = Poisson(shape, rate)
copy_num_spikes = Copy(poisson_process.num_spikes, num_samples)

# Group processes
init_processes = ProcessGroup([rng_init.process])
update_processes = ProcessGroup([poisson_process.process])
copy_processes = ProcessGroup([copy_num_spikes.process])

# Create model
model = Model([init_processes, update_processes, copy_processes])

# Create backend and use to generate sim code
backend = BackendFeNNHW() if device else BackendFeNNSim()
init_code = backend.generate_kernel([init_processes], model)

code = backend.generate_simulation_kernel([update_processes, copy_processes],
                                          [],
                                          num_samples, model)

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

# Set init instructions and run
runtime.set_instructions(init_code)
runtime.run()

# Set simulation instruction
runtime.set_instructions(code)

runtime.run()

num_spikes_copy_array, num_spikes_copy_view = get_array_view(runtime, copy_num_spikes.target,
                                                             np.int16)
num_spikes_copy_array.pull_from_device()

x = np.arange(np.amin(num_spikes_copy_view), 1 + np.amax(num_spikes_copy_view))

fig, axis = plt.subplots()
axis.hist(num_spikes_copy_view, bins=x - 0.5, density=True)
axis.plot(x, poisson.pmf(x, 5.0))
axis.set_xlabel("k")
axis.set_ylabel("P(x=k)")
plt.show()


