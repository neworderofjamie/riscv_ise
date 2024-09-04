import logging
import re

from utils import get_clean_name

logger = logging.getLogger(__name__)

# Useful sub-regular expressions
_inst = r"([a-z]+)"
_reg = r"x([0-9]+)"
_num = r"(-?(?:0x)?[0-9a-fA-F]+)"
_var = r"([a-zA-Z_][a-zA-Z0-9_]*)"

# Register-register operation
# TEST_RR_OP(inst, destreg, reg1, reg2, correctval, val1, val2, swreg, offset, testreg)
_match_test_rr_op = re.compile(fr"TEST_RR_OP\({_inst},\s*{_reg},\s*{_reg},\s*{_reg},\s*{_num},\s*{_num},\s*{_num},\s*{_reg},\s*{_num},\s*{_reg}\)")

# Base address setting operation
_match_sigbase = re.compile(fr"RVTEST_SIGBASE\(\s*{_reg},\s*{_var}\)")


def parse_code(lines, var_addresses):
    test_code = ""
    correct_outputs = []
    base_addresses = {}

    # Loop through lines
    in_code = False
    for i, l in enumerate(lines):
        # Strip initial whitespace
        l = l.lstrip()

        # Detect start and end of code segment
        if l.startswith("RVTEST_CODE_BEGIN"):
            in_code = True
        elif l.startswith("RVTEST_CODE_END"):
            in_code = False
        
        # If we're in the code segment
        elif in_code:
            # Pass through comments
            if l.startswith("//"):
                test_code += l
            # If line contains base address setting operation
            elif (match := _match_sigbase.search(l)) is not None:
                base_address = var_addresses[match.group(2)]
                base_reg = match.group(1)
                
                # Add base address to dictionary
                base_addresses[base_reg] = base_address

                # Generate code to load base address
                test_code += f"c.li(Reg::X{base_reg}, {base_address});\n"
       
            # If line contains register-register operation
            elif (match := _match_test_rr_op.search(l)) is not None:
                # Generate code to load immediate values, perform operation and store result
                test_code += (
                    f"c.li(Reg::X{match.group(3)}, MASK_XLEN({match.group(6)}));\n"
                    f"c.li(Reg::X{match.group(4)}, MASK_XLEN({match.group(7)}));\n"
                    f"c.{match.group(1)}(Reg::X{match.group(2)}, Reg::X{match.group(3)}, Reg::X{match.group(4)});\n"
                    f"c.sw(Reg::X{match.group(2)}, Reg::X{match.group(8)}, {match.group(9)});\n\n")
                
                # If there is a previous line and it starts with a comment
                if i > 0 and lines[i - 1].lstrip().startswith("//"):
                    description_start_idx = lines[i - 1].find("//")
                    description = get_clean_name(lines[i - 1][description_start_idx + 2:])
                else:
                    logger.warn(f"No comment with test description preceding test at line {i}")
                    description = "fTest{i}"
    
                # Calculate destination address and add result to check
                result_address = base_addresses[match.group(8)] + int(match.group(9), 0)
                correct_outputs.append((result_address, int(match.group(5), base=0), description))
            # If line contains test case definition, skip
            elif l.startswith("RVTEST_CASE"):
                continue
            # Otherwise, give warning if line appears to contain unsupported macro
            elif l.startswith("TEST_") or l.startswith("RVTEST_"):
                logger.warn(f"Unsupported directive '{l.rstrip()}' in code")

    return test_code, correct_outputs
