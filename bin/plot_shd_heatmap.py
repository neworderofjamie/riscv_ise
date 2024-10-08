import matplotlib.pyplot as plt
import numpy as np

heatmap = np.loadtxt("shd_heatmap.txt", dtype=int, delimiter=",")

# Named sets of op codes
op_code_set_names = ["Scalar ALU", "Scalar Memory", "Scalar Control",
                     "Vector ALU", "Vector Memory", "Other"]

def standard_op(funct1):
    return (funct1 << 2) | 3

def vector_op(funct1):
    return (funct1 << 2) | 2
    
op_code_map = {standard_op(4):  0,  # OP-IMM
               standard_op(12): 0,  # OP
               standard_op(0):  1,  # LOAD
               standard_op(8):  1,  # STORE
               standard_op(24): 2,  # BRANCH
               standard_op(25): 2,  # JALR
               standard_op(27): 2,  # JALR
               vector_op(0):    3,  # VSOP
               vector_op(0):    3,  # VSOP
               vector_op(4):    4,  # VLOAD
               vector_op(5):    4}  # VSTORE

sect_addresses = [8, 232, 508, 1028, 1136]
sect_names = ["Input spike", "Hidden spike", "Hidden neuron", "Output neuron"]
assert len(sect_names) == (len(sect_addresses) - 1)

sections = []
for start_addr, end_addr in zip(sect_addresses[0:-1], sect_addresses[1:]):
    sect_heatmap = heatmap[start_addr // 4:end_addr // 4,:]
    
    # Convert OP codes to class indices
    inst_class = np.asarray([op_code_map.get(o, 5) 
                             for o in sect_heatmap[:,0]])
    
    # Count instructions of each class in section
    sections.append([np.sum(sect_heatmap[inst_class == i,1])
                     for i, _ in enumerate(op_code_set_names)])

# Transpose sections
sections = list(zip(*sections))

fig, axis = plt.subplots()

bar_x = np.arange(len(sections[0]))
bottom = np.zeros_like(bar_x)
actors = []
for i, o in enumerate(op_code_set_names):
    actors.append(axis.bar(bar_x, sections[i], width=0.8, bottom=bottom))
    bottom += sections[i]

axis.set_xticks(bar_x, labels=sect_names)
fig.legend(actors, op_code_set_names, ncol=3)
plt.show()
