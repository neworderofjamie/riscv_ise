import numpy as np
import matplotlib.pyplot as plt

faithful_data = np.loadtxt("special_function_output.csv")

count = 3584
exp_x = np.linspace(1, 8, count)
exp_y_correct = np.rint(np.exp(exp_x) * 2**3)

print(faithful_data[:count])
fig, axis = plt.subplots()
axis.plot(exp_x, faithful_data[:count], label="Approx")
axis.plot(exp_x, exp_y_correct[:count], label="Correct")
axis.legend()
plt.show()