import itertools
import logging
import re

logger = logging.getLogger(__name__)


# Useful sub-regular expressions
_num = r"(-?(?:0x)?[0-9a-fA-F]+)"

# Labels
_match_label = re.compile(r"([a-zA-Z_][a-zA-Z0-9_]*):")

# Memory fill directive
_match_fill = re.compile(fr".fill\s*(.+)\s*,{_num}\s*,{_num}")

def parse_data(lines):
    mem_contents = []
    var_addresses = {}

    # Loop through lines
    in_model_data = False
    address = 0
    for l in lines:
        # Strip initial whitespace
        l = l.lstrip()
    
        # Detect start and end of data segment(s)
        if l.startswith("RVMODEL_DATA_BEGIN"):
            in_model_data = True
        elif l.startswith("RVMODEL_DATA_END"):
            in_model_data = False
        
        # If we're in the model data segment
        if in_model_data:
            # If line contains a label, store its address in dictionary
            if (match := _match_label.search(l)) is not None:
                var_addresses[match.group(1)] = address
            elif (match := _match_fill.search(l)) is not None:
                # Parse repeat, size and value 
                # **HACK** use python to evaluate expression
                repeat_code = match.group(1)
                repeat_code = repeat_code.replace("/", "//")
                repeat = int(eval(repeat_code, None, {"XLEN": 32}))
                size = int(match.group(2), 0)
                value = int(match.group(3), 0)

                # Add repeats of value to memory contents
                mem_contents.extend(itertools.repeat(value, repeat))

                # Update address
                address += (repeat * size)
            # Otherwise, give warning if line appears to contain unsupported directive
            elif l.startswith("."):
                logger.warn(f"Unsupported directive '{l.lstrip().rstrip()}' in data")

    return mem_contents, var_addresses
