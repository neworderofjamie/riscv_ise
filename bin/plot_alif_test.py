import numpy as np
import matplotlib.pyplot as plt
d = np.loadtxt("out_aliftest_14_9.txt", delimiter=",")

v = d[:, 0]
a = d[:, 1]
fig, axis = plt.subplots()
a_axis = axis.twinx()
axis.plot(v)
a_axis.plot(a)
plt.show()