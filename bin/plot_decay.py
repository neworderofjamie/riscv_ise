import matplotlib.pyplot as plt
import numpy as np

decay_data = np.loadtxt("out_decay.txt")
decay_data = np.reshape(decay_data, (-1, 3, 32))

decay_data = decay_data / (2.0**14)
decay_avg = np.average(decay_data, axis=2)
decay_std = np.std(decay_data, axis=2)
print(decay_std)
t = np.arange(5000)
decay_correct = np.exp(-t/1000.0)

fig, axis = plt.subplots()
axis.plot(t[1:], decay_correct[1:], linestyle="--", label="Reference")
axis.plot(t[1:], decay_avg[:-1,0], label="RZ")
axis.plot(t[1:], decay_avg[:-1,1], label="RN")
axis.plot(t[1:], decay_avg[:-1,2], label="RS")
axis.errorbar(t[1:], decay_avg[:-1,2], yerr=decay_std[:-1,2])
axis.legend()
axis.set_ylabel("Value")
axis.set_xlabel("T")
plt.show()
