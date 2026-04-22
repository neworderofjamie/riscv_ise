import numpy as np

from sparse_utils import generate_fixed_prob, pad_connectivity, split
NUM_NEURONS = 1024
NUM_CORES = 2
PROBABILITY_CONNECTION = 0.1

EXCITATORY_INHIBITORY_RATIO = 4.0

NUM_EXCITATORY = int(round((NUM_NEURONS * EXCITATORY_INHIBITORY_RATIO) / (1.0 + EXCITATORY_INHIBITORY_RATIO)))
NUM_INHIBITORY = NUM_NEURONS - NUM_EXCITATORY

np.random.seed(1234)

def get_theoretical_mem(row_ind):
    return sum(len(r) for r in row_ind) * 2
    
def pad_vector(num):
    return ((num + 31) // 32) * 32

# Generate connectivity matrices
ie_conn = generate_fixed_prob(NUM_INHIBITORY, NUM_EXCITATORY, PROBABILITY_CONNECTION)
ii_conn = generate_fixed_prob(NUM_INHIBITORY, NUM_INHIBITORY, PROBABILITY_CONNECTION)
ee_conn = generate_fixed_prob(NUM_EXCITATORY, NUM_EXCITATORY, PROBABILITY_CONNECTION)
ei_conn = generate_fixed_prob(NUM_EXCITATORY, NUM_INHIBITORY, PROBABILITY_CONNECTION)
theoretical_bytes = (get_theoretical_mem(ie_conn) + get_theoretical_mem(ii_conn)
                     + get_theoretical_mem(ee_conn) + get_theoretical_mem(ei_conn))

# Split between cores
padded_num_excitatory_per_core = pad_vector(NUM_EXCITATORY // NUM_CORES)
padded_num_inhibitory_per_core = pad_vector(NUM_INHIBITORY // NUM_CORES)
excitatory_split = np.cumsum([padded_num_excitatory_per_core] * (NUM_CORES - 1))
inhibitory_split = np.cumsum([padded_num_inhibitory_per_core] * (NUM_CORES - 1))
ie_conn = split(ie_conn, excitatory_split)
ii_conn = split(ii_conn, inhibitory_split)
ee_conn = split(ee_conn, excitatory_split)
ei_conn = split(ei_conn, inhibitory_split)

# Pad
ie_conn_pad = pad_connectivity(ie_conn)
ii_conn_pad = pad_connectivity(ii_conn)
ee_conn_pad = pad_connectivity(ee_conn)
ei_conn_pad = pad_connectivity(ei_conn)
print(f"IE :{ie_conn_pad[0].shape}")
print(f"II :{ii_conn_pad[0].shape}")
print(f"EE :{ee_conn_pad[0].shape}")
print(f"EI :{ei_conn_pad[0].shape}")


# Save binary files
for c in range(NUM_CORES):
    #total_bytes = ie_conn_pad[c].nbytes + ii_conn_pad[c].nbytes + ee_conn_pad[c].nbytes + ei_conn_pad[c].nbytes
    #print(f"Core {c} connectivity requires {total_bytes / 1024}KB memory vs {theoretical_bytes / 1024}KB theoretical vs {NUM_NEURONS * NUM_NEURONS * 2 / 1024}KB dense")

    ie_conn_pad[c].tofile(f"va_benchmark_ie_{c}.bin")
    ii_conn_pad[c].tofile(f"va_benchmark_ii_{c}.bin")
    ee_conn_pad[c].tofile(f"va_benchmark_ee_{c}.bin")
    ei_conn_pad[c].tofile(f"va_benchmark_ei_{c}.bin")