import numpy as np

NUM_VECTORS = 680

# Generate poisson data
data = np.random.randint(np.iinfo(np.int16).min, np.iinfo(np.int16).max,
                         size=(NUM_VECTORS, 2, 32), dtype=np.int16)
zeros = np.zeros((NUM_VECTORS, 1, 32), dtype=np.int16)
data = np.concatenate((data, zeros), axis=1)
data.tofile("stoch_rand_operands.bin")
