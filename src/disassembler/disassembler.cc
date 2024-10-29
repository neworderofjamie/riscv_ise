// Standard C++ includes
#include <functional>
#include <iostream>
#include <sstream>
#include <unordered_map>

// Standard C includes
#include <cassert>

// Common include
#include "common/isa.h"

/*
    JALR    = 0b11001,
    JAL     = 0b11011,
    AUIPC   = 0b00101,*/
// Anonymous namespace
namespace
{
using disassembleFunc = std::function<void(std::ostream&, uint32_t)>;

void disassembleLoad(std::ostream &os, uint32_t inst)
{
    auto [imm, rs1, funct3, rd] = decodeIType(inst);
    auto type = getLoadType(funct3);
    os << type._to_string() << " X" << rd << ", " << imm << "(" << rs1 << ")";
}

void disassembleStore(std::ostream &os, uint32_t inst)
{
    auto [imm, rs2, rs1, funct3] = decodeSType(inst);
    auto type = getStoreType(funct3);
    os << type._to_string() << " X" << rs2 << ", " << imm << "(" << rs1 << ")";
}

void disassembleBranch(std::ostream &os, uint32_t inst)
{
    auto [imm, rs2, rs1, funct3] = decodeBType(inst);
    auto type = getBranchType(funct3);
    os << type._to_string() << " X" << rs1 << ", X" << rs2 << ", " << imm;
}

void disassembleOP(std::ostream &os, uint32_t inst)
{
    auto [funct7, rs2, rs1, funct3, rd] = decodeRType(inst);
    const auto type = getOpType(funct7, funct3);
    os << type._to_string() << " X" << rd << ", X" << rs1 << ", X" << rs2;
}

void disassembleOPImm(std::ostream &os, uint32_t inst)
{
    auto [imm, rs1, funct3, rd] = decodeIType(inst);
    const auto type = getOpImmType(imm, funct3);
    os << type._to_string() << " X" << rd << ", X" << rs1 << ", " << imm;
}

void disassembleLUI(std::ostream &os, uint32_t inst)
{
    auto [imm, rd] = decodeUType(inst);
    os << "LUI X" << rd << ", " << imm;
}

const std::unordered_map<StandardOpCode, disassembleFunc> standardInstructionDecoders{
    {StandardOpCode::LOAD, &disassembleLoad},
    {StandardOpCode::STORE, &disassembleStore},
    {StandardOpCode::BRANCH, &disassembleBranch},
    {StandardOpCode::OP, &disassembleOP},
    {StandardOpCode::OP_IMM, &disassembleOPImm},
    {StandardOpCode::LUI, &disassembleLUI},
};
}

void disassemble(std::ostream &os, uint32_t inst)
{
    // Extract 2-bit quadrant
    const uint32_t quadrant = inst & 0b11;
    const uint32_t opCode = (inst & 0b1111100) >> 2;

    // If instruction is in standard quadrant
    if(quadrant == standardQuadrant) {
        const auto i = standardInstructionDecoders.find(static_cast<StandardOpCode>(opCode));
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
        std::cout << std::endl;
        return 0;
    }
    catch(const std::exception &ex){
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}