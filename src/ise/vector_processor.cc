#include "ise/vector_processor.h"

// Standard C++ includes
#include <algorithm>
#include <iostream>
#include <iomanip>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>

namespace
{
template<typename F>
Vector binaryOp(const Vector &val, const Vector &val2, F func)
{
    Vector result;
    std::transform(val.cbegin(), val.cend(), val2.cbegin(), result.begin(), func);
    return result;
}

template<typename F>
uint32_t maskOp(const Vector &val, const Vector &val2, F func)
{
    uint32_t mask = 0;
    for(size_t i = 0; i < 32; i++) {
        if(func(val[i], val2[i])) {
            mask |= (1 << i);
        }
        
    }
    return mask;
}
}
//----------------------------------------------------------------------------
// VectorDataMemory
//----------------------------------------------------------------------------
VectorDataMemory::VectorDataMemory(const std::vector<int16_t> &data)
:   m_Data(data)
{
}
//----------------------------------------------------------------------------
Vector VectorDataMemory::readVector(uint32_t addr) const
{
    if (addr & 63) {
        throw Exception(Exception::Cause::MISALIGNED_LOAD, addr);
    }

    const size_t startHalfWord = addr / 2;
    if ((startHalfWord + 64) > m_Data.size())  {
        throw Exception(Exception::Cause::FAULT_LOAD, addr);
    } 

    Vector vector;
    for(size_t i = 0; i < 32; i++) {
        vector[i] = m_Data[startHalfWord + i];
    }
    return vector;
}
//----------------------------------------------------------------------------
void VectorDataMemory::writeVector(uint32_t addr, const Vector &vector)
{
    if (addr & 63) {
        throw Exception(Exception::Cause::MISALIGNED_STORE, addr);
    }
    
    const size_t startHalfWord = addr / 2;
    if ((startHalfWord + 64) > m_Data.size())  {
        throw Exception(Exception::Cause::FAULT_STORE, addr);
    }

    for(size_t i = 0; i < 32; i++) {
        m_Data[startHalfWord + i] = vector[i];
    }
}


//----------------------------------------------------------------------------
// VectorProcessor
//----------------------------------------------------------------------------
VectorProcessor::VectorProcessor(const std::vector<int16_t> &data)
:   m_VectorDataMemory(data)
{
}    
//------------------------------------------------------------------------
void VectorProcessor::executeInstruction(uint32_t inst, uint32_t (&reg)[32], 
                                         ScalarDataMemory &scalarDataMemory)
{
    const auto opcode = static_cast<VectorOpCode>((inst & 0b1111100) >> 2);
    switch(opcode) {
    case VectorOpCode::VLOAD:
    {
        auto [imm, rs1, funct3, rd] = decodeIType(inst);
        const uint32_t addr = reg[rs1] + imm;

        // VLOADV
        if(funct3 == 0x0) {
            PLOGD << "VLOADV " << rs1 << " " << imm;
            PLOGD << "\t" << rd;
            m_VReg[rd] = m_VectorDataMemory.readVector(addr);
        }
        // VLOADS
        else if(funct3 == 0x4) {
            PLOGD << "VLOADS " << rs1 << " " << imm;
            PLOGD << "\t" << rd;
    
            std::fill(m_VReg[rd].begin(), m_VReg[rd].end(), 
                      scalarDataMemory.read16(addr));
        }
        else {
            throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
        }
        
        break;
    }

    case VectorOpCode::VLUI:
    {
        auto [imm, rd] = decodeUType(inst);
        PLOGD << "VLUI " << imm;
        PLOGD << "\t" << rd;
        std::fill(m_VReg[rd].begin(), m_VReg[rd].end(), imm);
        break;
    }

    case VectorOpCode::VSOP:
    {
        auto [funct7, rs2, rs1, funct3, rd] = decodeRType(inst);
        m_VReg[rd] = calcOpResult(inst, funct7, rs2, rs1, funct3);
        PLOGD << "\t" << rd;
        break;
    }

    case VectorOpCode::VSTORE:
    {
        auto [imm, rs2, rs1, funct3] = decodeSType(inst);
        PLOGD << "VSTORE " << rs2 << " " << rs1 << " " << imm;
        m_VectorDataMemory.writeVector(reg[rs1] + imm, 
                                       m_VReg[rs2]);
        break;
    }
    
    case VectorOpCode::VSEL:
    {
        auto [funct7, rs2, rs1, funct3, rd] = decodeRType(inst);
        PLOGD << "VSEL " << rs1 << " " << rs2;
        PLOGD << "\t" << rd;
        const uint32_t mask = reg[rs1];
        auto &val = m_VReg[rd];
        const auto &val2 = m_VReg[rs2];        
        for(size_t i = 0; i < 32; i++) {
            val[i] = (mask & (1 << i)) ? val2[i] : val[i];
        }
        break;
    }
    
    case VectorOpCode::VTST:
    {
        auto [funct7, rs2, rs1, funct3, rd] = decodeRType(inst);
        const uint32_t val = calcTestResult(inst, rs2, rs1, funct3);
        PLOGD << "\t" << rd;
        if (rd != 0) {
            reg[rd] = val;
        }
        break;
    }
    }
}
//------------------------------------------------------------------------
void VectorProcessor::dumpRegisters() const
{
    // Dump standard register file
    std::cout << "Vector processor register file:" << std::endl;
    for(int i = 0; i < 32; i++) {
        std::cout << "\tv" << i << " = ";
        for(int j = 0; j < 32; j++) {
            std::cout << std::hex << std::setw(4) << std::setfill('0') << m_VReg[i][j] << ", ";
        }
        std::cout << std::endl;
    }
}
//------------------------------------------------------------------------
Vector VectorProcessor::calcOpResult(uint32_t inst, uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3) const
{
    const auto &val = m_VReg[rs1];
    const auto &val2 = m_VReg[rs2];

    switch(funct3)
    {
    // VADD
    case 0x0:
    {
        PLOGD << "VADD " << rs1 << " " << rs2;
        return binaryOp(val, val2, [](int16_t a, int16_t b){ return a + b; });
    }

    // VSUB
    case 0x2:
    {
        PLOGD << "VSUB " << rs1 << " " << rs2;
        return binaryOp(val, val2, [](int16_t a, int16_t b){ return a - b; });
    }
    // VMUL
    case 0x4:
    {
        PLOGD << "VMUL " << rs1 << " " << rs2;
        if ((funct7 & ~15) != 0) {
            throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
        }
        return binaryOp(val, val2, 
                        [funct7](int16_t a, int16_t b){ return ((int32_t)a * b) >> funct7; });
    }
    default:
    {
        throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
    }
    }
}
//------------------------------------------------------------------------
uint32_t VectorProcessor::calcTestResult(uint32_t inst, uint32_t rs2, uint32_t rs1, uint32_t funct3) const
{
    const auto &val = m_VReg[rs1];
    const auto &val2 = m_VReg[rs2];
    
    switch(funct3)
    {
    // VTEQ
    case 0x0:
    {
        PLOGD << "VTEQ " << rs1 << " " << rs2;
        return maskOp(val, val2, [](int16_t a, int16_t b){ return a == b; });
    }

    // VTNE
    case 0x2:
    {
        PLOGD << "VTNE " << rs1 << " " << rs2;
        return maskOp(val, val2, [](int16_t a, int16_t b){ return a != b; });
    }
    // VTLT
    case 0x4:
    {
        PLOGD << "VTLT " << rs1 << " " << rs2;
        return maskOp(val, val2, [](int16_t a, int16_t b){ return a < b; });
    }
    // VTGE
    case 0x6:
    {
        PLOGD << "VTGE " << rs1 << " " << rs2;
        return maskOp(val, val2, [](int16_t a, int16_t b){ return a >= b; });
    }
    default:
    {
        throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
    }
    }
}
