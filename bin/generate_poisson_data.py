import numpy as np

RATE = 7846 / 1370
NUM_TIMESTEPS = 5000
BAD_BOOST = 2

# Generate poisson data
data = np.random.poisson(RATE, NUM_TIMESTEPS)

data_gappy = np.zeros(NUM_TIMESTEPS)
data_gappy[0:2000] = np.random.poisson(RATE, 2000)
data_gappy[4000:5000] = np.random.poisson(RATE, 1000)
#data_bad = data.copy()
#data_bad[3000:3500] = np.random.poisson(RATE * 1.2, 500)

repeated_data = np.repeat(data[:,None], 32, axis=1).astype(np.int16)
repeated_data_bad = np.repeat(data_gappy[:,None], 32, axis=1).astype(np.int16)


repeated_data.tofile("poisson_data.bin")
repeated_data_bad.tofile("poisson_data_bad.bin")
