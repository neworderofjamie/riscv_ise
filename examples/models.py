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


class Linear_LIF_STDP:
    def __init__(self, shape, alpha: float, c_tau:float, j_c:float,
                 v_thresh: float, v_reset: float,
                 record_timesteps: int = 1, fixed_point: int = 9,
                 dt: float = 1.0, name: str = ""):
        self.shape = shape
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"
        decay_dtype = "s0_15_sat_t"
        self.v = Variable(self.shape, dtype, name=f"{name}_v")
        self.i = Variable(self.shape, dtype, name=f"{name}_i")
        self.c = Variable(self.shape, dtype, name=f"{name}_c")
        self.decay_product = Variable(self.shape, decay_dtype, name=f"{name}_time_since_spike")
        self.out_spikes = EventContainer(self.shape, record_timesteps)
        self.process = NeuronUpdateProcess(
            f"""
            C *= CTau;

            if(V > VThresh) {{
                V = VReset;
                C += Jc;
            }}
            V += I-Alpha;
            if(V >= VThresh) {{
                Spike();
            }} 
            else if(V<VReset) {{
                V = VReset;
            }}
            I = 0.0h{fixed_point};
            """,
            {"Alpha": Parameter(alpha, dtype),
             "VThresh": Parameter(v_thresh, dtype),
             "VReset": Parameter(v_reset,dtype),
             "CTau": Parameter(np.exp(-1/c_tau),decay_dtype),
             "Jc": Parameter(j_c,dtype)},
            {"V": self.v, "I": self.i, "C":self.c, "DecayProd":self.decay_product},
            {"Spike": self.out_spikes},
            name)   
        

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
                 record_timesteps: int = 1, fixed_point: int = 5, name: str = ""):
        self.shape = shape
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"
        rand_dtype = "s0_15_sat_t"
        self.out_spikes = EventContainer(self.shape, record_timesteps)
        self.num_spikes = Variable(self.shape, dtype, name=f"{name}_num_spikes")
        self.process = NeuronUpdateProcess(
            f"""
            if(ProbSpike >= fennrand()) {{
               Spike();
            }}
            """,
            {"ProbSpike": Parameter(prob_spike, rand_dtype)},
            {"NumSpikes": self.num_spikes},
            {"Spike": self.out_spikes},
            name)

# Remember calling Spike() activates the event associated with that event container
# Remember that fennrand() outputs a 15 fractional bit number (between 0 and 1) so 
# any comparisons must also be with numbers containing 15 fractional bits.
