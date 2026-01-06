import seaborn as sns
import sys

presentation = "presentation" in sys.argv[1:]
poster = "poster" in sys.argv[1:]
paper = not presentation and not poster

# Set the plotting style
if presentation:
    sns.set(context="talk")
    sns.set_style("whitegrid", {"font.family":"sans-serif", "font.sans-serif":"Verdana"})
elif poster:
    sns.set(context="poster")
    sns.set_style("whitegrid", {"font.family":"sans-serif", "font.sans-serif":"Verdana"})
else:
    sns.set(context="paper", rc={"font.size":7, "axes.labelsize":7, "axes.titlesize": 8,
                                 "legend.fontsize":7, "xtick.labelsize":7, "ytick.labelsize":7})
    sns.set_style("whitegrid", {"font.family":"sans-serif", "font.serif":"OpenSans"})

# **HACK** fix bug with markers
sns.set_context(rc={"lines.markeredgewidth": 1.0})

# Set palette
sns.set_palette("deep")

mm_to_inches = 0.039370079
column_width = 85.0 * mm_to_inches
double_column_width = 150.0 * mm_to_inches
