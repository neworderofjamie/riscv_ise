import matplotlib.pyplot as plt
import numpy as np

import plot_settings

fig, axes = plt.subplots(1, 3, figsize=(plot_settings.double_column_width, 2.0))

# Instruction counts
num_total = 4870831687
num_total_vector = 2048015638
num_scalar_jump = 671647121
num_scalar_mem = 219857040
num_scalar_alu = 1728445796
num_vector_mem = 1292095482
num_vector_alu = 549015134

num_other = (num_total - num_scalar_alu - num_scalar_mem - num_scalar_jump
             - num_vector_alu - num_vector_mem) 
axes[0].pie([num_scalar_alu, num_scalar_mem, num_scalar_jump, num_vector_alu, num_vector_mem, num_other],
             labels=["Scalar ALU", "Scalar Memory", "Scalar Control", "Vector ALU", "Vector Memory", "Other"])
plt.show()