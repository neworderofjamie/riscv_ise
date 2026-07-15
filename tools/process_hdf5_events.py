import os
os.environ["HDF5_PLUGIN_PATH"]="/usr/lib/x86_64-linux-gnu/hdf5/plugins"

import h5py
import numpy as np

N = 32768

with h5py.File("courtyard_walk_stereo.master.hdf5", "r") as f:
    events = f["CD"]["events"]
    time_ms = events["t"] // 1000

    lose_early_mask = (time_ms > 1)
    lose_early_events = events[lose_early_mask]
    lose_early_time_ms = time_ms[lose_early_mask]

    bin_count = np.bincount(lose_early_time_ms[:N].astype(int))
    cum_bin_count = np.cumsum(bin_count)
    events_sliced = [lose_early_events[start:end]
                     for start, end in zip(cum_bin_count[:-1], cum_bin_count[1:])]
    data = np.empty(N,dtype=np.uint32)

    i = 0
    for t, e in enumerate(events_sliced):
        if len(e) > 0:
            if (i + 1 + len(e)) < len(data):
                data[i] = t | (1 << 31)
                i += 1
                data[i:i + len(e)] = (e["p"].astype(np.uint32) << 18) | (e["x"].astype(np.uint32) << 9) | e["y"].astype(np.uint32)
                print(e["y"])
                i += len(e)
            else:
                print(f"Truncating at t={t}")
                data = data[:i]
                break

    print(data)
    data.tofile("courtyard.bin")
