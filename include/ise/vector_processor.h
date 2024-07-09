#pragma once

// Standard C++ includes
#include <array>
#include <vector>

// RISC-V includes
#include "ise/riscv.h"

using Vector = std::array<int16_t, 32>;

//----------------------------------------------------------------------------
// VectorDataMemory
//----------------------------------------------------------------------------
class VectorDataMemory
{
public:
    VectorDataMemory(const std::vector<int16_t> &data);

    Vector readVector(uint32_t addr) const;
    void writeVector(uint32_t addr, const Vector &vector);

private:
    std::vector<int16_t> m_Data;
};


//----------------------------------------------------------------------------
// VectorProcessor
//----------------------------------------------------------------------------
class VectorProcessor : public RISCV::ICoprocessor
{
public:
    VectorProcessor(const std::vector<int16_t> &data);

    //------------------------------------------------------------------------
    // ICoprocessor virtuals
    //------------------------------------------------------------------------
    virtual void executeInstruction(uint32_t inst, uint32_t (&reg)[32], 
                                    ScalarDataMemory &scalarDataMemory) override final;

private:
    Vector calcOpResult(uint32_t inst, uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3) const;
    uint32_t calcTestResult(uint32_t inst, uint32_t rs2, uint32_t rs1, uint32_t funct3) const;

    VectorDataMemory m_VectorDataMemory;
    Vector m_VReg[32];
};
