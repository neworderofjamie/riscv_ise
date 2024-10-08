import math
import matplotlib.pyplot as plt
import numpy as np

from argparse import ArgumentParser
from os import path

parser = ArgumentParser()
parser.add_argument("input_filenames", type=str, nargs="+",
                    help="Numpy format binary files to quantise")
parser.add_argument("--num-pre", type=int, default=None,
                    help="When padding post, need to provide this")
parser.add_argument("--pad-post", action="store_true",
                    help="In current FeNN implementation, postsynaptic population size needs to be P.O.T.")
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
    
    if args.pad_post:
        assert args.num_pre is not None
        
        data = np.reshape(data, (args.num_pre, -1))
        pot_num_post = 2 ** (math.ceil(math.log(data.shape[1], 2)))
        print(f"Padding number of postsynaptic neurons from {data.shape[1]} to {pot_num_post}")
        data = np.pad(data, ((0, 0), (0, pot_num_post - data.shape[1])))    

    # Scale, round and convert to int16
    fp_one = 2.0 ** args.fractional_bits
    data_fp = np.round(data * fp_one).astype(np.int16)

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
    
