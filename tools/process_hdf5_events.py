import os
os.environ["HDF5_PLUGIN_PATH"]="/usr/lib/x86_64-linux-gnu/hdf5/plugins"

import h5py
import numpy as np

N = 32768

with h5py.File("courtyard_walk_stereo.master.hdf5", "r") as f:
    # Convert microsecond timestamps to milliseconds
    events = f["CD"]["events"]
    time_ms = events["t"] // 1000

    # First couple of bins contain crap so throw away
    lose_early_mask = (time_ms > 1)
    lose_early_events = events[lose_early_mask]
    lose_early_time_ms = time_ms[lose_early_mask]

    # Get cumulative sum of spikes per-timestamp in first N events
    bin_count = np.bincount(lose_early_time_ms[:N].astype(int))
    cum_bin_count = np.cumsum(bin_count)
    
    # Slice events into timesteps
    events_sliced = [lose_early_events[start:end]
                     for start, end in zip(cum_bin_count[:-1], cum_bin_count[1:])]

    # Build flat list to hold FeNN format events
    data = np.empty(N,dtype=np.uint32)

    # Loop through slices of events
    i = 0
    for t, e in enumerate(events_sliced):
        # If there's anything in this slice
        if len(e) > 0:
            # If data has space for it
            if (i + 1 + len(e)) < len(data):
                # Write timestamp to data
                data[i] = t | (1 << 31)
                i += 1
                
                # Write FeNN-format event to data
                data[i:i + len(e)] = (e["p"].astype(np.uint32) << 18) | (e["x"].astype(np.uint32) << 9) | e["y"].astype(np.uint32)
                i += len(e)
            # Otherwise, truncate data
            else:
                print(f"Truncating at t={t}")
                data = data[:i]
                break

    data.tofile("courtyard.bin")
