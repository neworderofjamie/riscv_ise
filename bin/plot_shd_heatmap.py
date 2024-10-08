import matplotlib.pyplot as plt
import numpy as np

heatmap = np.loadtxt("shd_heatmap.txt", dtype=int)

sections = [8, 232, 508, 1028, 1136]
names = ["Input spike", "Hidden spike", "Hidden neuron", "Output neuron"]
assert len(names) == (len(sections) - 1)

section_num = [np.sum(heatmap[start // 4:end // 4])
               for start, end in zip(sections[0:-1], sections[1:])]
assert len(section_num) == len(names)

fig, axis = plt.subplots()
axis.pie(section_num, labels=names)
plt.show()