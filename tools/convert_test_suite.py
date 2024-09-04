import numpy as np
import re

from argparse import ArgumentParser
from jinja2 import Template
from textwrap import dedent, indent

from rvtest_code_parser import parse_code
from rvtest_data_parser import parse_data

simulator_template = Template(
"""
// Standard C includes
#include <cassert>
#include <cstring>

// RISC-V assembler includes
#include "assembler/xbyak_riscv.hpp"

// RISC-V ISE includes
#include "ise/riscv.h"

#define XLEN 32
#define MASK (((1<<(XLEN-1))-1) + (1<<(XLEN-1))) // XLEN bits of 1s
#define MASK_XLEN(val)  val&MASK // shortens 64b values to XLEN when XLEN==32

using namespace Xbyak_riscv;

CodeGenerator generateCode()
{
    CodeGenerator c;
{{test_code}}
    
    // End
    c.ecall();
    return c;
}

int main()
{
    // Generate word-aligned vector to initialise memory
    const std::vector<uint32_t> scalarWordInitData{
    {% for m in mem_contents %}{{m}},{% endfor %}};
    
    std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
    std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                scalarWordInitData.size() * 4);

    // Create RISC-V core with instruction and scalar data
    RISCV riscV(generateCode().getCode(), scalarInitData);
     
    if(!riscV.run()) {
        return 1;
    }

    // Get word-aligned pointer to scalar memory
    uint32_t *scalarWordData = reinterpret_cast<uint32_t*>(riscV.getScalarDataMemory().getData().data());
    
    // Test results
    {% for c in correct_outputs %}
    if(scalarWordData[{{c[0] // 4}}] != {{c[1]}}) {
        std::cerr << "Test failure" << std::endl;
    }
    {% endfor %}
    return 0;
}
""")

parser = ArgumentParser()
parser.add_argument("test_filenames", metavar='N', type=str, nargs="+",
                    help="RISC-V architecture test files")
args = parser.parse_args()


# Loop through architecture test files
for t in args.test_filenames:
    # Read test data
    with open(t, "r") as f:
        test_lines = f.readlines()

    # Parse data
    mem_contents, var_addresses = parse_data(test_lines)

    # Parse code
    test_code, correct_outputs = parse_code(test_lines, var_addresses)
    
    print(simulator_template.render(test_code=indent(test_code, "    "),
                                    mem_contents=mem_contents,
                                    correct_outputs=correct_outputs))

    #print(simulator_template.safe_substitute(test_code=indent(test_code, "    ")))
