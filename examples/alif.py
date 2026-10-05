import numpy as np
import matplotlib.pyplot as plt
import pyfenn.fenn_backend as backend


from pyfenn.utils import get_views, seed_and_push, zero_and_push

device = False
num_timesteps = 1000
background_rate = 0.5
rate = 7846 / 1370
disassemble_code = False

class ALIF:
    def __init__(self, backend, shape, tau_m: float, tau_a: float, tau_refrac: int,
                 v_thresh: float, beta: float, weight:float, num_timesteps: int):
        self.shape = (shape,)
        v_dtype = "s6_9_sat_t"
        a_dtype = "s6_9_sat_t"
        decay_dtype = "s0_15_sat_t"
        
        alpha = np.exp(-1.0 / tau_m)
        rho = np.exp(-1.0 / tau_a)

        self.v = backend.Variable((num_timesteps + 1,) + self.shape, v_dtype)
        self.a = backend.Variable((num_timesteps + 1,) + self.shape, a_dtype)
        self.i = backend.Variable((num_timesteps + 1,) + self.shape, "int16_t")
        self.refrac_time = backend.Variable(self.shape, "int16_t")
        self.process = backend.NeuronUpdateProcess(
            f"""
            V = ({alpha}h15 * V) + ({weight}h9 * I);
            A = A * {rho}h15;

            if (RefracTime > 0) {{
               RefracTime -= 1;
            }}
            else if(V >= ({v_thresh}h9 + ({beta}h9 * A))) {{
               V -= {v_thresh}h9;
               A += 1.0h9;
               RefracTime = {tau_refrac};
            }}
            """,
            {"V": backend.SlicedVariable(self.v, True), 
             "A": backend.SlicedVariable(self.a, True), 
             "I": backend.SlicedVariable(self.i, True),
             "RefracTime": self.refrac_time},
             {}, name="ALIF")

# Generate poisson data with two periods of average firing interspersed by background
data = np.zeros(num_timesteps + 1)
data[0:] = np.random.poisson(rate, num_timesteps + 1)
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

log_appender = backend.ConsoleAppender()#PythonLogAppender()
backend.init_logging(log_appender, backend.PlogSeverity.DEBUG)

# Model
#rng_init = RNGInit()
neurons = ALIF(backend, 32, 20.0, 2000, 5, 0.6, 0.0174, 0.01, num_timesteps)

# Group processes
#init_processes = backend.ProcessGroup([rng_init.process])
neuron_update_processes = backend.ProcessGroup([neurons.process])

sim_kernel = backend.SimulationLoopKernel(
    num_timesteps, [neuron_update_processes],
    [], [])

# Create backend
runtime = (backend.RuntimeHW([sim_kernel], 1) if device 
           else backend.RuntimeSim([sim_kernel], 1))

# Disassemble if required
if disassemble_code:
    code = runtime.get_kernel_code(sim_kernel)
    for i, c in enumerate(code):
        print(f"{i * 4} : {backend.disassemble(c)}")

# Allocate memory for model
runtime.allocate()

# Zero remaining state
zero_and_push(neurons.v, runtime)
zero_and_push(neurons.a, runtime)
zero_and_push(neurons.refrac_time, runtime)

# Copy input currents to device
input_i_views = get_views(runtime, neurons.i)
input_i_views[0][:] = repeated_data
runtime.push_state_to_device(neurons.i)

# Get array and view
#seed_and_push(rng_init.seed, runtime)

# Set init instructions and run
#runtime.run(init_kernel)

# Simulate
runtime.run(sim_kernel)

neurons_v_views = get_views(runtime, neurons.v)
neurons_a_views = get_views(runtime, neurons.a)

runtime.pull_state_from_device(neurons.v)
runtime.pull_state_from_device(neurons.a)


# Calculate mean and standard deviation
neurons_v_mean = np.average(neurons_v_views[0], axis=1)
neurons_v_std = np.std(neurons_v_views[0], axis=1)
neurons_a_mean = np.average(neurons_a_views[0], axis=1)
neurons_a_std = np.std(neurons_a_views[0], axis=1)

fig, axis = plt.subplots()

a_axis = axis.twinx()

timesteps = np.arange(num_timesteps + 1)
axis.plot(timesteps, neurons_v_mean, color="red")
axis.fill_between(timesteps, (neurons_v_mean - neurons_v_std), 
                  (neurons_v_mean + neurons_v_std),
                  alpha=0.5, color="red")

a_axis.plot(timesteps, neurons_a_mean, color="blue")
a_axis.fill_between(timesteps, (neurons_a_mean - neurons_a_std), 
                    (neurons_a_mean + neurons_a_std),
                    alpha=0.5, color="blue")
plt.show()
