import sys, os, re, subprocess
from datetime import datetime
from os import listdir
from os.path import isfile, join
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Get all files in results directory
# Call this file from inside results directory
result_dir = "./output/"
result_files = [f for f in listdir(result_dir) if isfile(join(result_dir, f))]


# Initialize data structure holding summarized data
summarized_data = dict()
total = 0
# Loop through each file
for file in result_files:
    print(f"Processing: {file}")
    results_df = pd.read_csv(file)
    # Convert post synaptic spike rate to integer
    results_df.post_spike_freq = results_df.post_spike_freq.astype(int)

    total += len(results_df.index)
    all_post_rates = results_df.post_spike_freq.unique()
    all_post_rates.sort()
    binned_presyn_freqs = results_df.binned_presyn_freqs.unique()
    binned_presyn_freqs.sort()
    for pre_rate in binned_presyn_freqs:
        rate_df = results_df[results_df["binned_presyn_freqs"] == pre_rate]
        post_spike_freqs = rate_df.post_spike_freq.unique()
        
        for post_rate in all_post_rates:
            if post_rate in post_spike_freqs:
                post_rate_df = rate_df[rate_df["post_spike_freq"] == post_rate]
                success_df = post_rate_df[post_rate_df["LTP_transition"] == 1]
                success_count = len(success_df.index)
                if pre_rate in summarized_data:
                    if post_rate in summarized_data[pre_rate][0]:
                        summarized_data[pre_rate][1][summarized_data[pre_rate][0].index(post_rate)] += success_count
                    else:
                        summarized_data[pre_rate][0].append(post_rate)
                        summarized_data[pre_rate][1].append(success_count)  
                else:
                    summarized_data[pre_rate] = [[],[]]
                    summarized_data[pre_rate][0].append(post_rate)
                    summarized_data[pre_rate][1].append(success_count)                

# Plot
fig, ax = plt.subplots()
for pre_rate in summarized_data.keys():
    ax.plot(summarized_data[pre_rate][0], summarized_data[pre_rate][1], 'o-', label=str(pre_rate)+" Hz", alpha=0.5)
leg = ax.legend(title=f"Presyn spike rate (N={total})")
ax.set_xlabel("Postsyn spike rate")
ax.set_ylabel("Probability of LTP transition")
plt.savefig('/its/home/cg610/Desktop/fenn/results/summarized_data_plog.png')

# Save results
np.save('./', summarized_data, allow_pickle=True)