import numpy as np
import matplotlib.pyplot as plt

faithful_data = np.loadtxt("special_function_output.csv")

exp_x = np.linspace(-3.4, 3.4, 3482)
exp_y_correct = np.rint(np.exp(exp_x) * 2**9)

fig, axis = plt.subplots()
axis.plot(exp_x, faithful_data[:3482], label="Approx")
axis.plot(exp_x, exp_y_correct[:3482], label="Correct")
axis.legend()
plt.show()