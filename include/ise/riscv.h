#pragma once

// Standard C++ includes
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
    InstructionMemory(const std::vector<uint32_t> &instructions);

    uint32_t getInstruction(uint32_t addr) const;

private:
    std::vector<uint32_t> m_Instructions;
};

//----------------------------------------------------------------------------
// ScalarDataMemory
//----------------------------------------------------------------------------
class ScalarDataMemory
{
public:
    ScalarDataMemory(const std::vector<uint8_t> &data);

    uint32_t read32(uint32_t addr) const;
    void write32(uint32_t addr, uint32_t value);

    uint16_t read16(uint32_t addr) const;
    void write16(uint32_t addr, uint16_t value);

    uint8_t read8(uint32_t addr) const;
    void write8(uint32_t addr, uint8_t value);
    
    auto &getData(){ return m_Data; }
    const auto &getData() const{ return m_Data; }

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
        MISALIGNED_FETCH    = 0x0,
        FAULT_FETCH         = 0x1,
        ILLEGAL_INSTRUCTION = 0x2,
        //BREAKPOINT          = 0x3,
        MISALIGNED_LOAD     = 0x4,
        FAULT_LOAD          = 0x5,
        MISALIGNED_STORE    = 0x6,
        FAULT_STORE         = 0x7,
        //USER_ECALL          = 0x8,
        //SUPERVISOR_ECALL    = 0x9,
        //HYPERVISOR_ECALL    = 0xa,
        //MACHINE_ECALL       = 0xb,
        //FETCH_PAGE_FAULT    = 0xc,
        //LOAD_PAGE_FAULT     = 0xd,
        //STORE_PAGE_FAULT    = 0xf,
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
    };

    RISCV(const std::vector<uint32_t> &instructions, const std::vector<uint8_t> &data);

    void run();

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
    
    ScalarDataMemory &getScalarDataMemory(){ return m_ScalarDataMemory; }
    const ScalarDataMemory &getScalarDataMemory() const{ return m_ScalarDataMemory; }
    
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
    bool m_MachineRunning;

    // CPU state
    uint32_t m_PC;
    uint32_t m_NextPC;
    uint32_t m_Reg[32];

    InstructionMemory m_InstructionMemory;
    ScalarDataMemory m_ScalarDataMemory;

    std::unique_ptr<ICoprocessor> m_Coprocessors[3];
};
