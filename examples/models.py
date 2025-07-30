import numpy as np

from typing import Optional

from pyfenn import (CopyProcess, EventContainer, EventPropagationProcess,
                    MemsetProcess, NeuronUpdateProcess, NumericValue,
                    Parameter, RNGInitProcess, Shape, UnresolvedType,
                    Variable)

class RNGInit:
    def __init__(self):
        self.seed = Variable(Shape([64]), UnresolvedType("int16_t"))
        self.process = RNGInitProcess(self.seed)

class Copy:
    def __init__(self, source: Variable, num_timesteps: int = 1):
        self.target = Variable(source.shape, 
                               UnresolvedType(source.type),
                               num_timesteps,
                               f"{source.name}_copy")
        self.process = CopyProcess(source, self.target)

class Memset:
    def __init__(self, target: Variable, name: str = ""):
        self.process = MemsetProcess(target, name)
        
class LIF:
    def __init__(self, shape, tau_m: float, tau_refrac: int, v_thresh: float,
                 record_spikes: bool = False, fixed_point: int = 5,
                 name: str = ""):
        self.shape = Shape(shape)
        dtype = UnresolvedType(f"s{15 - fixed_point}_{fixed_point}_sat_t")
        self.v = Variable(self.shape, dtype)
        self.i = Variable(self.shape, dtype)
        self.refrac_time = Variable(self.shape, UnresolvedType("int16_t"))
        self.out_spikes = EventContainer(self.shape, 
                                         (num_timesteps if record_spikes
                                          else 1))
        self.process = NeuronUpdateProcess(
            f"""
            V = (Alpha * V) + I;
            I = 0.0h{fixed_point};
            if (RefracTime > 0) {{
               RefracTime -= 1;
            }}
            else if(V >= VThresh) {{
               Spike();
               V -= VThresh;
               RefracTime = TauRefrac;
            }}
            """,
            {"Alpha": Parameter(NumericValue(np.exp(-1.0 / tau_m)), dtype),
             "VThresh": Parameter(NumericValue(v_thresh), dtype),
             "TauRefrac": Parameter(NumericValue(tau_refrac), UnresolvedType("int16_t"))},
            {"V": self.v, "I": self.i, "RefracTime": self.refrac_time},
            {"Spike": self.out_spikes},
            name)

class ALIF:
    def __init__(self, shape, tau_m: float, tau_a: float, tau_refrac: int,
                 v_thresh: float, beta: float = 0.0174,
                 record_spikes: bool = False, fixed_point: int = 9,
                 name: str = ""):
        self.shape = Shape(shape)
        dtype = UnresolvedType(f"s{15 - fixed_point}_{fixed_point}_sat_t")
        decay_dtype = UnresolvedType("s0_15_sat_t")
        self.v = Variable(self.shape, dtype)
        self.a = Variable(self.shape, dtype)
        self.i = Variable(self.shape, dtype)
        self.refrac_time = Variable(self.shape, UnresolvedType("int16_t"))
        self.out_spikes = EventContainer(self.shape, 
                                         (num_timesteps if record_spikes
                                          else 1))
        self.process = NeuronUpdateProcess(
            f"""
            V = mul_rs(Alpha, V) + I;
            A = mul_rs(A, Rho);
            I = 0.0h{fixed_point};
            if (RefracTime > 0) {{
               RefracTime -= 1;
            }}
            else if(V >= (VThresh + (Beta * A))) {{
               Spike();
               V -= VThresh;
               A += 1.0h{fixed_point};
               RefracTime = TauRefrac;
            }}
            """,
            {"Alpha": Parameter(NumericValue(np.exp(-1.0 / tau_m)), decay_dtype),
             "Rho": Parameter(NumericValue(np.exp(-1.0 / tau_a)), decay_dtype),
             "Beta": Parameter(NumericValue(beta), dtype),
             "VThresh": Parameter(NumericValue(v_thresh), dtype),
             "TauRefrac": Parameter(NumericValue(tau_refrac), UnresolvedType("int16_t"))},
            {"V": self.v, "A": self.a, "I": self.i, "RefracTime": self.refrac_time},
            {"Spike": self.out_spikes},
            name)

class LI:
    def __init__(self, shape, tau_m: float, num_timesteps: int,
                 fixed_point: int = 5, name: str = ""):
        self.shape = Shape(shape)
        dtype = UnresolvedType(f"s{15 - fixed_point}_{fixed_point}_sat_t")

        self.v = Variable(self.shape, dtype)
        self.i = Variable(self.shape, dtype)
        self.v_avg = Variable(self.shape, dtype)
        self.bias = Variable(self.shape, dtype)
        self.process = NeuronUpdateProcess(
            f"""
            V = (Alpha * V) + I + Bias;
            I = 0.0h{fixed_point};
            VAvg += (VAvgScale * V);
            """,
            {"Alpha": Parameter(NumericValue(np.exp(-1.0 / tau_m)), dtype), 
             "VAvgScale": Parameter(NumericValue(1.0 / (num_timesteps / 2)), dtype)},
            {"V": self.v, "VAvg": self.v_avg, "I": self.i, "Bias": self.bias},
            {}, name)

class Linear:
    def __init__(self, source_events: EventContainer, target_var: Variable,
                 weight_dtype: str, max_row_length: Optional[int] = None, 
                 num_sparse_connectivity_bits: int = 0, name: str = ""):
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
                                               name)
