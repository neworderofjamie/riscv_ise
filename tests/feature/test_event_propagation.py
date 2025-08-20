import numpy as np
import pytest

from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer,
                    EventPropagationProcess, MemsetProcess, Model,
                    NeuronUpdateProcess, ProcessGroup, Runtime, Shape,
                    UnresolvedType, Variable)

from pyfenn import disassemble, init_logging
from pyfenn.utils import (build_sparse_connectivity, ceil_divide,
                          copy_and_push, generate_fixed_prob, pull_spikes,
                          read_perf_counter, zero_and_push)
                          

class Memset:
    def __init__(self, target: Variable, name: str = ""):
        self.process = MemsetProcess(target, name)

class PostNeuron:
    def __init__(self, shape, num_timesteps, name: str = ""):
        self.shape = Shape(shape)
        dtype = UnresolvedType("int16_t")
        self.i = Variable(self.shape, dtype, num_timesteps + 1, name=f"{name}_I")
        self.process = NeuronUpdateProcess("", {}, {"I": self.i}, {}, name)

class Linear:
    def __init__(self, source_events: EventContainer, target_var: Variable,
                 weight_dtype: str, max_row_length: Optional[int] = None, 
                 num_sparse_connectivity_bits: int = 0, 
                 num_delay_bits: int = 0, name: str = ""):
        self.shape = Shape([source_events.shape.num_neurons,
                            target_var.shape.num_neurons])
        weight_shape = Shape(
            [source_events.shape.num_neurons,
             (target_var.shape.num_neurons 
              if num_sparse_connectivity_bits == 0 
              else max_row_length)])
        weight_dtype = UnresolvedType(weight_dtype)
        self.weight = Variable(weight_shape, weight_dtype, 1, f"{name}_weight")
        self.process = EventPropagationProcess(source_events, self.weight,
                                               target_var, 
                                               num_sparse_connectivity_bits,
                                               num_delay_bits, name)

def test_forward(device):
    input_spikes = EventContainer(Shape([16]), 16)
    
    # Create spike source array to generate one-hot pattern to decode
    #ss_pop = model.add_neuron_population("SpikeSource", 16, "SpikeSourceArray",
    #                                     {}, {"startSpike": np.arange(16), "endSpike": np.arange(1, 17)})
    #ss_pop.extra_global_params["spikeTimes"].set_init_values(np.arange(16.0))

    # Build sparse connectivity
    conn = []
    for i in range(16):
        row = []
        for j in range(4):
            j_value = 1 << j
            if ((i + 1) & j_value) != 0:
                row.append(j)
        conn.append(np.asarray(row))
    
    conn = build_sparse_connectivity(conn, 1, 3)
    
    # Use to build dense matrix
    dense = np.zeros((16, 4), dtype=np.int16)
    dense[pre_inds,post_inds] = 1
    
    # Create one output neuron pop with sparse decoder population
    sparse_n_pop = PostNeuron([4], 16, "SparseNPop")
    dense_n_pop = PostNeuron([4], 16, "DenseNPop")

    #manual_sparse_s_pop.set_sparse_connections(pre_inds, post_inds)
    
    input_sparse = Linear(input_spikes, sparse_n_pop.i, "int16_t", 
                          max_row_length=16, num_sparse_connectivity_bits=3,
                          name="input_sparse")
    input_dense = Linear(input_spikes, dense_n_pop.i, "int16_t", name="input_dense")

    # Initialisation
    sparse_zero = Memset(sparse_n_pop.i)
    dense_zero = Memset(dense_n_pop.i)
    
    # Group processes
    init_processes = ProcessGroup([sparse_zero.process, dense_zero.process])
    neuron_update_processes = ProcessGroup([sparse_n_pop.process, dense_n_pop.process])
    synapse_update_processes = ProcessGroup([input_sparse.process, input_dense.process])
    
    # Create backend
    backend = BackendFeNNHW() if device else BackendFeNNSim()

    # Create model
    model = Model([init_processes, neuron_update_processes, synapse_update_processes],
                   backend)

    # Generate sim code
    code = backend.generate_simulation_kernel([synapse_update_processes, neuron_update_processes],
                                              [init_processes], [],
                                              16, model)
    
    # Create runtime
    runtime = Runtime(model, backend)

    # Allocate memory for model
    runtime.allocate()

    # Initialise weights
    copy_and_push(conn.flatten(), input_sparse.weight, runtime)
    copy_and_push(dense.flatten(), dense_zero.weight, runtime)
   
    # Set sim instructions
    runtime.set_instructions(code)

    # Simulate
    runtime.run()

    output_place_values = 2 ** np.arange(4)
    output_populations = [sparse_constant_weight_n_pop,
                          sparse_constant_weight_pre_n_pop,
                          manual_sparse_constant_weight_n_pop,
                          sparse_n_pop, sparse_pre_n_pop, sparse_hybrid_n_pop,
                          manual_sparse_n_pop, bitmask_n_pop,
                          word_packed_bitmask_n_pop, dense_n_pop,
                          manual_dense_n_pop, sparse_event_n_pop]
    
    """
    while model.timestep < 16:
        model.step_time()

        # Loop through output populations
        for pop in output_populations:
            # Pull state variable
            pop.vars["x"].pull_from_device()

            # Convert to binary mask
            output_binary = np.isclose(np.ones(4), pop.vars["x"].view)

            # Sum up active place values
            output_value = np.sum(output_place_values[output_binary])
            if output_value != (model.timestep - 1):
                assert False, f"{pop.name} decoding incorrect ({output_value} rather than {model.timestep - 1})"
    """

"""
def test_forward_den_delay(device):
    model = make_model(precision, "test_forward_den_delay", backend=backend)
    model.dt = 1.0
    model.batch_size = batch_size

    # Create spike source array to generate one-hot pattern to decode
    # **NOTE** startSpike and endSpike will be broadcast across batch dimension
    ss_pop = model.add_neuron_population("SpikeSource", 10, "SpikeSourceArray",
                                         {}, {"startSpike": np.arange(10), "endSpike": np.arange(1, 11)})
    ss_pop.extra_global_params["spikeTimes"].set_init_values(np.arange(10.0))

    # Create one output neuron pop with dense decoder population
    delay = np.arange(9, -1, -1)
    dense_n_pop = model.add_neuron_population(
        "PostDenseNeuron", 1, post_neuron_model,
        {}, {"x": 0.0})
    dense_s_pop = model.add_synapse_population(
        "PostDenseSynapse", "DENSE",
        ss_pop, dense_n_pop,
        init_weight_update("StaticPulseDendriticDelay", {}, {"g": 1.0, "d": delay}),
        init_postsynaptic("DeltaCurr", {}, {}))
    dense_s_pop.max_dendritic_delay_timesteps = 10

    # Build model and load
    model.build()
    model.load()

    # Simulate for 11 timesteps
    output_populations = [dense_n_pop, sparse_n_pop, sparse_pre_n_pop]
    while model.timestep < 11:
        model.step_time()

        # Loop through output populations
        correct = 10.0 if model.timestep == 11 else 0.0
        for pop in output_populations:
            # Pull state variable
            pop.vars["x"].pull_from_device()

            # If not close to correct value, error
            # **NOTE** all close to handle batching
            if not np.allclose(pop.vars["x"].view, correct):
                assert False, f"{pop.name} decoding incorrect ({pop.vars['x'].view} rather than {correct})"
"""

