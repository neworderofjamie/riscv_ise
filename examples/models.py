import numpy as np

from types import ModuleType

class LIF:
    def __init__(self, backend: ModuleType, shape, tau_m: float, 
                 tau_refrac: int, v_thresh: float, record_timesteps: int = 1,
                 fixed_point: int = 5, dt: float = 1.0, name: str = ""):
        self.shape = shape
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"
        self.v = backend.Variable(self.shape, dtype, name=f"{name}_v")
        self.i = backend.Variable(self.shape, dtype, name=f"{name}_i")
        self.refrac_time = backend.Variable(self.shape, "int16_t",
                                            name=f"{name}_refrac_time")
        channel = backend.EventChannel(self.shape, name=f"{name}_out_spikes")
        self.out_spikes = channel.source
        self.process = backend.NeuronUpdateProcess(
            f"""
            V = ({np.exp(-dt / tau_m)}h{fixed_point} * V) + I;
            I = 0.0h{fixed_point};
            if (RefracTime > 0) {{
               RefracTime -= 1;
            }}
            else if(V >= {v_thresh}h{fixed_point}) {{
               Spike();
               V -= {v_thresh}h{fixed_point};
               RefracTime = {tau_refrac};
            }}
            """,
            {"V": self.v, "I": self.i, "RefracTime": self.refrac_time},
            {"Spike": channel.sink},
            name=name)

class ALIF:
    def __init__(self, shape, tau_m: float, tau_a: float, tau_refrac: int,
                 v_thresh: float, beta: float = 0.0174,
                 record_timesteps: int = 1, fixed_point: int = 9,
                 dt: float = 1.0, name: str = ""):
        self.shape = shape
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"
        decay_dtype = "s0_15_sat_t"
        self.v = Variable(self.shape, dtype, name=f"{name}_v")
        self.a = Variable(self.shape, dtype, name=f"{name}_a")
        self.i = Variable(self.shape, dtype, name=f"{name}_i")
        self.refrac_time = Variable(self.shape, "int16_t",
                                    name=f"{name}_refrac_time")
        self.out_spikes = EventContainer(self.shape, record_timesteps,
                                         name=f"{name}_out_spikes")
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
            {"Alpha": Parameter(np.exp(-dt / tau_m), decay_dtype),
             "Rho": Parameter(np.exp(-dt / tau_a), decay_dtype),
             "Beta": Parameter(beta, dtype),
             "VThresh": Parameter(v_thresh, dtype),
             "TauRefrac": Parameter(int(round(tau_refrac / dt)), "int16_t")},
            {"V": self.v, "A": self.a, "I": self.i, "RefracTime": self.refrac_time},
            {"Spike": self.out_spikes},
            name=name)

class LI:
    def __init__(self, backend, shape, tau_m: float, num_timesteps: int,
                 fixed_point: int = 5, dt: float = 1.0, name: str = ""):
        self.shape = shape
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"

        self.v = backend.Variable(self.shape, dtype, name=f"{name}_v")
        self.i = backend.Variable(self.shape, dtype, name=f"{name}_i")
        self.v_avg = backend.Variable(self.shape, dtype, name=f"{name}_v_avg")
        self.bias = backend.Variable(self.shape, dtype, name=f"{name}_bias")
        self.process = backend.NeuronUpdateProcess(
            f"""
            V = ({np.exp(-dt / tau_m)}h{fixed_point} * V) + I + Bias;
            I = 0.0h{fixed_point};
            VAvg += ({1.0 / (num_timesteps / 2)}h{fixed_point} * V);
            """,
            {"V": self.v, "VAvg": self.v_avg, "I": self.i, "Bias": self.bias},
            {}, name=name)
