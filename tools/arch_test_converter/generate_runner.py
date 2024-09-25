from os import path

from argparse import ArgumentParser
from jinja2 import FileSystemLoader, Environment

from utils import get_clean_name

parser = ArgumentParser()
parser.add_argument("test_filenames", type=str, nargs="+",
                    help="RISC-V architecture test files")
parser.add_argument("-t", "--template", type=str, required=True,
                    help="Jinja2 template to build test")
parser.add_argument("-o", "--output-file", type=str,
                    help="Name of to write runner to (stdout if empty)")
args = parser.parse_args()

# Create Jinja environment where templates are loaded from the filesystem
template_loader = FileSystemLoader(searchpath="./templates")
template_env = Environment(loader=template_loader)

# Build list of titles
titles = [path.splitext(path.split(t)[1])[0]
          for t in args.test_filenames]

# Add clean versions
titles = [(t, get_clean_name(t)) for t in titles]

# Load template
template = template_env.get_template(args.template)

# Render runner
generated_code = template.render(titles=titles)

# If output file is specified
if args.output_file is not None:
    with open(args.output_file, "w") as f:
        f.write(generated_code)
else:
    print(generated_code)
