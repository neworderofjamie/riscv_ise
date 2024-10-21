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

    auto &getData(){ return m_Data; }
    const auto &getData() const{ return m_Data; }

private:
    std::vector<int16_t> m_Data;
};

//----------------------------------------------------------------------------
// LaneLocalMemory
//----------------------------------------------------------------------------
class LaneLocalMemory
{
public:
    explicit LaneLocalMemory(size_t numEntries);

    int16_t read(uint32_t addr) const;
    void write(uint32_t addr, int16_t data);

    auto &getData(){ return m_Data; }
    const auto &getData() const{ return m_Data; }

private:
    std::vector<int16_t> m_Data;
};

//----------------------------------------------------------------------------
// VectorProcessor
//----------------------------------------------------------------------------
class VectorProcessor : public RISCV::ICoprocessor
{
public:
    VectorProcessor(const std::vector<int16_t> &data, size_t laneLocalMemorySize = 2304);

    //------------------------------------------------------------------------
    // ICoprocessor virtuals
    //------------------------------------------------------------------------
    virtual void executeInstruction(uint32_t inst, uint32_t (&reg)[32], 
                                    ScalarDataMemory &scalarDataMemory) override final;
    virtual void dumpRegisters() const override final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Access vector data memory
    auto &getVectorDataMemory(){ return m_VectorDataMemory; }
    const auto &getVectorDataMemory() const{ return m_VectorDataMemory; }
    const auto &getLaneLocalMemory(size_t lane) const{ return m_LaneLocalMemories.at(lane); }
    
    size_t getNumInstructionsExecuted(const std::array<size_t, 32> &counts, VectorOpCode opCode) const;
    size_t getNumMemory(const std::array<size_t, 32> &counts) const;
    size_t getNumALU(const std::array<size_t, 32> &counts) const;

private:
    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    std::array<uint16_t, 32> sampleRNG();

    Vector calcOpResult(uint32_t inst, uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3);
    uint32_t calcTestResult(uint32_t inst, uint32_t rs2, uint32_t rs1, uint32_t funct3) const;

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    VectorDataMemory m_VectorDataMemory;
    std::vector<LaneLocalMemory> m_LaneLocalMemories;
    Vector m_VReg[32];

    // Xoroshiro32++ state
    Vector m_S0;
    Vector m_S1;
};
