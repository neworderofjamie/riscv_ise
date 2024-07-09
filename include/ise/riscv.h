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
        //FAULT_FETCH         = 0x1,
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

    template<typename T, typename... CoProcessorArgs>
    void addCoProcessor(uint32_t quadrant, CoProcessorArgs&&... coProcessorArg)
    {
        m_CoProcessors[quadrant].reset(new T(std::forward<CoProcessorArgs>(coProcessorArg)...));
    }

private:
    // M status CSR
    static constexpr uint32_t MStatusSPIEShift = 5;
    static constexpr uint32_t MStatusMPIEShift = 7;
    static constexpr uint32_t MStatusSPPShift = 8;
    static constexpr uint32_t MStatusMPPShift = 11;
    static constexpr uint32_t MStatusFSShift = 13;
    static constexpr uint32_t MStatusUXLShift = 32;
    static constexpr uint32_t MStatusSXLShift = 34;

    static constexpr uint32_t MStatusUIE = (1 << 0);
    static constexpr uint32_t MStatusSIE = (1 << 1);
    static constexpr uint32_t MStatusHIE = (1 << 2);
    static constexpr uint32_t MStatusMIE = (1 << 3);
    static constexpr uint32_t MStatusUPIE = (1 << 4);
    static constexpr uint32_t MStatusSPIE = (1 << MStatusSPIEShift);
    static constexpr uint32_t MStatusHPIE = (1 << 6);
    static constexpr uint32_t MStatusMPIE = (1 << MStatusMPIEShift);
    static constexpr uint32_t MStatusSPP = (1 << MStatusSPPShift);
    static constexpr uint32_t MStatusHPP = (3 << 9);
    static constexpr uint32_t MStatusMPP = (3 << MStatusMPPShift);
    static constexpr uint32_t MStatusFS = (3 << MStatusFSShift);
    static constexpr uint32_t MStatusXS = (3 << 15);
    static constexpr uint32_t MStatusMPRV = (1 << 17);
    static constexpr uint32_t MStatusSum = (1 << 18);
    static constexpr uint32_t MStatusMXR = (1 << 19);
    static constexpr uint64_t MStatusUXLMask = ((uint64_t)3 << MStatusUXLShift);
    static constexpr uint64_t MStatusSXLMask = ((uint64_t)3 << MStatusSXLShift);

    static constexpr uint32_t SStatusMask0 =  (MStatusUIE | MStatusSIE |
                                               MStatusUPIE | MStatusSPIE |
                                               MStatusSPP |
                                               MStatusFS | MStatusXS |
                                               MStatusSum | MStatusMXR);
    static constexpr uint32_t MStatusMask = (MStatusUIE | MStatusSIE | MStatusMIE |
                                             MStatusUPIE | MStatusSPIE | MStatusMPIE |
                                             MStatusSPP | MStatusMPP |
                                             MStatusFS |
                                             MStatusMPRV | MStatusSum | MStatusMXR);

    // cycle and insn counters
    static constexpr uint32_t CounterEnMask = (1 << 0) | (1 << 2);

    uint32_t getMStatus(uint32_t mask) const;

    void setMStatus(uint32_t val);

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
    uint32_t m_Insn;
    uint32_t m_Reg[32];

    uint8_t m_FS;

    // CSRs
    uint32_t m_MStatus;
    uint32_t m_MTVec;
    uint32_t m_MScratch;
    uint32_t m_EEPC;
    uint32_t m_MCause;
    uint32_t m_MTVal;
    uint32_t m_mhartid; /* ro */
    uint32_t m_MISA;
    uint32_t m_MIE;
    uint32_t m_MIP;
    uint32_t m_MEDeleg;
    uint32_t m_MIDeleg;
    uint32_t m_MCounterEn;

    uint32_t m_STVvec;
    uint32_t m_SScratch;
    uint32_t m_SEPC;
    uint32_t m_SCause;
    uint32_t m_STVal;
    uint32_t m_SATP;
    uint32_t m_SCounterEN;

    InstructionMemory m_InstructionMemory;
    ScalarDataMemory m_ScalarDataMemory;

    std::unique_ptr<ICoprocessor> m_CoProcessors[3];
};
