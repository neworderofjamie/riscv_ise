import numpy as np
import pytest
from typing import Optional

import pyfenn.fenn_backend as backend

from pyfenn.models import DelayLinear, DenseLinear, Memset, SparseLinear
from pyfenn.utils import (build_delay_weights, build_sparse_connectivity,
                          build_spike_array, copy_and_push, get_views, 
                          pull_and_get)
                          


class PostNeuron:
    def __init__(self, backend, shape, num_x_timesteps, name: str = ""):
        self.shape = shape
        self.i = backend.Variable(self.shape, "int16_t", name=f"{name}_I")
        self.x = backend.Variable((num_x_timesteps,) + self.shape, "int16_t", name=f"{name}_X")
        self.process = backend.NeuronUpdateProcess(
            """
            X = I;
            I = 0;
            """, 
            {"I": self.i, 
             "X": backend.SlicedVariable(self.x, True)}, {},
            name=name)

class PostNeuronDelay:
    def __init__(self, backend, shape, num_x_timesteps, num_delay_timesteps, name: str = ""):
        self.shape = shape
        self.i = backend.Variable(self.shape, "int16_t", name=f"{name}_I")
        self.x = backend.Variable((num_x_timesteps,) + self.shape, "int16_t", name=f"{name}_X")
        self.den_delay_buffer = backend.Variable((num_delay_timesteps,) + self.shape, 
                                                 "int16_t", name=f"{name}_den_delay")

        self.den_delay_update_process = backend.DendriticDelayUpdateProcess(
            self.den_delay_buffer, self.i, name=f"{name}_den_delay_update")
        self.process = backend.NeuronUpdateProcess(
            """
            X = I;
            I = 0;
            """, 
            {"I": self.i, 
             "X": backend.SlicedVariable(self.x, True)}, {},
            name=name)

@pytest.mark.parametrize("use_dram_for_weights", [True, False])
@pytest.mark.parametrize("num_cores", [1, 2])
def test_forward(device, use_dram_for_weights, num_cores):
    log_appender = backend.ConsoleAppender()
    backend.init_logging(log_appender, backend.PlogSeverity.DEBUG)

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
    dense = np.zeros((16, 4), dtype=np.int16)
    for i, row in enumerate(conn):
        dense[i,row] = 1

    # Convert into internal format
    conn = build_sparse_connectivity([conn], 1, 3)[0]

    # Create input spike source buffer
    input_spikes = backend.EventSourceBuffer((16,), len(spike_array), name="input_events")

    # Create one output neuron pop with sparse decoder population
    #sparse_n_pop = PostNeuron(backend, (4,), 17, "SparseNPop")
    dense_n_pop = PostNeuron(backend, (4,), 17, "DenseNPop")

    #input_sparse = SparseLinear(backend, input_spikes, sparse_n_pop.i, 
    #                            "int16_t", max_row_length=conn.shape[1], num_sparse_connectivity_bits=3,
    #                            name="input_sparse")
    input_dense = DenseLinear(backend, input_spikes, dense_n_pop.i, 
                              "int16_t", name="input_dense")

    # Initialisation
    #sparse_zero = Memset(backend, sparse_n_pop.i)
    dense_zero = Memset(backend, dense_n_pop.i)
    
    # Group processes
    #init_processes = backend.ProcessGroup([sparse_zero.process, dense_zero.process])
    #neuron_update_processes = backend.ProcessGroup([sparse_n_pop.process, dense_n_pop.process])
    #synapse_update_processes = backend.ProcessGroup([input_sparse.process, input_dense.process])
    init_processes = backend.ProcessGroup([dense_zero.process])
    neuron_update_processes = backend.ProcessGroup([dense_n_pop.process])
    synapse_update_processes = backend.ProcessGroup([input_dense.process])


    # Create simulation kernel
    sim_kernel = backend.SimulationLoopKernel(
        16, [synapse_update_processes, neuron_update_processes],
        [init_processes], [])


    # Create runtime
    runtime_params = {"use_dram_for_weights": use_dram_for_weights}
    runtime = (backend.RuntimeHW([sim_kernel], num_cores, **runtime_params) if device 
               else backend.RuntimeSim([sim_kernel], num_cores, **runtime_params))

    # Allocate memory for model
    runtime.allocate()

    # Initialise weights
    copy_and_push(spike_array, input_spikes, runtime)
    #copy_and_push(conn, input_sparse.weight, runtime)
    copy_and_push(dense, input_dense.weight, runtime)
   
    # Simulate
    runtime.run(sim_kernel)

    # Loop through output processes
    output_place_values = 2 ** np.arange(4)
    #for p in [sparse_n_pop, dense_n_pop]:
    for p in [dense_n_pop]:
        # Get value of x
        x_val = pull_and_get(runtime, p.x)

        # Remove first timestep and convert to bool
        x_val = x_val[1:,:].astype(bool)
        
        for t in range(16):
            correct_value = (t + 1) % 16
            output_value = np.sum(output_place_values[x_val[t]])
            if output_value != correct_value:
                assert False, f"{p.process.name} decoding incorrect ({output_value} rather than {correct_value})"
    
@pytest.mark.parametrize("use_dram_for_weights", [True, False])
def test_forward_den_delay(device, use_dram_for_weights):
    log_appender = backend.ConsoleAppender()
    backend.init_logging(log_appender, backend.PlogSeverity.INFO)

    num_pre = 10
    num_post = 4
    num_delay_bits = 5

    # Build spike array
    spike_neuron_ids = np.arange(num_pre)
    spike_timesteps = np.arange(num_pre)
    spike_array = build_spike_array(spike_timesteps, spike_neuron_ids)

    # Build combined
    delays = np.reshape(np.arange(num_pre - 1, -1, -1), (num_pre, 1))
    delay_weights = build_delay_weights(np.ones_like(delays), delays, num_delay_bits)
    delay_weights = np.tile(delay_weights, (1, num_post))

    # Create input spike source buffer
    input_spikes = backend.EventSourceBuffer((num_pre,), len(spike_array), name="input_events")

    # Create one output neuron pop
    pop = PostNeuronDelay(backend, (num_post,), num_pre + 1, 2**(num_delay_bits - 1), "DenseNPop")

    # Create delayed connection from input spikes to dense
    input_pop = DelayLinear(backend, input_spikes, pop.den_delay_buffer, "int16_t", 
                            num_delay_bits=num_delay_bits, name="input_dense")

    # Initialisation
    i_zero = Memset(backend, pop.i)
    den_delay_zero = Memset(backend, pop.den_delay_buffer)
    
    # Group processes
    init_processes = backend.ProcessGroup([i_zero.process, den_delay_zero.process])
    neuron_update_processes = backend.ProcessGroup([pop.process])
    den_delay_update_processes = backend.ProcessGroup([pop.den_delay_update_process], name="den-delay update")
    synapse_update_processes = backend.ProcessGroup([input_pop.process])
    
    # Create simulation kernel
    sim_kernel = backend.SimulationLoopKernel(
        num_pre, [synapse_update_processes, den_delay_update_processes, neuron_update_processes],
        [init_processes], [])


    # Create runtime
    runtime_params = {"use_dram_for_weights": use_dram_for_weights}
    runtime = (backend.RuntimeHW([sim_kernel], 1, **runtime_params) if device 
               else backend.RuntimeSim([sim_kernel], 1, **runtime_params))

    # Allocate memory for model
    runtime.allocate()

    # Initialise weights
    copy_and_push(spike_array, input_spikes, runtime)
    copy_and_push(delay_weights, input_pop.weight, runtime)

    # Simulate
    runtime.run(sim_kernel)

    # Simulate for 11 timesteps
    correct = np.reshape(np.zeros(num_pre), (num_pre, 1))
    correct[-1] = num_pre

    for p in [pop]:
        x_view = get_views(runtime, p.x)[0]
        runtime.pull_state_from_device(p.x)

        # Remove first timestep and padding neurons
        x_view = x_view[1:,:num_post]
        if np.any(x_view != correct):
            assert False, f"{p.process.name} decoding incorrect ({x_view} rather than {correct})"

