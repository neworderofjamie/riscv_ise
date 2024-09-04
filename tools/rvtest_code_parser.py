import logging
import re

from utils import get_clean_name

logger = logging.getLogger(__name__)


def _get_description(lines, i):
    # If there is a previous line and it starts with a comment
    if i > 0 and lines[i - 1].lstrip().startswith("//"):
        description_start_idx = lines[i - 1].find("//")
        return get_clean_name(lines[i - 1][description_start_idx + 2:])
    else:
        logger.warn(f"No comment with test description preceding test at line {i}")
        return "fTest{i}"

def _get_assembler_func(op_code):
    return (op_code + "_" if op_code in _underscore_op_codes
            else op_code)

# Useful sub-regular expressions
def _reg(name):
    return fr"x(?P<{name}>[0-9]+)"

def _num(name):
    return fr"(?P<{name}>-?(?:0x)?[0-9a-fA-F]+)"

_inst = r"(?P<inst>[a-z]+)"
_var = r"([a-zA-Z_][a-zA-Z0-9_]*)"

# Register-register operation
# TEST_RR_OP(inst, destreg, reg1, reg2, correctval, val1, val2, swreg, offset, testreg)
_match_test_rr_op = re.compile(fr"TEST_RR_OP\(\s*{_inst},\s*{_reg('destreg')},\s*{_reg('reg1')},\s*{_reg('reg2')},\s*{_num('correctval')},\s*{_num('val1')},\s*{_num('val2')},\s*{_reg('swreg')},\s*{_num('offset')},\s*{_reg('testreg')}\)")

# Register-immediate instruction
# TEST_IMM_OP( inst, destreg, reg, correctval, val, imm, swreg, offset, testreg)	
_match_test_imm_op = re.compile(fr"TEST_IMM_OP\(\s*{_inst},\s*{_reg('destreg')},\s*{_reg('reg')},\s*{_num('correctval')},\s*{_num('val')},\s*{_num('imm')},\s*{_reg('swreg')},\s*{_num('offset')},\s*{_reg('testreg')}\)")

# Base address setting operation
_match_sigbase = re.compile(fr"RVTEST_SIGBASE\(\s*{_reg('basereg')},\s*{_var}\)")

# Set of op-codes whose names are keywords in C++ and thus have underscores
_underscore_op_codes = set(["xor", "or", "and"])


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
                base_reg = match.group("basereg")
                
                # Add base address to dictionary
                base_addresses[base_reg] = base_address

                # Generate code to load base address
                test_code += f"c.li(Reg::X{base_reg}, {base_address});\n"
       
            # If line contains register-register operation
            elif (match := _match_test_rr_op.search(l)) is not None:
                # Generate code to load operands from immediates, perform operation and store result
                test_code += (
                    f"c.li(Reg::X{match.group('reg1')}, MASK_XLEN({match.group('val1')}));\n"
                    f"c.li(Reg::X{match.group('reg2')}, MASK_XLEN({match.group('val2')}));\n"
                    f"c.{_get_assembler_func(match.group('inst'))}(Reg::X{match.group('destreg')}, Reg::X{match.group('reg1')}, Reg::X{match.group('reg2')});\n"
                    f"c.sw(Reg::X{match.group('destreg')}, Reg::X{match.group('swreg')}, {match.group('offset')});\n\n")
                
                # Calculate destination address and add result to check
                result_address = base_addresses[match.group("swreg")] + int(match.group("offset"), 0)
                correct_outputs.append((result_address, int(match.group("correctval"), base=0),
                                        _get_description(lines, i)))
            # If line contains register-immediate operation
            elif (match := _match_test_imm_op.search(l)) is not None:
                # Generate code to load operands from immediates, perform operation and store result
                test_code += (
                    f"c.li(Reg::X{match.group('reg')}, MASK_XLEN({match.group('val')}));\n"
                    f"c.{_get_assembler_func(match.group('inst'))}(Reg::X{match.group('destreg')}, Reg::X{match.group('reg')}, SEXT_IMM({match.group('imm')}));\n"
                    f"c.sw(Reg::X{match.group('destreg')}, Reg::X{match.group('swreg')}, {match.group('offset')});\n\n")
                
                # Calculate destination address and add result to check
                result_address = base_addresses[match.group("swreg")] + int(match.group("offset"), 0)
                correct_outputs.append((result_address, int(match.group("correctval"), base=0),
                                        _get_description(lines, i)))
            # If line contains test case definition, skip
            elif l.startswith("RVTEST_CASE"):
                continue
            # Otherwise, give warning if line appears to contain unsupported macro
            elif l.startswith("TEST_") or l.startswith("RVTEST_"):
                logger.warn(f"Unsupported directive '{l.rstrip()}' in code")

    return test_code, correct_outputs
