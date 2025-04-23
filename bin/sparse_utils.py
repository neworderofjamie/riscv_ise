from typing import Sequence
import numpy as np

def pad_connectivity(row_ind: Sequence[np.ndarray]) -> np.ndarray:
    num_pre = len(row_ind)

    # Determine which lane each postsynaptic index belongs in
    row_lane = [r % 32 for r in row_ind]

    # Sort rows of indices by their lane
    row_ind_sorted = [i[np.argsort(l)] for i, l in zip(row_ind, row_lane)]

    # Count how many connections each lane needs to process in each row
    row_conns_per_lane = [np.bincount(l) for l in row_lane]

    # Determine maximum number of vectors
    num_vectors = max(np.amax(c) for c in row_conns_per_lane)

    # Calculate cumulative sum of bin count to determine where to split per-bank
    row_conn_lane_ind = [np.cumsum(c) for c in row_conns_per_lane]

    padded_rows = []
    for i, l in zip(row_ind_sorted, row_conn_lane_ind):
        # Split, pad list of connections with  
        # **NOTE** we only care about which L.L.M. address of target in bytes
        conn_id_banked = np.transpose(np.vstack([np.pad((a // 32) * 2, (0, num_vectors - len(a)), 
                                                        constant_values=-2)
                                                for a in np.split(i, l[:-1])]))
        conn_id_banked = np.pad(conn_id_banked, ((0, 0), (0, 32 - conn_id_banked.shape[1])),
                                constant_values=-2)
        
        padded_rows.append(np.reshape(conn_id_banked, 32 * num_vectors))
    
    return np.vstack(padded_rows)

def generate_fixed_prob(num_pre: int, num_post: int,
                        prob: float) -> Sequence[np.ndarray]:
    # Loop through presynaptic neurons
    rows = []
    for i in range(num_pre):
        # Make num_post bernoulli trials
        row_mask = np.random.choice([True, False], num_post,
                                    p=[prob, 1.0 - prob])
        # Add indices of 1s to row
        rows.append(np.where(row_mask)[0])
    return rows