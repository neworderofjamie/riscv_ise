import itertools
import logging
import re

logger = logging.getLogger(__name__)


# Useful sub-regular expressions
_num = r"(-?(?:0x)?[0-9a-fA-F]+)"

# Labels
_match_label = re.compile(r"([a-zA-Z_][a-zA-Z0-9_]*):")

# Fill directive
_match_fill = re.compile(fr".fill\s+(.+)\s*,{_num}\s*,{_num}")

# Align directive
_match_align = re.compile(fr".align\s+{_num}")

# Word directive
_match_word = re.compile(fr".word\s+{_num}")

def parse_data(lines):
    mem_contents = [0]
    var_addresses = {}

    # Loop through lines
    in_model_data = False
    in_test_data = False
    address = 4
    for l in lines:
        # Strip initial whitespace
        l = l.lstrip()
    
        # Detect start and end of model data
        if l.startswith("RVMODEL_DATA_BEGIN"):
            assert not in_test_data and not in_model_data
            in_model_data = True
        elif l.startswith("RVMODEL_DATA_END"):
            in_model_data = False
        
        # Detect start and end of test data
        if l.startswith("RVTEST_DATA_BEGIN"):
            assert not in_test_data and not in_model_data
            in_test_data = True
        elif l.startswith("RVTEST_DATA_END"):
            in_test_data = False
            
        # If we're in the model data segment
        if in_model_data or in_test_data:
            # If line contains a label, store its address in dictionary
            if (match := _match_label.search(l)) is not None:
                var_addresses[match.group(1)] = address
            # If line contains a fill
            elif (match := _match_fill.search(l)) is not None:
                # Parse repeat, size and value 
                # **HACK** use python to evaluate expression
                repeat_code = match.group(1)
                repeat_code = repeat_code.replace("/", "//")
                repeat = int(eval(repeat_code, None, {"XLEN": 32}))
                size = int(match.group(2), 0)
                value = int(match.group(3), 0)
                assert size == 4

                # Add repeats of value to memory contents
                mem_contents.extend(itertools.repeat(value, repeat))

                # Update address
                address += (repeat * size)
            # If line contains an alignment directive
            elif (match := _match_align.search(l)) is not None:
                assert int(match.group(1)) == 4
            # If line contains a word directive
            elif (match := _match_word.search(l)) is not None:
                mem_contents.append(int(match.group(1), 0))
                address += 4
            # Otherwise, give warning if line appears to contain unsupported directive
            elif l.startswith("."):
                logger.warn(f"Unsupported directive '{l.lstrip().rstrip()}' in data")

    return mem_contents, var_addresses
