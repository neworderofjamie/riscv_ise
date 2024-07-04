#include "riscv.h"

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>

namespace
{
// funct7 rs2 rs1 funct3 rd
std::tuple<uint32_t, uint32_t, uint32_t, uint32_t, uint32_t> decodeRType(uint32_t inst)
{
    const uint32_t funct7 = 
    const uint32_t rs2 = (inst >> 20) & 0x1f;
    const uint32_t rs1 = (inst >> 15) & 0x1f;
    const uint32_t funct3 = (inst >> 12) & 7;
    const uint32_t rd = (inst >> 7) & 0x1f;
    return std::make_tuple(funct7, rs2, rs1, funct1, rd);
}

// imm[11:0] rs1 funct3 rd
std::tuple<int32_t, uint32_t, uint32_t, uint32_t> decodeIType(uint32_t inst)
{
    const int32_t imm = (int32_t)(inst >> 20);
    const uint32_t rs1 = (inst >> 15) & 0x1f;
    const uint32_t funct3 = (inst >> 12) & 7;
    const uint32_t rd = (inst >> 7) & 0x1f;
    return std::make_tuple(imm, rs1, funct3, rd);
}

// imm[12] imm[10:5] rs2 rs1 funct3 imm[4:1] imm[11]
std::tuple<int32_t, uint32_t, uint32_t, uint32_t> decodeBType(uint32_t inst)
{
    int32_t imm = ((inst >> (31 - 12)) & (1 << 12)) |
                  ((inst >> (25 - 5)) & 0x7e0) |
                  ((inst >> (8 - 1)) & 0x1e) |
                  ((inst << (11 - 7)) & (1 << 11));
    imm = (imm << 19) >> 19;

    const uint32_t rs2 = (inst >> 20) & 0x1f;
    const uint32_t rs1 = (inst >> 15) & 0x1f;
    const uint32_t funct3 = (inst >> 12) & 7;
    return std::make_tuple(imm, rs2, rs1, funct3);
}

// imm[31:12] rd
std::tuple<int32_t, uint32_t> decodeUType(uint32_t inst)
{
    const int32_t imm =  (int32_t)(inst & 0xfffff000);
    const uint32_t rd = (inst >> 7) & 0x1f;
    return std::make_tuple(imm, rd);
}

}
 uint32_t RISCV::getMStatus(uint32_t mask) const
{
    int sd;
    uint32_t val = m_MStatus | (m_FS << MStatusFSShift);
    val &= mask;
    sd = ((val & MStatusFS) == MStatusFS) |
            ((val & MStatusXS) == MStatusXS);
    if (sd) {
        val |= (uint32_t)1 << 31;
    }
    return val;
}

void RISCV::setMStatus(uint32_t val)
{
    m_FS = (val >> MStatusFSShift) & 3;

    const uint32_t mask = MStatusMask & ~MStatusFS;
    m_MStatus = (m_MStatus & ~mask) | (val & mask);
}

bool RISCV::calcBranchCondition(uint32_t rs2, uint32_t rs1, uint32_t funct3) const
{
    /*void beq(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), 0x63, 0, rs1, rs2); opJmp(label, jmp); }
void bne(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), 0x63, 1, rs1, rs2); opJmp(label, jmp); }
void blt(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), 0x63, 4, rs1, rs2); opJmp(label, jmp); }
void bge(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), 0x63, 5, rs1, rs2); opJmp(label, jmp); }
void bltu(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), 0x63, 6, rs1, rs2); opJmp(label, jmp); }
void bgeu(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), 0x63, 7,*/
    switch(funct3) {
    case 0: // BEQ
    {
#ifdef DEBUG_EXTRA
            dprintf(">>> BEQ\n");
            stats[4]++;
#endif
        return (m_Reg[rs1] == m_Reg[rs2]);
    }
    case 1: // BNE
    {
#ifdef DEBUG_EXTRA
            dprintf(">>> BNE\n");
            stats[5]++;
#endif
        return (m_Reg[rs1] != m_Reg[rs2]);
    }
    
    case 2: /* blt/bge */
    {
#ifdef DEBUG_EXTRA
        if(!(funct3 & 1)) {
            dprintf(">>> BLT\n");
            stats[6]++;
        } else  {
            dprintf(">>> BGE\n");
            stats[7]++;
        }
#endif
        cond = ((int32_t)reg[rs1] < (int32_t)reg[rs2]);
        break;
    }
    case 3: /* bltu/bgeu */
    {
#ifdef DEBUG_EXTRA
        if(!(funct3 & 1)) {
            dprintf(">>> BLTU\n");
            stats[8]++;
        } else  {
            dprintf(">>> BGEU\n");
            stats[9]++;
        }
#endif
        cond = (reg[rs1] < reg[rs2]);
        break;
    }
    default:
    {
        raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
        return;
    }
    }
    cond ^= (funct3 & 1);
}
void RISCV::executeStandardInstruction(StandardOpCode opcode, uint32_t inst)
{
    
    switch(opcode) {

    case StandardOpCode::LUI:
    {
#ifdef DEBUG_EXTRA
        dprintf(">>> LUI\n");
        stats[0]++;
#endif
        auto [imm, rd] = decodeUType(inst);
        if (rd != 0) {
            m_Reg[rd] = imm;
        }
        break;
    }

    case StandardOpCode::AUIPC:
    {
#ifdef DEBUG_EXTRA
        dprintf(">>> AUIPC\n");
        stats[1]++;
#endif
        auto [imm, rd] = decodeUType(inst);
        if (rd != 0) {
            m_Reg[rd] = (int32_t)(m_PC + imm);
        }
        break;
    }

    case StandardOpCode::JAL:
    {
#ifdef DEBUG_EXTRA
        dprintf(">>> JAL\n");
        stats[2]++;
#endif
        auto [imm, rs1, funct3, rd] = decodeIType(inst);

        /*imm = ((insn >> (31 - 20)) & (1 << 20)) |
              ((insn >> (21 - 1)) & 0x7fe) |
              ((insn >> (20 - 11)) & (1 << 11)) |
             (insn & 0xff000);
        imm = (imm << 11) >> 11;*/
        if (rd != 0) {
            m_Reg[rd] = m_PC + 4;
        }
        m_NextPC = (int32_t)(m_PC + imm);
        /*if(m_NextPC > m_PC) {
            forward_counter++;
        }
        else {
            backward_counter++;
        }
        jump_counter++;*/
        break;
    }

    case StandardOpCode::JALR:
    {
#ifdef DEBUG_EXTRA
        dprintf(">>> JALR\n");
        stats[3]++;
#endif
        auto [imm, rs1, funct3, rd] = decodeIType(inst);
        const uint32_t val  = m_PC + 4;
        m_NextPC = (int32_t)(m_Reg[rs1] + imm) & ~1;
        if (rd != 0) {
            m_Reg[rd] = val;
        }
        /*if(next_pc > pc) forward_counter++;
        else backward_counter++;
        jump_counter++;*/
        break;

    case StandardOpCode::BRANCH:
    {
        auto [imm, rs2, rs1, funct3] = decodeBType(inst);
        const bool cond = calcBranchCondition(rs2, rs1, funct3);
        if (cond) {
            m_NextPC = (int32_t)(m_PC + imm);
            /*if(next_pc > pc) forward_counter++;
            else backward_counter++;
            jump_counter++;
            true_counter++;*/
        } 
        else {
            while(false);
            //false_counter++;
        }
        break;
    }

    case StandardOpCode::LOAD:
    {
        funct3 = (insn >> 12) & 7;
        imm = (int32_t)insn >> 20;
        addr = reg[rs1] + imm;
        switch(funct3) {

        case 0: /* lb */
        {
#ifdef DEBUG_EXTRA
            dprintf(">>> LB\n");
            stats[10]++;
#endif
            uint8_t rval;
            if (target_read_u8(&rval, addr)) {
                raise_exception(pending_exception, pending_tval);
                return;
            }
            val = (int8_t)rval;
        }
        break;

        case 1: /* lh */
        {
#ifdef DEBUG_EXTRA
            dprintf(">>> LH\n");
            stats[11]++;
#endif
            uint16_t rval;
            if (target_read_u16(&rval, addr)) {
                raise_exception(pending_exception, pending_tval);
                return;
            }
            val = (int16_t)rval;
        }
        break;

        case 2: /* lw */
        {
#ifdef DEBUG_EXTRA
            dprintf(">>> LW\n");
            stats[12]++;
#endif
            uint32_t rval;
            if (target_read_u32(&rval, addr)) {
                raise_exception(pending_exception, pending_tval);
                return;
            }
            val = (int32_t)rval;
        }
        break;

        case 4: /* lbu */
        {
#ifdef DEBUG_EXTRA
            dprintf(">>> LBU\n");
            stats[13]++;
#endif
            uint8_t rval;
            if (target_read_u8(&rval, addr)) {
                raise_exception(pending_exception, pending_tval);
                return;
            }
            val = rval;
        }
        break;

        case 5: /* lhu */
        {
#ifdef DEBUG_EXTRA
            dprintf(">>> LHU\n");
            stats[14]++;
#endif
            uint16_t rval;
            if (target_read_u16(&rval, addr)) {
                raise_exception(pending_exception, pending_tval);
                return;
            }
            val = rval;
        }
        break;

        default:
            raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
            return;
        }
        if (rd != 0)
            reg[rd] = val;
        break;
    }

    case StandardOpCode::STORE:
    {
        funct3 = (insn >> 12) & 7;
        imm = rd | ((insn >> (25 - 5)) & 0xfe0);
        imm = (imm << 20) >> 20;
        addr = reg[rs1] + imm;
        val = reg[rs2];
        switch(funct3) {

        case 0: /* sb */
#ifdef DEBUG_EXTRA
            dprintf(">>> SB\n");
            stats[15]++;
#endif
            if (target_write_u8(addr, val)) {
                raise_exception(pending_exception, pending_tval);
                return;
            }
            break;

        case 1: /* sh */
#ifdef DEBUG_EXTRA
            dprintf(">>> SH\n");
            stats[16]++;
#endif
            if (target_write_u16(addr, val)) {
                raise_exception(pending_exception, pending_tval);
                return;
            }
            break;

        case 2: /* sw */
#ifdef DEBUG_EXTRA
            dprintf(">>> SW\n");
            stats[17]++;
#endif
            if (target_write_u32(addr, val)) {
                raise_exception(pending_exception, pending_tval);
                return;
            }
            break;

        default:
            raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
            return;
        }
        break;

    case 0x13: /* OP-IMM */

        funct3 = (insn >> 12) & 7;
        imm = (int32_t)insn >> 20;
        switch(funct3) {
        case 0: /* addi */
#ifdef DEBUG_EXTRA
            dprintf(">>> ADDI\n");
            stats[18]++;
            if(rs1==0) stats[47]++; /* li */
#endif
            val = (int32_t)(reg[rs1] + imm);
            break;
        case 1: /* slli */
#ifdef DEBUG_EXTRA
            dprintf(">>> SLLI\n");
            stats[24]++;
#endif
            if ((imm & ~(XLEN - 1)) != 0) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            val = (int32_t)(reg[rs1] << (imm & (XLEN - 1)));
            break;
        case 2: /* slti */
#ifdef DEBUG_EXTRA
            dprintf(">>> SLTI\n");
            stats[19]++;
#endif
            val = (int32_t)reg[rs1] < (int32_t)imm;
            break;
        case 3: /* sltiu */
#ifdef DEBUG_EXTRA
            dprintf(">>> SLTIU\n");
            stats[20]++;
#endif
            val = reg[rs1] < (uint32_t)imm;
            break;
        case 4: /* xori */
#ifdef DEBUG_EXTRA
            dprintf(">>> XORI\n");
            stats[21]++;
#endif
            val = reg[rs1] ^ imm;
            break;
        case 5: /* srli/srai */
            if ((imm & ~((XLEN - 1) | 0x400)) != 0) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            if (imm & 0x400)
            {
#ifdef DEBUG_EXTRA
                dprintf(">>> SRAI\n");
                stats[26]++;
#endif
                val = (int32_t)reg[rs1] >> (imm & (XLEN - 1));
            }
            else
            {
#ifdef DEBUG_EXTRA
                dprintf(">>> SRLI\n");
                stats[25]++;
#endif
                val = (int32_t)((uint32_t)reg[rs1] >> (imm & (XLEN - 1)));
            }
            break;
        case 6: /* ori */
#ifdef DEBUG_EXTRA
            dprintf(">>> ORI\n");
            stats[22]++;
#endif
            val = reg[rs1] | imm;
            break;
        case 7: /* andi */
#ifdef DEBUG_EXTRA
            dprintf(">>> ANDI\n");
            stats[23]++;
#endif
            val = reg[rs1] & imm;
            break;
        }
        if (rd != 0)
            reg[rd] = val;
        break;

    case 0x33: /* OP */

        imm = insn >> 25;
        val = reg[rs1];
        val2 = reg[rs2];
#ifndef STRICT_RV32I
        if (imm == 1) {
            funct3 = (insn >> 12) & 7;
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
            if (imm & ~0x20) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            funct3 = ((insn >> 12) & 7) | ((insn >> (30 - 3)) & (1 << 3));
            switch(funct3) {
            case 0: /* add */
#ifdef DEBUG_EXTRA
                dprintf(">>> ADD\n");
                stats[27]++;
#endif
                val = (int32_t)(val + val2);
                break;
            case 0 | 8: /* sub */
#ifdef DEBUG_EXTRA
                dprintf(">>> SUB\n");
                stats[28]++;
#endif
                val = (int32_t)(val - val2);
                break;
            case 1: /* sll */
#ifdef DEBUG_EXTRA
                dprintf(">>> SLL\n");
                stats[29]++;
#endif
                val = (int32_t)(val << (val2 & (XLEN - 1)));
                break;
            case 2: /* slt */
#ifdef DEBUG_EXTRA
                dprintf(">>> SLT\n");
                stats[30]++;
#endif
                val = (int32_t)val < (int32_t)val2;
                break;
            case 3: /* sltu */
#ifdef DEBUG_EXTRA
                dprintf(">>> SLTU\n");
                stats[31]++;
#endif
                val = val < val2;
                break;
            case 4: /* xor */
#ifdef DEBUG_EXTRA
                dprintf(">>> XOR\n");
                stats[32]++;
#endif
                val = val ^ val2;
                break;
            case 5: /* srl */
#ifdef DEBUG_EXTRA
                dprintf(">>> SRL\n");
                stats[33]++;
#endif
                val = (int32_t)((uint32_t)val >> (val2 & (XLEN - 1)));
                break;
            case 5 | 8: /* sra */
#ifdef DEBUG_EXTRA
                dprintf(">>> SRA\n");
                stats[34]++;
#endif
                val = (int32_t)val >> (val2 & (XLEN - 1));
                break;
            case 6: /* or */
#ifdef DEBUG_EXTRA
                dprintf(">>> OR\n");
                stats[35]++;
#endif
                val = val | val2;
                break;
            case 7: /* and */
#ifdef DEBUG_EXTRA
                dprintf(">>> AND\n");
                stats[36]++;
#endif
                val = val & val2;
                break;
            default:
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
        }
        if (rd != 0)
            reg[rd] = val;
        break;

    case 0x73: /* SYSTEM */

        funct3 = (insn >> 12) & 7;
        imm = insn >> 20;
        if (funct3 & 4)
            val = rs1;
        else
            val = reg[rs1];
        funct3 &= 3;
        switch(funct3) {

        case 1: /* csrrw & csrrwi */
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
                /* pc = pc + 4; */
            }
            break;

        case 2: /* csrrs & csrrsi */
        case 3: /* csrrc & csrrci */
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
            break;

        case 0:
            switch(imm) {
            case 0x000: /* ecall */
#ifdef DEBUG_EXTRA
                dprintf(">>> ECALL\n");
                stats[39]++;
#endif
                if (insn & 0x000fff80) {
                    raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                    return;
                }
                /*
                    compliance test specific: if bit 0 of gp (x3) is 0, it is a syscall,
                    otherwise it is the program end, with the exit code in the bits 31:1
                */
                if (begin_signature) {
                    if (reg[3] & 1) {
#ifdef DEBUG_OUTPUT
                        printf("program end, result: %04x\n", reg[3] >> 1);
#endif
                        machine_running = FALSE;
                        return;

                    } else {
#ifdef DEBUG_OUTPUT
                        printf("syscall: %04x\n", reg[3]);
#endif
                        raise_exception(CAUSE_USER_ECALL + priv, 0);
                    }
                } else {
                    /* on real hardware, an exception is raised, the I-ECALL-01 compliance test tests this as well */
                    raise_exception(CAUSE_USER_ECALL + priv, 0);
                    return;
                }
                break;

            case 0x001: /* ebreak */
#ifdef DEBUG_EXTRA
                dprintf(">>> EBREAK\n");
                stats[40]++;
#endif
                if (insn & 0x000fff80) {
                    raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                    return;
                }
                raise_exception(CAUSE_BREAKPOINT, 0);
                return;

            case 0x102: /* sret */
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

            case 0x105: /* wfi */
#ifdef DEBUG_EXTRA
                dprintf(">>> WFI\n");
                stats[61]++;
#endif
                /* wait for interrupt: it is allowed to execute it as nop */
                break;

            case 0x302: /* mret */
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
            break;

            default:
                if ((imm >> 5) == 0x09) {
#ifdef DEBUG_EXTRA
                    dprintf(">>> SFENCE.VMA\n");
                    stats[62]++;
#endif
                    /* sfence.vma */
                    if ((insn & 0x00007f80) || (priv == PRV_U)) {
                        raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                        return;
                    }
                } else {
                    raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                    return;
                }
                break;
            }
            break;

        default:
            raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
            return;
        }
        break;

    case 0x0f: /* MISC-MEM */

        funct3 = (insn >> 12) & 7;
        switch(funct3) {

        case 0: /* fence */
#ifdef DEBUG_EXTRA
            dprintf(">>> FENCE\n");
            stats[37]++;
#endif
            if (insn & 0xf00fff80) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            break;

        case 1: /* fence.i */
#ifdef DEBUG_EXTRA
            dprintf(">>> FENCE.I\n");
            stats[38]++;
#endif
            if (insn != 0x0000100f) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            break;

        default:
            raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
            return;
        }
        break;

#ifndef STRICT_RV32I

    case 0x2f: /* AMO */

        funct3 = (insn >> 12) & 7;
        switch(funct3) {
        case 2:
        {
            uint32_t rval;

            addr = reg[rs1];
            funct3 = insn >> 27;
            switch(funct3) {

            case 2: /* lr.w */
#ifdef DEBUG_EXTRA
                dprintf(">>> LR.W\n");
                stats[56]++;
#endif
                if (rs2 != 0) {
                    raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                    return;
                }
                if (target_read_u32(&rval, addr)) {
                    raise_exception(pending_exception, pending_tval);
                    return;
                }
                val = (int32_t)rval;
                load_res = addr;
                break;

            case 3: /* sc.w */
#ifdef DEBUG_EXTRA
                dprintf(">>> SC.W\n");
                stats[57]++;
#endif
                if (load_res == addr) {
                    if (target_write_u32(addr, reg[rs2])) {
                        raise_exception(pending_exception, pending_tval);
                        return;
                    }
                    val = 0;
                } else {
                    val = 1;
                }
                break;

            case 1: /* amiswap.w */
            case 0: /* amoadd.w */
            case 4: /* amoxor.w */
            case 0xc: /* amoand.w */
            case 0x8: /* amoor.w */
            case 0x10: /* amomin.w */
            case 0x14: /* amomax.w */
            case 0x18: /* amominu.w */
            case 0x1c: /* amomaxu.w */

#ifdef DEBUG_EXTRA
                dprintf(">>> AM...\n");
                stats[63]++;
#endif
                if (target_read_u32(&rval, addr)) {
                    raise_exception(pending_exception, pending_tval);
                    return;
                }
                val = (int32_t)rval;
                val2 = reg[rs2];
                switch(funct3) {
                case 1: /* amiswap.w */
                    break;
                case 0: /* amoadd.w */
                    val2 = (int32_t)(val + val2);
                    break;
                case 4: /* amoxor.w */
                    val2 = (int32_t)(val ^ val2);
                    break;
                case 0xc: /* amoand.w */
                    val2 = (int32_t)(val & val2);
                    break;
                case 0x8: /* amoor.w */
                    val2 = (int32_t)(val | val2);
                    break;
                case 0x10: /* amomin.w */
                    if ((int32_t)val < (int32_t)val2)
                        val2 = (int32_t)val;
                    break;
                case 0x14: /* amomax.w */
                    if ((int32_t)val > (int32_t)val2)
                        val2 = (int32_t)val;
                    break;
                case 0x18: /* amominu.w */
                    if ((uint32_t)val < (uint32_t)val2)
                        val2 = (int32_t)val;
                    break;
                case 0x1c: /* amomaxu.w */
                    if ((uint32_t)val > (uint32_t)val2)
                        val2 = (int32_t)val;
                    break;
                default:
                    raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                    return;
                }
                if (target_write_u32(addr, val2)) {
                    raise_exception(pending_exception, pending_tval);
                    return;
                }
                break;
            default:
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
        }
        break;
        default:
            raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
            return;
        }
        if (rd != 0)
            reg[rd] = val;
        break;

#endif

    default:
        raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
        return;
    }
}
void RISCV::executeInstruction(uint32_t inst)
{
    // Extract 2-bit quadrant and opcode
    const uint32_t quadrant = inst & 0b11;
    const uint32_t opcode = (inst & 0b1111100) >> 2;
    
    if(quadrant == 0b11) {
        executeStandardInstruction(static_cast<StandardOpCode>(opcode), inst); 
    }


}