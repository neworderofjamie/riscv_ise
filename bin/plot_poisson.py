import numpy as np
import matplotlib.pyplot as plt

from scipy.stats import poisson

poisson_data = np.loadtxt("out_poisson_device.txt", dtype=int)
x = np.arange(np.amin(poisson_data), 1 + np.amax(poisson_data))

fig, axis = plt.subplots()
axis.hist(poisson_data, bins=x, density=True)
axis.plot(x, poisson.pmf(x, 5.12))
axis.set_xlabel("k")
axis.set_ylabel("P(x=k)")
plt.show()
