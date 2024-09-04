import numpy as np
from os import path
import re

from argparse import ArgumentParser
from jinja2 import FileSystemLoader, Environment
from textwrap import dedent, indent

from rvtest_code_parser import parse_code
from rvtest_data_parser import parse_data
from utils import get_clean_name

parser = ArgumentParser()
parser.add_argument("test_filenames", type=str, nargs="+",
                    help="RISC-V architecture test files")
parser.add_argument("-t", "--template", type=str, required=True,
                    help="Jinja2 template to build test")
parser.add_argument("-o", "--output-dir", type=str,
                    help="Directory to write tests to (stdout if empty)")
args = parser.parse_args()

# Create Jinja environment where templates are loaded from the filesystem
template_loader = FileSystemLoader(searchpath="./templates")
template_env = Environment(loader=template_loader)

# Load template
template = template_env.get_template(args.template)

# Loop through architecture test files
for t in args.test_filenames:
    # Extract title of source test
    title = path.splitext(path.split(t)[1])[0]

    # Read test data
    with open(t, "r") as f:
        test_lines = f.readlines()

    # Parse data
    mem_contents, var_addresses = parse_data(test_lines)

    # Parse code
    test_code, correct_outputs = parse_code(test_lines, var_addresses)

    # Render test
    generated_code = template.render(test_code=test_code,
                                     mem_contents=mem_contents,
                                     correct_outputs=correct_outputs,
                                     clean_title=get_clean_name(title))
    # If output directory is specified
    if args.output_dir is not None:
        with open(path.join(args.output_dir, title + ".cc"), "w") as f:
            f.write(generated_code)
    else:
        print(generated_code)
