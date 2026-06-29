from typing import Optional

from pyfenn import (BroadcastProcess, EventContainer, EventPropagationProcess,
                    STDPEventPropagationProcess, MemsetProcess, RNGInitProcess, Variable)

class RNGInit:
    def __init__(self):
        self.seed = Variable(64, "int16_t")
        self.process = RNGInitProcess(self.seed)

class Memset:
    def __init__(self, target: Variable, name: str = ""):
        self.process = MemsetProcess(target, name)

class ExpLUTBroadcast:
    def __init__(self):
        self.lut = Variable(65, "int16_t", name="exp_lut_source")
        self.process = BroadcastProcess(self.lut, 2, "exp_lut_broadcast")

class Linear:
    def __init__(self, source_events: EventContainer, target_var: Variable,
                 weight_dtype: str, max_row_length: Optional[int] = None, 
                 num_sparse_connectivity_bits: int = 0, 
                 num_delay_bits: int = 0, name: str = ""):
        self.shape = (source_events.shape.num_neurons,
                      target_var.shape.num_neurons)
        weight_shape = (source_events.shape.num_neurons,
                        (target_var.shape.num_neurons 
                         if num_sparse_connectivity_bits == 0 
                         else max_row_length))
        self.weight = Variable(weight_shape, weight_dtype, 1, f"{name}_weight")
        self.process = EventPropagationProcess(source_events, self.weight,
                                               target_var, 
                                               num_sparse_connectivity_bits,
                                               num_delay_bits, name)

class LinearWithSTDP:
    def __init__(self, source_events: EventContainer, target_var: Variable, v_pre:Variable, 
                 c_pre:Variable, x_dtype: str, theta_x: int, a: int, 
                 b: int, alpha: int, beta: int, 
                 j_minus: int, j_plus: int, theta_v:int, 
                 theta_low_up: float, theta_low_down:float,
                 theta_high_up: float, theta_high_down:float,
                 name: str = ""):
        # Remember the synaptic threshold is divided by 2**(num fractional bits)
        self.shape = (source_events.shape.num_neurons,
                      target_var.shape.num_neurons)
        weight_shape = (source_events.shape.num_neurons,
                        (target_var.shape.num_neurons))
        self.x = Variable(weight_shape, x_dtype, 1, f"{name}_x")


        self.process = STDPEventPropagationProcess(source_events, self.x,
                                               target_var, 
                                               v_pre,
                                               c_pre, 
                                               theta_x,
                                               a,
                                               b,
                                               alpha,
                                               beta,
                                               j_minus,
                                               j_plus,
                                               theta_v,
                                               theta_low_up,
                                               theta_low_down,
                                               theta_high_up,
                                               theta_high_down, name)
        