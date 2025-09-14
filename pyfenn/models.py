from typing import Optional

from pyfenn import (BroadcastProcess, EventContainer, EventPropagationProcess,
                    MemsetProcess, RNGInitProcess, Shape, Variable)

class RNGInit:
    def __init__(self):
        self.seed = Variable(Shape([64]), "int16_t")
        self.process = RNGInitProcess(self.seed)

class Memset:
    def __init__(self, target: Variable, name: str = ""):
        self.process = MemsetProcess(target, name)

class ExpLUTBroadcast:
    def __init__(self):
        self.lut = Variable(Shape([65]), "int16_t", name="exp_lut_source")
        self.process = BroadcastProcess(self.lut, 2, "exp_lut_broadcast")

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
        self.weight = Variable(weight_shape, weight_dtype, 1, f"{name}_weight")
        self.process = EventPropagationProcess(source_events, self.weight,
                                               target_var, 
                                               num_sparse_connectivity_bits,
                                               num_delay_bits, name)