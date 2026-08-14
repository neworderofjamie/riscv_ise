from types import ModuleType
from typing import Optional

from pyfenn._frontend import EventSource, Variable
from pyfenn.utils import pad

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
        assert len(source_events.shape) == 1
        assert len(target_var.shape) == 1
        self.shape = (source_events.shape[0], target_var.shape[0])

        self.weight = backend.Variable(self.shape, weight_dtype, f"{name}_weight")
        self.process = backend.DenseEventPropagationProcess(source_events, 
                                                            self.weight, 
                                                            target_var, name)
                                                            
class SparseLinear:
    def __init__(self, backend: ModuleType, source_events: EventSource, 
                 target_var: Variable, weight_dtype: str, max_row_length: int,
                 num_sparse_connectivity_bits: int, name: str = ""):
        assert len(source_events.shape) == 1
        assert len(target_var.shape) == 1
        self.shape = (source_events.shape[0], target_var.shape[0])
        
        weight_shape = (source_events.shape[0], max_row_length)
        self.weight = backend.Variable(weight_shape, weight_dtype, f"{name}_weight")
        self.process = backend.SparseEventPropagationProcess(
            source_events, self.weight, target_var, 
            num_sparse_connectivity_bits, name)

class DelayLinear:
    def __init__(self, backend: ModuleType, source_events: EventSource, 
                 target_var: Variable, weight_dtype: str, num_delay_bits: int,
                 name: str = ""):
        assert len(source_events.shape) == 1
        assert len(target_var.shape) == 1
        self.shape = (source_events.shape[0], target_var.shape[0])

        self.weight = backend.Variable(self.shape, weight_dtype, f"{name}_weight")
        self.process = backend.DelayEventPropagationProcess(source_events,
                                                            self.weight,
                                                            target_var,
                                                            num_delay_bits,
                                                            name)