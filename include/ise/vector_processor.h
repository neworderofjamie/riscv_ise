#pragma once

// Standard C++ includes
#include <array>
#include <vector>

// ISE includes
#include "ise/riscv.h"
#include "ise/ise_export.h"

using Vector = std::array<int16_t, 32>;

//----------------------------------------------------------------------------
// VectorDataMemory
//----------------------------------------------------------------------------
class ISE_EXPORT VectorDataMemory
{
public:
    VectorDataMemory(size_t numHalfWords);

    Vector readVector(uint32_t addr) const;
    void writeVector(uint32_t addr, const Vector &vector);

    void setData(const std::vector<int16_t> &data);

    const int16_t *getData() const{ return m_Data.data(); }
    int16_t *getData(){ return m_Data.data(); }

private:
    std::vector<int16_t> m_Data;
};

//----------------------------------------------------------------------------
// LaneLocalMemory
//----------------------------------------------------------------------------
class ISE_EXPORT LaneLocalMemory
{
public:
    explicit LaneLocalMemory(size_t numEntries);

    int16_t read(uint32_t addr) const;
    void write(uint32_t addr, int16_t data);

    const int16_t *getData() const{ return m_Data.data(); }
    int16_t *getData(){ return m_Data.data(); }

private:
    std::vector<int16_t> m_Data;
};

//----------------------------------------------------------------------------
// VectorProcessor
//----------------------------------------------------------------------------
class ISE_EXPORT VectorProcessor : public RISCV::ICoprocessor
{
public:
    VectorProcessor(size_t vectorMemoryHalfWords = 262144, size_t laneLocalMemoryHalfWords = 1024);

    //------------------------------------------------------------------------
    // ICoprocessor virtuals
    //------------------------------------------------------------------------
    virtual void executeInstruction(uint32_t inst, uint32_t (&reg)[32], 
                                    ScalarDataMemory &scalarDataMemory, uint32_t pc) override final;
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
