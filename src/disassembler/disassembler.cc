// Standard C++ includes
#include <functional>
#include <iostream>
#include <sstream>
#include <unordered_map>

// Standard C includes
#include <cassert>

// Common include
#include "common/isa.h"

namespace
{
using disassembleFunc = std::function<void(std::ostream&, uint32_t)>;

void disassembleOP(std::ostream &os, uint32_t inst)
{
    // Decode
    auto [funct7, rs2, rs1, funct3, rd] = decodeRType(inst);

    // Get sub-operation
    const auto opType = getOpType(funct7, funct3);

    // Print
    os << opType._to_string() << " X" << rd << ", X" << rs1 << ", X" << rs2 << std::endl;
}

void disassembleOPImm(std::ostream &os, uint32_t inst)
{
    // Decode
    auto [imm, rs1, funct3, rd] = decodeIType(inst);

    // Get sub-operation
    const auto opType = getOpImmType(imm, funct3);

    // Print
    os << opType._to_string() << " X" << rd << ", X" << rs1 << " " << imm << std::endl;
}

const std::unordered_map<uint32_t, disassembleFunc> standardInstructionDecoders{
    {static_cast<uint32_t>(StandardOpCode::OP), &disassembleOP},
    {static_cast<uint32_t>(StandardOpCode::OP_IMM), &disassembleOPImm}
};
}

std::string disassemble(std::ostream &os, uint32_t inst)
{
    // Extract 2-bit quadrant
    const uint32_t quadrant = inst & 0b11;

    // If instruction is in standard quadrant
    if(quadrant == standardQuadrant) {
        const auto i = standardInstructionDecoders.find((inst & 0b1111100) >> 2);
        if(i != standardInstructionDecoders.cend()) {
            i->second(os, inst);
        }
        else {
            throw std::runtime_error("Unsupported standard instruction");
        }
    }
    // Otherwise, if it is from vector quadrant
    else if(quadrant == vectorQuadrant){
        
    }
    // Otherwise, throw
    else {
        throw std::runtime_error("Unsupported quadrant");
    }
}

int main(int argc, char *argv[])
{
    assert(argc == 2);

    try {
        disassemble(std::cout, std::stoul(argv[1], nullptr, 0));
        return 0;
    }
    catch(const std::exception &ex){
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}