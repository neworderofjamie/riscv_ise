import numpy as np

from tonic.datasets import DVSGesture
from tonic.transforms import Compose, Denoise, Downsample

from tqdm.auto import tqdm

dataset = DVSGesture(save_to="./data", train=False,
                     transform=Compose([Denoise(filter_time=10000),
                                        Downsample(spatial_factor=0.25)]))

events_per_timestep = []
for events, label in tqdm(dataset, "Preprocessing dataset"):
    events_per_timestep.append( np.bincount(np.rint(events["t"] / 1000).astype(int)))

# Loop over potential chunk sizes
for b in range(100, 4000, 100):
    all_good = True
    # Loop through per-timestep event counts
    for e in events_per_timestep:
        # Pad number of timesteps to multiple of chunk size
        padded_length = int(np.ceil(len(e) / b) * b)
        padded_e = np.pad(e, (0, padded_length - len(e)))
        
        # Reshape so each row contains the number of events per-chunk and sum
        padded_e = np.reshape(padded_e, (-1, b))
        chunk_event_count = np.sum(padded_e, axis=1)

        # If any chunks won't fit in memory, stop
        if np.any(chunk_event_count > (60000 - b)):
            all_good = False
            break
    if all_good:
        print(f"Chunking to {b} fine")
    else:
        print(f"Chunking to {b} overflows")
        break