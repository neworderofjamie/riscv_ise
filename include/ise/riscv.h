#pragma once

// Standard C++ includes
#include <array>
#include <memory>
#include <optional>
#include <vector>

// Standard C includes
#include <cassert>
#include <cstdint>

// RISC-V includes
#include "common/isa.h"

//----------------------------------------------------------------------------
// InstructionMemory
//----------------------------------------------------------------------------
class InstructionMemory
{
public:
    InstructionMemory(size_t numWords);

    uint32_t getInstruction(uint32_t addr) const;

    void setInstructions(const std::vector<uint32_t> &instructions);

    size_t getNumInstructions() const{ return m_Instructions.size(); }

private:
    std::vector<uint32_t> m_Instructions;
};

//----------------------------------------------------------------------------
// ScalarDataMemory
//----------------------------------------------------------------------------
class ScalarDataMemory
{
public:
    ScalarDataMemory(size_t numWords);

    uint32_t read32(uint32_t addr) const;
    void write32(uint32_t addr, uint32_t value);

    uint16_t read16(uint32_t addr) const;
    void write16(uint32_t addr, uint16_t value);

    uint8_t read8(uint32_t addr) const;
    void write8(uint32_t addr, uint8_t value);
    
    void setData(const std::vector<uint8_t> &data);

    const uint8_t *getData() const{ return m_Data.data(); }
    uint8_t *getData(){ return m_Data.data(); }
    
private:
    std::vector<uint8_t> m_Data;
};

//----------------------------------------------------------------------------
// Exception
//----------------------------------------------------------------------------
class Exception : std::exception
{
public:
    enum class Cause : uint32_t
    {
        MISALIGNED_FETCH,
        FAULT_FETCH,
        ILLEGAL_INSTRUCTION,
        MISALIGNED_LOAD,
        FAULT_LOAD,
        MISALIGNED_STORE,
        FAULT_STORE,
        ECALL
    };

    Exception(Cause cause, uint32_t context) : m_Cause(cause), m_Context(context)
    {}

    Cause getCause() const{ return m_Cause; }
    uint32_t getContext() const{ return m_Context; }

private:
    Cause m_Cause;
    uint32_t m_Context;
};

//----------------------------------------------------------------------------
// RISCV
//----------------------------------------------------------------------------
class RISCV
{
public:
    //------------------------------------------------------------------------
    // ICoprocessor
    //------------------------------------------------------------------------
    class ICoprocessor
    {
    public:
        virtual void executeInstruction(uint32_t inst, uint32_t (&reg)[32], ScalarDataMemory &scalarDataMemory) = 0;
        virtual void dumpRegisters() const = 0;
    };

    RISCV(size_t numInstructionWords = 1024, size_t numDataWords = 32768);

    bool run();
    void dumpRegisters() const;

    template<typename T, typename... CoprocessorArgs>
    void addCoprocessor(uint32_t quadrant, CoprocessorArgs&&... coprocessorArg)
    {
        m_Coprocessors[quadrant].reset(new T(std::forward<CoprocessorArgs>(coprocessorArg)...));
    }
    
    template<typename T>
    T *getCoprocessor(uint32_t quadrant)
    {
        return dynamic_cast<T*>(m_Coprocessors[quadrant].get());
    }
    
    template<typename T>
    const T *getCoprocessor(uint32_t quadrant) const
    {
        return dynamic_cast<const T*>(m_Coprocessors[quadrant].get());
    }
    
    auto &getScalarDataMemory(){ return m_ScalarDataMemory; }
    const auto &getScalarDataMemory() const{ return m_ScalarDataMemory; }

    void setPC(uint32_t pc){ m_PC = pc; }
    uint32_t getPC() const{ return m_PC; }

    void setInstructions(const std::vector<uint32_t> &instructions);

    void resetStats();
    const auto &getNumInstructionExecuted() const{ return m_NumInstructionsExecuted; }
    std::array<size_t, 32> getNumCoprocessorInstructionsExecuted(uint32_t quadrant) const;
    const std::vector<size_t> &getInstructionHeatmap() const{ return m_InstructionHeatmap; }
    size_t getNumInstructionsExecuted(StandardOpCode opCode) const;
    size_t getTotalNumInstructionsExecuted() const;
    size_t getTotalNumCoprocessorInstructionsExecuted(uint32_t quadrant) const;
    size_t getNumTrueBranches() const{ return m_NumTrueBranches; }
    size_t getNumFalseBranches() const{ return m_NumFalseBranches; }
    size_t getNumJumps() const;
    size_t getNumMemory() const;
    size_t getNumALU() const;

private:
    void setNextPC(uint32_t nextPC);

    bool calcBranchCondition(uint32_t inst, uint32_t rs2, uint32_t rs1, uint32_t funct3) const;
    uint32_t calcOpImmResult(uint32_t inst, int32_t imm, uint32_t rs1, uint32_t funct3) const;
    uint32_t calcOpResult(uint32_t inst, uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3) const;
    uint32_t loadValue(uint32_t inst, int32_t imm, uint32_t rs1, uint32_t funct3) const;
    void executeStandardInstruction(uint32_t inst);

    void executeInstruction(uint32_t inst);

    // -----------------------------------------------------------------------
    // Members
    // -----------------------------------------------------------------------
    // CPU state
    uint32_t m_PC;
    uint32_t m_NextPC;
    uint32_t m_Reg[32];

    InstructionMemory m_InstructionMemory;
    ScalarDataMemory m_ScalarDataMemory;

    std::unique_ptr<ICoprocessor> m_Coprocessors[3];

    // Stats
    std::array<size_t, 128> m_NumInstructionsExecuted;
    std::vector<size_t> m_InstructionHeatmap;
    size_t m_NumTrueBranches;
    size_t m_NumFalseBranches;
};
