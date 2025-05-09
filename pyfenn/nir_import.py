import math
import nir
import numpy as np

from numbers import Number

from collections import defaultdict

from . import (EventContainer, EventPropagationProcess,
               NeuronUpdateProcess, NumericValue, Parameter,
               ProcessGroup, Shape, UnresolvedType, Variable)

class InputSpikes:
    def __init__(self, name: str, node: nir.Input, fixed_point: int):
        num_timesteps = 79 # **TODO**
        self.shape = Shape(node.output_type["output"])
        self.out_spikes = EventContainer(self.shape, num_timesteps,
                                         f"{name}_out_spikes")

class CubaLIF:
    def __init__(self, name: str, node: nir.CubaLIF, fixed_point: int):
        self.shape = Shape(node.output_type["output"])
        dtype = _get_type(fixed_point, True)
        decay_dtype = UnresolvedType("s1_14_sat_t")
        self.v = Variable(self.shape, dtype, 1, f"{name}_v")
        self.i = Variable(self.shape, dtype, 1, f"{name}_i")
        self.out_spikes = EventContainer(self.shape, 1,
                                         f"{name}_out_spikes")
 
        # If all thresholds are the same, implement as parameter
        assert np.all(node.v_threshold == node.v_threshold[0])
        assert np.all(node.tau_mem == node.tau_mem[0])
        assert np.all(node.tau_syn == node.tau_syn[0])
        assert np.all(node.r == node.r[0])
        assert np.all(node.w_in == node.w_in[0])
        
        alpha = np.exp(-1.0 / node.tau_mem[0])
        self.process = NeuronUpdateProcess(
            f"""
            V = (Alpha * V) + (IScale * I);
            I *= Beta;
            
            if(V >= VThresh) {{
               Spike();
               V -= VThresh;
            }}
            """,
            {"Alpha": Parameter(NumericValue(alpha), decay_dtype),
             "IScale": Parameter(NumericValue((1.0 - alpha) * node.r[0] 
                                              * node.w_in[0]), dtype),
             "Beta": Parameter(NumericValue(np.exp(-1.0 / node.tau_syn[0])), decay_dtype),
             "VThresh": Parameter(NumericValue(node.v_threshold[0]), dtype)},
            {"V": self.v, "I": self.i}, {"Spike": self.out_spikes}, name)

class Linear:
    def __init__(self, name: str, node: nir.Linear, 
                 source_event_container: EventContainer,
                 target_variable: Variable,
                 fixed_point: int):
        self.shape = Shape([source_event_container.shape.num_neurons,
                            target_variable.shape.num_neurons])
        weight_dtype = _get_type(fixed_point, False)

        self.weight = Variable(self.shape, weight_dtype, 1,
                               f"{name}_weight")
        self.process = EventPropagationProcess(source_event_container,
                                               self.weight, target_variable,
                                               name)


def _get_type(frac_bits: int, saturation: bool):
     int_bits = 15 - frac_bits
     return UnresolvedType(f"s{int_bits}_{frac_bits}_sat_t" if saturation
                           else f"s{int_bits}_{frac_bits}_t")
     
# **TODO** should operate in terms of fixed point
def _find_signed_scale(data, num_bits: int, percentile: float):
    # Calculate desired percentile
    if isinstance(data, Number):
        max_val = data
    else:
        # Split data into positive and negative
        positive_mask = (data > 0)
        positive_data = data[positive_mask]
        negative_data = data[np.logical_not(positive_mask)]

        # Calculate desired percentile
        positive_perc = np.percentile(positive_data, percentile)
        negative_perc = np.percentile(-negative_data, percentile)

        # Calculate the largest of these
        max_val = max(positive_perc, negative_perc)
    
    # Calculate high bit and low bit
    # **NOTE** we floor so max is 2**(high_bit + 1) - 1
    # **NOTE** one bit is used for sign
    high_bit =  math.floor(math.log(max_val, 2))
    low_bit = high_bit - (num_bits - 2)

    # We scale to multiples of the low bit
    scale = (2.0 ** low_bit)
    
    # Calculate min and max
    min_quant = (-2.0 ** (high_bit + 1))
    max_quant = (2.0 ** (high_bit + 1)) - scale

    # Return range and number of fractional bits
    return min_quant, max_quant, -low_bit

# Check graph only contains supported node types
def _validate_graph(graph: nir.NIRGraph, neuron_update_nodes, 
                    event_prop_nodes) -> str:
    # Build tuple of all supported node types
    supported_nodes = (tuple(neuron_update_nodes.keys())
                       + tuple(event_prop_nodes.keys())
                       + (nir.Output,))
    
    # Loop through nodes
    input_name = None
    for name, node in graph.nodes.items():
        # If node is sub-graph, recurse
        if isinstance(node, nir.NIRGraph):
            # **TODO** implement
            assert False
            validate_graph(node, neuron_update_nodes, event_prop_nodes)
        elif isinstance(node, nir.Input):
            if input_name is not None:
                raise NotImplementedError("Multiple input nodes encountered")
            input_name = name
        elif not isinstance(node, supported_nodes):
            raise NotImplementedError(f"Node {node} not supported.")

    if input_name is None:
        raise RuntimeError("No input node encountered in graph")

    return input_name

# Build neuron update processes from nodes in graph
def _build_neuron_update_processes(graph: nir.NIRGraph, neuron_update_nodes,
                                   event_prop_nodes, target_node_quant: dict,
                                   dt: float) -> dict:
    # Loop through nodes
    node_processes = {}
    neuron_nodes_tuple = tuple(neuron_update_nodes.keys())
    event_prop_nodes_tuple = tuple(event_prop_nodes.keys())
    for name, node in graph.nodes.items():
        # If node is sub-graph, recurse
        if isinstance(node, nir.NIRGraph):
            # **TODO** implement
            assert False
            node_processes.update(
                build_neuron_update_processes(node, target_node_quant,
                                              event_prop_nodes,
                                              node_processes))
        elif isinstance(node, neuron_nodes_tuple):
            process_type = neuron_update_nodes[type(node)]
            fixed_point = (target_node_quant[name][2]
                           if name in target_node_quant else None)
            node_processes[name] = process_type(name, node, fixed_point)
        elif not isinstance(node, event_prop_nodes_tuple
                            + (nir.Input, nir.Output)):
            assert False

    return node_processes

# Build event propagation processes from nodes in graph
def _build_event_prop_processes(graph: nir.NIRGraph, neuron_update_nodes,
                                event_prop_nodes, target_node_quant: dict,
                                event_prop_source_target,
                                neuron_node_processes: dict):
    # Loop through nodes
    neuron_nodes_tuple = tuple(neuron_update_nodes.keys())
    event_prop_nodes_tuple = tuple(event_prop_nodes.keys())
    variable_values = {}
    node_processes = {}
    for name, node in graph.nodes.items():
        # If node is sub-graph, recurse
        if isinstance(node, nir.NIRGraph):
            # **TODO** implement
            assert False
            build_event_prop_processes(node, target_node_quant,
                                       event_prop_nodes,
                                       neuron_node_processes, input_node)
        elif isinstance(node, event_prop_nodes_tuple):
            # Get source and target node names
            # **TODO** quantise weights based on target node
            source_node, target_node = event_prop_source_target[name]

            # Get process type and create
            process_type = event_prop_nodes[type(node)]
            quantisation = target_node_quant[target_node]
            node_processes[name] = process_type(
                name, node, neuron_node_processes[source_node].out_spikes,
                neuron_node_processes[target_node].i, quantisation[2])
            
            # Quantise weights
            scale = 2.0 ** -quantisation[2]
            quant_weights = np.clip(
                scale * np.round(node.weight / scale),
                quantisation[0], quantisation[1]) / scale
            variable_values[node_processes[name].weight] =\
                quant_weights.astype(np.int16)
        elif not isinstance(node, neuron_nodes_tuple + (nir.Output,)):
            assert False

    return node_processes, variable_values

# Build data structures required for building FeNN model
def _build_mappings(graph: nir.NIRGraph, neuron_update_nodes,
                    event_prop_nodes):
    neuron_nodes_tuple = tuple(neuron_update_nodes.keys())
    event_prop_nodes_tuple = tuple(event_prop_nodes.keys())
    
    # Loop through edges to determine which edges target same neuron process
    # **THINK** does this work with sub-networks?
    node_inputs = defaultdict(list)
    event_prop_source_target = defaultdict(lambda: [None, None])
    output_name = None
    for edge in graph.edges:
        source = graph.nodes[edge[0]]
        target = graph.nodes[edge[1]]
        
        # If source is an event propagation node and target is a neuron node
        # i.e. the POSTSYNAPTIC end of a connection
        if (isinstance(source, event_prop_nodes_tuple)
                and isinstance(target, neuron_nodes_tuple)):
            node_inputs[edge[1]].append(edge[0])
            event_prop_source_target[edge[0]][1] = edge[1]
        # Otherwise, if source is a neuron update node and target is an`
        # event propagation node i.e the PRESYNAPTIC end of a connection
        elif (isinstance(source, neuron_nodes_tuple)
                and isinstance(target, event_prop_nodes_tuple)):
            event_prop_source_target[edge[1]][0] = edge[0]
        # Otherwise, if source is a neuron update node and target is an
        # Output node, 
        elif (isinstance(source, neuron_nodes_tuple)
                and isinstance(target, nir.Output)):
            if output_name is not None:
                raise NotImplementedError("Multiple edges to output "
                                          "node(s) encountered")
            output_name = edge[0]
        else:
            raise NotImplementedError(f"Node {source} cannot be "
                                      f"directly connected to "
                                      f"node {target}")

    if output_name is None:
        raise RuntimeError("No output node encountered in graph")

    return node_inputs, event_prop_source_target, output_name

# **TODO** return neuron and synapse update groups, input and output variables
def parse(filename, num_timesteps: int, dt: float = 1.0, 
          quant_percentile: float = 99.0, num_weight_bits: int = 8):
    neuron_update_nodes = {nir.CubaLIF: CubaLIF, nir.Input: InputSpikes}
    event_prop_nodes = {nir.Linear: Linear}

    # Validate graph
    graph = nir.read(filename)
    input_name = _validate_graph(graph, neuron_update_nodes, event_prop_nodes)
    
    # Build additional data structures
    node_inputs, event_prop_source_target, output_name =\
        _build_mappings(graph, neuron_update_nodes, event_prop_nodes)
    
    print(f"Node inputs: {node_inputs}")
    print(f"Event propagation source and target nodes: {event_prop_source_target}")
    # Loop through edges, sorted by target to quantize weights together
    target_node_quant = {}
    for target_node, source_nodes in node_inputs.items():
        # Flatten and concatenate weights and find scaling factors
        weights_concat = np.concatenate([graph.nodes[s].weight.flatten()
                                         for s in source_nodes])
        target_node_quant[target_node] = _find_signed_scale(weights_concat,
                                                            num_weight_bits,
                                                            quant_percentile)
    print(f"Target node quantisation: {target_node_quant}")

    # Create neuron update processes from nodes
    # **NOTE** we need the target_node_quant to build these nodes with the right fixed point format
    neuron_node_processes = _build_neuron_update_processes(
        graph, neuron_update_nodes, event_prop_nodes,
        target_node_quant, dt)

    print(f"Neuron node processes: {neuron_node_processes}")
    print(f"Input neuron node: '{input_name}', "
          f"output neuron node: '{output_name}'")

    # Create event propagation processes from nodes
    event_prop_node_processes, variable_values =\
        _build_event_prop_processes(graph, neuron_update_nodes,
                                    event_prop_nodes, target_node_quant,
                                    event_prop_source_target,
                                    neuron_node_processes)

    # Group processes
    neuron_update_process_group = ProcessGroup(
        [p.process for p in neuron_node_processes.values()
         if hasattr(p, "process")])
    event_prop_process_group = ProcessGroup(
        [p.process for p in event_prop_node_processes.values()])

    return (neuron_node_processes[input_name],
            neuron_node_processes[output_name],
            neuron_update_process_group, event_prop_process_group,
            variable_values)
