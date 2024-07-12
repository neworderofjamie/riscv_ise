#include "ise/riscv.h"

// Standard C++ includes
#include <iomanip>
#include <iostream>
#include <stdexcept>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>

// Platform includes
#ifdef _WIN32
#include <intrin.h>
#endif

// Anonymous namespace
namespace
{
int clz(uint32_t value)
{
#ifdef _WIN32
    unsigned long leadingZero = 0;
    if(_BitScanReverse(&leadingZero, value)) {
        return 31 - leadingZero;
    }
    else {
        return 32;
    }
#else
    return __builtin_clz(value);
#endif
}

int ctz(uint32_t value)
{
#ifdef _WIN32
    unsigned long trailingZero = 0;
    if(_BitScanForward(&trailingZero, value)) {
        return trailingZero;
    }
    else {
        return 32;
    }
#else
    return __builtin_ctz(value);
#endif
}

int popCount(uint32_t value)
{
#ifdef _WIN32
    return __popcnt(value);
#else
    return __builtin_popcount(value);
#endif
}

void breakPoint()
{
#ifdef _WIN32
    return __debugbreak();
#else
    return __builtin_trap();
#endif
}
}
//----------------------------------------------------------------------------
// InstructionMemory
//----------------------------------------------------------------------------
InstructionMemory::InstructionMemory(const std::vector<uint32_t> &instructions)
:   m_Instructions(instructions)
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
// ScalarDataMemory
//----------------------------------------------------------------------------
ScalarDataMemory::ScalarDataMemory(const std::vector<uint8_t> &data) 
:   m_Data(data)
{}
//----------------------------------------------------------------------------
uint32_t ScalarDataMemory::read32(uint32_t addr) const
{
    if (addr & 3) {
        throw Exception(Exception::Cause::MISALIGNED_LOAD, addr);
    }

    if ((addr + 4) > m_Data.size())  {
        throw Exception(Exception::Cause::FAULT_LOAD, addr);
    } 
    else {
        return m_Data[addr] | (m_Data[addr + 1] << 8) | (m_Data[addr + 2] << 16) | (m_Data[addr + 3] << 24);
    }
}
//----------------------------------------------------------------------------
void ScalarDataMemory::write32(uint32_t addr, uint32_t value)
{
    if (addr & 3) {
        throw Exception(Exception::Cause::MISALIGNED_LOAD, addr);
    }

    if ((addr + 4) > m_Data.size())  {
        throw Exception(Exception::Cause::FAULT_STORE, addr);
    } 
    else {
        m_Data[addr] = value & 0xff;
        m_Data[addr + 1] = (value >> 8) & 0xff;
        m_Data[addr + 2] = (value >> 16) & 0xff;
        m_Data[addr + 3] = (value >> 24) & 0xff;

    }
}
//----------------------------------------------------------------------------
uint16_t ScalarDataMemory::read16(uint32_t addr) const
{
    if (addr & 1) {
        throw Exception(Exception::Cause::MISALIGNED_LOAD, addr);
    }
    
    if ((addr + 2) > m_Data.size())  {
        throw Exception(Exception::Cause::FAULT_LOAD, addr);
    } 
    else {
        return m_Data[addr] | (m_Data[addr + 1] << 8);
    }

}
//----------------------------------------------------------------------------
void ScalarDataMemory::write16(uint32_t addr, uint16_t value)
{
    if (addr & 1) {
        throw Exception(Exception::Cause::MISALIGNED_STORE, addr);

    }
    if ((addr + 2) > m_Data.size()) {
        throw Exception(Exception::Cause::FAULT_STORE, addr);
    } 
    else {
        m_Data[addr] = value & 0xff;
        m_Data[addr + 1] = (value >> 8) & 0xff;
    }
}
//----------------------------------------------------------------------------
uint8_t ScalarDataMemory::read8(uint32_t addr) const
{
    if ((addr + 1) > m_Data.size()) {
        throw Exception(Exception::Cause::FAULT_LOAD, addr);
    } 
    else {
        return m_Data[addr];
    }

}
//----------------------------------------------------------------------------
void ScalarDataMemory::write8(uint32_t addr, uint8_t value)
{
    if ((addr + 1) > m_Data.size()) {
        throw Exception(Exception::Cause::FAULT_STORE, addr);
    } 
    else {
        m_Data[addr] = value;
    }
}

//----------------------------------------------------------------------------
// RISCV
//----------------------------------------------------------------------------
RISCV::RISCV(const std::vector<uint32_t> &instructions, const std::vector<uint8_t> &data)
:   m_PC(0), m_NextPC(0), m_Reg{0}, m_InstructionMemory(instructions), m_ScalarDataMemory(data)
{
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
                //insn_counter++;

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
    switch(funct3) {
    case 0: // BEQ
    {
        PLOGD << "BEQ " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
        stats[4]++;
#endif
        return (m_Reg[rs1] == m_Reg[rs2]);
    }

    case 1: // BNE
    {
        PLOGD << "BNE " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
        stats[5]++;
#endif
        return (m_Reg[rs1] != m_Reg[rs2]);
    }

    case 4: // BLT
    {
        PLOGD << "BLT " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
        stats[6]++;
#endif
        return ((int32_t)m_Reg[rs1] < (int32_t)m_Reg[rs2]);
    }

    case 5: // BGE
    {
        PLOGD << "BGE " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
        stats[7]++;
#endif
        return ((int32_t)m_Reg[rs1] >= (int32_t)m_Reg[rs2]);
    }
    case 6: // BLTU
    {
        PLOGD << "BLTU " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
        stats[8]++;
#endif
        return (m_Reg[rs1] < m_Reg[rs2]);
    }
    case 7: // BGEU
    {
        PLOGD << "BGEU " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
        stats[9]++;
#endif
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
    switch(funct3) {
    case 0: // ADDI
    {
        PLOGD << "ADDI " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
        stats[18]++;
#endif
        return (int32_t)(val + imm);
    }
    case 1: // SLLI/CLZ/CPOP/CTZ/SEXT
    {
        // Split immediate into shamt (shift) and upper field
        const uint32_t shamt = imm & 0b11111;
        const uint32_t pre = imm >> 5;

        // SLLI
        if (pre == 0) {
            PLOGD << "SLLI " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
            stats[24]++;
#endif
            return (int32_t)(val << shamt);
        }
        // CLZ/CPOP/CTZ/SEXT
        else if(pre == 0b110000) {
            switch(shamt) {
            case 0: // CLZ
            {
                PLOGD << "CLZ " << rs1;
                return (val == 0) ? 32 : clz(val);
            }

            case 1: // CTZ
            {
                PLOGD << "CTZ " << rs1;
                return (val == 0) ? 32 : ctz(val);
            }

            case 2: // CPOP
            {
                PLOGD << "CPOP " << rs1;
                return popCount(val);
            }

            case 4: // SEXT.B
            {
                PLOGD << "SEXT.B " << rs1;
                return (int32_t)(val << 24) >> 24;
            }

            case 5: // SEXT.H
            {
                PLOGD << "SEXT.H " << rs1;
                return (int32_t)(val << 16) >> 16;
            }

            default:
            {
                throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
            }
            }
        }
        else {
            throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
        }
    }

    case 2: // SLTI
    {
        PLOGD << "SLTI " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
        stats[19]++;
#endif
        return ((int32_t)val < (int32_t)imm);
    }
    case 3: // SLTIU
    {
        PLOGD << "SLTIU " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
        stats[20]++;
#endif
        return (val < (uint32_t)imm);
    }
    case 4: // XORI
    {
        PLOGD << "XORI " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
        dprintf(">>> XORI\n");
        stats[21]++;
#endif
        return (val ^ imm);
    }
    case 5: // SRLI / SRAI
    {
        // Error if any bits other than the 11th bit are set in immediate field
        if ((imm & ~(0b11111 | 0x400)) != 0) {
            throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
        }

        // If 11th bit set, SRAI
        if (imm & 0x400) {
            PLOGD << "SRAI " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
            stats[26]++;
#endif
            return ((int32_t)val >> (imm & 31));
        }
        // Otherwise SRLI
        else
        {
            PLOGD << "SRLI " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
            stats[25]++;
#endif
            return (int32_t)((uint32_t)val >> (imm & 31));
        }
    }
    case 6: // ORI
    {
        PLOGD << "ORI " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
        stats[22]++;
#endif
        return (val | imm);
    }
    case 7: // ANDI
    {
        PLOGD << "ANDI " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
        stats[23]++;
#endif
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
#ifdef STRICT_RV32I
    if (imm == 1) {
        switch(funct3) {
        case 0: /* mul */
#ifdef DEBUG_EXTRA
            dprintf(">>> MUL\n");
            stats[48]++;
#endif
            val = (int32_t)((int32_t)val * (int32_t)val2);
            break;
        case 1: /* mulh */
#ifdef DEBUG_EXTRA
            dprintf(">>> MULH\n");
            stats[49]++;
#endif
            val = (int32_t)mulh32(val, val2);
            break;
        case 2:/* mulhsu */
#ifdef DEBUG_EXTRA
            dprintf(">>> MULHSU\n");
            stats[50]++;
#endif
            val = (int32_t)mulhsu32(val, val2);
            break;
        case 3:/* mulhu */
#ifdef DEBUG_EXTRA
            dprintf(">>> MULHU\n");
            stats[51]++;
#endif
            val = (int32_t)mulhu32(val, val2);
            break;
        case 4:/* div */
#ifdef DEBUG_EXTRA
            dprintf(">>> DIV\n");
            stats[52]++;
#endif
            val = div32(val, val2);
            break;
        case 5:/* divu */
#ifdef DEBUG_EXTRA
            dprintf(">>> DIVU\n");
            stats[53]++;
#endif
            val = (int32_t)divu32(val, val2);
            break;
        case 6:/* rem */
#ifdef DEBUG_EXTRA
            dprintf(">>> REM\n");
            stats[54]++;
#endif
            val = rem32(val, val2);
            break;
        case 7:/* remu */
#ifdef DEBUG_EXTRA
            dprintf(">>> REMU\n");
            stats[55]++;
#endif
            val = (int32_t)remu32(val, val2);
            break;
        default:
            raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
            return;
        }
    }
    else
#endif
    {
        // If any bits are set in funct7 aside from the one used for distinguishing ops
        if (funct7 & ~0x20) {
            throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
        }
        switch(funct3) {
        case 0: // ADD/SUB
        {
            if(funct7 == 0) {
                PLOGD << "ADD " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
                stats[27]++;
#endif
                return (int32_t)(val + val2);
            }
            else {
                PLOGD << "SUB " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
                stats[28]++;
#endif
                return (int32_t)(val - val2);
            }
        }

        case 1: // SLL
        {
            PLOGD << "SLL " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
            stats[29]++;
#endif
            return (int32_t)(val << (val2 & 31));
        }

        case 2: // SLT
        {
            PLOGD << "SLT " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
            stats[30]++;
#endif
            return (int32_t)val < (int32_t)val2;
        }
        case 3: // SLTU
        {
            PLOGD << "SLTU " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
            stats[31]++;
#endif
            return val < val2;
        }

        case 4: // XOR
        {
            PLOGD << "XOR " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
            stats[32]++;
#endif
            return val ^ val2;
        }

        case 5: // SRL/SRA
        {
            if(funct7 == 0) {
                PLOGD << "SRL " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
                stats[33]++;
#endif
                return (int32_t)((uint32_t)val >> (val2 & 31));
            }
            else {
                PLOGD << "SRA " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
                stats[34]++;
#endif
                return (int32_t)val >> (val2 & 31);
            }
        }

        case 6: // OR
        {
            PLOGD << "OR " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
            stats[35]++;
#endif
            return val | val2;
        }
        case 7: // AND
        {
            PLOGD << "AND " << rs1 << " " << rs2;
#ifdef DEBUG_EXTRA
            stats[36]++;
#endif
            return val & val2;
        }
        default:
        {
            throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
        }
        }
    }
}
//----------------------------------------------------------------------------
uint32_t RISCV::loadValue(uint32_t inst, int32_t imm, uint32_t rs1, uint32_t funct3) const
{
    const uint32_t addr = m_Reg[rs1] + imm;
    switch(funct3) {
    case 0: // LB
    {
        PLOGD << "LB " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
        stats[10]++;
#endif
        return (int8_t)m_ScalarDataMemory.read8(addr);
    }

    case 1: // LH
    {
        PLOGD << "LH " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
        stats[11]++;
#endif
        return (int16_t)m_ScalarDataMemory.read16(addr);
    }

    case 2: // LW
    {
        PLOGD << "LW " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
        stats[12]++;
#endif
        return (int32_t)m_ScalarDataMemory.read32(addr);
    }
    
    case 4: // LBU
    {
        PLOGD << "LBU " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
        stats[13]++;
#endif
        return m_ScalarDataMemory.read8(addr);
    }
    break;

    case 5: // LHU
    {
        PLOGD << "LHU " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
        stats[14]++;
#endif
        return m_ScalarDataMemory.read16(addr);
    }

    default:
    {
        throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
    }
    }
}
//----------------------------------------------------------------------------
void RISCV::executeStandardInstruction(uint32_t inst)
{
    const auto opcode = static_cast<StandardOpCode>((inst & 0b1111100) >> 2);
    switch(opcode) {
    case StandardOpCode::LUI:
    {
        auto [imm, rd] = decodeUType(inst);
        PLOGD << "LUI " << imm;
        PLOGD << "\t"  << rd;
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
        auto [imm, rd] = decodeUType(inst);
        PLOGD << "AUIPC " << imm;
        PLOGD << "\t"  << rd;
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
        auto [imm, rd] = decodeUType(inst);
        // **TODO** this is gross but I honestly don't understand this instruction format
        imm = ((inst >> (31 - 20)) & (1 << 20)) |
              ((inst >> (21 - 1)) & 0x7fe) |
              ((inst >> (20 - 11)) & (1 << 11)) |
              (inst & 0xff000);
        imm = (imm << 11) >> 11;

        PLOGD << "JAL " << rd << " " << imm;
        PLOGD << "\t"  << rd;
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
        auto [imm, rs1, funct3, rd] = decodeIType(inst);
        PLOGD << "JALR " << rs1 << " " << imm;
        PLOGD << "\t"  << rd;
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
        auto [imm, rs2, rs1, funct3] = decodeBType(inst);
        if (calcBranchCondition(inst, rs2, rs1, funct3)) {
            PLOGD << "\t" << (m_PC + imm);
            setNextPC((int32_t)(m_PC + imm));
        } 
        else {
            while(false);
            //false_counter++;
        }
        break;
    }

    case StandardOpCode::LOAD:
    {
        auto [imm, rs1, funct3, rd] = decodeIType(inst);
        const auto value = loadValue(inst, imm, rs1, funct3);
        PLOGD << "\t" << rd;
        if (rd != 0) {
            m_Reg[rd] = value;
        }
        break;
    }

    case StandardOpCode::STORE:
    {
        auto [imm, rs2, rs1, funct3] = decodeSType(inst);
        const uint32_t addr = m_Reg[rs1] + imm;
        const uint32_t val = m_Reg[rs2];
        switch(funct3) {
        case 0: // SB
        {
            PLOGD << "SB " << rs2 << " " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
            stats[15]++;
#endif
            m_ScalarDataMemory.write8(addr, val);
            break;
        }

        case 1: // SH
        {
            PLOGD << "SH " << rs2 << " " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
            dprintf(">>> SH\n");
            stats[16]++;
#endif
            m_ScalarDataMemory.write16(addr, val);
            break;
        }

        case 2: // SW
        {
            PLOGD << "SW " << rs2 << " " << rs1 << " " << imm;
#ifdef DEBUG_EXTRA
            stats[17]++;
#endif
            m_ScalarDataMemory.write32(addr, val);
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
        auto [imm, rs1, funct3, rd] = decodeIType(inst);
        const uint32_t val = calcOpImmResult(inst, imm, rs1, funct3);
        PLOGD << "\t" << rd;
        if (rd != 0) {
            m_Reg[rd] = val;
        }
        break;
    }

    case StandardOpCode::OP:
    {
        auto [funct7, rs2, rs1, funct3, rd] = decodeRType(inst);
        const uint32_t val = calcOpResult(inst, funct7, rs2, rs1, funct3);
        PLOGD << "\t" << rd;
        if (rd != 0) {
            m_Reg[rd] = val;
        }
        break;
    }

    case StandardOpCode::SYSTEM:
    {
        const uint32_t funct12 = inst >> 20;
        //const uint32_t rs1 = (inst >> 15) & 0x1f;
        const uint32_t funct3 = (inst >> 12) & 3;
        //const uint32_t rd = (inst >> 7) & 0x1f;
        
        switch(funct3) {

        /*case 1: //csrrw & csrrwi
#ifdef DEBUG_EXTRA
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
#endif
            if (csr_read(&val2, imm, TRUE)) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            val2 = (int32_t)val2;
            err = csr_write(imm, val);
            if (err < 0) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            if (rd != 0)
                reg[rd] = val2;
            if (err > 0) {
                //pc = pc + 4;
            }
            break;

        case 2: //csrrs & csrrsi 
        case 3: // csrrc & csrrci 
            if (csr_read(&val2, imm, (rs1 != 0))) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            val2 = (int32_t)val2;
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
                if (funct3 == 2)
                {
                    val = val2 | val;
                }
                else
                {
                    val = val2 & ~val;
                }
                err = csr_write(imm, val);
                if (err < 0) {
                    raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                    return;
                }
            }
            else
            {
                err = 0;
            }
            if (rd != 0)
                reg[rd] = val2;
            break;*/

        case 0:
        {
            switch(funct12) {
            case 0x000: // ecall 
            {
                PLOGD << "ECALL";
#ifdef DEBUG_EXTRA
                stats[39]++;
#endif
                if (inst & 0x000fff80) {
                    throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
                }
                else {
                    throw Exception(Exception::Cause::ECALL, inst);
                }
            }
            case 0x001: // EBREAK
            {
                PLOGD << "EBREAK";
#ifdef DEBUG_EXTRA
                stats[40]++;
#endif
                if (inst & 0x000fff80) {
                    throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
                }
                else {
                    breakPoint();
                }
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
        }
        
        default:
        {
            throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
        }
        }
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
    // Extract 2-bit quadrant
    const uint32_t quadrant = inst & 0b11;
    
    // If instruction is in standard quadrant
    if(quadrant == standardQuadrant) {
        executeStandardInstruction(inst); 
    }
    // Otherwise, if there is a co-processor defined to handle this quadrant
    else if(m_Coprocessors[quadrant]){
        m_Coprocessors[quadrant]->executeInstruction(inst, m_Reg, m_ScalarDataMemory);
    }
    // Otherwise, throw
    else {
        throw Exception(Exception::Cause::ILLEGAL_INSTRUCTION, inst);
    }
}
