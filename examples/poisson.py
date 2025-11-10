import numpy as np
import matplotlib.pyplot as plt

from argparse import ArgumentParser
from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model,
                    NeuronUpdateProcess, Parameter, ProcessGroup, Runtime,
                    Variable)
from pyfenn.models import RNGInit

from pyfenn import disassemble, init_logging
from pyfenn.utils import get_array_view, seed_and_push, zero_and_push
from scipy.stats import poisson

class Poisson:
    def __init__(self, shape, num_samples, rate: float):
        self.shape = shape
        self.num_spikes = Variable(self.shape, "int16_t",
                                   num_samples + 1)
        
        self.process = NeuronUpdateProcess(
            """
            s1_14_t p = 1.0h14;
            NumSpikes = 0;
            do
            {
                ++NumSpikes;
                p *= fennrand();
            } while (p > ExpMinusLambda);
            --NumSpikes;
            """,
            {"ExpMinusLambda": Parameter(np.exp(-(rate / 1000)), "s1_14_t")},
            {"NumSpikes": self.num_spikes},
            {})


parser = ArgumentParser("Poisson generator")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
parser.add_argument("--num-samples", type=int, default=100, help="Number of samples each neuron should generate")
parser.add_argument("--num-generators", type=int, default=32, help="Number of generators")
parser.add_argument("--rate", type=float, default=5000.0, help="Rate in spikes/second each neuron should generate at")
args = parser.parse_args()

init_logging()

# Model
rng_init = RNGInit()
poisson_process = Poisson(args.num_generators, args.num_samples, args.rate)

# Group processes
init_processes = ProcessGroup([rng_init.process])
update_processes = ProcessGroup([poisson_process.process])

# Create backend
backend = BackendFeNNHW() if args.device else BackendFeNNSim()

# Create model
model = Model([init_processes, update_processes],
              backend)

# Generate init and sim code
init_code = backend.generate_kernel([init_processes], model)
code = backend.generate_simulation_kernel([update_processes],
                                          [], [],
                                          args.num_samples, model)

# Disassemble if required
if args.disassemble:
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

# Set simulation instruction and run
runtime.set_instructions(code)
runtime.run()

num_spikes_array, num_spikes_view = get_array_view(runtime, poisson_process.num_spikes,
                                                   np.int16, (args.num_samples + 1, args.num_generators))
num_spikes_array.pull_from_device()

# Remove first timestep of data
num_spikes_view = num_spikes_view[1:,:].flatten()
x = np.arange(np.amin(num_spikes_view), 1 + np.amax(num_spikes_view))

fig, axis = plt.subplots()
axis.hist(num_spikes_view, bins=x - 0.5, density=True)
axis.plot(x, poisson.pmf(x, args.rate / 1000.0))
axis.set_xlabel("k")
axis.set_ylabel("P(x=k)")
plt.show()


