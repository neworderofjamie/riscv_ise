import numpy as np

from pyfenn import Runtime

def get_array_view(runtime: Runtime, state, dtype):
    array = runtime.get_array(state)
    view = np.asarray(array.host_view).view(dtype)
    assert not view.flags["OWNDATA"]
    return array, view

# Divide two integers, rounding up i.e. effectively taking ceil
def ceil_divide(numerator, denominator):
    return (numerator + denominator - 1) // denominator

def load_and_push(filename: str, state, runtime: Runtime):
    # Load data from file
    data = np.fromfile(filename, dtype=np.uint8)
    
    # Get array and view
    array, view = get_array_view(runtime, state, np.uint8)
    assert array.host_view.nbytes == data.nbytes

    # Copy data to array host pointer
    view[:] = data

    # Push to device
    array.push_to_device()

def zero_and_push(state, runtime: Runtime):
    # Get array and view
    array, view = get_array_view(runtime, state, np.uint8)
 
    # Zero
    view[:] = 0

    # Push to device
    array.push_to_device()


def seed_and_push(state, runtime: Runtime):
    # Get array and view
    array, view = get_array_view(runtime, state, np.int16)
 
    # Zero
    int16_info = np.iinfo(np.int16)
    view[:] = np.random.randint(int16_info.min, int16_info.max, 64, dtype=np.int16)

    # Push to device
    array.push_to_device()