import numpy as np

from pyfenn import (EventContainer, NeuronUpdateProcess,
                    Parameter, Variable)

class LIF:
    def __init__(self, shape, tau_m: float, tau_refrac: int, v_thresh: float,
                 record_timesteps: int = 1, fixed_point: int = 5,
                 dt: float = 1.0, name: str = ""):
        self.shape = shape
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"
        self.v = Variable(self.shape, dtype, name=f"{name}_v")
        self.i = Variable(self.shape, dtype, name=f"{name}_i")
        self.refrac_time = Variable(self.shape, "int16_t",
                                    name=f"{name}_refrac_time")
        self.out_spikes = EventContainer(self.shape, record_timesteps)
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
            {"Alpha": Parameter(np.exp(-dt / tau_m), dtype),
             "VThresh": Parameter(v_thresh, dtype),
             "TauRefrac": Parameter(int(round(tau_refrac / dt)), "int16_t")},
            {"V": self.v, "I": self.i, "RefracTime": self.refrac_time},
            {"Spike": self.out_spikes},
            name)

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
        self.out_spikes = EventContainer(self.shape, record_timesteps)
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
            name)


class ALIF_STDP:
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
        self.out_spikes = EventContainer(self.shape, record_timesteps)
        self.num_spikes = Variable(self.shape, dtype, name=f"{name}_num_spikes")
        self.process = NeuronUpdateProcess(
            f"""
            V = I;
            NumSpikes = 0.0h{fixed_point};
            if(V >= (VThresh)) {{
               Spike();
               RefracTime = TauRefrac;
            }}
            """,
            {"Alpha": Parameter(np.exp(-dt / tau_m), decay_dtype),
             "Rho": Parameter(np.exp(-dt / tau_a), decay_dtype),
             "Beta": Parameter(beta, dtype),
             "VThresh": Parameter(v_thresh, dtype),
             "TauRefrac": Parameter(int(round(tau_refrac / dt)), "int16_t")},
            {"V": self.v, "A": self.a, "I": self.i, "RefracTime": self.refrac_time, "NumSpikes": self.num_spikes},
            {"Spike": self.out_spikes},
            name)

class LI_SIMPLE:
    def __init__(self, shape, tau_m: float, num_timesteps: int,
                 fixed_point: int = 5, dt: float = 1.0, name: str = ""):
        self.shape = shape
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"

        self.v = Variable(self.shape, dtype, name=f"{name}_v")
        self.i = Variable(self.shape, dtype, name=f"{name}_i")
        self.bias = Variable(self.shape, dtype, name=f"{name}_bias")
        print("Here is self.bias: ", self.bias)
        ## REMEMBER TO DELETE THE BIAS =0 I=0
        self.process = NeuronUpdateProcess(
            f"""
            Bias = 0.0h{fixed_point};
            V = I + Bias;
            """,
            {"Alpha": Parameter(np.exp(-dt / tau_m), dtype), 
             "VAvgScale": Parameter(1.0 / (num_timesteps / 2), dtype)},
            {"V": self.v, "I": self.i, "Bias": self.bias},
            {}, name)
# Looks like the bias defaults to approx -133       
        

class LI:
    def __init__(self, shape, tau_m: float, num_timesteps: int,
                 fixed_point: int = 5, dt: float = 1.0, name: str = ""):
        self.shape = shape
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"

        self.v = Variable(self.shape, dtype, name=f"{name}_v")
        self.i = Variable(self.shape, dtype, name=f"{name}_i")
        self.v_avg = Variable(self.shape, dtype, name=f"{name}_v_avg")
        self.bias = Variable(self.shape, dtype, name=f"{name}_bias")
        self.process = NeuronUpdateProcess(
            f"""
            V = (Alpha * V) + I + Bias;
            I = 0.0h{fixed_point};
            VAvg += (VAvgScale * V);
            """,
            {"Alpha": Parameter(np.exp(-dt / tau_m), dtype), 
             "VAvgScale": Parameter(1.0 / (num_timesteps / 2), dtype)},
            {"V": self.v, "VAvg": self.v_avg, "I": self.i, "Bias": self.bias},
            {}, name)
        
class Bernoulli:
    def __init__(self, shape, prob_spike: float,
                 record_timesteps: int = 1, fixed_point: int = 9, name: str = ""):
        self.shape = shape
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"
        decay_dtype = "s0_15_sat_t"
        self.out_spikes = EventContainer(self.shape, record_timesteps)
        self.num_spikes = Variable(self.shape, dtype, name=f"{name}_v")
        self.process = NeuronUpdateProcess(
            f"""
            if(3 >= 6) {{
               Test();
            }}
            """,
            {"ProbSpike": Parameter(prob_spike, decay_dtype)},
            {"NumSpikes": self.num_spikes},
            {"Test": self.out_spikes},
            name)
        
            # f"""
            # if(ProbSpike >= fennrand()) {{
            #    Test();
            # }}
            # else {{
            #    Test();
            # }}
            # """,