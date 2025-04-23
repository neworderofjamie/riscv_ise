import numpy as np
import sparse_utils

from sparse_utils import generate_fixed_prob, pad_connectivity
NUM_NEURONS = 512

PROBABILITY_CONNECTION = 0.1

EXCITATORY_INHIBITORY_RATIO = 4.0

NUM_EXCITATORY = int(round((NUM_NEURONS * EXCITATORY_INHIBITORY_RATIO) / (1.0 + EXCITATORY_INHIBITORY_RATIO)))
NUM_INHIBITORY = NUM_NEURONS - NUM_EXCITATORY

np.random.seed(1234)

def get_theoretical_mem(row_ind):
    return sum(len(r) for r in row_ind) * 2

# Generate connectivity matrices
ie_conn = generate_fixed_prob(NUM_INHIBITORY, NUM_EXCITATORY, PROBABILITY_CONNECTION)
ii_conn = generate_fixed_prob(NUM_INHIBITORY, NUM_INHIBITORY, PROBABILITY_CONNECTION)
ee_conn = generate_fixed_prob(NUM_EXCITATORY, NUM_EXCITATORY, PROBABILITY_CONNECTION)
ei_conn = generate_fixed_prob(NUM_EXCITATORY, NUM_INHIBITORY, PROBABILITY_CONNECTION)
theoretical_bytes = (get_theoretical_mem(ie_conn) + get_theoretical_mem(ii_conn)
                     + get_theoretical_mem(ee_conn) + get_theoretical_mem(ei_conn))

print(ii_conn[0])

# Pad
ie_conn = pad_connectivity(ie_conn).astype(np.int16)
ii_conn = pad_connectivity(ii_conn).astype(np.int16)
ee_conn = pad_connectivity(ee_conn).astype(np.int16)
ei_conn = pad_connectivity(ei_conn).astype(np.int16)
print(ii_conn[0])
print(f"IE :{ie_conn.shape}")
print(f"II :{ii_conn.shape}")
print(f"EE :{ee_conn.shape}")
print(f"EI :{ei_conn.shape}")

total_bytes = ie_conn.nbytes + ii_conn.nbytes + ee_conn.nbytes + ei_conn.nbytes
print(f"Connectivity requires {total_bytes / 1024}KB memory vs {theoretical_bytes / 1024}KB theoretical vs {NUM_NEURONS * NUM_NEURONS * 2 / 1024}KB dense")

# Save binary files
ie_conn.tofile("va_benchmark_ie.bin")
ii_conn.tofile("va_benchmark_ii.bin")
ee_conn.tofile("va_benchmark_ee.bin")
ei_conn.tofile("va_benchmark_ei.bin")