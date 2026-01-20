#include "ise/riscv.h"

// Standard C++ includes
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>

// RISC-V ISE include
#include "ise/dma_controller_sim.h"
#include "ise/router_sim.h"

// RISC-V utils include
#include "common/utils.h"

//----------------------------------------------------------------------------
// InstructionMemory
//----------------------------------------------------------------------------
InstructionMemory::InstructionMemory(size_t numWords)
:   m_Instructions(numWords, 0xDEADBEEF)
{
}
//----------------------------------------------------------------------------
uint32_t InstructionMemory::getInstruction(uint32_t addr) const
{
    if (addr & 3) {
        throw Exception(Exception::Cause::MISALIGNED_FETCH, addr);
    }
    
    if (((addr / 4) + 1) > m_Instructions.size())  {
        throw Exception(Exception::Cause::FAULT_FETCH, addr);
    } 
    else {
        return m_Instructions.at(addr / 4); 
    }
}
//----------------------------------------------------------------------------
void InstructionMemory::setInstructions(const std::vector<uint32_t> &instructions)
{ 
    // Check we're not overflowing instruction memory
    assert(instructions.size() <= m_Instructions.size());

    // Copy instructions
    std::copy(instructions.cbegin(), instructions.cend(), m_Instructions.begin());
}

//----------------------------------------------------------------------------
// ScalarDataMemory
//----------------------------------------------------------------------------
ScalarDataMemory::ScalarDataMemory(size_t numBytes, size_t startAddressBytes, uint8_t poissonVal) 
:   m_Data(numBytes, poissonVal), m_StartAddressBytes(startAddressBytes)
{}
//----------------------------------------------------------------------------
uint32_t ScalarDataMemory::read32(uint32_t addr) const
{
    // Subtract start address from address
    const uint32_t intAddr = addr - m_StartAddressBytes;

    if (intAddr & 3) {
        throw Exception(Exception::Cause::MISALIGNED_LOAD, addr);
    }

    if ((intAddr + 4) > m_Data.size())  {
        throw Exception(Exception::Cause::FAULT_LOAD, addr);
    } 
    else {
        return (m_Data[intAddr] | (m_Data[intAddr + 1] << 8)
                | (m_Data[intAddr + 2] << 16) | (m_Data[intAddr + 3] << 24));
    }
}
//----------------------------------------------------------------------------
void ScalarDataMemory::write32(uint32_t addr, uint32_t value)
{
    // Subtract start address from address
    const uint32_t intAddr = addr - m_StartAddressBytes;

    if (intAddr & 3) {
        throw Exception(Exception::Cause::MISALIGNED_LOAD, addr);
    }

    if ((intAddr + 4) > m_Data.size())  {
        throw Exception(Exception::Cause::FAULT_STORE, addr);
    } 
    else {
        m_Data[intAddr] = value & 0xff;
        m_Data[intAddr + 1] = (value >> 8) & 0xff;
        m_Data[intAddr + 2] = (value >> 16) & 0xff;
        m_Data[intAddr + 3] = (value >> 24) & 0xff;

    }
}
//----------------------------------------------------------------------------
uint16_t ScalarDataMemory::read16(uint32_t addr) const
{
    // Subtract start address from address
    const uint32_t intAddr = addr - m_StartAddressBytes;

    if (intAddr & 1) {
        throw Exception(Exception::Cause::MISALIGNED_LOAD, addr);
    }
    
    if ((intAddr + 2) > m_Data.size())  {
        throw Exception(Exception::Cause::FAULT_LOAD, addr);
    } 
    else {
        return m_Data[intAddr] | (m_Data[intAddr + 1] << 8);
    }
}
//----------------------------------------------------------------------------
void ScalarDataMemory::write16(uint32_t addr, uint16_t value)
{
    // Subtract start address from address
    const uint32_t intAddr = addr - m_StartAddressBytes;

    if (intAddr & 1) {
        throw Exception(Exception::Cause::MISALIGNED_STORE, addr);

    }
    if ((intAddr + 2) > m_Data.size()) {
        throw Exception(Exception::Cause::FAULT_STORE, addr);
    } 
    else {
        m_Data[intAddr] = value & 0xff;
        m_Data[intAddr + 1] = (value >> 8) & 0xff;
    }
}
//----------------------------------------------------------------------------
uint8_t ScalarDataMemory::read8(uint32_t addr) const
{
    // Subtract start address from address
    const uint32_t intAddr = addr - m_StartAddressBytes;

    if ((intAddr + 1) > m_Data.size()) {
        throw Exception(Exception::Cause::FAULT_LOAD, addr);
    } 
    else {
        return m_Data[intAddr];
    }

}
//----------------------------------------------------------------------------
void ScalarDataMemory::write8(uint32_t addr, uint8_t value)
{
    // Subtract start address from address
    const uint32_t intAddr = addr - m_StartAddressBytes;

    if ((addr + 1) > m_Data.size()) {
        throw Exception(Exception::Cause::FAULT_STORE, addr);
    } 
    else {
        m_Data[intAddr] = value;
    }
}
//----------------------------------------------------------------------------
void ScalarDataMemory::setData(const std::vector<uint8_t> &data)
{ 
    // Check we're not overflowing instruction memory
    assert(data.size() <= m_Data.size());

    // Copy instructions
    std::copy(data.cbegin(), data.cend(), m_Data.begin());
}

//----------------------------------------------------------------------------
// RISCV
//----------------------------------------------------------------------------
RISCV::RISCV(size_t numInstructionWords, size_t numDataBytes, size_t numSpikeBytes)
:   m_PC(0), m_NextPC(0), m_Reg{0}, m_InstructionMemory(numInstructionWords), 
    m_ScalarDataMemory(numDataBytes, 0, 0xDE), m_SpikeDataMemory(numSpikeBytes, numDataBytes, 0xAD), 
    m_DMAController(nullptr), m_Router(nullptr), m_CountInhibit(0b101)
{
    resetStats();
}
//----------------------------------------------------------------------------
bool RISCV::run()
{
    try
    {
        while (true) 
        {
            // Default value for next PC is next instruction, can be changed by branches or exceptions
            m_NextPC = m_PC + 4;

            // test for timer interrupt
            /*if (mtimecmp <= mtime) {
                mip |= MIP_MTIP;
            }
            if ((mip & mie) != 0 && (mstatus & MSTATUS_MIE)) {
                raise_interrupt();
            } else {*/
                // normal instruction execution
                const uint32_t inst = m_InstructionMemory.getInstruction(m_PC);
                m_NumOpCodesExecuted[inst & 0b1111111]++;
                m_InstructionHeatmap[m_PC / 4]++;
#ifdef DEBUG_OUTPUT
                printf("[%08x]=%08x, mtime: %lx, mtimecmp: %lx\n", pc, insn, mtime, mtimecmp);
#endif
                executeInstruction(inst);
            //}

            // test for misaligned fetches
            if (m_NextPC & 3) {
                throw Exception(Exception::Cause::MISALIGNED_FETCH, m_NextPC);
            }

            // update current PC
            m_PC = m_NextPC;
        }
    }
    catch(const Exception &ex)
    {
        switch(ex.getCause()) {
        case Exception::Cause::MISALIGNED_FETCH:
        {
            PLOGE << "Misaligned fetch at " << ex.getContext();
            break;
        }
        
        case Exception::Cause::FAULT_FETCH:
        {
            PLOGE << "Fetch fault at " << ex.getContext();
            break;
        }
        
        case Exception::Cause::ILLEGAL_INSTRUCTION:
        {
            PLOGE << "Illegal instruction " << ex.getContext();
            break;
        }
        
        case Exception::Cause::MISALIGNED_LOAD:
        {
            PLOGE << "Misaligned load at " << ex.getContext();
            break;
        }
        
        case Exception::Cause::FAULT_LOAD:
        {
            PLOGE << "Load fault at " << ex.getContext();
            break;
        }
        
        case Exception::Cause::MISALIGNED_STORE:
        {
            PLOGE << "Misaligned store at " << ex.getContext();
            break;
        }
        
        case Exception::Cause::FAULT_STORE:
        {
            PLOGE << "Store fault at " << ex.getContext();
            break;
        }

        case Exception::Cause::RAW_HAZARD:
        {
            PLOGE << "RAW hazard at " << ex.getContext();
            break;
        }

        case Exception::Cause::ECALL:
        {
            return true;
        }
        
        default:
        {
            PLOGE << "Unhandled exception";
            break;
        }
        }

        dumpRegisters();
        return false;
    }
    catch(const std::exception &exc) {
        PLOGE << "Internal error: " << exc.what();
        dumpRegisters();
        return false;
    }

}
//----------------------------------------------------------------------------
void RISCV::dumpRegisters() const
{
    // Dump standard register file
    std::cout << "Standard RISC-V register file:" << std::endl;
    std::cout << "\tPC = " << std::hex << std::setw(8) << std::setfill('0') << m_PC << std::endl;
    for(int i = 0; i < 32; i++) {
        std::cout << "\tx" << i << " = " << std::hex << std::setw(8) << std::setfill('0') << m_Reg[i] << std::endl;
    }

    // Loop through co-processor quadrants
    for(int c = 0; c < 3; c++) {
        // If co-processor present, dump it's registers
        if(m_Coprocessors[c]) {
            m_Coprocessors[c]->dumpRegisters();
        }
    }
}
//----------------------------------------------------------------------------
void RISCV::setInstructions(const std::vector<uint32_t> &instructions)
{
    m_InstructionMemory.setInstructions(instructions);
    
    // Reset heatmap
    m_InstructionHeatmap.assign(instructions.size(), 0);
}
//----------------------------------------------------------------------------
void RISCV::resetStats()
{
    // Reset standard stats
    std::fill(m_NumOpCodesExecuted.begin(), m_NumOpCodesExecuted.end(), 0);
    m_NumCycles = 0;
    m_NumInstructionsExecuted = 0;
    m_NumTrueBranches = 0;
    m_NumFalseBranches = 0;

    // Reset heatmap
    m_InstructionHeatmap.assign(m_InstructionMemory.getNumInstructions(), 0);
}
//----------------------------------------------------------------------------
size_t RISCV::getTotalNumInstructionsExecuted() const
{
    return std::accumulate(m_NumOpCodesExecuted.cbegin(), m_NumOpCodesExecuted.cend(), size_t{0});
}
//----------------------------------------------------------------------------
size_t RISCV::getTotalNumCoprocessorInstructionsExecuted(uint32_t quadrant) const
{
    const auto numCoprocessor = getNumCoprocessorInstructionsExecuted(quadrant);
    return std::accumulate(numCoprocessor.cbegin(), numCoprocessor.cend(), size_t{0});
}
//----------------------------------------------------------------------------
std::array<size_t, 32> RISCV::getNumCoprocessorInstructionsExecuted(uint32_t quadrant) const
{
    std::array<size_t, 32> num;
    for(size_t i = 0; i < 32; i++) {
        num[i] = m_NumOpCodesExecuted[(i << 2) | quadrant];
    }
    return num;
}
//----------------------------------------------------------------------------
size_t RISCV::getNumInstructionsExecuted(StandardOpCode opCode) const
{
    return m_NumOpCodesExecuted[(static_cast<uint32_t>(opCode) << 2) | standardQuadrant];
}
//----------------------------------------------------------------------------
size_t RISCV::getNumJumps() const
{
    return (getNumInstructionsExecuted(StandardOpCode::JAL) + 
            getNumInstructionsExecuted(StandardOpCode::JALR) + 
            getNumInstructionsExecuted(StandardOpCode::BRANCH));
}
//----------------------------------------------------------------------------
size_t RISCV::getNumMemory() const
{
    return (getNumInstructionsExecuted(StandardOpCode::LOAD) + 
            getNumInstructionsExecuted(StandardOpCode::STORE));
}
//----------------------------------------------------------------------------
size_t RISCV::getNumALU() const
{
    return (getNumInstructionsExecuted(StandardOpCode::OP) + 
            getNumInstructionsExecuted(StandardOpCode::OP_IMM));
}
//----------------------------------------------------------------------------
void RISCV::setNextPC(uint32_t nextPC)
{
    m_NextPC = nextPC;

    /*if(m_NextPC > m_PC) {
        forward_counter++;
    }
    else {
        backward_counter++;
    }
    jump_counter++;*/
}
//----------------------------------------------------------------------------
bool RISCV::calcBranchCondition(uint32_t inst, uint32_t rs2, uint32_t rs1, uint32_t funct3) const
{
    switch(getBranchType(funct3)) {
    case BranchType::BEQ:
    {
        PLOGV << "BEQ " << rs1 << " " << rs2;
        return (m_Reg[rs1] == m_Reg[rs2]);
    }

    case BranchType::BNE:
    {
        PLOGV << "BNE " << rs1 << " " << rs2;
        return (m_Reg[rs1] != m_Reg[rs2]);
    }

    case BranchType::BLT:
    {
        PLOGV << "BLT " << rs1 << " " << rs2;
        return ((int32_t)m_Reg[rs1] < (int32_t)m_Reg[rs2]);
    }

    case BranchType::BGE:
    {
        PLOGV << "BGE " << rs1 << " " << rs2;
        return ((int32_t)m_Reg[rs1] >= (int32_t)m_Reg[rs2]);
    }
    case BranchType::BLTU:
    {
        PLOGV << "BLTU " << rs1 << " " << rs2;
        return (m_Reg[rs1] < m_Reg[rs2]);
    }
    case BranchType::BGEU: // BGEU
    {
        PLOGV << "BGEU " << rs1 << " " << rs2;
        return (m_Reg[rs1] >= m_Reg[rs2]);
    }
    default:
    {
        throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
    }
    }
}
//----------------------------------------------------------------------------
uint32_t RISCV::calcOpImmResult(uint32_t inst, int32_t imm, uint32_t rs1, uint32_t funct3) const
{
    const uint32_t val = m_Reg[rs1];
    const uint32_t shamt = imm & 0b11111;
    switch(getOpImmType(imm, funct3)) {
    case OpImmType::ADDI:
    {
        PLOGV << "ADDI " << rs1 << " " << imm;
        return (int32_t)(val + imm);
    }

    case OpImmType::SLLI:
    {
        PLOGV << "SLLI " << rs1 << " " << imm;
        return (int32_t)(val << shamt);
    }

    case OpImmType::CLZ:
    {
        PLOGV << "CLZ " << rs1;
        return (val == 0) ? 32 : clz(val);
    }

    case OpImmType::CTZ:
    {
        PLOGV << "CTZ " << rs1;
        return (val == 0) ? 32 : ctz(val);
    }

    case OpImmType::CPOP:
    {
        PLOGV << "CPOP " << rs1;
        return popCount(val);
    }

    case OpImmType::SEXT_B:
    {
        PLOGV << "SEXT.B " << rs1;
        return (int32_t)(val << 24) >> 24;
    }

    case OpImmType::SEXT_H:
    {
        PLOGV << "SEXT.H " << rs1;
        return (int32_t)(val << 16) >> 16;
    }
    
    case OpImmType::SLTI:
    {
        PLOGV << "SLTI " << rs1 << " " << imm;
        return ((int32_t)val < (int32_t)imm);
    }

    case OpImmType::SLTIU:
    {
        PLOGV << "SLTIU " << rs1 << " " << imm;
        return (val < (uint32_t)imm);
    }

    case OpImmType::XORI:
    {
        PLOGV << "XORI " << rs1 << " " << imm;
        return (val ^ imm);
    }

    case OpImmType::SRAI:
    {
        PLOGV << "SRAI " << rs1 << " " << imm;
        return ((int32_t)val >> (imm & 31));
    }

    case OpImmType::SRLI:
    {
        PLOGV << "SRLI " << rs1 << " " << imm;
        return (int32_t)((uint32_t)val >> (imm & 31));
    }
    
    case OpImmType::ORI:
    {
        PLOGV << "ORI " << rs1 << " " << imm;
        return (val | imm);
    }

    case OpImmType::ANDI:
    {
        PLOGV << "ANDI " << rs1 << " " << imm;
        return (val & imm);
    }

    default:
    {
        throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
    }
    }
}
//----------------------------------------------------------------------------
uint32_t RISCV::calcOpResult(uint32_t inst, uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3) const
{
    const uint32_t val = m_Reg[rs1];
    const uint32_t val2 = m_Reg[rs2];

    switch(getOpType(funct7, funct3)) {
    case OpType::ADD:
    {
        PLOGV << "ADD " << rs1 << " " << rs2;
        return (int32_t)(val + val2);
    }
    
    case OpType::SUB:
    {
        PLOGV << "SUB " << rs1 << " " << rs2;
        return (int32_t)(val - val2);
    }

    case OpType::SLL:
    {
        PLOGV << "SLL " << rs1 << " " << rs2;
        return (int32_t)(val << (val2 & 31));
    }

    case OpType::SLT:
    {
        PLOGV << "SLT " << rs1 << " " << rs2;
        return (int32_t)val < (int32_t)val2;
    }

    case OpType::SLTU:
    {
        PLOGV << "SLTU " << rs1 << " " << rs2;
        return val < val2;
    }

    case OpType::XOR:
    {
        PLOGV << "XOR " << rs1 << " " << rs2;
        return val ^ val2;
    }

    case OpType::SRL:
    {
        PLOGV << "SRL " << rs1 << " " << rs2;
        return (int32_t)((uint32_t)val >> (val2 & 31));
    }

    case OpType::SRA:
    {
        PLOGV << "SRA " << rs1 << " " << rs2;
        return (int32_t)val >> (val2 & 31);
    }
    
    case OpType::OR:
    {
        PLOGV << "OR " << rs1 << " " << rs2;
        return val | val2;
    }

    case OpType::AND:
    {
        PLOGV << "AND " << rs1 << " " << rs2;
        return val & val2;
    }

    case OpType::MUL:
    {
        PLOGV << "MUL " << rs1 << " " << rs2;
        return (int32_t)((int32_t)val * (int32_t)val2);
    }

    default:
    {
        throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
    }
    }
}
//----------------------------------------------------------------------------
uint32_t RISCV::loadValue(uint32_t inst, int32_t imm, uint32_t rs1, uint32_t funct3) const
{
    const uint32_t addr = m_Reg[rs1] + imm;
    const auto &memory = getScalarMemory(addr);
    switch(getLoadType(funct3)) {
    case LoadType::LB:
    {
        PLOGV << "LB " << rs1 << " " << imm;
        return (int8_t)memory.read8(addr);
    }

    case LoadType::LH:
    {
        PLOGV << "LH " << rs1 << " " << imm;
        return (int16_t)memory.read16(addr);
    }

    case LoadType::LW:
    {
        PLOGV << "LW " << rs1 << " " << imm;
        return (int32_t)memory.read32(addr);
    }
    
    case LoadType::LBU:
    {
        PLOGV << "LBU " << rs1 << " " << imm;
        return memory.read8(addr);
    }
    break;

    case LoadType::LHU:
    {
        PLOGV << "LHU " << rs1 << " " << imm;
        return memory.read16(addr);
    }

    default:
    {
        throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
    }
    }
}
//----------------------------------------------------------------------------
std::optional<uint32_t> RISCV::readCSR(uint32_t csr, bool willWrite) const
{
#ifdef DEBUG_EXTRA
    printf("csr_read: csr=0x%03x %i\n", csr, willWrite);
#endif

    // Read-only CSR 
    if (((csr & 0xc00) == 0xc00) && willWrite) {
        return std::nullopt; 
    }
    #define IMPLEMENT_READ_DMA_REG(REG)                                             \
        case CSR::REG:                                                              \
        {                                                                           \
            if(m_DMAController) {                                                   \
                return m_DMAController->readReg(DMAControllerSim::Register::REG);   \
            }                                                                       \
            else {                                                                  \
                return std::nullopt;                                                \
            }                                                                       \
        }
    
    #define IMPLEMENT_READ_ROUTER_REG(REG)                                          \
        case CSR::REG:                                                              \
        {                                                                           \
            if(m_Router) {                                                          \
                return m_Router->readReg(RouterSim::Register::REG);                 \
            }                                                                       \
            else {                                                                  \
                return std::nullopt;                                                \
            }                                                                       \
        }

    switch(static_cast<CSR>(csr)) {
    /*case CSR::MSTATUS:
        return getMStatus((uint32_t)-1);
    case CSR::MISA:
        uint32_t val = misa;
        val |= (uint32_t)mxl << (XLEN - 2);
        return val;
    case CSR::MIE:
        return mie;
    case CSR::MTVEC:
        return mtvec;*/
    case CSR::MCOUNTINHIBIT:
        return m_CountInhibit;
    /*case CSR::MSCRATCH:
        return mscratch;
    case CSR::MEPC:
        return mepc;
    case CSR::MCAUSE:
        return mcause;
    case CSR::MTVAL:
        return mtval;
    case CSR::MIP:
        return mip;*/
    case CSR::MCYCLE:
        return (int64_t)m_NumCycles;
    case CSR::MINSTRET:
        return (int64_t)m_NumInstructionsExecuted;
    case CSR::MCYCLEH:
        return m_NumCycles >> 32;
    case CSR::MINSTRETH:
        return m_NumInstructionsExecuted >> 32;
    /*case CSR::MHARTID:
        return mhartid;*/
    
    IMPLEMENT_READ_DMA_REG(MM2S_SRC_ADDR)
    IMPLEMENT_READ_DMA_REG(MM2S_DST_ADDR)
    IMPLEMENT_READ_DMA_REG(MM2S_COUNT)
    IMPLEMENT_READ_DMA_REG(MM2S_CONTROL)
    IMPLEMENT_READ_DMA_REG(MM2S_STATUS)
    IMPLEMENT_READ_DMA_REG(S2MM_SRC_ADDR)
    IMPLEMENT_READ_DMA_REG(S2MM_DST_ADDR)
    IMPLEMENT_READ_DMA_REG(S2MM_COUNT)
    IMPLEMENT_READ_DMA_REG(S2MM_CONTROL)
    IMPLEMENT_READ_DMA_REG(S2MM_STATUS)
    
    IMPLEMENT_READ_ROUTER_REG(MASTER_EVENT_ID_BASE)
    IMPLEMENT_READ_ROUTER_REG(MASTER_EVENT_BITFIELD)
    IMPLEMENT_READ_ROUTER_REG(MASTER_SEND_BARRIER)
    IMPLEMENT_READ_ROUTER_REG(SLAVE_EVENT_ADDRESS)
    IMPLEMENT_READ_ROUTER_REG(SLAVE_BARRIER_COUNT)

    default:
        return std::nullopt;
    }

    #undef IMPLEMENT_READ_DMA_REG
    #undef IMPLEMENT_READ_ROUTER_REG
}
//----------------------------------------------------------------------------
bool RISCV::writeCSR(uint32_t csr, uint32_t val)
{
    #define IMPLEMENT_WRITE_DMA_REG(REG)                                            \
        case CSR::REG:                                                              \
        {                                                                           \
            if(m_DMAController) {                                                   \
                m_DMAController->writeReg(DMAControllerSim::Register::REG, val);    \
            }                                                                       \
            return true;                                                            \
        }

    #define IMPLEMENT_WRITE_ROUTER_REG(REG)                                            \
        case CSR::REG:                                                              \
        {                                                                           \
            if(m_Router) {                                                   \
                m_Router->writeReg(RouterSim::Register::REG, val);    \
            }                                                                       \
            return true;                                                            \
        }

    switch(static_cast<CSR>(csr)) {
    /*case CSR::MSTATUS:
        setMStatus(val);
        break;
    case CSR::MISA:
        break;
    case CSR::MIE:
        const uint32_t mask = MIP_MSIP | MIP_MTIP | MIP_SSIP | MIP_STIP | MIP_SEIP;
        mie = (mie & ~mask) | (val & mask);
        break;
    case CSR::MTVEC:
        mtvec = val & ~3;
        break;*/
    case CSR::MCOUNTINHIBIT:
        m_CountInhibit = val & 0b101;
        return true;
    /*case CSR::MSCRATCH:
        mscratch = val;
        break;
    case CSR::MEPC:
        mepc = val & ~1;
        break;
    case CSR::MCAUSE:
        mcause = val;
        break;
    case CSR::MTVAL:
        mtval = val;
        break;
    case CSR::MIP:
        const uint32_t mask = MIP_SSIP | MIP_STIP;
        mip = (mip & ~mask) | (val & mask);
        break;*/
    IMPLEMENT_WRITE_DMA_REG(MM2S_SRC_ADDR)
    IMPLEMENT_WRITE_DMA_REG(MM2S_DST_ADDR)
    IMPLEMENT_WRITE_DMA_REG(MM2S_COUNT)
    IMPLEMENT_WRITE_DMA_REG(MM2S_CONTROL)
    IMPLEMENT_WRITE_DMA_REG(S2MM_SRC_ADDR)
    IMPLEMENT_WRITE_DMA_REG(S2MM_DST_ADDR)
    IMPLEMENT_WRITE_DMA_REG(S2MM_COUNT)
    IMPLEMENT_WRITE_DMA_REG(S2MM_CONTROL)
    
    IMPLEMENT_WRITE_ROUTER_REG(MASTER_EVENT_ID_BASE)
    IMPLEMENT_WRITE_ROUTER_REG(MASTER_EVENT_BITFIELD)
    IMPLEMENT_WRITE_ROUTER_REG(MASTER_SEND_BARRIER)
    IMPLEMENT_WRITE_ROUTER_REG(SLAVE_EVENT_ADDRESS)
    IMPLEMENT_WRITE_ROUTER_REG(SLAVE_BARRIER_COUNT)

    default:
        return true;
    }

    #undef IMPLEMENT_WRITE_DMA_REG
    #undef IMPLEMENT_WRITE_ROUTER_REG
}
//----------------------------------------------------------------------------
void RISCV::executeStandardInstruction(uint32_t inst)
{
    const auto opcode = static_cast<StandardOpCode>((inst & 0b1111100) >> 2);
    switch(opcode) {
    case StandardOpCode::LUI:
    {
        const auto [imm, rd] = decodeUType(inst);
        PLOGV << "LUI " << imm;
        PLOGV << "\t"  << rd;
#ifdef DEBUG_EXTRA
        stats[0]++;
#endif
        if (rd != 0) {
            m_Reg[rd] = (imm << 12);
        }
        break;
    }

    case StandardOpCode::AUIPC:
    {
        const auto [imm, rd] = decodeUType(inst);
        PLOGV << "AUIPC " << imm;
        PLOGV << "\t"  << rd;
#ifdef DEBUG_EXTRA
        stats[1]++;
#endif
        if (rd != 0) {
            m_Reg[rd] = (int32_t)(m_PC + (imm << 12));
        }
        break;
    }

    case StandardOpCode::JAL:
    {
        const auto [imm, rd] = decodeJType(inst);
        
        PLOGV << "JAL " << rd << " " << imm;
        PLOGV << "\t"  << rd;
#ifdef DEBUG_EXTRA
        stats[2]++;
#endif
        if (rd != 0) {
            m_Reg[rd] = m_PC + 4;
        }

        setNextPC((int32_t)(m_PC + imm));
       
        break;
    }

    case StandardOpCode::JALR:
    {
        const auto [imm, rs1, funct3, rd] = decodeIType(inst);
        PLOGV << "JALR " << rs1 << " " << imm;
        PLOGV << "\t"  << rd;
#ifdef DEBUG_EXTRA
        stats[3]++;
#endif
        
        const uint32_t val  = m_PC + 4;
        setNextPC((int32_t)(m_Reg[rs1] + imm) & ~1);
        if (rd != 0) {
            m_Reg[rd] = val;
        }
        break;
    }

    case StandardOpCode::BRANCH:
    {
        const auto [imm, rs2, rs1, funct3] = decodeBType(inst);
        if (calcBranchCondition(inst, rs2, rs1, funct3)) {
            PLOGV << "\t" << (m_PC + imm);

            m_NumTrueBranches++;
            setNextPC((int32_t)(m_PC + imm));
        } 
        else {
            m_NumFalseBranches++;
        }
        break;
    }

    case StandardOpCode::LOAD:
    {
        const auto [imm, rs1, funct3, rd] = decodeIType(inst);
        const auto value = loadValue(inst, imm, rs1, funct3);
        PLOGV << "\t" << rd;
        if (rd != 0) {
            m_Reg[rd] = value;
        }
        break;
    }

    case StandardOpCode::STORE:
    {
        const auto [imm, rs2, rs1, funct3] = decodeSType(inst);
        const uint32_t addr = m_Reg[rs1] + imm;
        auto &memory = getScalarMemory(addr);
        const uint32_t val = m_Reg[rs2];
        switch(getStoreType(funct3)) {
        case StoreType::SB:
        {
            PLOGV << "SB " << rs2 << " " << rs1 << " " << imm;
            memory.write8(addr, val);
            break;
        }

        case StoreType::SH:
        {
            PLOGV << "SH " << rs2 << " " << rs1 << " " << imm;
            memory.write16(addr, val);
            break;
        }

        case StoreType::SW:
        {
            PLOGV << "SW " << rs2 << " " << rs1 << " " << imm;
            memory.write32(addr, val);
            break;
        }

        default:
        {
            throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
        }
        }
        break;
    }

    case StandardOpCode::OP_IMM:
    {
        const auto [imm, rs1, funct3, rd] = decodeIType(inst);
        const uint32_t val = calcOpImmResult(inst, imm, rs1, funct3);
        PLOGV << "\t" << rd;
        if (rd != 0) {
            m_Reg[rd] = val;
        }
        break;
    }

    case StandardOpCode::OP:
    {
        const auto [funct7, rs2, rs1, funct3, rd] = decodeRType(inst);
        const uint32_t val = calcOpResult(inst, funct7, rs2, rs1, funct3);
        PLOGV << "\t" << rd;
        if (rd != 0) {
            m_Reg[rd] = val;
        }
        break;
    }

    case StandardOpCode::SYSTEM:
    {
        const auto [imm, rs1, funct3, rd] = decodeIType<uint32_t>(inst);

        const uint32_t val = (funct3 & 4) ? rs1 : m_Reg[rs1];
        switch(getSystemType(imm, funct3)) {
        case SystemType::CSRRW:
        case SystemType::CSRRWI:
        {
/*#ifdef DEBUG_EXTRA
            if ((insn >> 12) & 4)
            {
               dprintf(">>> CSRRWI\n");
               stats[44]++;
            }
            else
            {
               dprintf(">>> CSRRW\n");
               stats[41]++;
            }
#endif*/
            const auto newVal = readCSR(imm, true);
            if (newVal) {
                if(!writeCSR(imm, val)) {
                    throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
                }
                if (rd != 0) {
                    m_Reg[rd] = newVal.value();
                }
            }
            else {
                throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
            }
            
            break;
        }

        case SystemType::CSRRS:
        case SystemType::CSRRSI:
        case SystemType::CSRRC:
        case SystemType::CSRRCI:
        {
            const auto newVal = readCSR(imm, rs1 != 0);
            if(newVal) {
    #ifdef DEBUG_EXTRA
                switch((insn >> 12) & 7)
                {
                   case 2: dprintf(">>> CSRRS\n"); stats[42]++; break;
                   case 3: dprintf(">>> CSRRC\n"); stats[43]++; break;
                   case 6: dprintf(">>> CSRRSI\n"); stats[45]++; break;
                   case 7: dprintf(">>> CSRRCI\n"); stats[46]++; break;
                }
    #endif
                if (rs1 != 0)
                {
                    // CSRRSI or CSRRS
                    const uint32_t writeVal = ((funct3 & 3) == 2) ? (newVal.value() | val) :  (newVal.value() & ~val);
                    
                    if (!writeCSR(imm, writeVal)) {
                        throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
                    }
                }
                
                if (rd != 0) {
                    m_Reg[rd] = newVal.value();
                }
            }
            else {
                throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
            }
            break;
        }

        case SystemType::ECALL:
        {
            PLOGV << "ECALL";

            if (inst & 0x000fff80) {
                throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
            }
            else {
                throw Exception(Exception::Cause::ECALL, inst);
            }
        }
        
        case SystemType::EBREAK:
        {
            PLOGV << "EBREAK";
            if (inst & 0x000fff80) {
                throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
            }
            else {
                breakPoint();
            }
            break;
        }

        /*case 0x102: //sret
        {
#ifdef DEBUG_EXTRA
            dprintf(">>> SRET\n");
            stats[59]++;
#endif
            if ((insn & 0x000fff80) || (priv < PRV_S)) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            handle_sret();
            return;
        }
        break;

        case 0x105: // wfi 
#ifdef DEBUG_EXTRA
            dprintf(">>> WFI\n");
            stats[61]++;
#endif
            //wait for interrupt: it is allowed to execute it as nop 
            break;

        case 0x302: // mret 
        {
#ifdef DEBUG_EXTRA
            dprintf(">>> MRET\n");
            stats[60]++;
#endif
            if ((insn & 0x000fff80) || (priv < PRV_M)) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            handle_mret();
            return;
        }
        break;*/

        default:
        {
            throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
        }
        }
        
        break;
    }

    default:
    {
        throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
    }
    }
}
//----------------------------------------------------------------------------
void RISCV::executeInstruction(uint32_t inst)
{
    // Tick co-processors
    for(auto &c : m_Coprocessors) {
        if(c) {
            c->tick();
        }
    }

    // Tick DMA controller and router if present
    if(m_DMAController) {
        m_DMAController->tick();
    }
    if(m_Router) {
        m_Router->tick();
    }

    // Extract 2-bit quadrant
    const uint32_t quadrant = inst & 0b11;

    // If instruction is in standard quadrant
    if(quadrant == standardQuadrant) {
        executeStandardInstruction(inst); 
    }
    // Otherwise, if there is a co-processor defined to handle this quadrant
    else if(m_Coprocessors[quadrant]){
        m_Coprocessors[quadrant]->executeInstruction(inst, m_Reg, m_ScalarDataMemory, m_PC);
    }
    // Otherwise, throw
    else {
        throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
    }

    // Increment counters if not inhibited
    // **NOTE** no difference between instruction and cycle count in 
    // simulation, distinction is just for CVC32E40X
    if(!(m_CountInhibit & 0b001)) {
        m_NumCycles++;
    }
    if(!(m_CountInhibit & 0b100)) {
        m_NumInstructionsExecuted++;
    }
}
