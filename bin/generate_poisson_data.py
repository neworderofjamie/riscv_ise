import numpy as np

BACKGROUND_RATE = 0.5
RATE = 7846 / 1370
NUM_TIMESTEPS = 5000

# Generate poisson data with two periods of average firing interspersed by background
data = np.zeros(NUM_TIMESTEPS)
data[0:2000] = np.random.poisson(RATE, 2000)
data[2000:4000] = np.random.poisson(BACKGROUND_RATE, 2000)
data[4000:5000] = np.random.poisson(RATE, 1000)

data_bad = np.zeros(NUM_TIMESTEPS)
data_bad[0:2000] = np.random.poisson(RATE, 2000)
data_bad[2000:4000] = np.random.poisson(RATE * 1.4, 2000)
data_bad[4000:5000] = np.random.poisson(RATE * 1.7, 1000)

repeated_data = np.repeat(data[:,None], 32, axis=1).astype(np.int16)
repeated_data_bad = np.repeat(data_bad[:,None], 32, axis=1).astype(np.int16)


repeated_data.tofile("poisson_data.bin")
repeated_data_bad.tofile("poisson_data_bad.bin")
