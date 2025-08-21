import numpy as np
import matplotlib.pyplot as plt

faithful_data = np.loadtxt("special_function_output.csv", delimiter=",")


exp_x_fixed = faithful_data[:,0]
exp_x = exp_x_fixed / (2 ** 9)
exp_y_correct = np.rint(np.exp(exp_x) * 2**3)

fig, axis = plt.subplots()
axis.plot(exp_x, faithful_data[:,1], label="Approx")
axis.plot(exp_x, exp_y_correct, label="Correct")
axis.legend()
plt.show()