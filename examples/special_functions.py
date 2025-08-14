import numpy as np
import matplotlib.pyplot as plt

from pyfenn import (BackendFeNNHW, BackendFeNNSim, Model, NeuronUpdateProcess,
                    ProcessGroup, Runtime, Shape, UnresolvedType, Variable)
from models import ExpLUTBroadcast

from pyfenn import disassemble, init_logging
from pyfenn.utils import (ceil_divide, copy_and_push, 
                          generate_exp_lut_and_push, get_array_view)


device = False
fixed_point = 9
min_val = -3.4
max_val = 3.4
disassemble_code = False

class Exp:
    def __init__(self, input: Variable, dtype):
        self.val = Variable(input.shape, dtype)
        
        self.process = NeuronUpdateProcess(
            """
            Val = exp(In);
            """,
            {}, {"Val": self.val, "In": input})



# Generate input data
fixed_point_one = 2 ** fixed_point
min_val_fixed = np.round(min_val * fixed_point_one)
max_val_fixed = np.round(max_val * fixed_point_one)
count_padded = ceil_divide(max_val_fixed - min_val_fixed, 32) * 32
input_data = np.arange(min_val_fixed, min_val_fixed + count_padded).astype(np.int16)
init_logging()

# Model
dtype = UnresolvedType(f"s{15 - fixed_point}_{fixed_point}_t")
input_var = Variable(Shape(input_data.shape), dtype)
exp = Exp(input_var, dtype)
lut_broadcast = ExpLUTBroadcast()

# Group processes
init_processes = ProcessGroup([lut_broadcast.process])
exp_processes = ProcessGroup([exp.process])

# Create backend
backend = BackendFeNNHW() if device else BackendFeNNSim()

# Create model
model = Model([init_processes, exp_processes], backend)

# Generate init and sim code
code = backend.generate_kernel([init_processes, exp_processes], model)

# Disassemble if required
if disassemble_code:
    for i, c in enumerate(code):
        print(f"{i * 4} : {disassemble(c)}")

# Create runtime
runtime = Runtime(model, backend)

# Allocate memory for model
runtime.allocate()

# **TODO** broadcast LUT

# Copy input currents to device
copy_and_push(input_data, input_var, runtime)
generate_exp_lut_and_push(lut_broadcast.lut, runtime)

# Set instructions and run
runtime.set_instructions(code)
runtime.run()



exp_array, exp_view = get_array_view(runtime, exp.val,
                                     np.int16)
exp_array.pull_from_device()


exp_x = input_data / fixed_point_one
exp_y_correct = np.rint(np.exp(exp_x) * fixed_point_one)

fig, axis = plt.subplots()
axis.plot(exp_x, exp_view[:len(input_data)], label="Approx")
axis.plot(exp_x, exp_y_correct, label="Correct")
axis.legend()
plt.show()