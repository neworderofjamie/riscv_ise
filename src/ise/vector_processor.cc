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
inline int16_t saturate(int val) 
{
    return std::clamp(val, (int)std::numeric_limits<int16_t>::min(),
                      (int)std::numeric_limits<int16_t>::max()); 
}

static inline uint16_t rol(uint16_t x, uint16_t k)
{
    return (x << k) | (x >> ((sizeof(x) * 8) - k));
}

template<typename F>
Vector binaryOp(const Vector &val, const Vector &val2, bool saturateResult, F func)
{
    Vector result;
    if(saturateResult) {
        std::transform(val.cbegin(), val.cend(), val2.cbegin(), result.begin(), 
                       [func](int16_t a, int16_t b){ return saturate(func(a, b)); });
    }
    else {
        std::transform(val.cbegin(), val.cend(), val2.cbegin(), result.begin(), 
                       func);
    }
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
    if ((startHalfWord + 32) > m_Data.size())  {
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
    if ((startHalfWord + 32) > m_Data.size())  {
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
                                         ScalarDataMemory&)
{
    const auto opcode = static_cast<VectorOpCode>((inst & 0b1111100) >> 2);
    switch(opcode) {
    case VectorOpCode::VLOAD:
    {
        auto [imm, rs1, funct3, rd] = decodeIType(inst);
        const uint32_t addr = reg[rs1] + imm;

        // VLOADV
        if(funct3 == 0b000) {
            PLOGV << "VLOADV " << rs1 << " " << imm;
            PLOGV << "\t" << rd;
            m_VReg[rd] = m_VectorDataMemory.readVector(addr); 
        }
        // VLOADR0
        else if(funct3 == 0b001) {
            PLOGV << "VLOADR0 " << rs1 << " " << imm;
            PLOGV << "\t" << rd;
            m_S0 = m_VectorDataMemory.readVector(addr); 
        }
        // VLOADR1/VLOADR1I
        else if(funct3 == 0b101) {
            PLOGV << "VLOADR1 " << rs1 << " " << imm;
            PLOGV << "\t" << rd;
            m_S1 = m_VectorDataMemory.readVector(addr); 
        }
        
        break;
    }

    case VectorOpCode::VLUI:
    {
        auto [imm, rd] = decodeUType(inst);
        PLOGV << "VLUI " << imm;
        PLOGV << "\t" << rd;
        std::fill(m_VReg[rd].begin(), m_VReg[rd].end(), imm);
        break;
    }

    case VectorOpCode::VFILL:
    {
        auto [imm, rs1, funct3, rd] = decodeIType(inst);

        PLOGV << "VFILL " << rs1;
        PLOGV << "\t" << rd;
        const uint32_t val = reg[rs1];
        std::fill(m_VReg[rd].begin(), m_VReg[rd].end(), 
                  (int16_t)((val & 0x80000000) >> 16 | (val & 0x7FFF)));
        break;
    }

    case VectorOpCode::VSPC:
    {
        auto [imm, rs1, funct3, rd] = decodeIType(inst);

        // VRNG
        if(funct3 == 0x0) {
            if (rs1 != 0) {
                throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
            }

            PLOGV << "VRNG";
            PLOGV << "\t" << rd;
            // **NOTE** shift down result down to get 0,int16_max range
            m_VReg[rd] = sampleRNG(1);
        }
        else {
            throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
        }
        break;
    }

    case VectorOpCode::VSOP:
    {
        auto [funct7, rs2, rs1, funct3, rd] = decodeRType(inst);
        m_VReg[rd] = calcOpResult(inst, funct7, rs2, rs1, funct3);
        PLOGV << "\t" << rd;
        break;
    }

    case VectorOpCode::VSTORE:
    {
        auto [imm, rs2, rs1, funct3] = decodeSType(inst);
        const uint32_t addr = reg[rs1] + imm;

        if(funct3 == 0x2) {
            PLOGV << "VSTOREI " << rs2 << " " << rs1 << " " << imm;
            reg[rs1] += 64;
        }
        else {
            PLOGV << "VSTORE " << rs2 << " " << rs1 << " " << imm;
        }

        m_VectorDataMemory.writeVector(addr, m_VReg[rs2]);
        break;
    }
    
    case VectorOpCode::VSEL:
    {
        auto [funct7, rs2, rs1, funct3, rd] = decodeRType(inst);
        PLOGV << "VSEL " << rs1 << " " << rs2;
        PLOGV << "\t" << rd;
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
        PLOGV << "\t" << rd;
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
Vector VectorProcessor::sampleRNG(int shift)
{
    constexpr uint16_t a = 13;
    constexpr uint16_t b = 5;
    constexpr uint16_t c = 10;
    constexpr uint16_t d = 9;

    Vector result;
    for(size_t i = 0; i < 32; i++) {
        uint16_t s0 = (uint16_t)m_S0[i];
        uint16_t s1 = (uint16_t)m_S1[i];

        result[i] = (rol(s0 + s1, d) + s0) >> shift;

        s1 ^= s0;
        s0 = rol(s0, a ) ^ s1 ^ (s1 << b);
        s1 = rol(s1, c);

        m_S0[i] = (int16_t)s0;
        m_S1[i] = (int16_t)s1;
    }

    return result;
}
//------------------------------------------------------------------------
Vector VectorProcessor::calcOpResult(uint32_t inst, uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3)
{
    const auto &val = m_VReg[rs1];
    const auto &val2 = m_VReg[rs2];

    // Split funct7 into mode and fixed point position
    const uint32_t mode = (funct7 >> 4);
    const uint32_t fixedPoint = (funct7 & 0b1111);
    const bool saturateResult = (mode & 0b100);
    const uint32_t roundMode = (mode & 0b011);
    switch(funct3)
    {
    // VADD
    case 0b000:
    {
        PLOGV << "VADD " << rs1 << " " << rs2;
        if(roundMode != 0) {
            throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
        }
        return binaryOp(val, val2, saturateResult,
                        [](int16_t a, int16_t b){ return a + b; });
    }

    // VSUB
    case 0b010:
    {
        PLOGV << "VSUB " << rs1 << " " << rs2;
        if(roundMode != 0) {
            throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
        }

        return binaryOp(val, val2, saturateResult,
                        [](int16_t a, int16_t b){ return a - b; });
    }
    // VMUL
    case 0b100:
    {
        PLOGV << "VMUL " << rs1 << " " << rs2;
  
        // Round-to-zero
        if(roundMode == 0b00) {
            return binaryOp(val, val2, saturateResult,
                            [fixedPoint](int16_t a, int16_t b)
                            {
                                return (a * b) >> fixedPoint; 
                            });
        }
        // Round-to-nearest (half up)
        else if(roundMode == 0b01) {
            const int16_t half = 1 << (fixedPoint - 1);
            return binaryOp(val, val2, saturateResult,
                            [half,fixedPoint](int16_t a, int16_t b)
                            {
                                return ((a * b) + half) >> fixedPoint; 
                            });
        }
        // Stochastic round
        else if (roundMode == 0b10) {
            // **NOTE** We want to generate random fractional bits and
            /// there are (fixedPoint - 1) fractional bit so we
            // need to shift off difference between 16 and (fixedPoint - 1).
            const auto stoch = sampleRNG(16 - (fixedPoint - 1));
            Vector result;
            if(saturateResult) {
                for(size_t i = 0; i < 32; i++) {
                    result[i] = ((val[i] * val2[i]) + stoch[i]) >> fixedPoint;
                }
            }
            else {
                for(size_t i = 0; i < 32; i++) {
                    result[i] = saturate(((val[i] * val2[i]) + stoch[i]) >> fixedPoint);

                }
            }
            return result;
        }
        else {
            throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
        }
        
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
    case 0b000:
    {
        PLOGV << "VTEQ " << rs1 << " " << rs2;
        return maskOp(val, val2, [](int16_t a, int16_t b){ return a == b; });
    }

    // VTNE
    case 0b010:
    {
        PLOGV << "VTNE " << rs1 << " " << rs2;
        return maskOp(val, val2, [](int16_t a, int16_t b){ return a != b; });
    }
    // VTLT
    case 0b100:
    {
        PLOGV << "VTLT " << rs1 << " " << rs2;
        return maskOp(val, val2, [](int16_t a, int16_t b){ return a < b; });
    }
    // VTGE
    case 0b110:
    {
        PLOGV << "VTGE " << rs1 << " " << rs2;
        return maskOp(val, val2, [](int16_t a, int16_t b){ return a >= b; });
    }
    default:
    {
        throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
    }
    }
}
