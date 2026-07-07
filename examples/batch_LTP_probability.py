import sys, os, re, subprocess
from datetime import datetime

timestamp = datetime.now().strftime('%m-%d-%y-%H_%M_%S')
base_results =  sys.argv[1]
results = f"{base_results}{timestamp}/"

if not os.path.exists(results):
    os.makedirs(results)
    print(f"Created results directory {results}")

if not os.path.exists(f"{results}logs/"):
    os.makedirs(f"{results}logs/")
    print(f"Created results-logs directory {results}logs/")


if not os.path.exists(f"{results}output/"):
    os.makedirs(f"{results}output/")
    print(f"Created results-output directory {results}output/")


ssub_path = './examples/LTP_probability.ssub'

for id in range(5):
    stdout_name = f"{results}/logs/id{id}_%J.stdout"
    stderr_name = f"{results}/logs/id{id}_%J.stderr"

    results = f"{results}/output/results{id}.csv"
    jobname = f'LTP_prob_id{id}'
    os.system(f"sbatch -J {jobname} -o {stdout_name} -e {stderr_name} {ssub_path} {results}")

    print(f"SUBMITTED JOB [{jobname}]")

# python /its/home/cg610/Desktop/fenn/riscv_ise/batch_LTP_probability.py "/its/home/cg610/Desktop/fenn/results/"