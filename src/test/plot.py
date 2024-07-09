import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("out.txt")
data = np.reshape(data, (-1,32))

FIXED_POINT = 13
scale = 2 ** FIXED_POINT
fig, axis = plt.subplots()
for a in range(32):
    axis.plot((a*2) + (data[:,a] / scale))
plt.show()
