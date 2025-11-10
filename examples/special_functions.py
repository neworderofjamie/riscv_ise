import numpy as np
import matplotlib.pyplot as plt

from argparse import ArgumentParser
from pyfenn import (BackendFeNNHW, BackendFeNNSim, Model, NeuronUpdateProcess,
                    ProcessGroup, Runtime, Variable)
from pyfenn.models import ExpLUTBroadcast

from pyfenn import disassemble, init_logging
from pyfenn.utils import (ceil_divide, copy_and_push, 
                          generate_exp_lut_and_push, get_array_view)

parser = ArgumentParser("Special function test")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
parser.add_argument("--input-fixed-point", type=int, default=9, help="Position of fixed-point in input values")
parser.add_argument("--output-fixed-point", type=int, default=3, help="Position of fixed-point in output values")
parser.add_argument("--min-val", type=float, default=1.0, help="Minimum value to evaluate")
parser.add_argument("--max-val", type=float, default=8.0, help="Maximum value to evaluate")
args = parser.parse_args()

class Exp:
    def __init__(self, input: Variable, fixed_point: int):
        dtype = f"s{15 - fixed_point}_{fixed_point}_t"
        self.val = Variable(input.shape, dtype)
        
        self.process = NeuronUpdateProcess(
            f"""
            Val = exp_{fixed_point}(In);
            """,
            {}, {"Val": self.val, "In": input})



# Generate input data
in_fixed_point_one = 2 ** args.input_fixed_point
min_val_fixed = np.round(args.min_val * in_fixed_point_one)
max_val_fixed = np.round(args.max_val * in_fixed_point_one)
count_padded = ceil_divide(max_val_fixed - min_val_fixed, 32) * 32
input_data = np.arange(min_val_fixed, min_val_fixed + count_padded).astype(np.int16)
init_logging()

# Model
in_dtype = f"s{15 - args.input_fixed_point}_{args.input_fixed_point}_t"
input_var = Variable(input_data.shape, in_dtype)
exp = Exp(input_var, args.output_fixed_point)
lut_broadcast = ExpLUTBroadcast()

# Group processes
init_processes = ProcessGroup([lut_broadcast.process])
exp_processes = ProcessGroup([exp.process])

# Create backend
backend = BackendFeNNHW() if args.device else BackendFeNNSim()

# Create model
model = Model([init_processes, exp_processes], backend)

# Generate init and sim code
code = backend.generate_kernel([init_processes, exp_processes], model)

# Disassemble if required
if args.disassemble:
    for i, c in enumerate(code):
        print(f"{i * 4} : {disassemble(c)}")

# Create runtime
runtime = Runtime(model, backend)

# Allocate memory for model
runtime.allocate()

# Copy input currents to device
copy_and_push(input_data, input_var, runtime)
generate_exp_lut_and_push(lut_broadcast.lut, runtime)

# Set instructions and run
runtime.set_instructions(code)
runtime.run()


exp_array, exp_view = get_array_view(runtime, exp.val,
                                     np.int16)
exp_array.pull_from_device()


out_fixed_point_one = 2 ** args.output_fixed_point
exp_x = input_data / in_fixed_point_one
exp_y_correct = np.rint(np.exp(exp_x))

fig, axis = plt.subplots()
axis.plot(exp_x, exp_view[:len(input_data)] / out_fixed_point_one, label="Approx")
axis.plot(exp_x, exp_y_correct, label="Correct")
axis.legend()
plt.show()