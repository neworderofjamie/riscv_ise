import numpy as np
import pytest
from typing import Optional

import pyfenn.fenn_backend as backend

from pyfenn.models import DenseLinear, Memset, SparseLinear
from pyfenn.utils import (build_delay_weights, build_sparse_connectivity,
                          build_spike_array, copy_and_push, get_views)
                          


class PostNeuron:
    def __init__(self, backend, shape, num_i_timesteps, num_x_timesteps, name: str = ""):
        self.shape = shape
        self.i = backend.Variable((num_i_timesteps,) + self.shape, "int16_t", name=f"{name}_I")
        self.x = backend.Variable((num_x_timesteps,) + self.shape, "int16_t", name=f"{name}_X")
        self.process = backend.NeuronUpdateProcess(
            """
            X = I;
            I = 0;
            """, 
            {"I": backend.SlicedVariable(self.i, True), 
             "X": backend.SlicedVariable(self.x, True)}, {},
            name=name)

@pytest.mark.parametrize("use_dram_for_weights", [True, False])
def test_forward(device, use_dram_for_weights):    
    log_appender = backend.ConsoleAppender()
    backend.init_logging(log_appender, backend.PlogSeverity.INFO)

    # Build spike array
    spike_neuron_ids = np.arange(16)
    spike_timesteps = np.arange(16)
    spike_array = build_spike_array(spike_timesteps, spike_neuron_ids)

    # Build sparse connectivity
    j = np.arange(4)
    j_value = 1 << j
    conn = [np.where((j_value & (i + 1)) != 0)[0]
            for i in range(16)]

    # Use to build dense matrix
    dense = np.zeros((16, 32), dtype=np.int16)
    for i, row in enumerate(conn):
        dense[i,row] = 1

    # Convert into internal format
    conn = build_sparse_connectivity(conn, 1, 3)

    # Create input spike container
    input_spikes = backend.EventSourceBuffer((16,), 16, name="input_events")

    # Create one output neuron pop with sparse decoder population
    sparse_n_pop = PostNeuron(backend, (4,), 1, 17, "SparseNPop")
    dense_n_pop = PostNeuron(backend, (4,), 1, 17, "DenseNPop")

    input_sparse = SparseLinear(backend, input_spikes, backend.SlicedVariable(sparse_n_pop.i, True), 
                                "int16_t", max_row_length=4, num_sparse_connectivity_bits=3,
                                name="input_sparse")
    input_dense = DenseLinear(backend, input_spikes, backend.SlicedVariable(dense_n_pop.i, True), 
                              "int16_t", name="input_dense")

    # Initialisation
    sparse_zero = Memset(backend, sparse_n_pop.i)
    dense_zero = Memset(backend, dense_n_pop.i)
    
    # Group processes
    init_processes = backend.ProcessGroup([sparse_zero.process, dense_zero.process])
    neuron_update_processes = backend.ProcessGroup([sparse_n_pop.process, dense_n_pop.process])
    synapse_update_processes = backend.ProcessGroup([input_sparse.process, input_dense.process])

    # Create simulation kernel
    sim_kernel = backend.SimulationLoopKernel(
        16, [synapse_update_processes, neuron_update_processes],
        [init_processes], [])


    # Create runtime
    runtime_params = {"use_dram_for_weights": use_dram_for_weights}
    runtime = (backend.RuntimeHW([sim_kernel], 1, **runtime_params) if device 
            else backend.RuntimeSim([sim_kernel], 1, **runtime_params))

    # Allocate memory for model
    runtime.allocate()

    # Initialise weights
    copy_and_push(spike_array, input_spikes, runtime)
    copy_and_push(conn.flatten(), input_sparse.weight, runtime)
    copy_and_push(dense.flatten(), input_dense.weight, runtime)
   
    # Simulate
    runtime.run(sim_kernel)

    # Loop through output processes
    output_place_values = 2 ** np.arange(4)
    for p in [sparse_n_pop, dense_n_pop]:
        x_view = get_views(runtime, p.x)
        runtime.pull_state_from_device(p.x)
        
        # Remove first timestep and padding neurons; and convert to bool
        x_view = x_view[1:,:4].astype(bool)
        
        for t in range(16):
            correct_value = (t + 1) % 16
            output_value = np.sum(output_place_values[x_view[t]])
            if output_value != correct_value:
                assert False, f"{p.process.name} decoding incorrect ({output_value} rather than {correct_value})"
    
@pytest.mark.parametrize("use_dram_for_weights", [True, False])
def test_forward_den_delay(device, use_dram_for_weights):
    log_appender = backend.ConsoleAppender()
    backend.init_logging(log_appender, backend.PlogSeverity.INFO)

    num_pre = 10
    num_post = 4
    num_delay_bits = 5

    num_pre_vecs = ceil_divide(num_pre, 32)
    num_post_vecs = ceil_divide(num_post, 32)

    # Create one-hot pattern of spikes to decode
    spikes = np.identity(num_pre, dtype=bool)

    # Pack along neurons axis
    spikes = np.packbits(spikes, axis=1, bitorder="little")

    # Pad neuron axes to 4 bytes
    spikes = np.pad(spikes, ((0, 0), (0, (num_pre_vecs * 4) - spikes.shape[1])))

    # Convert to uint32 and flatten
    spikes = spikes.view(np.uint32).flatten()

    # Build combined
    delays = np.reshape(np.arange(num_pre - 1, -1, -1), (num_pre, 1))
    delay_weights = build_delay_weights(np.ones_like(delays), delays, num_delay_bits)
    delay_weights = np.tile(delay_weights, (1, num_post))
    delay_weights = np.pad(delay_weights, ((0, 0), (0, (num_post_vecs * 32) - delay_weights.shape[1])))

    # Create input spike container
    input_spikes = EventContainer(num_pre, num_pre)

    # Create one output neuron pop
    dense_n_pop = PostNeuron(num_post, 2**(num_delay_bits - 1), num_pre + 1, "DenseNPop")

    # Create delayed connection from input spikes to dense
    input_dense = Linear(input_spikes, dense_n_pop.i, "int16_t", 
                         num_delay_bits=num_delay_bits, name="input_dense")

    # Initialisation
    dense_zero = Memset(dense_n_pop.i)
    
    # Group processes
    init_processes = ProcessGroup([dense_zero.process])
    neuron_update_processes = ProcessGroup([dense_n_pop.process])
    synapse_update_processes = ProcessGroup([input_dense.process])
    
    # Create backend
    backend_kwargs = {"use_dram_for_weights": use_dram_for_weights}
    backend = BackendFeNNHW(**backend_kwargs) if device else BackendFeNNSim(**backend_kwargs)

    # Create model
    model = Model([init_processes, neuron_update_processes, synapse_update_processes],
                   backend)

    # Generate sim code
    code = backend.generate_simulation_kernel([synapse_update_processes, neuron_update_processes],
                                              [init_processes], [],
                                              num_pre, model)
    # Create runtime
    runtime = Runtime(model, backend)

    # Allocate memory for model
    runtime.allocate()

    # Initialise weights
    copy_and_push(spikes, input_spikes, runtime)
    copy_and_push(delay_weights.flatten(), input_dense.weight, runtime)

     # Set sim instructions
    runtime.set_instructions(code)

    # Simulate
    runtime.run()

    # Simulate for 11 timesteps
    correct = np.reshape(np.zeros(num_pre), (num_pre, 1))
    correct[-1] = num_pre

    for p in [dense_n_pop]:
        x_array, x_view = get_array_view(runtime, p.x, np.int16, (num_pre + 1, num_post_vecs * 32))
        x_array.pull_from_device()

        # Remove first timestep and padding neurons
        x_view = x_view[1:,:num_post]
        if np.any(x_view != correct):
            assert False, f"{p.process.name} decoding incorrect ({x_view} rather than {correct})"

