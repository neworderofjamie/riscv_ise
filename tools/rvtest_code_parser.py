import logging
import re

from utils import get_clean_name

logger = logging.getLogger(__name__)


def _get_description(lines, line_idx):
    # If there is a previous line and it starts with a comment
    if line_idx > 0 and lines[line_idx - 1].lstrip().startswith("//"):
        description_start_idx = lines[line_idx - 1].find("//")
        return get_clean_name(lines[line_idx - 1][description_start_idx + 2:])
    else:
        logger.warn(f"No comment with test description "
                    f"preceding test at line {line_idx}")
        return "fTest{line_idx}"

def _add_correct_output(base_addresses, correct_outputs, match, lines, line_idx,
                        correct_value=None):
    correct_value = (correct_value if correct_value is not None
                     else int(match.group("correctval"), 0))
    # Calculate destination address and add result to check
    result_address = base_addresses[match.group("swreg")] + int(match.group("offset"), 0)
    correct_outputs.append((result_address, correct_value,
                            _get_description(lines, line_idx)))

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
_label = r"(?P<label>[0-9][fb])"

# Register-register operation
# TEST_RR_OP(inst, destreg, reg1, reg2, correctval, val1, val2, swreg, offset, testreg)
_match_test_rr_op = re.compile(fr"TEST_RR_OP\(\s*{_inst},\s*{_reg('destreg')},\s*{_reg('reg1')},\s*{_reg('reg2')},\s*{_num('correctval')},\s*{_num('val1')},\s*{_num('val2')},\s*{_reg('swreg')},\s*{_num('offset')},\s*{_reg('testreg')}\)")

# Register-immediate instruction
# TEST_IMM_OP( inst, destreg, reg, correctval, val, imm, swreg, offset, testreg)
_match_test_imm_op = re.compile(fr"TEST_IMM_OP\(\s*{_inst},\s*{_reg('destreg')},\s*{_reg('reg')},\s*{_num('correctval')},\s*{_num('val')},\s*{_num('imm')},\s*{_reg('swreg')},\s*{_num('offset')},\s*{_reg('testreg')}\)")

# Load instruction
#define TEST_LOAD(swreg,testreg,index,rs1,destreg,imm_val,offset,inst,adj);\
_match_test_load_op = re.compile(fr"TEST_LOAD\(\s*{_reg('swreg')},\s*{_reg('testreg')},\s*{_num('index')},\s*{_reg('rs1')},\s*{_reg('destreg')},\s*{_num('imm_val')},\s*{_num('offset')},\s*{_inst},\s*{_num('adj')}\)")

# Branch instruction
# TEST_BRANCH_OP(inst, tempreg, reg1, reg2, val1, val2, imm, label, swreg, offset,adj)
_match_test_branch_op = re.compile(fr"TEST_BRANCH_OP\(\s*{_inst},\s*{_reg('tempreg')},\s*{_reg('reg1')},\s*{_reg('reg2')},\s*{_num('val1')},\s*{_num('val2')},\s*{_num('imm')},\s*{_label},\s*{_reg('swreg')},\s*{_num('offset')},\s*{_num('adj')}\)")

# Plain test case for simple instructions
# TEST_CASE(testreg, destreg, correctval, swreg, offset, code... )
_match_test_case = re.compile(fr"TEST_CASE\(\s*{_reg('testreg')},\s*{_reg('destreg')},\s*{_num('correctval')},\s*{_reg('swreg')},\s*{_num('offset')},\s*(?P<code>.*)\)")

# **YUCK** dedicated regular expression to match lui instructions used in TEST_CASE
_match_test_case_lui = re.compile(fr"\s*lui\s+{_reg('reg')},\s*{_num('imm')}")

# Base address setting operation
_match_sigbase = re.compile(fr"RVTEST_SIGBASE\(\s*{_reg('basereg')},\s*{_var}\)")

# Set of op-codes whose names are keywords in C++ and thus have underscores
_underscore_op_codes = set(["xor", "or", "and"])


def parse_code(lines, var_addresses):
    test_code = ""
    correct_outputs = []
    base_addresses = {}
    lab_count = [0] * 10

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
                
                # Add correct output to list
                _add_correct_output(base_addresses, correct_outputs, match, lines, i)                
            # If line contains register-immediate operation
            elif (match := _match_test_imm_op.search(l)) is not None:
                # Generate code to load operands from immediates, perform operation and store result
                test_code += (
                    f"c.li(Reg::X{match.group('reg')}, MASK_XLEN({match.group('val')}));\n"
                    f"c.{_get_assembler_func(match.group('inst'))}(Reg::X{match.group('destreg')}, Reg::X{match.group('reg')}, SEXT_IMM({match.group('imm')}));\n"
                    f"c.sw(Reg::X{match.group('destreg')}, Reg::X{match.group('swreg')}, {match.group('offset')});\n\n")
                
                # Add correct output to list
                _add_correct_output(base_addresses, correct_outputs, match, lines, i)
            # If line contains load instruction
            elif (match := _match_test_load_op.search(l)) is not None:
                # Generate code to generate address, load value from memory and store result
                test_code += (
                    f"c.li(Reg::X{match.group('rs1')}, {var_addresses['rvtest_data']}+({match.group('index')}*4)+({match.group('adj')})-({match.group('imm_val')}));\n"
                    f"c.{_get_assembler_func(match.group('inst'))}(Reg::X{match.group('destreg')}, Reg::X{match.group('rs1')}, {match.group('imm_val')});\n"
                    f"c.sw(Reg::X{match.group('destreg')}, Reg::X{match.group('swreg')}, {match.group('offset')});\n\n")
                
                # Add correct output to list
                # **TODO** halfword and byte
                _add_correct_output(base_addresses, correct_outputs, match, lines, i, correct_value=0xBABECAFE)
            # If line contains branch operation
            elif (match := _match_test_branch_op.search(l)) is not None:
                # Parse adj and imm
                adj = int(match.group("adj"), 0)
                imm = int(match.group("imm"), 0)
                label = match.group("label")
                label_num = int(label[:-1])
                label_dir = label[-1]
                assert adj == 0

                test_code += (
                    f"c.li(Reg::X{match.group('reg1')}, MASK_XLEN({match.group('val1')}));\n"
                    f"c.li(Reg::X{match.group('reg2')}, MASK_XLEN({match.group('val2')}));\n"
                    f"c.li(Reg::X{match.group('tempreg')}, 0);\n"
                    f"c.j_(label2_{lab_count[2]});\n")

                #-------------------------------------------------------------
                # 1
                #-------------------------------------------------------------
                test_code += f"c.L(label1_{lab_count[1]});\n"
                lab_count[1] += 1
                #if adj & 2 == 2:
                #    .fill 2,1,0x00
                
                test_code += (
                    f"c.addi(Reg::X{match.group('tempreg')}, Reg::X{match.group('tempreg')}, 0x1);\n"
                    f"c.j_(label4_{lab_count[4]});\n")

                #if adj & 2 == 2:
                #    .fill 2,1,0x00
                
                if ((imm // 2) - 2) >= 0:
                    num = (imm // 2) - 2
                else:
                    num = 0

                if label == "3f":
                    num = 0

                for _ in range(num):
                    test_code += "c.nop();\n"

                #-------------------------------------------------------------
                # 2
                #-------------------------------------------------------------
                test_code += f"c.L(label2_{lab_count[2]});\n"
                lab_count[2] += 1
    
                # If target label is declared AFTER instruction`
                target_number = label_num + adj
                
                # If label is forward
                if label_dir == "f":
                    label_name = f"label{target_number}_{lab_count[target_number]}"
                else:
                    label_name = f"label{target_number}_{lab_count[target_number] - 1}"

                test_code += (
                    f"c.{match.group('inst')}(Reg::X{match.group('reg1')}, Reg::X{match.group('reg2')}, {label_name});\n" 
                    f"c.addi(Reg::X{match.group('tempreg')}, Reg::X{match.group('tempreg')}, 0x2);\n"
                    f"c.j_(label4_{lab_count[4]});\n")

                if ((imm // 4) - 3) >= 0:
                    num = (imm // 4) - 3
                else:
                    num = 0

                if label == "1b":
                    num = 0
                
                for _ in range(num):
                    test_code += "c.nop();\n"

                #-------------------------------------------------------------
                # 3
                #-------------------------------------------------------------
                test_code += f"c.L(label3_{lab_count[3]});\n"
                lab_count[3] += 1
                #if adj & 2 == 2:
                #    .fill 2,1,0x00

                test_code += (
                    f"c.addi(Reg::X{match.group('tempreg')}, Reg::X{match.group('tempreg')}, 0x3);\n"
                    f"c.j_(label4_{lab_count[4]});\n")

                #if adj & 2 == 2:
                #   .fill 2,1,0x00

                #-------------------------------------------------------------
                # 4
                #-------------------------------------------------------------
                test_code += f"c.L(label4_{lab_count[4]});\n"
                test_code += f"c.sw(Reg::X{match.group('tempreg')}, Reg::X{match.group('swreg')}, {match.group('offset')});\n\n"
                lab_count[4] += 1
                
                # Add correct output to list
                _add_correct_output(base_addresses, correct_outputs, match,
                                    lines, i, correct_value=label_num + adj)
        
            # If line contains plain test case
            elif (match := _match_test_case.search(l)) is not None:
                # If this test case is a LUI
                if (lui_match := _match_test_case_lui.search(match.group("code"))) is not None:
                    # Check we are loading register we're checking
                    assert lui_match.group('reg') == match.group('destreg')
                    
                    # Generate code to load register and store result
                    test_code += (
                        f"c.lui(Reg::X{match.group('destreg')}, {lui_match.group('imm')});\n"
                        f"c.sw(Reg::X{match.group('destreg')}, Reg::X{match.group('swreg')}, {match.group('offset')});\n\n")
                    
                    # Add correct output to list
                    _add_correct_output(base_addresses, correct_outputs, match, lines, i)
                else:
                    raise NotImplementedError(f"Test case '{l}' not supported")
            # If line contains test case definition, skip
            elif l.startswith("RVTEST_CASE"):
                continue
            # Otherwise, give warning if line appears to contain unsupported macro
            elif l.startswith("TEST_") or l.startswith("RVTEST_"):
                logger.warn(f"Unsupported directive '{l.rstrip()}' in code")
    
    # Generate labels
    labels = ""
    for i, c in enumerate(lab_count):
        for j in range(c):
            labels += f"Label label{i}_{j};\n"
    
    return labels + test_code, correct_outputs
