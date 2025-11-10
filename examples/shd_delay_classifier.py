import numpy as np
import mnist

from argparse import ArgumentParser
from pyfenn import (BackendFeNNHW, BackendFeNNSim, EventContainer, Model,
                    NeuronUpdateProcess, PlogSeverity, ProcessGroup,
                    Parameter, PerformanceCounter, RoundingMode, Runtime,
                    Variable)
from pyfenn.models import Linear, Memset
from tonic.datasets import SHD

from pyfenn import disassemble, init_logging
from pyfenn.utils import (build_delay_weights, ceil_divide, copy_and_push, 
                          get_array_view, load_quantise_and_push, pull_spikes,
                          read_perf_counter, quantise, zero_and_push)
from tqdm.auto import tqdm, trange

class LIF:
    def __init__(self, shape, tau_m: float, tau_syn: float, v_thresh: float,
                 fixed_point: int = 5, spike_record_timesteps: int = 1,
                 i_delay_timesteps: int = 1, name: str = ""):
        self.shape = shape
        decay_dtype = "s0_15_sat_t"
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"
        self.v = Variable(self.shape, dtype, name=f"{name}_v")
        self.i = Variable(self.shape, dtype, i_delay_timesteps, name=f"{name}_i")
        self.out_spikes = EventContainer(self.shape, spike_record_timesteps)
        self.process = NeuronUpdateProcess(
            f"""
            // Synapse
            s{15 - fixed_point}_{fixed_point}_sat_t inSyn = I * IScale;
            I *= Beta;

            // Neuron
            V = (Alpha * V) + (OneMinusAlpha * inSyn);
            
            if(V >= VThresh) {{
               Spike();
               V = 0.0h{fixed_point};
            }}
            """,
            {"Alpha": Parameter(np.exp(-1.0 / tau_m), decay_dtype),
             "OneMinusAlpha": Parameter(1.0 - np.exp(-1.0 / tau_m), decay_dtype),
             "Beta": Parameter(np.exp(-1.0 / tau_syn), decay_dtype),
             "IScale": Parameter(tau_syn * (1.0 - np.exp(-1.0 / tau_syn)), decay_dtype),
             "VThresh": Parameter(v_thresh, dtype)},
            {"V": self.v, "I": self.i},
            {"Spike": self.out_spikes},
            name)

class LI:
    def __init__(self, shape, tau_m: float, tau_syn: float, num_timesteps: int,
                 fixed_point: int = 5, name: str = ""):
        self.shape = shape
        decay_dtype = "s0_15_sat_t"
        dtype = f"s{15 - fixed_point}_{fixed_point}_sat_t"

        self.v = Variable(self.shape, dtype, name=f"{name}_v")
        self.v_avg = Variable(self.shape, dtype, name=f"{name}_v_avg")
        self.i = Variable(self.shape, dtype, name=f"{name}_i")
        self.process = NeuronUpdateProcess(
            f"""
            // Synapse
            s{15 - fixed_point}_{fixed_point}_sat_t inSyn = I * IScale;
            I *= Beta;

            // Neuron
            V = (Alpha * V) + (OneMinusAlpha * inSyn);
            VAvg += (VAvgScale * V);
            """,
            {"Alpha": Parameter(np.exp(-1.0 / tau_m), decay_dtype), 
             "OneMinusAlpha": Parameter(1.0 - np.exp(-1.0 / tau_m), decay_dtype),
             "Beta": Parameter(np.exp(-1.0 / tau_syn), decay_dtype),
             "IScale": Parameter(tau_syn * (1.0 - np.exp(-1.0 / tau_syn)), decay_dtype),
             "VAvgScale": Parameter(1.0 / (num_timesteps / 2), decay_dtype)},
            {"V": self.v, "VAvg": self.v_avg, "I": self.i},
            {}, name)
        
num_timesteps = 1170
input_shape = 700
hidden_shape = 256
output_shape = 20
input_hidden_shape = (input_shape, hidden_shape)
hidden_hidden_shape = (hidden_shape, hidden_shape)
hidden_output_shape = (hidden_shape, output_shape)
num_delay_bits = 7

parser = ArgumentParser("SHD classifier using trained synaptic delays")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--time", action="store_true", help="Record detailed timings using performance counters")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
parser.add_argument("--num-cores", type=int, default=1, help="Number of FeNN cores to distribute inference across")
args = parser.parse_args()

# Load and preprocess SHD
dataset = SHD(save_to="data", train=False)

# Loop through dataset
shd_spikes = []
shd_labels = []
timestep_range = np.arange(num_timesteps + 1)
neuron_range = np.arange((ceil_divide(input_shape, 32) * 32) + 1)
for events, label in tqdm(dataset, "Preprocessing dataset"):
    # Build histogram
    spike_event_histogram = np.histogram2d(events["t"] / 1000.0, events["x"], (timestep_range, neuron_range))[0]
    spike_event_histogram = np.minimum(spike_event_histogram, 1).astype(bool)
    spike_event_bits = np.packbits(spike_event_histogram, axis=1, bitorder="little")
    shd_spikes.append(spike_event_bits.view(np.uint32).flatten())
    shd_labels.append(label)

init_logging(PlogSeverity.INFO)

# Input spikes
input_spikes = EventContainer(input_shape, num_timesteps)

# Model
hidden = LIF(hidden_shape, 20.0, 5.0, 1.0,
             8, 1, 2**(num_delay_bits - 1), name="hidden")
output = LI(output_shape, 20.0, 5.0, num_timesteps, 8, name="output")

input_hidden = Linear(input_spikes, hidden.i, "s7_8_sat_t", 
                      num_delay_bits=num_delay_bits, name="input_hidden")
hidden_hidden = Linear(hidden.out_spikes, hidden.i, "s7_8_sat_t", 
                       num_delay_bits=num_delay_bits, name="hidden_hidden")
hidden_output = Linear(hidden.out_spikes, output.i, "s7_8_sat_t", name="hidden_output")


# Zero remaining state
hidden_i_zero = Memset(hidden.i)
hidden_v_zero = Memset(hidden.v)
output_i_zero = Memset(output.i)
output_v_zero = Memset(output.v)
output_v_avg_zero = Memset(output.v_avg)

# Group processes
neuron_update_processes = ProcessGroup([hidden.process, output.process],
                                       PerformanceCounter() if args.time else None)
synapse_update_processes = ProcessGroup([input_hidden.process, hidden_hidden.process, 
                                         hidden_output.process],
                                        PerformanceCounter() if args.time else None)
reset_processes = ProcessGroup([hidden_i_zero.process, hidden_v_zero.process,
                                output_i_zero.process, output_v_zero.process,
                                output_v_avg_zero.process],
                               PerformanceCounter() if args.time else None)

# Create backend for each core
if args.device:
    backends = [BackendFeNNHW(rounding_mode=RoundingMode.STOCHASTIC, 
                              core=i, num_cores=args.num_cores)
                for i in range(args.num_cores)]
else:
    assert args.num_cores == 1
    backends = [BackendFeNNSim(rounding_mode=RoundingMode.STOCHASTIC)]

# Create model for each core (not really necessary)
models = [Model([neuron_update_processes, synapse_update_processes, reset_processes], b)
          for b in backends]

# Generate code
sim_codes = [b.generate_simulation_kernel([synapse_update_processes, neuron_update_processes],
                                          [reset_processes], [],
                                          num_timesteps, m)
             for b, m in zip(backends, models)]

# Disassemble if required
if args.disassemble:
    print("Simulation:")
    for i, c in enumerate(sim_codes[0]):
        print(f"{i * 4} : {disassemble(c)}")

# Create runtimes
runtimes = [Runtime(m, b) for b, m in zip(backends, models)]


# Load and round delays
in_hid_delays = np.round(np.load("checkpoints_6_1_256_62_1_0_1.0_1_5e-12/best-Conn_Pop0_Pop1-d.npy")).astype(np.uint16)
hid_hid_delays = np.round(np.load("checkpoints_6_1_256_62_1_0_1.0_1_5e-12/best-Conn_Pop1_Pop1-d.npy")).astype(np.uint16)

# Load and quantise weights
in_hid_weights = quantise(np.load("checkpoints_6_1_256_62_1_0_1.0_1_5e-12/best-Conn_Pop0_Pop1-g.npy"),
                          8)
                          
hid_hid_weights = quantise(np.load("checkpoints_6_1_256_62_1_0_1.0_1_5e-12/best-Conn_Pop1_Pop1-g.npy"),
                           8) 

# Loop through runtimes
input_spike_array_views = []
output_v_avg_array_views = []
for r, s in zip(runtimes, sim_codes):
    r.allocate()

    # Combine weights and delays and push
    copy_and_push(build_delay_weights(in_hid_weights, in_hid_delays, num_delay_bits),
                  input_hidden.weight, r)
    copy_and_push(build_delay_weights(hid_hid_weights, hid_hid_delays, num_delay_bits),
                  hidden_hidden.weight, r)

    # Load and quantise output weights
    load_quantise_and_push("checkpoints_6_1_256_62_1_0_1.0_1_5e-12/best-Conn_Pop1_Pop2-g.npy",
                           8, hidden_output.weight, r, hidden_shape, True)

    if args.time:
        zero_and_push(neuron_update_processes.performance_counter, r)
        zero_and_push(synapse_update_processes.performance_counter, r)
        zero_and_push(reset_processes.performance_counter, r)
        
    # Set sim instructions
    r.set_instructions(s)

    # Loop through examples
    input_spike_array_views.append(get_array_view(r, input_spikes, np.uint32))
    output_v_avg_array_views.append(get_array_view(r, output.v_avg, np.int16))
    
num_correct = 0
for e in trange(0, len(shd_spikes), args.num_cores, desc="Simulating"):
    # Copy spikes to each core and start classifying
    for r, i, s in zip(runtimes, input_spike_array_views, shd_spikes[e:e + args.num_cores]):
        i[1][:] = s
        i[0].push_to_device()
        
        r.start_run()

    # Wait
    for r, o, l in zip(runtimes, output_v_avg_array_views, shd_labels[e:e + args.num_cores]):
        r.wait_run()
    
        # Copy output V sum from device
        o[0].pull_from_device()

        # Determine if output is correct
        classification = np.argmax(o[1])
        if classification == l:
            num_correct += 1

print(f"{num_correct} / {len(shd_labels)} correct {100.0 * (num_correct / len(shd_labels))}%")


if args.time:
    for i, r in enumerate(runtimes):
        print(f"Core {i}")
        neuron_update_cycles, neuron_update_instructions = read_perf_counter(
            neuron_update_processes.performance_counter, r)
        synapse_update_cycles, synapse_update_instructions = read_perf_counter(
            synapse_update_processes.performance_counter, r)
        reset_cycles, reset_instructions = read_perf_counter(
            reset_processes.performance_counter, r)
        print(f"\tNeuron update {neuron_update_cycles} cycles, {neuron_update_instructions} instruction ({neuron_update_instructions / neuron_update_cycles})")
        print(f"\tSynapse update {synapse_update_cycles} cycles, {synapse_update_instructions} instruction ({synapse_update_instructions / synapse_update_cycles})")
        print(f"\tReset {reset_cycles} cycles, {reset_instructions} instruction ({reset_instructions / reset_cycles})")

