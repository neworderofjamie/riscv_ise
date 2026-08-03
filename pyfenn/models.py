from typing import ModuleType, Optional

from pyfenn._frontend import EventSource

class RNGInit:
    def __init__(self, backend: ModuleType):
        self.seed = backend.Variable(64, "int16_t")
        self.process = backend.RNGInitProcess(self.seed)

class Memset:
    def __init__(self, backend: ModuleType, target: Variable, name: str = ""):
        self.process = backend.MemsetProcess(target, name)
"""
class ExpLUTBroadcast:
    def __init__(self, backend: ModuleType):
        self.lut = Variable(65, "int16_t", name="exp_lut_source")
        self.process = backend.BroadcastProcess(self.lut, 2, "exp_lut_broadcast")
"""
class DenseLinear:
    def __init__(self, backend: ModuleType, source_events: EventSource, 
                 target_var: Variable, weight_dtype: str, name: str = ""):
        self.shape = (source_events.shape.num_neurons,
                      target_var.shape.num_neurons)
        weight_shape = (source_events.shape.num_neurons,
                        (target_var.shape.num_neurons 
                         if num_sparse_connectivity_bits == 0 
                         else max_row_length))
        self.weight = backend.Variable(weight_shape, weight_dtype, 1, f"{name}_weight")
        self.process = backend.DenseEventPropagationProcess(source_events, 
                                                            self.weight, 
                                                            target_var, name)
