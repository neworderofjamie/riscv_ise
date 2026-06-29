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
    def __init__(self, source_events: EventContainer, target_var: Variable, post_syn_voltage:Variable, 
                 post_syn_calcium:Variable, weight_dtype: str, syn_thresh: int, syn_inc_without_spike: int, 
                 syn_dec_without_spike: int,syn_inc_with_spike: int, syn_dec_with_spike: int, 
                 pos_syn_weight: int, neg_syn_weight: int, voltage_thresh:int, 
                 high_volt_calcium_low_thresh: int, high_volt_calcium_high_thresh:int,
                 low_volt_calcium_low_thresh:int, low_volt_calcium_high_thresh:int,
                 max_row_length: Optional[int] = None, 
                 num_sparse_connectivity_bits: int = 0, 
                 num_delay_bits: int = 0, name: str = ""):
        # Remember the synaptic threshold is divided by 2**(num fractional bits)
        self.shape = (source_events.shape.num_neurons,
                      target_var.shape.num_neurons)
        weight_shape = (source_events.shape.num_neurons,
                        (target_var.shape.num_neurons 
                         if num_sparse_connectivity_bits == 0 
                         else max_row_length))
        self.weight = Variable(weight_shape, weight_dtype, 1, f"{name}_weight")
        self.syn_thresh = syn_thresh
        self.syn_inc_without_spike = syn_inc_without_spike
        self.syn_dec_without_spike = syn_dec_without_spike
        self.syn_inc_with_spike = syn_inc_with_spike
        self.syn_dec_with_spike = syn_dec_with_spike

        self.pos_syn_weight = pos_syn_weight
        self.neg_syn_weight = neg_syn_weight

        self.voltage_thresh = voltage_thresh
        self.high_volt_calcium_low_thresh = high_volt_calcium_low_thresh
        self.high_volt_calcium_high_thresh = high_volt_calcium_high_thresh
        self.low_volt_calcium_low_thresh = low_volt_calcium_low_thresh
        self.low_volt_calcium_high_thresh = low_volt_calcium_high_thresh


        self.process = STDPEventPropagationProcess(source_events, self.weight,
                                               target_var, post_syn_voltage,
                                               post_syn_calcium, self.syn_thresh,
                                               self.syn_inc_without_spike,
                                               self.syn_dec_without_spike,
                                               self.syn_inc_with_spike,
                                               self.syn_dec_with_spike,
                                               self.pos_syn_weight,
                                               self.neg_syn_weight,
                                               self.voltage_thresh,
                                               self.high_volt_calcium_low_thresh,
                                               self.high_volt_calcium_high_thresh,
                                               self.low_volt_calcium_low_thresh,
                                               self.low_volt_calcium_high_thresh,
                                               num_sparse_connectivity_bits,
                                               num_delay_bits, name)
        