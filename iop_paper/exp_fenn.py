import numpy as np

from argparse import ArgumentParser
from pyfenn import (BackendFeNNHW, BackendFeNNSim, Model, NeuronUpdateProcess,
                    ProcessGroup, Runtime, Variable)
from pyfenn.models import ExpLUTBroadcast

from pyfenn import disassemble, init_logging
from pyfenn.utils import (copy_and_push, generate_exp_lut_and_push,
                          get_array_view)

parser = ArgumentParser("Special function test")
parser.add_argument("--device", action="store_true", help="Run model on FeNN hardware")
parser.add_argument("--disassemble", action="store_true", help="Disassemble generated code")
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
min_val_fixed = np.iinfo(np.int16).min
max_val_fixed = np.iinfo(np.int16).max

input_data = np.arange(min_val_fixed, max_val_fixed + 1).astype(np.int16)
assert (len(input_data) % 32) == 0
init_logging()

# Model
input_var = Variable(input_data.shape, "s2_13_t")
exp = Exp(input_var, 9)
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

#exp_y_correct = np.rint(np.exp(exp_x) * out_fixed_point_one) / out_fixed_point_one

# Convert x and ex
np.savez("exp_fenn.npz", x=(input_data / (2 ** 13)), 
         exp_x=(exp_view[:len(input_data)] / (2 ** 9)))
