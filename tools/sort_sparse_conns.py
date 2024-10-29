import numpy as np


# Generate some random connectivity
conn_mask = np.random.choice([True, False], 392, p=[0.1,0.9])
conn_id = np.where(conn_mask)[0]

# Determine which lane each connection belongs in
conn_id_lane = conn_id % 32

# Sort connections by bank
conn_id_order = np.argsort(conn_id_lane)
conn_id = conn_id[conn_id_order]

# Determine how many vectors we need 
num_conn_per_bank = np.bincount(conn_id_lane)
num_vectors = np.amax(num_conn_per_bank)

# Calculte cumulative sum of bin count to determine where to split per-bank
conn_bank_ind = np.cumsum(num_conn_per_bank)

# Split, pad list of connections with  
conn_id_banked = np.transpose(np.vstack([np.pad(a, (0, num_vectors - len(a)), constant_values=-1)
                                         for a in np.split(conn_id, conn_bank_ind)]))
print(conn_id_banked)
print(conn_id_banked % 32)
