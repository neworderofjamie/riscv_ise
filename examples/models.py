import numpy as np

from pyfenn import (CopyProcess, EventContainer, EventPropagationProcess,
                    NeuronUpdateProcess, NumericValue, Parameter,
                    RNGInitProcess, Shape, UnresolvedType, Variable)

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

class LIF:
    def __init__(self, shape, tau_m: float, tau_refrac: int, v_thresh: float,
                 record_spikes: bool = False, dtype = "s10_5_sat_t",
                 name: str = ""):
        self.shape = Shape(shape)
        dtype = UnresolvedType(dtype)
        self.v = Variable(self.shape, dtype)
        self.i = Variable(self.shape, dtype)
        self.refrac_time = Variable(self.shape, UnresolvedType("int16_t"))
        self.out_spikes = EventContainer(self.shape, 
                                         (num_timesteps if record_spikes
                                          else 1))
        self.process = NeuronUpdateProcess(
            """
            V = (Alpha * V) + I;
            I = 0.0h5;
            if (RefracTime > 0) {
               RefracTime -= 1;
            }
            else if(V >= VThresh) {
               Spike();
               V -= VThresh;
               RefracTime = TauRefrac;
            }
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
                 record_spikes: bool = False, dtype = "s6_9_sat_t",
                 name: str = ""):
        self.shape = Shape(shape)
        dtype = UnresolvedType(dtype)
        decay_dtype = UnresolvedType("s0_15_sat_t")
        self.v = Variable(self.shape, dtype)
        self.a = Variable(self.shape, dtype)
        self.i = Variable(self.shape, dtype)
        self.refrac_time = Variable(self.shape, UnresolvedType("int16_t"))
        self.out_spikes = EventContainer(self.shape, 
                                         (num_timesteps if record_spikes
                                          else 1))
        self.process = NeuronUpdateProcess(
            """
            V = mul_rs(Alpha, V) + I;
            A = mul_rs(A, Rho);
            I = 0.0h7;
            if (RefracTime > 0) {
               RefracTime -= 1;
            }
            else if(V >= (VThresh + (Beta * A))) {
               Spike();
               V -= VThresh;
               A += 1.0h7;
               RefracTime = TauRefrac;
            }
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
                 dtype: str = "s10_5_sat_t", name: str = ""):
        self.shape = Shape(shape)
        dtype = UnresolvedType(dtype)

        self.v = Variable(self.shape, dtype)
        self.i = Variable(self.shape, dtype)
        self.v_avg = Variable(self.shape, dtype)
        self.bias = Variable(self.shape, dtype)
        self.process = NeuronUpdateProcess(
            """
            V = (Alpha * V) + I + Bias;
            I = 0.0h6;
            VAvg += (VAvgScale * V);
            """,
            {"Alpha": Parameter(NumericValue(np.exp(-1.0 / tau_m)), dtype), 
             "VAvgScale": Parameter(NumericValue(1.0 / (num_timesteps / 2)), dtype)},
            {"V": self.v, "VAvg": self.v_avg, "I": self.i, "Bias": self.bias},
            {}, name)

class Linear:
    def __init__(self, source_events: EventContainer, target_var: Variable,
                 weight_dtype: str, name: str = ""):
        self.shape = Shape([source_events.shape.num_neurons,
                            target_var.shape.num_neurons])
        weight_dtype = UnresolvedType(weight_dtype)

        self.weight = Variable(self.shape, weight_dtype)
        self.process = EventPropagationProcess(source_events, self.weight,
                                               target_var, name)

