import matplotlib.pyplot as plt
import numpy as np
import pyfenn.fenn_backend as backend

from argparse import ArgumentParser
from pyfenn.models import SparseLinear, Memset
from pyfenn.utils import PythonLogAppender

from pyfenn.utils import (build_sparse_connectivity, ceil_divide,
                          copy_and_push, generate_fixed_prob, pull_spikes,
                          read_perf_counter, zero_and_push)

from tqdm.auto import tqdm
from time import perf_counter

class CUBALIF:
    def __init__(self, backend, shape, tau_m: float, tau_syn_exc: float, tau_syn_inh, 
                 tau_refrac: int, v_thresh: float, i_offset: float = 0.0,
                 num_timesteps: int = 1, name: str = ""):
        self.shape = shape
        dtype = "s5_10_sat_t"
        decay_dtype = "s0_15_sat_t"

        exc_scale = tau_syn_exc * (1.0 - np.exp(-1.0 / tau_syn_exc))
        inh_scale = tau_syn_inh * (1.0 - np.exp(-1.0 / tau_syn_inh))
        beta_exc = np.exp(-1.0 / tau_syn_exc)
        beta_inh = np.exp(-1.0 / tau_syn_inh)
        alpha = np.exp(-1.0 / tau_m)

        self.v = backend.Variable(self.shape, dtype, name=f"{name}_V")
        self.i_exc = backend.Variable(self.shape, "s2_13_sat_t", name=f"{name}_IExc")
        self.i_inh = backend.Variable(self.shape, "s2_13_sat_t", name=f"{name}_IInh")
        self.refrac_time = backend.Variable(self.shape, "int16_t", name=f"{name}_RefracTime")
        self.out_spikes = backend.EventChannel((num_timesteps + 1,) + self.shape, True)
        self.process = backend.NeuronUpdateProcess(
            f"""
            s5_10_sat_t inSyn;
            // Excitatory
            {{
                inSyn = (IExc * {exc_scale}h10);
                IExc *= {beta_exc}h15;
            }}
            
            // Inhibitory
            {{
                inSyn += (IInh * {inh_scale}h10);
                IInh *= {beta_inh}h15;
            }}
            
            if (RefracTime > 0) {{
               RefracTime -= 1;
            }}
            else {{
                const s5_10_sat_t VAlpha = {tau_m / 1.0}h10 * (inSyn + {i_offset}h10);
                V = VAlpha - ({alpha}h15 * (VAlpha - V));
            }}
            
            if(V >= {float(v_thresh)}h10) {{
               Spike();
               V = 0.0h10;
               RefracTime = {tau_refrac};
            }}
            """,
            {"V": self.v, "IExc": self.i_exc, "IInh": self.i_inh, "RefracTime": self.refrac_time},
            {"Spike": backend.SlicedEventSink(self.out_spikes, True)},
            name=name)

parser = ArgumentParser("VA benchmark")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--time", action="store_true", help="Record detailed timings using performance counters")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
parser.add_argument("--uram", action="store_true", help="Use URAM to store weights (only for small models)")
parser.add_argument("--num-excitatory", type=int, default=2048, help="Number of excitatory neurons to simulate")
parser.add_argument("--num-timesteps", type=int, default=1000, help="Number of timesteps to simulate")
parser.add_argument("--probability-connection", type=float, default=0.1, help="Probability to connect neurons with")
args = parser.parse_args()


excitatory_inhibitory_ratio = 4
num_inhibitory = args.num_excitatory // excitatory_inhibitory_ratio
num_neurons = args.num_excitatory + num_inhibitory
scale = (4000.0 / num_neurons) * (0.02 / args.probability_connection)
inh_weight = -51.0E-3 * scale
exc_weight = 4.0E-3 * scale
num_exc_sparse_connectivity_bits = int(np.ceil(np.log2(ceil_divide(args.num_excitatory, 32)))) + 1
num_inh_sparse_connectivity_bits = int(np.ceil(np.log2(ceil_divide(num_inhibitory, 32)))) + 1
num_timestep_spike_bytes = ceil_divide(num_neurons, 32) * 4
num_blocks = ceil_divide(args.num_timesteps, (110 * 1024) // num_timestep_spike_bytes)
num_timesteps_per_block = int(round(args.num_timesteps / num_blocks))


print(f"{args.num_excitatory} excitatory neurons, {num_inhibitory} inhibitory neurons")

log_appender = PythonLogAppender()
backend.init_logging(log_appender, backend.PlogSeverity.INFO)

# Generate connectivity matrices
ie_conn = generate_fixed_prob(num_inhibitory, args.num_excitatory, args.probability_connection)
ii_conn = generate_fixed_prob(num_inhibitory, num_inhibitory, args.probability_connection)
ee_conn = generate_fixed_prob(args.num_excitatory, args.num_excitatory, args.probability_connection)
ei_conn = generate_fixed_prob(args.num_excitatory, num_inhibitory, args.probability_connection)

print(f"Weight inhibitory: {int(round(inh_weight * 2**13))} ({inh_weight}), excitatory: {int(round(exc_weight * 2**13))} ({exc_weight})")
# Pad
ie_conn = build_sparse_connectivity(ie_conn, int(round(inh_weight * 2**13)), num_exc_sparse_connectivity_bits)
ii_conn = build_sparse_connectivity(ii_conn, int(round(inh_weight * 2**13)), num_inh_sparse_connectivity_bits)
ee_conn = build_sparse_connectivity(ee_conn, int(round(exc_weight * 2**13)), num_exc_sparse_connectivity_bits)
ei_conn = build_sparse_connectivity(ei_conn, int(round(exc_weight * 2**13)), num_inh_sparse_connectivity_bits)

print(f"Num sparse connectivity bits excitatory: {num_exc_sparse_connectivity_bits}, inhibitory: {num_inh_sparse_connectivity_bits}")
print(f"Stride ee:{ee_conn.shape[1]} ei:{ei_conn.shape[1]} ii:{ii_conn.shape[1]} ie:{ie_conn.shape[1]}")
print(f"Mean row length ee:{np.average(ee_conn[:,0]) * 32} ei:{np.average(ei_conn[:,0]) * 32} ii:{np.average(ii_conn[:,0]) * 32} ie:{np.average(ie_conn[:,0]) * 32}")
# Neurons
e_pop = CUBALIF(backend, (args.num_excitatory,), tau_m=20.0, tau_syn_exc=5.0, tau_syn_inh=10.0,
                tau_refrac=5, v_thresh=10, i_offset=0.55,
                num_timesteps=num_timesteps_per_block, name="E")

i_pop = CUBALIF(backend, (num_inhibitory,), tau_m=20.0, tau_syn_exc=5.0, tau_syn_inh=10.0,
                tau_refrac=5, v_thresh=10, i_offset=0.55,
                num_timesteps=num_timesteps_per_block, name="I")

# Synapses
ee_pop = SparseLinear(backend, e_pop.out_spikes, 
                      e_pop.i_exc, weight_dtype="s2_13_sat_t", max_row_length=ee_conn.shape[1],
                      num_sparse_connectivity_bits=num_exc_sparse_connectivity_bits, 
                      name="EE")
ei_pop = SparseLinear(backend, e_pop.out_spikes, 
                      i_pop.i_exc, weight_dtype="s2_13_sat_t", max_row_length=ei_conn.shape[1],
                      num_sparse_connectivity_bits=num_inh_sparse_connectivity_bits, 
                      name="EI")
ii_pop = SparseLinear(backend, i_pop.out_spikes, 
                      i_pop.i_inh, weight_dtype="s2_13_sat_t", max_row_length=ii_conn.shape[1],
                      num_sparse_connectivity_bits=num_inh_sparse_connectivity_bits, 
                      name="II")
ie_pop = SparseLinear(backend, i_pop.out_spikes, 
                      e_pop.i_inh, weight_dtype="s2_13_sat_t", max_row_length=ie_conn.shape[1],
                      num_sparse_connectivity_bits=num_exc_sparse_connectivity_bits, 
                      name="IE")

# Initialisation
ee_zero = Memset(backend, e_pop.i_exc)
ei_zero = Memset(backend, e_pop.i_inh)
ie_zero = Memset(backend, i_pop.i_exc)
ii_zero = Memset(backend, i_pop.i_inh)

# Group processes
i_zero_processes = backend.ProcessGroup([ee_zero.process, ei_zero.process,
                                         ie_zero.process, ii_zero.process])
neuron_update_processes = backend.ProcessGroup([e_pop.process, i_pop.process])
synapse_update_processes = backend.ProcessGroup([ee_pop.process, ei_pop.process,
                                                 ii_pop.process, ie_pop.process])

# Create init kernel
init_kernel = backend.SimpleKernel([i_zero_processes])

# Create simulation kernel
sim_kernel = backend.SimulationLoopKernel(
    num_timesteps_per_block, [synapse_update_processes, neuron_update_processes],
    [], [])
    
# Create runtime
runtime_params = {"use_dram_for_weights": not args.uram, "dma_buffer_size": 64 * 1024 * 1024}
runtime = (backend.RuntimeHW([init_kernel, sim_kernel], 1, **runtime_params) if args.device 
           else backend.RuntimeSim([init_kernel, sim_kernel], 1, **runtime_params))

# Disassemble if required
if args.disassemble:
    print("Init:")
    code = runtime.get_kernel_code(init_kernel)
    for i, c in enumerate(code):
        print(f"{i * 4} : {backend.disassemble(c)}")

    print("Simulation:")
    code = runtime.get_kernel_code(sim_kernel)
    for i, c in enumerate(code):
        print(f"{i * 4} : {backend.disassemble(c)}")

# Allocate memory for model
runtime.allocate()

# Copy connectivity
copy_and_push(ee_conn.flatten(), ee_pop.weight, runtime)
copy_and_push(ei_conn.flatten(), ei_pop.weight, runtime)
copy_and_push(ii_conn.flatten(), ii_pop.weight, runtime)
copy_and_push(ie_conn.flatten(), ie_pop.weight, runtime)

# Initialise membrane voltages
# **TODO** use init kernel
v_thresh_fixed = int(round(10.0 * 2**10))
num_excitatory_padded = ceil_divide(args.num_excitatory, 32) * 32
num_inhibitory_padded = ceil_divide(num_inhibitory, 32) * 32
copy_and_push(np.random.randint(0, v_thresh_fixed, num_excitatory_padded,
                                dtype=np.int16),
              e_pop.v, runtime)
copy_and_push(np.random.randint(0, v_thresh_fixed, num_inhibitory_padded,
                                dtype=np.int16),
              i_pop.v, runtime)

# Zero remaining state
# **TODO** use init kernel
zero_and_push(e_pop.refrac_time, runtime)
zero_and_push(i_pop.refrac_time, runtime)
zero_and_push(e_pop.out_spikes, runtime)
zero_and_push(i_pop.out_spikes, runtime)

if args.time:
    zero_and_push(neuron_update_processes.performance_counter, runtime)
    zero_and_push(synapse_update_processes.performance_counter, runtime)

# Initialise
print("Initialising")
runtime.run(init_kernel)

# Loop through simulation blocks
print(f"Simulating {num_blocks} block of {num_timesteps_per_block} timesteps")
sim_time = 0.0
e_spike_times = []
e_spike_ids = []
i_spike_times = []
i_spike_ids = []
block_start_timestep = 0
for b in range(num_blocks):
    # Simulate block
    start_time = perf_counter()
    runtime.run(sim_kernel)
    sim_time += (perf_counter() - start_time)

    # Pull excitatory spikes and add to lists
    block_e_spikes = pull_spikes(num_timesteps_per_block + 1, e_pop.out_spikes, runtime)
    assert len(block_e_spikes) == 1
    e_spike_times.append(block_e_spikes[0][0] + block_start_timestep)
    e_spike_ids.append(block_e_spikes[0][1])

    # Pull inhibitory spikes and add to lists
    block_i_spikes = pull_spikes(num_timesteps_per_block + 1, i_pop.out_spikes, runtime)
    assert len(block_i_spikes) == 1
    i_spike_times.append(block_i_spikes[0][0] + block_start_timestep)
    i_spike_ids.append(block_i_spikes[0][1])
    
    # Update start time of next timestep
    block_start_timestep += num_timesteps_per_block

i_spike_times = np.concatenate(i_spike_times)
i_spike_ids = np.concatenate(i_spike_ids)
e_spike_times = np.concatenate(e_spike_times)
e_spike_ids = np.concatenate(e_spike_ids)

print(f"Simulation time {sim_time}")
if args.time:
    neuron_update_cycles, neuron_update_instructions = read_perf_counter(
        neuron_update_processes.performance_counter, runtime)
    synapse_update_cycles, synapse_update_instructions = read_perf_counter(
        synapse_update_processes.performance_counter, runtime)
   
    print(f"Neuron update {neuron_update_cycles} cycles, {neuron_update_instructions} instruction ({neuron_update_instructions / neuron_update_cycles})")
    print(f"Synapse update {synapse_update_cycles} cycles, {synapse_update_instructions} instruction ({synapse_update_instructions / synapse_update_cycles})")

# Plot
fig, axis = plt.subplots()

axis.scatter(e_spike_times, e_spike_ids, s=1)
axis.scatter(i_spike_times, i_spike_ids + args.num_excitatory, s=1)

print(f"{len(e_spike_times)} excitatory spikes, {len(i_spike_times)} inhibitory spikes")
print(f"{((len(e_spike_times) + len(i_spike_times)) * 1000) / (num_neurons * args.num_timesteps)} spikes/second")
plt.show()
