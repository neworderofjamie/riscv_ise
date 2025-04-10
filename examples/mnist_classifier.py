import numpy as np

from pyfenn import (EventContainer, EventPropagationProcess,
                    NeuronUpdateProcess, Shape, UnresolvedType, 
                    Variable)

num_timesteps = 79
input_shape = Shape([28 * 28])
hidden_shape = Shape([128])
output_shape = Shape([10])
input_hidden_shape = Shape([28 * 28, 128])
hidden_output_shape = Shape([128, 10])
device = False
record = False
    
# Input spikes
input_spikes = EventContainer(input_shape, num_timesteps)

# Hidden neurons
hidden_v = Variable(hidden_shape, "s10_5_sat_t")
hidden_i = Variable(hidden_shape, "s10_5_sat_t")
hidden_refrac_time = Variable(hidden_shape, "int16_t")
hidden_spikes = EventContainer(hidden_shape, num_timesteps if record else 1);
hidden = NeuronUpdateProcess(
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
    {"Alpha": Parameter(np.exp(-1.0 / 20.0), "s10_5_sat_t"),
     "VThresh": Parameter(0.61, "s10_5_sat_t"),
     "TauRefrac": Parameter(5, "int16_t")},
    {"V": hidden_v, "I": hidden_i, "RefracTime": hidden_refrac_time},
    {"Spike": hidden_spikes})

# Output neurons
output_v = Variable(outputShape, "s9_6_sat_t")
output_i = Variable(outputShape, "s9_6_sat_t")
output_v_avg = Variable(outputShape, "s9_6_sat_t")
output_bias = Variable(outputShape, "s9_6_sat_t")
output = NeuronUpdateProcess(
    """
    V = (Alpha * V) + I + Bias;
    I = 0.0h6;
    VAvg += (VAvgScale * V);
    """,
    {"Alpha": Parameter(np.exp(-1.0 / 20.0), "s9_6_sat_t"), 
     "VAvgScale": Parameter(1.0 / (numTimesteps / 2), "s9_6_sat_t")},
    {"V": output_v, "VAvg": output_v_avg, "I": output_i, "Bias": outputBias})

# Input->Hidden event propagation
input_hidden_weight = Variable(input_hidden_shape, "s10_5_sat_t")
input_hidden = EventPropagationProcess(input_spikes, input_hidden_weight, hidden_i)

# Hidden->Output event propagation
hidden_output_weight = Variable(hidden_output_shape, "s9_6_sat_t", hidden_output_weight)
hidden_output = EventPropagationProcess(hidden_spikes, hiddenOutputWeight, output_i)

# Group processes
neuron_update_processes = ProcessGroup([hidden, output])
synapse_update_processes = ProcessGroup([input_hidden, hidden_output])