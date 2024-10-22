from bitstring import Bits
from sys import argv

b = Bits("0x" + argv[1])

print([b[i:i+16].int for i in range(len(b) - 16, -1, -16)])
