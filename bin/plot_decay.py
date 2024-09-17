import matplotlib.pyplot as plt
import numpy as np

# Load and reshape fixed point decay data
decay_data = np.loadtxt("out_decay.txt")
decay_data = np.reshape(decay_data, (-1, 3, 32))

# Convert to floating point and calculate average 
# and standard deviation of each algorithm
decay_data = decay_data / (2.0 ** 14)
decay_avg = np.average(decay_data, axis=2)
decay_std = np.std(decay_data, axis=2)

# Generate correct exponential decay
t = np.arange(decay_data.shape[0])
decay_correct = np.exp(-t / 5000.0)

error = np.sqrt(np.sum(np.square(decay_avg - np.reshape(decay_correct, (-1,1))), axis=0))

# Plot
fig, axis = plt.subplots()
axis.plot(t[1:], decay_correct[1:], linestyle="--", label="Reference")
axis.plot(t[1:], decay_avg[:-1,0], label=f"RZ ({error[0]})")
axis.plot(t[1:], decay_avg[:-1,1], label=f"RN ({error[1]})")
axis.errorbar(t[1:], decay_avg[:-1,2], yerr=decay_std[:-1,2], label=f"RS ({error[2]})")
axis.legend()
axis.set_ylabel("Value")
axis.set_xlabel("T")
plt.show()
