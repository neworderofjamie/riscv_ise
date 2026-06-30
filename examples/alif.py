import numpy as np
import matplotlib.pyplot as plt

from argparse import ArgumentParser
from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model,
                    NeuronUpdateProcess, Parameter, PlogSeverity,
                    ProcessGroup, Runtime, Variable)
from pyfenn.models import RNGInit

from pyfenn import disassemble, init_logging
from pyfenn.utils import get_array_view, seed_and_push, zero_and_push

background_rate = 0.5
rate = 7846 / 1370

parser = ArgumentParser("Adaptive LIF neuron")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
parser.add_argument("--num-timesteps", type=int, default=1000, help="Number of timesteps to sample for")
args = parser.parse_args()


class ALIF:
    def __init__(self, shape, tau_m: float, tau_a: float, tau_refrac: int,
                 v_thresh: float, beta: float, weight:float, num_timesteps: int):
        self.shape = shape
        v_dtype = "s6_9_sat_t"
        a_dtype = "s6_9_sat_t"
        decay_dtype = "s0_15_sat_t"
        self.v = Variable(self.shape, v_dtype, num_timesteps + 1)
        self.a = Variable(self.shape, a_dtype, num_timesteps + 1)
        self.i = Variable(self.shape, "const int16_t", num_timesteps + 1)
        self.refrac_time = Variable(self.shape, "int16_t")
        self.process = NeuronUpdateProcess(
            """
            V = (Alpha * V) + (Weight * I);
            A *= Rho;

            if (RefracTime > 0) {
               RefracTime -= 1;
            }
            else if(V >= (VThresh + (Beta * A))) {
               V -= VThresh;
               A += 1.0h9;
               RefracTime = TauRefrac;
            }
            """,
            {"Alpha": Parameter(np.exp(-1.0 / tau_m), decay_dtype),
             "Rho": Parameter(np.exp(-1.0 / tau_a), decay_dtype),
             "VThresh": Parameter(v_thresh, v_dtype),
             "Beta": Parameter(beta, v_dtype),
             "Weight": Parameter(weight, v_dtype),
             "TauRefrac": Parameter(tau_refrac, "int16_t")},
            {"V": self.v, "A": self.a, "I": self.i, "RefracTime": self.refrac_time})

# Generate poisson data with two periods of average firing interspersed by background
data = np.zeros(args.num_timesteps + 1)
data[0:] = np.random.poisson(rate, args.num_timesteps + 1)
#data[0:2000] = np.random.poisson(rate, 2000)
#data[2000:4000] = np.random.poisson(background_rate, 2000)
#data[4000:5000] = np.random.poisson(rate, 1000)

"""
data_bad = np.zeros(num_timesteps)
data_bad[0:2000] = np.random.poisson(rate, 2000)
data_bad[2000:4000] = np.random.poisson(rate * 1.4, 2000)
data_bad[4000:5000] = np.random.poisson(rate * 1.7, 1000)
"""
repeated_data = np.repeat(data[:,None], 32, axis=1).astype(np.int16)
#repeated_data_bad = np.repeat(data_bad[:,None], 32, axis=1).astype(np.int16)

init_logging()

# Model
rng_init = RNGInit()
neurons = ALIF(32, 20.0, 2000, 5, 0.6, 0.0174, 0.01, args.num_timesteps)

# Group processes
init_processes = ProcessGroup([rng_init.process])
neuron_update_processes = ProcessGroup([neurons.process])

# Create backend
backend = BackendFeNNHW() if args.device else BackendFeNNSim()

# Create model
model = Model([init_processes, neuron_update_processes],
              backend)

# Generate init and sim code
init_code = backend.generate_kernel([init_processes], model)
code = backend.generate_simulation_kernel([neuron_update_processes],
                                          [], [],
                                          args.num_timesteps, model)

# Disassemble if required
if args.disassemble:
    for i, c in enumerate(code):
        print(f"{i * 4} : {disassemble(c)}")

# Create runtime
runtime = Runtime(model, backend)

# Allocate memory for model
runtime.allocate()

# Zero remaining state
zero_and_push(neurons.v, runtime)
zero_and_push(neurons.a, runtime)
zero_and_push(neurons.refrac_time, runtime)

# Copy input currents to device
input_i_array, input_i_view = get_array_view(runtime, neurons.i, np.int16)
input_i_view[:] = repeated_data.flatten()
input_i_array.push_to_device()

# Get array and view
seed_and_push(rng_init.seed, runtime)

# Set init instructions and run
runtime.set_instructions(init_code)
runtime.run()

# Set instructions
runtime.set_instructions(code)

# Simulate
runtime.run()

neurons_v_array, neurons_v_view = get_array_view(runtime, neurons.v, np.int16)
neurons_a_array, neurons_a_view = get_array_view(runtime, neurons.a, np.int16)

neurons_v_array.pull_from_device()
neurons_a_array.pull_from_device()

# **YUCK** reshape
neurons_v_view = np.reshape(neurons_v_view, (-1, 32))
neurons_a_view = np.reshape(neurons_a_view, (-1, 32))

# Calculate mean and standard deviation
neurons_v_mean = np.average(neurons_v_view, axis=1)
neurons_v_std = np.std(neurons_v_view, axis=1)
neurons_a_mean = np.average(neurons_a_view, axis=1)
neurons_a_std = np.std(neurons_a_view, axis=1)

fig, axis = plt.subplots()

a_axis = axis.twinx()

timesteps = np.arange(args.num_timesteps + 1)
axis.plot(timesteps, neurons_v_mean, color="red")
axis.fill_between(timesteps, (neurons_v_mean - neurons_v_std), 
                  (neurons_v_mean + neurons_v_std),
                  alpha=0.5, color="red")

a_axis.plot(timesteps, neurons_a_mean, color="blue")
a_axis.fill_between(timesteps, (neurons_a_mean - neurons_a_std), 
                    (neurons_a_mean + neurons_a_std),
                    alpha=0.5, color="blue")
plt.show()
