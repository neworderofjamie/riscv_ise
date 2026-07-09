import sys, os, re, subprocess
from datetime import datetime
from os import listdir
from os.path import isfile, join
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

process_data = False
make_clean_plot = True

if process_data:
    # Get all files in results directory
    # Call this file from inside results directory
    result_dir = './output/'
    result_files = [f for f in listdir(result_dir) if isfile(join(result_dir, f))]


    # Initialize data structure holding summarized data
    summarized_data = dict()
    total = 0
    # Loop through each file
    for file in result_files:
        print(f"Processing: {file}")
        results_df = pd.read_csv(f'./output/{file}')
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
    plt.savefig('./summarized_data_plot.png')

    # Save results
    np.save('./summarized_data.npy', summarized_data, allow_pickle=True)


if make_clean_plot:
    # Clean up plot
    summarized_data = np.load('./examples/STDP/summarized_data.npy',allow_pickle=True)
    # This is hacky, but note from the image that the total is 4795200
    total = 4795200
    clean_summarized_data = summarized_data.item()
    clean_summarized_data.pop('>110')
    clean_summarized_data.pop('90-110')
    clean_summarized_data.pop('70-90')
    fig, ax = plt.subplots()
    for pre_rate in clean_summarized_data.keys():
        clean_summarized_data[pre_rate][1] = [LTP_transitions/total for LTP_transitions in clean_summarized_data[pre_rate][1]]
        ax.plot(clean_summarized_data[pre_rate][0], clean_summarized_data[pre_rate][1], 'o-', label=str(pre_rate)+" Hz", alpha=0.5)
    leg = ax.legend(title=f"Pre-synaptic spike rate")
    ax.set_xlabel("Postsyn spike rate")
    ax.set_ylabel("Probability of LTP transition")
    plt.savefig('./examples/STDP/summarized_data_plot_clean.png')
