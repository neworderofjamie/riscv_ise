import csv
import matplotlib.pyplot as plt
import numpy as np
import pyfenn.fenn_backend as backend

from pyfenn.models import DenseLinear, SparseLinear, Memset
from pyfenn.utils import PythonLogAppender

from pyfenn.utils import (build_sparse_connectivity, ceil_divide,
                          copy_and_push, generate_fixed_prob, pull_spikes,
                          split_sparse_connectivity, zero_and_push)

from tqdm.auto import tqdm
from time import perf_counter

class CUBALIF:
    def __init__(self, backend, shape, tau_m: float, tau_syn_exc: float, tau_syn_inh, 
                 tau_refrac: int, v_thresh: float, i_offset: float = 0.0,
                 num_timesteps: int = 1, name: str = ""):
        self.shape = shape
        dtype = "s5_10_sat_t"

        exc_scale = tau_syn_exc * (1.0 - np.exp(-1.0 / tau_syn_exc))
        inh_scale = tau_syn_inh * (1.0 - np.exp(-1.0 / tau_syn_inh))
        beta_exc = np.exp(-1.0 / tau_syn_exc)
        beta_inh = np.exp(-1.0 / tau_syn_inh)
        alpha = np.exp(-1.0 / tau_m)

        self.v = backend.Variable(self.shape, dtype, name=f"{name}_V")
        self.i_exc = backend.Variable(self.shape, "s2_13_sat_t", name=f"{name}_IExc")
        self.i_inh = backend.Variable(self.shape, "s2_13_sat_t", name=f"{name}_IInh")
        self.refrac_time = backend.Variable(self.shape, "int16_t", name=f"{name}_RefracTime")
        channel = backend.EventChannel((num_timesteps + 1,) + self.shape, self.shape,
                                       True, name=f"{name}_out_spikes")
        self.spike_sink = channel.sink
        self.out_spikes = channel.source
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
            {"Spike": backend.SlicedEventSink(self.spike_sink, True)},
            name=name)

def build_dense_connectivity(row_ind, weight, num_post):
    weights = np.zeros((len(row_ind), num_post), dtype=np.int16)
    for i, r in enumerate(row_ind):
        weights[i, r] = weight
    return weights
    
def simulate_fenn(device, num_cores, num_excitatory=2048, num_timesteps=1000, dense=False,
                  probability_connection = 0.1, use_dram_for_weights=True, disassemble_code=False):
    
    excitatory_inhibitory_ratio = 4
    num_inhibitory = num_excitatory // excitatory_inhibitory_ratio
    num_neurons = num_excitatory + num_inhibitory
    scale = (4000.0 / num_neurons) * (0.02 / probability_connection)
    inh_weight = -51.0E-3 * scale
    exc_weight = 4.0E-3 * scale
    
    # Use backend helper to determine how connectivity will be split
    num_excitatory_per_core = [backend.get_split_dimension((num_excitatory,), c, 0, num_cores, 32)
                            for c in range(num_cores)]
    num_inhibitory_per_core = [backend.get_split_dimension((num_inhibitory,), c, 0, num_cores, 32)
                            for c in range(num_cores)]

    # Calculate edges
    # **TODO** this really could go in split
    excitatory_splits = np.cumsum(num_excitatory_per_core)[:-1]
    inhibitory_splits = np.cumsum(num_inhibitory_per_core)[:-1]

    num_exc_sparse_connectivity_bits = int(np.ceil(np.log2(ceil_divide(num_excitatory, 32)))) + 1
    num_inh_sparse_connectivity_bits = int(np.ceil(np.log2(ceil_divide(num_inhibitory, 32)))) + 1
    num_timestep_spike_bytes = ceil_divide(num_neurons, 32) * 4
    recording_memory = num_cores * 46
    num_blocks = ceil_divide(num_timesteps, (recording_memory * 1024) // num_timestep_spike_bytes)
    num_timesteps_per_block = int(round(num_timesteps / num_blocks))

    print(f"\t{num_excitatory} excitatory neurons, {num_inhibitory} inhibitory neurons")

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
        # Split connectivity between cores
        ie_conn = split_sparse_connectivity(ie_conn, excitatory_splits)
        ii_conn = split_sparse_connectivity(ii_conn, inhibitory_splits)
        ee_conn = split_sparse_connectivity(ee_conn, excitatory_splits)
        ei_conn = split_sparse_connectivity(ei_conn, inhibitory_splits)
        
        ie_conn = build_sparse_connectivity(ie_conn, int(round(inh_weight * 2**13)), num_exc_sparse_connectivity_bits)
        ii_conn = build_sparse_connectivity(ii_conn, int(round(inh_weight * 2**13)), num_inh_sparse_connectivity_bits)
        ee_conn = build_sparse_connectivity(ee_conn, int(round(exc_weight * 2**13)), num_exc_sparse_connectivity_bits)
        ei_conn = build_sparse_connectivity(ei_conn, int(round(exc_weight * 2**13)), num_inh_sparse_connectivity_bits)

        for i in range(num_cores):
            print(f"Core {i}")
            print(f"\tNum excitatory:{num_excitatory_per_core[i]}, Num inhibitory: {num_inhibitory_per_core[i]}")
            print(f"\tStride ee:{ee_conn[i].shape[1]} ei:{ei_conn[i].shape[1]} ii:{ii_conn[i].shape[1]} ie:{ie_conn[i].shape[1]}")
            print(f"\tMean row length ee:{np.average(ee_conn[i][:,0]) * 32} ei:{np.average(ei_conn[i][:,0]) * 32} ii:{np.average(ii_conn[i][:,0]) * 32} ie:{np.average(ie_conn[i][:,0]) * 32}")

    # Neurons
    e_pop = CUBALIF(backend, (num_excitatory,), tau_m=20.0, tau_syn_exc=5.0, tau_syn_inh=10.0,
                    tau_refrac=5, v_thresh=10, i_offset=0.55,
                    num_timesteps=num_timesteps_per_block, name="E")

    i_pop = CUBALIF(backend, (num_inhibitory,), tau_m=20.0, tau_syn_exc=5.0, tau_syn_inh=10.0,
                    tau_refrac=5, v_thresh=10, i_offset=0.55,
                    num_timesteps=num_timesteps_per_block, name="I")

    # Synapses
    if dense:
        ee_pop = DenseLinear(backend, e_pop.out_spikes, e_pop.i_exc,
                             weight_dtype="s2_13_sat_t", name="EE")
        ei_pop = DenseLinear(backend, e_pop.out_spikes, i_pop.i_exc,
                             weight_dtype="s2_13_sat_t", name="EI")
        ii_pop = DenseLinear(backend, i_pop.out_spikes, i_pop.i_inh,
                             weight_dtype="s2_13_sat_t", name="II")
        ie_pop = DenseLinear(backend, i_pop.out_spikes, e_pop.i_inh,
                             weight_dtype="s2_13_sat_t", name="IE")
    else:
        ee_pop = SparseLinear(backend, e_pop.out_spikes, e_pop.i_exc,
                              weight_dtype="s2_13_sat_t", max_row_length=sum(c.shape[1] for c in ee_conn),
                              num_sparse_connectivity_bits=None if dense else num_exc_sparse_connectivity_bits, 
                              name="EE")
        ei_pop = SparseLinear(backend, e_pop.out_spikes, i_pop.i_exc,
                              weight_dtype="s2_13_sat_t", max_row_length=sum(c.shape[1] for c in ei_conn),
                              num_sparse_connectivity_bits=None if dense else num_inh_sparse_connectivity_bits, 
                              name="EI")
        ii_pop = SparseLinear(backend, i_pop.out_spikes, i_pop.i_inh,
                              weight_dtype="s2_13_sat_t", max_row_length=sum(c.shape[1] for c in ii_conn),
                              num_sparse_connectivity_bits=num_inh_sparse_connectivity_bits, 
                              name="II")
        ie_pop = SparseLinear(backend, i_pop.out_spikes, e_pop.i_inh,
                              weight_dtype="s2_13_sat_t", max_row_length=sum(c.shape[1] for c in ie_conn),
                              num_sparse_connectivity_bits=num_exc_sparse_connectivity_bits, 
                              name="IE")

    # Initialisation
    ee_zero = Memset(backend, e_pop.i_exc, name="zero_e_exc")
    ei_zero = Memset(backend, e_pop.i_inh, name="zero_e_inh")
    ie_zero = Memset(backend, i_pop.i_exc, name="zero_i_exc")
    ii_zero = Memset(backend, i_pop.i_inh, name="zero_i_exc")

    # Group processes
    perf_counters = ["num_instructions", "num_clock_cycles", "num_slave_buffer_overflows"]
    i_zero_processes = backend.ProcessGroup([ee_zero.process, ei_zero.process,
                                            ie_zero.process, ii_zero.process])
    neuron_update_processes = backend.ProcessGroup([e_pop.process, i_pop.process],
                                                   perf_counters)
    synapse_update_processes = backend.ProcessGroup([ee_pop.process, ei_pop.process,
                                                    ii_pop.process, ie_pop.process],
                                                    perf_counters)

    # Create init kernel
    init_kernel = backend.SimpleKernel([i_zero_processes])

    # Create simulation kernel
    sim_kernel = backend.SimulationLoopKernel(
        num_timesteps_per_block, [synapse_update_processes, neuron_update_processes],
        [], [])
    
    # Create runtime
    runtime_params = {"use_dram_for_weights": use_dram_for_weights, "dma_buffer_size": 64 * 1024 * 1024}
    runtime = (backend.RuntimeHW([init_kernel, sim_kernel], num_cores, **runtime_params) if device
               else backend.RuntimeSim([init_kernel, sim_kernel], num_cores, **runtime_params))

    # Disassemble if required
    if disassemble_code:
        print("\tInit:")
        for i, c in enumerate(code = runtime.get_kernel_code(init_kernel)):
            print(f"\t\t{i * 4} : {backend.disassemble(c)}")

        print("\\tSimulation:")
        for i, c in enumerate(runtime.get_kernel_code(sim_kernel)):
            print(f"\t\t{i * 4} : {backend.disassemble(c)}")

    # Allocate memory for model
    runtime.allocate()

    # Copy connectivity
    if dense:
        copy_and_push(ee_conn, ee_pop.weight, runtime)
        copy_and_push(ei_conn, ei_pop.weight, runtime)
        copy_and_push(ii_conn, ii_pop.weight, runtime)
        copy_and_push(ie_conn, ie_pop.weight, runtime)
    else:
        copy_and_push(np.concatenate(ee_conn, axis=1), ee_pop.weight, runtime)
        copy_and_push(np.concatenate(ei_conn, axis=1), ei_pop.weight, runtime)
        copy_and_push(np.concatenate(ii_conn, axis=1), ii_pop.weight, runtime)
        copy_and_push(np.concatenate(ie_conn, axis=1), ie_pop.weight, runtime)

    # Initialise membrane voltages
    # **TODO** use init kernel
    v_thresh_fixed = int(round(10.0 * 2**10))
    v_thresh_fixed = int(round(10.0 * 2**10))
    copy_and_push(np.random.randint(0, v_thresh_fixed, num_excitatory,
                                    dtype=np.int16),
                  e_pop.v, runtime)
    copy_and_push(np.random.randint(0, v_thresh_fixed, num_inhibitory,
                                    dtype=np.int16),
                  i_pop.v, runtime)

    # Zero remaining state
    # **TODO** use init kernel
    zero_and_push(e_pop.refrac_time, runtime)
    zero_and_push(i_pop.refrac_time, runtime)
    zero_and_push(e_pop.spike_sink, runtime)
    zero_and_push(i_pop.spike_sink, runtime)


    # Set init instructions
    print("\tInitialising")
    runtime.run(init_kernel)

    # Set sim instructions
    print(f"\tSimulating {num_blocks} block of {num_timesteps_per_block} timesteps")

    # Loop through simulation blocks
    sim_time = 0.0
    e_spike_times = []
    e_spike_ids = []
    i_spike_times = []
    i_spike_ids = []
    excitatory_offsets = np.concatenate(([0], excitatory_splits))
    inhibitory_offsets = np.concatenate(([0], inhibitory_splits))
    block_start_timestep = 0
    for b in range(num_blocks):
        # Simulate block
        start_time = perf_counter()
        runtime.run(sim_kernel)
        sim_time += (perf_counter() - start_time)

        # Pull excitatory spikes and add to lists
        block_e_spikes = pull_spikes(num_timesteps_per_block + 1, e_pop.spike_sink, runtime)
        e_spike_times.extend(s[0] + block_start_timestep for s in block_e_spikes)
        e_spike_ids.extend(s[1] + o for s, o in zip(block_e_spikes, excitatory_offsets))

        # Pull inhibitory spikes and add to lists
        block_i_spikes = pull_spikes(num_timesteps_per_block + 1, i_pop.spike_sink, runtime)
        i_spike_times.extend(s[0] + block_start_timestep for s in block_i_spikes)
        i_spike_ids.extend(s[1] + o for s, o in zip(block_i_spikes, inhibitory_offsets))
        
        # Update start time of next timestep
        block_start_timestep += num_timesteps_per_block

    e_spike_times = np.concatenate(e_spike_times)
    e_spike_ids = np.concatenate(e_spike_ids)
    i_spike_times = np.concatenate(i_spike_times)
    i_spike_ids = np.concatenate(i_spike_ids)
    
    print(f"\tSimulation time {sim_time}")
    neuron_update_cycles = sum(runtime.read_performance_counter(neuron_update_processes, 'num_clock_cycles'))
    neuron_update_instructions = sum(runtime.read_performance_counter(neuron_update_processes, 'num_instructions'))
    neuron_num_slave_buffer_overflows = sum(runtime.read_performance_counter(neuron_update_processes, 'num_slave_buffer_overflows'))
    
    synapse_update_cycles = runtime.read_performance_counter(synapse_update_processes, 'num_clock_cycles')[0]
    synapse_update_instructions = runtime.read_performance_counter(synapse_update_processes, 'num_instructions')[0]
    synapse_num_slave_buffer_overflows = runtime.read_performance_counter(synapse_update_processes, 'num_slave_buffer_overflows')[0]
    
    print(f"Neuron update {neuron_update_cycles} cycles, {neuron_update_instructions} instruction ({neuron_update_instructions / neuron_update_cycles})")
    print(f"Synapse update {synapse_update_cycles} cycles, {synapse_update_instructions} instruction ({synapse_update_instructions / synapse_update_cycles})")
    print(f"Slave buffer overflows: {neuron_num_slave_buffer_overflows + synapse_num_slave_buffer_overflows}")
    
    return (e_spike_times, e_spike_ids, i_spike_times, i_spike_ids, sim_time, neuron_update_instructions,
            synapse_update_instructions, neuron_update_cycles, synapse_update_cycles,
            sum(c.shape[1] for c in ee_conn), sum(c.shape[1] for c in ei_conn), 
            sum(c.shape[1] for c in ii_conn), sum(c.shape[1] for c in ie_conn))


log_appender = backend.ConsoleAppender()
backend.init_logging(log_appender, backend.PlogSeverity.INFO)

plot = False
device = False
num_cores = 2
with open(f"va_benchmark_{device}_perf.csv", "w") as csv_file:
    csv_writer = csv.writer(csv_file, delimiter=",")
    
    csv_writer.writerow(["Num excitatory neurons", "Using DRAM for weights", "Dense connectivity",
                         "Probability of connection", "Num excitatory spikes", "Num inhibitory spikes", 
                         "Simulation time [s]", "Num neuron update instructions", "Num event processing instructions",
                         "Num neuron update cycles", "Num event processing cycles", "EE stride", "EI stride", "II stride", "IE stride"])

    # Build configs
    configs = [(e, True, 0.1) for e in range(1024, 25601, 1024)] 
    
    if plot:
        fig, axes = plt.subplots(2, len(configs), sharex="col")
    
    # Loop through configurations
    for j, (num_excitatory, use_dram_for_weights, probability_connection) in enumerate(configs):
        # Run simulation
        print(f"{num_excitatory} neurons using {'DRAM' if use_dram_for_weights else 'URAM'} for weights, running on {num_cores} cores")
        
        # Loop through dense and sparse
        for i, dense in enumerate([False]):
            data = simulate_fenn(device, num_cores, num_excitatory=num_excitatory, num_timesteps=1000,
                                 dense=dense, probability_connection=probability_connection,
                                 use_dram_for_weights=use_dram_for_weights, disassemble_code=False)
            
            # Write CSV
            csv_writer.writerow([num_excitatory, use_dram_for_weights, dense, probability_connection,
                                 len(data[0]), len(data[2])] + list(data[4:]))
            
            if plot:
                axes[i, j].scatter(data[0], data[1], s=1)
                axes[i, j].scatter(data[2], data[3]+ num_excitatory, s=1)
    
if plot:
    for i in range(axes.shape[0]):
        axes[i,0].set_ylabel("Neuron ID")
    for j in range(axes.shape[1]):
        axes[-1,j].set_ylabel("Time [ms]")
    plt.show();
