import numpy as np
import matplotlib.pyplot as plt

faithful_data = np.loadtxt("special_function_output.csv")

log_2 = np.log(2)
exp_max = 0.5 * log_2

exp_x = np.arange(-exp_max, exp_max, (2 * exp_max) /  (32 * 1024))
exp_y_correct = np.rint(np.exp(exp_x) * 2**14)

fig, axis = plt.subplots()
axis.plot(exp_x, faithful_data, label="Approx")
axis.plot(exp_x, exp_y_correct, label="Correct")
axis.legend()
plt.show()