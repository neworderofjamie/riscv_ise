import numpy as np


# Generate row of random connectivity
np.random.seed(1234)
conn_mask = np.random.choice([True, False], 100, p=[0.1,0.9])
conn_id = np.where(conn_mask)[0]

# Determine which lane each connection belongs in
conn_id_lane = conn_id % 32

print(f"Conn ID and lane: {list(zip(conn_id, conn_id_lane))}")

# Sort connections by lane
conn_id_order = np.argsort(conn_id_lane)
conn_id = conn_id[conn_id_order]

# Determine how many connections will be processed by each lane
# Maximum of this is the number of vectors required
num_conn_per_lane = np.bincount(conn_id_lane)
num_vectors = np.amax(num_conn_per_lane)

# Calculate cumulative sum of bin count to determine where to split per-bank
conn_bank_ind = np.cumsum(num_conn_per_lane)

# Split, pad list of connections with  
conn_id_banked = np.transpose(np.vstack([np.pad(a, (0, num_vectors - len(a)), 
                                                constant_values=-1)
                                         for a in np.split(conn_id, conn_bank_ind)]))
conn_id_banked = np.pad(conn_id_banked, ((0, 0), (0, 32 - conn_id_banked.shape[1])),
                        constant_values=-1)

print(f"{num_vectors} vectors")
print(f"Conn ID banked: {conn_id_banked}")