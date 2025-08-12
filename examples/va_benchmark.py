import matplotlib.pyplot as plt
import numpy as np

from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer,
                    Model, NeuronUpdateProcess, NumericValue, Parameter,
                    PerformanceCounter, PlogSeverity, ProcessGroup,
                    Runtime, Shape, UnresolvedType, Variable)
from models import Linear, Memset

from pyfenn import disassemble, init_logging
from pyfenn.utils import (build_sparse_connectivity, ceil_divide,
                          copy_and_push, generate_fixed_prob, pull_spikes,
                          read_perf_counter, zero_and_push)

from tqdm.auto import tqdm

class CUBALIF:
    def __init__(self, shape, tau_m: float, tau_syn_exc: float, tau_syn_inh, 
                 tau_refrac: int, v_thresh: float, i_offset: float = 0.0,
                 num_timesteps: int = 1, name: str = ""):
        self.shape = Shape(shape)
        dtype = UnresolvedType("s5_10_sat_t")
        decay_dtype = UnresolvedType("s0_15_sat_t")
        self.v = Variable(self.shape, dtype, name=f"{name}_V")
        self.i_exc = Variable(self.shape, dtype, name=f"{name}_IExc")
        self.i_inh = Variable(self.shape, dtype, name=f"{name}_IInh")
        self.refrac_time = Variable(self.shape, UnresolvedType("int16_t"), 
                                    name=f"{name}_RefracTime")
        self.out_spikes = EventContainer(self.shape, num_timesteps)
        self.process = NeuronUpdateProcess(
            """
            s5_10_sat_t inSyn;
            // Excitatory
            {
                inSyn = (IExc * ExcScale);
                IExc *= BetaExc;
            }
            
            // Inhibitory
            {
                inSyn += (IInh * InhScale);
                IInh *= BetaInh;
            }
            
            if (RefracTime > 0) {
               RefracTime -= 1;
            }
            else {
                const s5_10_sat_t VAlpha = RMembrane * (inSyn + IOffset);
                V = VAlpha - (Alpha * (VAlpha - V));
            }
            
            if(V >= VThresh) {
               Spike();
               V = 0.0h10;
               RefracTime = TauRefrac;
            }
            """,
            {"Alpha": Parameter(NumericValue(np.exp(-1.0 / tau_m)), decay_dtype),
             "BetaExc": Parameter(NumericValue(np.exp(-1.0 / tau_syn_exc)), decay_dtype),
             "BetaInh": Parameter(NumericValue(np.exp(-1.0 / tau_syn_inh)), decay_dtype),
             "ExcScale": Parameter(NumericValue(tau_syn_exc * (1.0 - np.exp(-1.0 / tau_syn_exc))), dtype),
             "InhScale": Parameter(NumericValue(tau_syn_inh * (1.0 - np.exp(-1.0 / tau_syn_inh))), dtype),
             "IOffset": Parameter(NumericValue(i_offset), dtype),
             "RMembrane": Parameter(NumericValue(tau_m / 1.0), dtype),
             "VThresh": Parameter(NumericValue(v_thresh), dtype),
             "TauRefrac": Parameter(NumericValue(tau_refrac), UnresolvedType("int16_t"))},
            {"V": self.v, "IExc": self.i_exc, "IInh": self.i_inh, "RefracTime": self.refrac_time},
            {"Spike": self.out_spikes},
            name)
            
num_timesteps = 1000
num_neurons = 512
probability_connection = 0.1
excitatory_inhibitory_ratio = 4.0
inh_weight = -0.07968749999999998
exc_weight = 0.0062499999999999995

num_excitatory = int(round((num_neurons * excitatory_inhibitory_ratio) / (1.0 + excitatory_inhibitory_ratio)))
num_inhibitory = num_neurons - num_excitatory
num_sparse_connectivity_bits = 5
device = False
time = True
disassemble_code = False

print(f"{num_excitatory} excitatory neurons, {num_inhibitory} inhibitory neurons")
init_logging(PlogSeverity.INFO)

# Generate connectivity matrices
ie_conn = generate_fixed_prob(num_inhibitory, num_excitatory, probability_connection)
ii_conn = generate_fixed_prob(num_inhibitory, num_inhibitory, probability_connection)
ee_conn = generate_fixed_prob(num_excitatory, num_excitatory, probability_connection)
ei_conn = generate_fixed_prob(num_excitatory, num_inhibitory, probability_connection)

# Pad
ie_conn = build_sparse_connectivity(ie_conn, int(round(inh_weight * 2**10)), num_sparse_connectivity_bits)
ii_conn = build_sparse_connectivity(ii_conn, int(round(inh_weight * 2**10)), num_sparse_connectivity_bits)
ee_conn = build_sparse_connectivity(ee_conn, int(round(exc_weight * 2**10)), num_sparse_connectivity_bits)
ei_conn = build_sparse_connectivity(ei_conn, int(round(exc_weight * 2**10)), num_sparse_connectivity_bits)

# Neurons
e_pop = CUBALIF([num_excitatory], tau_m=20.0, tau_syn_exc=5.0, tau_syn_inh=10.0,
                tau_refrac=5, v_thresh=10, i_offset=0.55,
                num_timesteps=1024, name="E")

i_pop = CUBALIF([num_inhibitory], tau_m=20.0, tau_syn_exc=5.0, tau_syn_inh=10.0,
                tau_refrac=5, v_thresh=10, i_offset=0.55,
                num_timesteps=1024, name="I")

# Synapses
ee_pop = Linear(e_pop.out_spikes, e_pop.i_exc,
                weight_dtype="s5_10_sat_t", max_row_length=ee_conn.shape[1],
                num_sparse_connectivity_bits=num_sparse_connectivity_bits, 
                name="EE")
ei_pop = Linear(e_pop.out_spikes, i_pop.i_exc,
                weight_dtype="s5_10_sat_t", max_row_length=ei_conn.shape[1],
                num_sparse_connectivity_bits=num_sparse_connectivity_bits, 
                name="EI")
ii_pop = Linear(i_pop.out_spikes, i_pop.i_inh,
                weight_dtype="s5_10_sat_t", max_row_length=ii_conn.shape[1],
                num_sparse_connectivity_bits=num_sparse_connectivity_bits, 
                name="II")
ie_pop = Linear(i_pop.out_spikes, e_pop.i_inh,
                weight_dtype="s5_10_sat_t", max_row_length=ie_conn.shape[1],
                num_sparse_connectivity_bits=num_sparse_connectivity_bits, 
                name="IE")

# Initialisation
ee_zero = Memset(e_pop.i_exc)
ei_zero = Memset(e_pop.i_inh)
ie_zero = Memset(i_pop.i_exc)
ii_zero = Memset(i_pop.i_inh)

# Group processes
i_zero_processes = ProcessGroup([ee_zero.process, ei_zero.process,
                                 ie_zero.process, ii_zero.process])
neuron_update_processes = ProcessGroup([e_pop.process, i_pop.process],
                                       PerformanceCounter() if time else None)
synapse_update_processes = ProcessGroup([ee_pop.process, ei_pop.process,
                                         ii_pop.process, ie_pop.process],
                                        PerformanceCounter() if time else None)

# Create backend
backend = BackendFeNNHW() if device else BackendFeNNSim()

# Create model
model = Model([i_zero_processes, neuron_update_processes, synapse_update_processes],
              backend)

# Generate init and sim code
init_code = backend.generate_kernel([i_zero_processes], model)
code = backend.generate_simulation_kernel([synapse_update_processes, neuron_update_processes],
                                          [],
                                          num_timesteps, model)

# Disassemble if required
if disassemble_code:
    print("Init:")
    for i, c in enumerate(init_code):
        print(f"{i * 4} : {disassemble(c)}")

    print("Simulation:")
    for i, c in enumerate(code):
        print(f"{i * 4} : {disassemble(c)}")

# Create runtime
runtime = Runtime(model, backend)

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
num_excitatory_padded = ceil_divide(num_excitatory, 32) * 32
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

if time:
    zero_and_push(neuron_update_processes.performance_counter, runtime)
    zero_and_push(synapse_update_processes.performance_counter, runtime)

# Set init instructions
print("Initialising")
runtime.set_instructions(init_code)

# Initialise
runtime.run()

# Set sim instructions
print("Simulating")
runtime.set_instructions(code)

# Simulate
runtime.run()

if time:
    neuron_update_cycles, neuron_update_instructions = read_perf_counter(
        neuron_update_processes.performance_counter, runtime)
    synapse_update_cycles, synapse_update_instructions = read_perf_counter(
        synapse_update_processes.performance_counter, runtime)
   
    print(f"Neuron update {neuron_update_cycles} cycles, {neuron_update_instructions} instruction ({neuron_update_instructions / neuron_update_cycles})")
    print(f"Synapse update {synapse_update_cycles} cycles, {synapse_update_instructions} instruction ({synapse_update_instructions / synapse_update_cycles})")

# Pull spikes
e_spikes = pull_spikes(1024, e_pop.out_spikes, runtime)
i_spikes = pull_spikes(1024, i_pop.out_spikes, runtime)

# Plot
fig, axis = plt.subplots()

axis.scatter(e_spikes[0], e_spikes[1], s=1)
axis.scatter(i_spikes[0], i_spikes[1] + num_excitatory, s=1)

print(f"{(len(e_spikes[0]) + len(i_spikes[0])) / num_neurons} spikes/second")
plt.show()
