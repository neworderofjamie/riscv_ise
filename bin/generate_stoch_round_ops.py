import numpy as np

NUM_VECTORS = 1562

# Generate poisson data
data = np.random.randint(np.iinfo(np.int16).min, np.iinfo(np.int16).max,
                         size=NUM_VECTORS * 2 * 32, dtype=np.int16)

data.tofile("stoch_rand_operands.bin")
