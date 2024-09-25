import math
import matplotlib.pyplot as plt
import numpy as np

from argparse import ArgumentParser
from os import path

parser = ArgumentParser()
parser.add_argument("input_filenames", type=str, nargs="+",
                    help="Numpy format binary files to quantise")
parser.add_argument("-p", "--percentile", type=float, default=99.0,
                    help="What percentile of weights to attempt to quantise")
parser.add_argument("-f", "--fractional-bits", type=int,
                    help="Number of fractional bits")
parser.add_argument("-v", "--visualise", action="store_true", help="Visualise weight distributions")
args = parser.parse_args()

# Loop through files to quantise
for f in args.input_filenames:
    # Extract title of source test
    title = path.splitext(path.split(f)[1])[0]
    print(title)

    # Load
    data = np.load(f)

    # Split data into positive and negative
    positive_mask = (data > 0)
    positive_data = data[positive_mask]
    negative_data = data[np.logical_not(positive_mask)]

    # Calculate desired percentile
    positive_perc = np.percentile(positive_data, args.percentile)
    negative_perc = np.percentile(-negative_data, args.percentile)

    # Calculate the largest of these
    max_val = max(positive_perc, negative_perc)

    # Calculate high bit
    # **NOTE** we floor so max is 2**(high_bit + 1) - 1
    # **NOTE** one bit is used for sign
    high_bit =  math.floor(math.log(max_val, 2))
    
    # Calculate min and max
    min_quant = (-2.0 ** (high_bit + 1))
    max_quant = (2.0 ** (high_bit + 1)) - (2.0 ** -args.fractional_bits)
    
    # Clip data
    data_clip = np.clip(data, min_quant, max_quant)

    # Scale, round and convert to int16
    fp_one = 2.0 ** args.fractional_bits
    data_fp = np.round(data_clip * fp_one).astype(np.int16)

    # Write to file
    data_fp.tofile(title + ".bin")

    if args.visualise:
        fig, axes = plt.subplots(2, sharex=True)
        fig.suptitle(title)

        axes[0].set_title("Floating point")
        bins = axes[0].hist(data, bins=50)[1]

        axes[0].set_title("Fixed point")
        axes[1].hist(data_fp / fp_one, bins=bins)

if args.visualise:
    plt.show()
    