import csv
import matplotlib.pyplot as plt
import numpy as np

from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer,
                    Model, NeuronUpdateProcess, Parameter, PerformanceCounter,
                    PlogSeverity, ProcessGroup, Runtime, Variable)
from pyfenn.models import Linear, Memset

from itertools import product
from pyfenn import disassemble, init_logging
from pyfenn.utils import (build_sparse_connectivity, ceil_divide,
                          copy_and_push, generate_fixed_prob, pull_spikes,
                          read_perf_counter, zero_and_push)

from tqdm.auto import tqdm
from time import perf_counter

class CUBALIF:
    def __init__(self, shape, tau_m: float, tau_syn_exc: float, tau_syn_inh, 
                 tau_refrac: int, v_thresh: float, i_offset: float = 0.0,
                 num_timesteps: int = 1, name: str = ""):
        self.shape = shape
        dtype = "s5_10_sat_t"
        decay_dtype = "s0_15_sat_t"
        self.v = Variable(self.shape, dtype, name=f"{name}_V")
        self.i_exc = Variable(self.shape, "s2_13_sat_t", name=f"{name}_IExc")
        self.i_inh = Variable(self.shape, "s2_13_sat_t", name=f"{name}_IInh")
        self.refrac_time = Variable(self.shape, "int16_t", 
                                    name=f"{name}_RefracTime")
        self.out_spikes = EventContainer(self.shape, num_timesteps + 1)
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
            {"Alpha": Parameter(np.exp(-1.0 / tau_m), decay_dtype),
             "BetaExc": Parameter(np.exp(-1.0 / tau_syn_exc), decay_dtype),
             "BetaInh": Parameter(np.exp(-1.0 / tau_syn_inh), decay_dtype),
             "ExcScale": Parameter(tau_syn_exc * (1.0 - np.exp(-1.0 / tau_syn_exc)), dtype),
             "InhScale": Parameter(tau_syn_inh * (1.0 - np.exp(-1.0 / tau_syn_inh)), dtype),
             "IOffset": Parameter(i_offset, dtype),
             "RMembrane": Parameter(tau_m / 1.0, dtype),
             "VThresh": Parameter(v_thresh, dtype),
             "TauRefrac": Parameter(tau_refrac, "int16_t")},
            {"V": self.v, "IExc": self.i_exc, "IInh": self.i_inh, "RefracTime": self.refrac_time},
            {"Spike": self.out_spikes},
            name)

def build_dense_connectivity(row_ind, weight, num_post):
    weights = np.zeros((len(row_ind), num_post), dtype=np.int16)
    for i, r in enumerate(row_ind):
        weights[i, r] = weight
    return weights
    
def simulate_fenn(device, num_excitatory=2048, num_timesteps=1000, dense=False, use_dram_for_weights=True, disassemble_code=False):
    probability_connection = 0.1
    excitatory_inhibitory_ratio = 4
    num_inhibitory = num_excitatory // excitatory_inhibitory_ratio
    num_neurons = num_excitatory + num_inhibitory
    scale = (4000.0 / num_neurons) * (0.02 / probability_connection)
    inh_weight = -51.0E-3 * scale
    exc_weight = 4.0E-3 * scale
    num_exc_sparse_connectivity_bits = int(np.ceil(np.log2(ceil_divide(num_excitatory, 32)))) + 1
    num_inh_sparse_connectivity_bits = int(np.ceil(np.log2(ceil_divide(num_inhibitory, 32)))) + 1
    num_timestep_spike_bytes = ceil_divide(num_neurons, 32) * 4
    num_blocks = ceil_divide(num_timesteps, (110 * 1024) // num_timestep_spike_bytes)
    num_timesteps_per_block = int(round(num_timesteps / num_blocks))

    print(f"\t{num_excitatory} excitatory neurons, {num_inhibitory} inhibitory neurons")
    init_logging(PlogSeverity.INFO)

    # Generate connectivity matrices
    ie_conn = generate_fixed_prob(num_inhibitory, num_excitatory, probability_connection)
    ii_conn = generate_fixed_prob(num_inhibitory, num_inhibitory, probability_connection)
    ee_conn = generate_fixed_prob(num_excitatory, num_excitatory, probability_connection)
    ei_conn = generate_fixed_prob(num_excitatory, num_inhibitory, probability_connection)

    print(f"\tWeight inhibitory: {int(round(inh_weight * 2**13))} ({inh_weight}), excitatory: {int(round(exc_weight * 2**13))} ({exc_weight})")
    
    if dense:
        ie_conn = build_dense_connectivity(ie_conn, int(round(inh_weight * 2**13)), num_excitatory)
        ii_conn = build_dense_connectivity(ii_conn, int(round(inh_weight * 2**13)), num_inhibitory)
        ee_conn = build_dense_connectivity(ee_conn, int(round(exc_weight * 2**13)), num_excitatory)
        ei_conn = build_dense_connectivity(ei_conn, int(round(exc_weight * 2**13)), num_inhibitory)
    else:
        ie_conn = build_sparse_connectivity(ie_conn, int(round(inh_weight * 2**13)), num_exc_sparse_connectivity_bits)
        ii_conn = build_sparse_connectivity(ii_conn, int(round(inh_weight * 2**13)), num_inh_sparse_connectivity_bits)
        ee_conn = build_sparse_connectivity(ee_conn, int(round(exc_weight * 2**13)), num_exc_sparse_connectivity_bits)
        ei_conn = build_sparse_connectivity(ei_conn, int(round(exc_weight * 2**13)), num_inh_sparse_connectivity_bits)

        print(f"\tNum sparse connectivity bits excitatory: {num_exc_sparse_connectivity_bits}, inhibitory: {num_inh_sparse_connectivity_bits}")
        print(f"\tMean row length ee:{np.average(ee_conn[:,0]) * 32} ei:{np.average(ei_conn[:,0]) * 32} ii:{np.average(ii_conn[:,0]) * 32} ie:{np.average(ie_conn[:,0]) * 32}")
    print(f"\tStride ee:{ee_conn.shape[1]} ei:{ei_conn.shape[1]} ii:{ii_conn.shape[1]} ie:{ie_conn.shape[1]}")

    # Neurons
    e_pop = CUBALIF(num_excitatory, tau_m=20.0, tau_syn_exc=5.0, tau_syn_inh=10.0,
                    tau_refrac=5, v_thresh=10, i_offset=0.55,
                    num_timesteps=num_timesteps_per_block, name="E")

    i_pop = CUBALIF(num_inhibitory, tau_m=20.0, tau_syn_exc=5.0, tau_syn_inh=10.0,
                    tau_refrac=5, v_thresh=10, i_offset=0.55,
                    num_timesteps=num_timesteps_per_block, name="I")

    # Synapses
    if dense:
        ee_pop = Linear(e_pop.out_spikes, e_pop.i_exc,
                        weight_dtype="s2_13_sat_t", name="EE")
        ei_pop = Linear(e_pop.out_spikes, i_pop.i_exc,
                        weight_dtype="s2_13_sat_t", name="EI")
        ii_pop = Linear(i_pop.out_spikes, i_pop.i_inh,
                        weight_dtype="s2_13_sat_t", name="II")
        ie_pop = Linear(i_pop.out_spikes, e_pop.i_inh,
                        weight_dtype="s2_13_sat_t", name="IE")
    else:
        ee_pop = Linear(e_pop.out_spikes, e_pop.i_exc,
                        weight_dtype="s2_13_sat_t", max_row_length=ee_conn.shape[1],
                        num_sparse_connectivity_bits=None if dense else num_exc_sparse_connectivity_bits, 
                        name="EE")
        ei_pop = Linear(e_pop.out_spikes, i_pop.i_exc,
                        weight_dtype="s2_13_sat_t", max_row_length=ei_conn.shape[1],
                        num_sparse_connectivity_bits=None if dense else num_inh_sparse_connectivity_bits, 
                        name="EI")
        ii_pop = Linear(i_pop.out_spikes, i_pop.i_inh,
                        weight_dtype="s2_13_sat_t", max_row_length=ii_conn.shape[1],
                        num_sparse_connectivity_bits=num_inh_sparse_connectivity_bits, 
                        name="II")
        ie_pop = Linear(i_pop.out_spikes, e_pop.i_inh,
                        weight_dtype="s2_13_sat_t", max_row_length=ie_conn.shape[1],
                        num_sparse_connectivity_bits=num_exc_sparse_connectivity_bits, 
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
                                        PerformanceCounter())
    synapse_update_processes = ProcessGroup([ee_pop.process, ei_pop.process,
                                            ii_pop.process, ie_pop.process],
                                            PerformanceCounter())

    # Create backend
    backend_kwargs = {"use_dram_for_weights": use_dram_for_weights, "dma_buffer_size": 64 * 1024 * 1024}
    backend = BackendFeNNHW(**backend_kwargs) if device else BackendFeNNSim(**backend_kwargs)

    # Create model
    model = Model([i_zero_processes, neuron_update_processes, synapse_update_processes],
                backend)

    # Generate init and sim code
    init_code = backend.generate_kernel([i_zero_processes], model)
    code = backend.generate_simulation_kernel([synapse_update_processes, neuron_update_processes],
                                            [], [],
                                            num_timesteps_per_block, model)

    # Disassemble if required
    if disassemble_code:
        print("\tInit:")
        for i, c in enumerate(init_code):
            print(f"\t\t{i * 4} : {disassemble(c)}")

        print("\\tSimulation:")
        for i, c in enumerate(code):
            print(f"\t\t{i * 4} : {disassemble(c)}")

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

    zero_and_push(neuron_update_processes.performance_counter, runtime)
    zero_and_push(synapse_update_processes.performance_counter, runtime)

    # Set init instructions
    print("\tInitialising")
    runtime.set_instructions(init_code)

    # Initialise
    runtime.run()

    # Set sim instructions
    print(f"\tSimulating {num_blocks} block of {num_timesteps_per_block} timesteps")
    runtime.set_instructions(code)

    # Loop through simulation blocks
    sim_time = 0.0
    e_spike_times = []
    e_spike_ids = []
    i_spike_times = []
    i_spike_ids = []
    block_start_timestep = 0
    for b in range(num_blocks):
        # Simulate block
        start_time = perf_counter()
        runtime.run()
        sim_time += (perf_counter() - start_time)

        # Pull excitatory spikes and add to lists
        block_e_spikes = pull_spikes(num_timesteps_per_block + 1, e_pop.out_spikes, runtime)
        e_spike_times.append(block_e_spikes[0] + block_start_timestep)
        e_spike_ids.append(block_e_spikes[1])

        # Pull inhibitory spikes and add to lists
        block_i_spikes = pull_spikes(num_timesteps_per_block + 1, i_pop.out_spikes, runtime)
        i_spike_times.append(block_i_spikes[0] + block_start_timestep)
        i_spike_ids.append(block_i_spikes[1])
        
        # Update start time of next timestep
        block_start_timestep += num_timesteps_per_block

    e_spike_times = np.concatenate(e_spike_times)
    e_spike_ids = np.concatenate(e_spike_ids)
    i_spike_times = np.concatenate(i_spike_times)
    i_spike_ids = np.concatenate(i_spike_ids)
    
    print(f"\tSimulation time {sim_time}")
    neuron_update_cycles, neuron_update_instructions = read_perf_counter(
        neuron_update_processes.performance_counter, runtime)
    synapse_update_cycles, synapse_update_instructions = read_perf_counter(
        synapse_update_processes.performance_counter, runtime)

    print(f"\tNeuron update {neuron_update_cycles} cycles, {neuron_update_instructions} instruction ({neuron_update_instructions / neuron_update_cycles})")
    print(f"\tSynapse update {synapse_update_cycles} cycles, {synapse_update_instructions} instruction ({synapse_update_instructions / synapse_update_cycles})")
    
    return (e_spike_times, e_spike_ids, i_spike_times, i_spike_ids, sim_time, neuron_update_instructions,
            synapse_update_instructions, neuron_update_cycles, synapse_update_cycles,
            ee_conn.shape[1], ei_conn.shape[1], ii_conn.shape[1], ie_conn.shape[1])

plot = True
device = False
with open(f"va_benchmark_{device}_perf.csv", "w") as csv_file:
    csv_writer = csv.writer(csv_file, delimiter=",")
    
    csv_writer.writerow(["Num excitatory neurons", "Using DRAM for weights", "Dense connectivity",
                         "Num excitatory spikes", "Num inhibitory spikes", "Simulation time [s]",
                         "Num neuron update instructions", "Num event processing instructions",
                         "Num neuron update cycles", "Num event processing cycles",
                         "EE stride", "EI stride", "II stride", "IE stride"])

    # Build configs
    configs = [(256, True), (256, False)] + [(e, True) for e in range(512, 4000, 512)]
    
    if plot:
        fig, axes = plt.subplots(2, len(configs), sharex="col")
    
    # Loop through configurations
    for j, (num_excitatory, use_dram_for_weights) in enumerate(configs):
        # Run simulation
        print(f"{num_excitatory} neurons using {'DRAM' if use_dram_for_weights else 'URAM'} for weights")
        
        # :oop through dense and 
        for i, dense in enumerate([True, False]):
            data = simulate_fenn(device, num_excitatory=num_excitatory, num_timesteps=1000, dense=dense,
                                use_dram_for_weights=use_dram_for_weights, disassemble_code=False)
            
            # Write CSV
            csv_writer.writerow([num_excitatory, use_dram_for_weights, dense, len(data[0]), len(data[2])] + list(data[4:]))
            
            if plot:
                axes[i, j].scatter(data[0], data[1], s=1)
                axes[i, j].scatter(data[2], data[3]+ num_excitatory, s=1)
    
if plot:
    for i in range(axes.shape[0]):
        axes[i,0].set_ylabel("Neuron ID")
    for j in range(axes.shape[1]):
        axes[-1,j].set_ylabel("Time [ms]")
    plt.show();
