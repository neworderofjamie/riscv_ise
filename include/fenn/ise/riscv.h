#pragma once

// Standard C++ includes
#include <array>
#include <memory>
#include <optional>
#include <vector>

// Standard C includes
#include <cassert>
#include <cstdint>

// FeNN common includes
#include "fenn/common/isa.h"

// ISE includes
#include "fenn/ise/ise_export.h"

// Forward declarations
class DMAControllerSim;
class RouterSim;

//----------------------------------------------------------------------------
// InstructionMemory
//----------------------------------------------------------------------------
class ISE_EXPORT InstructionMemory
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
class ISE_EXPORT ScalarDataMemory
{
public:
    ScalarDataMemory(size_t numBytes, size_t startAddressBytes, uint8_t poissonVal);

    uint32_t read32(uint32_t addr) const;
    void write32(uint32_t addr, uint32_t value);

    uint16_t read16(uint32_t addr) const;
    void write16(uint32_t addr, uint16_t value);

    uint8_t read8(uint32_t addr) const;
    void write8(uint32_t addr, uint8_t value);
    
    void setData(const std::vector<uint8_t> &data);

    const uint8_t *getData() const{ return m_Data.data(); }
    uint8_t *getData(){ return m_Data.data(); }
    
    size_t getSizeBytes() const{ return m_Data.size(); }

private:
    size_t m_StartAddressBytes;
    std::vector<uint8_t> m_Data;
};

//----------------------------------------------------------------------------
// Exception
//----------------------------------------------------------------------------
class ISE_EXPORT Exception : std::exception
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
        RAW_HAZARD,
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
class ISE_EXPORT RISCV
{
public:
    //------------------------------------------------------------------------
    // ICoprocessor
    //------------------------------------------------------------------------
    class ICoprocessor
    {
    public:
        virtual void tick() = 0;
        virtual void executeInstruction(uint32_t inst, uint32_t (&reg)[32], ScalarDataMemory &scalarDataMemory, uint32_t pc) = 0;
        virtual void dumpRegisters() const = 0;
    };

    RISCV(size_t numInstructionWords = 1024, size_t numDataBytes = 126976, size_t numSpikeBytes = 4096);

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

    void setDMAController(DMAControllerSim *dmaController){ m_DMAController = dmaController; };
    void setRouter(RouterSim *router){ m_Router = router; };
    
    auto &getScalarDataMemory(){ return m_ScalarDataMemory; }
    const auto &getScalarDataMemory() const{ return m_ScalarDataMemory; }

    auto &getSpikeDataMemory(){ return m_SpikeDataMemory; }
    const auto &getSpikeDataMemory() const{ return m_SpikeDataMemory; }

    void setPC(uint32_t pc){ m_PC = pc; }
    uint32_t getPC() const{ return m_PC; }

    void setInstructions(const std::vector<uint32_t> &instructions);

    void resetStats();
    const auto &getNumOpCodesExecuted() const{ return m_NumOpCodesExecuted; }
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
    std::optional<uint32_t> readCSR(uint32_t csr, bool will_write) const;
    bool writeCSR(uint32_t csr, uint32_t val);

    ScalarDataMemory &getScalarMemory(uint32_t addr)
    {
        return (addr < m_ScalarDataMemory.getSizeBytes()) ? m_ScalarDataMemory : m_SpikeDataMemory;
    }

    const ScalarDataMemory &getScalarMemory(uint32_t addr) const
    {
        return (addr < m_ScalarDataMemory.getSizeBytes()) ? m_ScalarDataMemory : m_SpikeDataMemory;
    }

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
    ScalarDataMemory m_SpikeDataMemory;

    std::unique_ptr<ICoprocessor> m_Coprocessors[3];

    DMAControllerSim *m_DMAController;
    RouterSim *m_Router;

    // Stats
    uint64_t m_NumCycles;
    uint64_t m_NumInstructionsExecuted;
    uint32_t m_CountInhibit;
    std::array<size_t, 128> m_NumOpCodesExecuted;
    std::vector<size_t> m_InstructionHeatmap;
    size_t m_NumTrueBranches;
    size_t m_NumFalseBranches;
};
