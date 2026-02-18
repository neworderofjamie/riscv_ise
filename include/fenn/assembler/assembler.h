#pragma once
/*!
    @file xbyak_riscv.hpp
    @brief Xbyak_riscv ; JIT assembler for RISC-V
    @author herumi
    @url https://github.com/herumi/xbyak_riscv
    @note modified new BSD license
    http://opensource.org/licenses/BSD-3-Clause
*/

// Copyright (C), 2023, KNS Group LLC (YADRO)

// Standard C++ includes
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

// Standard C includes
#include <cassert>
#include <cstdint>

// Common includes
#include "common/enum.h"

// FeNN common includes
#include "fenn/common/isa.h"

// Assembler includes
#include "fenn/assembler/assembler_export.h"

//----------------------------------------------------------------------------
// FeNN::Assembler
//----------------------------------------------------------------------------
namespace FeNN::Assembler
{
// Forward declarations
class CodeGenerator;

BETTER_ENUM(AssemblerError, int, OFFSET_IS_TOO_BIG, IMM_IS_TOO_BIG, INVALID_IMM_OF_JAL,
            INVALID_IMM_OF_JALR, INVALID_IMM_OF_BRANCH, LABEL_IS_NOT_FOUND, LABEL_IS_REDEFINED)

using Label = std::shared_ptr<std::byte>;

//----------------------------------------------------------------------------
// Error
//----------------------------------------------------------------------------
class ASSEMBLER_EXPORT Error : public std::exception {
public:
    explicit Error(AssemblerError err);
    operator AssemblerError() const { return m_Err; }
    const char* what() const noexcept;

private:
    AssemblerError m_Err;
};

inline Label createLabel()
{
    return std::make_shared<std::byte>(std::byte{ 0 });
}

//----------------------------------------------------------------------------
// CodeGenerator
//----------------------------------------------------------------------------
class ASSEMBLER_EXPORT CodeGenerator
{
public:
    // constructor
    CodeGenerator()
    {
        reset();
    }

    CodeGenerator operator=(const CodeGenerator&) = delete;

    void L(Label label)
    {
        assert(label);
        const auto ret = m_LabelAddresses.try_emplace(label, getCurr());
        if (!ret.second) {
            throw Error(AssemblerError::LABEL_IS_REDEFINED);
        }
    }

    Label L()
    {
        auto label = createLabel();
        L(label);
        return label;
    }
    std::optional<uint32_t> getAddress(Label label) const
    {
        // If label isn't defined, it doesn't yet have an address
        const auto  i = m_LabelAddresses.find(label);
        if (i == m_LabelAddresses.end()) {
            return std::nullopt;
        }
        // Otherwise, return address
        else {
            return i->second;
        }
    }

    CodeGenerator& operator += (const CodeGenerator& other);

    std::vector<uint32_t> getCode() const;
    size_t getCodeSize() const{ return m_Code.size(); }

    void add(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 0, 0x0, rd, rs1, rs2); }
    void sub(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 0, 0x20, rd, rs1, rs2); }
    void sll(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 1, 0x0, rd, rs1, rs2); }
    void slt(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 2, 0x0, rd, rs1, rs2); }
    void sltu(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 3, 0x0, rd, rs1, rs2); }
    void xor_(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 4, 0x0, rd, rs1, rs2); }
    void srl(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 5, 0x0, rd, rs1, rs2); }
    void sra(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 5, 0x20, rd, rs1, rs2); }
    void or_(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 6, 0x0, rd, rs1, rs2); }
    void and_(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 7, 0x0, rd, rs1, rs2); }
    void mul(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 0, 0x1, rd, rs1, rs2); }
    void mulh(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 1, 0x1, rd, rs1, rs2); }
    void mulhsu(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 2, 0x1, rd, rs1, rs2); }
    void mulhu(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 3, 0x1, rd, rs1, rs2); }
    void div(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 4, 0x1, rd, rs1, rs2); }
    void divu(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 5, 0x1, rd, rs1, rs2); }
    void rem(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 6, 0x1, rd, rs1, rs2); }
    void remu(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 7, 0x1, rd, rs1, rs2); }
    void addi(Common::Reg rd, Common::Reg rs1, int imm) { Itype(Common::StandardOpCode::OP_IMM, 0, rd, rs1, imm); }
    void slti(Common::Reg rd, Common::Reg rs1, int imm) { Itype(Common::StandardOpCode::OP_IMM, 2, rd, rs1, imm); }
    void sltiu(Common::Reg rd, Common::Reg rs1, int imm) { Itype(Common::StandardOpCode::OP_IMM, 3, rd, rs1, imm); }
    void xori(Common::Reg rd, Common::Reg rs1, int imm) { Itype(Common::StandardOpCode::OP_IMM, 4, rd, rs1, imm); }
    void ori(Common::Reg rd, Common::Reg rs1, int imm) { Itype(Common::StandardOpCode::OP_IMM, 6, rd, rs1, imm); }
    void andi(Common::Reg rd, Common::Reg rs1, int imm) { Itype(Common::StandardOpCode::OP_IMM, 7, rd, rs1, imm); }
    // load-op rd, imm(addr); rd = addr[imm];
    void jalr(Common::Reg rd, Common::Reg addr, int imm = 0) { Itype(Common::StandardOpCode::JALR, 0, rd, addr, imm); }
    void lb(Common::Reg rd, Common::Reg addr, int imm = 0) { Itype(Common::StandardOpCode::LOAD, 0, rd, addr, imm); }
    void lh(Common::Reg rd, Common::Reg addr, int imm = 0) { Itype(Common::StandardOpCode::LOAD, 1, rd, addr, imm); }
    void lw(Common::Reg rd, Common::Reg addr, int imm = 0) { Itype(Common::StandardOpCode::LOAD, 2, rd, addr, imm); }
    void lbu(Common::Reg rd, Common::Reg addr, int imm = 0) { Itype(Common::StandardOpCode::LOAD, 4, rd, addr, imm); }
    void lhu(Common::Reg rd, Common::Reg addr, int imm = 0) { Itype(Common::StandardOpCode::LOAD, 5, rd, addr, imm); }
    void lwu(Common::Reg rd, Common::Reg addr, int imm = 0) { Itype(Common::StandardOpCode::LOAD, 6, rd, addr, imm); }
    void ld(Common::Reg rd, Common::Reg addr, int imm = 0) { Itype(Common::StandardOpCode::LOAD, 3, rd, addr, imm); }
    void auipc(Common::Reg rd, uint32_t imm) { Utype(Common::StandardOpCode::AUIPC, rd, imm); }
    void lui(Common::Reg rd, uint32_t imm) { Utype(Common::StandardOpCode::LUI, rd, imm); }
    void slli(Common::Reg rd, Common::Reg rs1, uint32_t shamt) { opShift(0x0, 1, Common::StandardOpCode::OP_IMM, rd, rs1, shamt); }
    void srli(Common::Reg rd, Common::Reg rs1, uint32_t shamt) { opShift(0x0, 5, Common::StandardOpCode::OP_IMM, rd, rs1, shamt); }
    void srai(Common::Reg rd, Common::Reg rs1, uint32_t shamt) { opShift(0x20, 5, Common::StandardOpCode::OP_IMM, rd, rs1, shamt); }
    void ecall() { append4B(0x73); }
    void ebreak() { append4B(0x00100073); }
    // store-op rs, imm(addr) ; addr[imm] = rs;
    void sb(Common::Reg rs, Common::Reg addr, int imm = 0) { Stype(Common::StandardOpCode::STORE, 0, addr, rs, imm); }
    void sh(Common::Reg rs, Common::Reg addr, int imm = 0) { Stype(Common::StandardOpCode::STORE, 1, addr, rs, imm); }
    void sw(Common::Reg rs, Common::Reg addr, int imm = 0) { Stype(Common::StandardOpCode::STORE, 2, addr, rs, imm); }
    void sd(Common::Reg rs, Common::Reg addr, int imm = 0) { Stype(Common::StandardOpCode::STORE, 3, addr, rs, imm); }
    void beq(Common::Reg rs1, Common::Reg rs2, const Label& label) { Jmp jmp(getCurr(), Common::StandardOpCode::BRANCH, 0, rs1, rs2); opJmp(label, jmp); }
    void bne(Common::Reg rs1, Common::Reg rs2, const Label& label) { Jmp jmp(getCurr(), Common::StandardOpCode::BRANCH, 1, rs1, rs2); opJmp(label, jmp); }
    void blt(Common::Reg rs1, Common::Reg rs2, const Label& label) { Jmp jmp(getCurr(), Common::StandardOpCode::BRANCH, 4, rs1, rs2); opJmp(label, jmp); }
    void bge(Common::Reg rs1, Common::Reg rs2, const Label& label) { Jmp jmp(getCurr(), Common::StandardOpCode::BRANCH, 5, rs1, rs2); opJmp(label, jmp); }
    void bltu(Common::Reg rs1, Common::Reg rs2, const Label& label) { Jmp jmp(getCurr(), Common::StandardOpCode::BRANCH, 6, rs1, rs2); opJmp(label, jmp); }
    void bgeu(Common::Reg rs1, Common::Reg rs2, const Label& label) { Jmp jmp(getCurr(), Common::StandardOpCode::BRANCH, 7, rs1, rs2); opJmp(label, jmp); }
    void beqz(Common::Reg rs, const Label& label) { beq(rs, Common::Reg::X0, label); }
    void bnez(Common::Reg rs, const Label& label) { bne(rs, Common::Reg::X0, label); }
    void blez(Common::Reg rs, const Label& label) { bge(Common::Reg::X0, rs, label); }
    void bgez(Common::Reg rs, const Label& label) { bge(rs, Common::Reg::X0, label); }
    void bltz(Common::Reg rs, const Label& label) { blt(rs, Common::Reg::X0, label); }
    void bgtz(Common::Reg rs, const Label& label) { blt(Common::Reg::X0, rs, label); }
    void bgt(Common::Reg rs, Common::Reg rt, const Label& label) { blt(rt, rs, label); }
    void ble(Common::Reg rs, Common::Reg rt, const Label& label) { bge(rt, rs, label); }
    void bgtu(Common::Reg rs, Common::Reg rt, const Label& label) { bltu(rt, rs, label); }
    void bleu(Common::Reg rs, Common::Reg rt, const Label& label) { bgeu(rt, rs, label); }
    void csrrw(Common::Reg rd, Common::CSR csr, Common::Reg rs1) { opCSR(0x1073, csr, rs1, rd); }
    void csrrs(Common::Reg rd, Common::CSR csr, Common::Reg rs1) { opCSR(0x2073, csr, rs1, rd); }
    void csrrc(Common::Reg rd, Common::CSR csr, Common::Reg rs1) { opCSR(0x3073, csr, rs1, rd); }
    void csrrwi(Common::Reg rd, Common::CSR csr, uint32_t imm) { opCSR(0x5073, csr, imm, rd); }
    void csrrsi(Common::Reg rd, Common::CSR csr, uint32_t imm) { opCSR(0x6073, csr, imm, rd); }
    void csrrci(Common::Reg rd, Common::CSR csr, uint32_t imm) { opCSR(0x7073, csr, imm, rd); }

    void nop() { addi(Common::Reg::X0, Common::Reg::X0, 0); }
    void li(Common::Reg rd, int imm);
    void mv(Common::Reg rd, Common::Reg rs) { addi(rd, rs, 0); }
    void not_(Common::Reg rd, Common::Reg rs) { xori(rd, rs, -1); }
    void neg(Common::Reg rd, Common::Reg rs) { sub(rd, Common::Reg::X0, rs); }
    void sext_b(Common::Reg rd, Common::Reg rs) { slli(rd, rs, 24); srai(rd, rd, 24); }
    void sext_h(Common::Reg rd, Common::Reg rs) { slli(rd, rs, 16); srai(rd, rd, 16); }
    void zext_b(Common::Reg rd, Common::Reg rs) { andi(rd, rs, 255); }
    void zext_h(Common::Reg rd, Common::Reg rs) { slli(rd, rs, 16); srli(rd, rd, 16); }
    void zext_w(Common::Reg rd, Common::Reg rs) { slli(rd, rs, 0); srli(rd, rd, 0); }
    void seqz(Common::Reg rd, Common::Reg rs) { sltiu(rd, rs, 1); }
    void snez(Common::Reg rd, Common::Reg rs) { sltu(rd, Common::Reg::X0, rs); }
    void sltz(Common::Reg rd, Common::Reg rs) { slt(rd, rs, Common::Reg::X0); }
    void sgtz(Common::Reg rd, Common::Reg rs) { slt(rd, Common::Reg::X0, rs); }
    void fence() { append4B(0x0ff0000f); }
    void j_(const Label& label) { jal(Common::Reg::X0, label); }
    void jal(Common::Reg rd, const Label& label) { Jmp jmp(getCurr(), rd); opJmp(label, jmp); }
    void jr(Common::Reg rs) { jalr(Common::Reg::X0, rs, 0); }
    void jalr(Common::Reg rs) { jalr(Common::Reg::X1, rs, 0); }
    void jalr(Common::Reg rd, Common::Reg rs, const Label& label) { Jmp jmp(getCurr(), rd, rs); opJmp(label, jmp); }
    void ret() { jalr(Common::Reg::X0, Common::Reg::X1); }

    void csrr(Common::Reg rd, Common::CSR csr) { csrrs(rd, csr, Common::Reg::X0); }
    void csrw(Common::CSR csr, Common::Reg rs) { csrrw(Common::Reg::X0, csr, rs); }
    void csrs(Common::CSR csr, Common::Reg rs) { csrrs(Common::Reg::X0, csr, rs); }
    void csrc(Common::CSR csr, Common::Reg rs) { csrrc(Common::Reg::X0, csr, rs); }
    void csrwi(Common::CSR csr, uint32_t imm) { csrrwi(Common::Reg::X0, csr, imm); }
    void csrsi(Common::CSR csr, uint32_t imm) { csrrsi(Common::Reg::X0, csr, imm); }
    void csrci(Common::CSR csr, uint32_t imm) { csrrci(Common::Reg::X0, csr, imm); }

    // Zbb instructions
    void clz(Common::Reg rd, Common::Reg rs) { opShift(0x30, 1, Common::StandardOpCode::OP_IMM, rd, rs, 0); }
    void ctz(Common::Reg rd, Common::Reg rs) { opShift(0x30, 1, Common::StandardOpCode::OP_IMM, rd, rs, 1); }
    void cpop(Common::Reg rd, Common::Reg rs) { opShift(0x30, 1, Common::StandardOpCode::OP_IMM, rd, rs, 2); }
    void sextb(Common::Reg rd, Common::Reg rs) { opShift(0x30, 1, Common::StandardOpCode::OP_IMM, rd, rs, 4); }
    void sexth(Common::Reg rd, Common::Reg rs) { opShift(0x30, 1, Common::StandardOpCode::OP_IMM, rd, rs, 5); }
    void max(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 0b110, 0b0000101, rd, rs1, rs2); }
    void maxu(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 0b111, 0b0000101, rd, rs1, rs2); }
    void min(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 0b100, 0b0000101, rd, rs1, rs2); }
    void minu(Common::Reg rd, Common::Reg rs1, Common::Reg rs2) { Rtype(Common::StandardOpCode::OP, 0b101, 0b0000101, rd, rs1, rs2); }

    // FeNN vector processor

    // VSOP
    void vadd(Common::VReg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSOP, 0b000, 0b0000000, rd, rs1, rs2); }
    void vadd_s(Common::VReg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSOP, 0b000, 0b1000000, rd, rs1, rs2); }
    void vsub(Common::VReg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSOP, 0b010, 0b0000000, rd, rs1, rs2); }
    void vsub_s(Common::VReg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSOP, 0b010, 0b1000000, rd, rs1, rs2); }
    void vand(Common::VReg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSOP, 0b011, 0b0000000, rd, rs1, rs2); }
    void vsll(Common::VReg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSOP, 0b001, 0b0000000, rd, rs1, rs2); }
    void vsra(Common::VReg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSOP, 0b101, 0b0000000, rd, rs1, rs2); }
    void vmul(Common::Bit<4> shift, Common::VReg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSOP, 0b100, shift, rd, rs1, rs2); }
    void vmul_rn(Common::Bit<4> shift, Common::VReg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSOP, 0b100, shift | 0b0010000, rd, rs1, rs2); }
    void vmul_rs(Common::Bit<4> shift, Common::VReg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSOP, 0b100, shift | 0b0100000, rd, rs1, rs2); }
    void vmul_s(Common::Bit<4> shift, Common::VReg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSOP, 0b100, shift | 0b1000000, rd, rs1, rs2); }
    void vmul_s_rn(Common::Bit<4> shift, Common::VReg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSOP, 0b100, shift | 0b1010000, rd, rs1, rs2); }
    void vmul_s_rs(Common::Bit<4> shift, Common::VReg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSOP, 0b100, shift | 0b1100000, rd, rs1, rs2); }

    // VSOP-IMM
    void vslli(Common::Bit<4> shift, Common::VReg rd, Common::VReg rs1) { Itype(Common::VectorOpCode::VSOP_IMM, 0b000, rd, rs1, shift); }
    void vsrai(Common::Bit<4> shift, Common::VReg rd, Common::VReg rs1) { Itype(Common::VectorOpCode::VSOP_IMM, 0b001, rd, rs1, shift); }
    void vsrai_rn(Common::Bit<4> shift, Common::VReg rd, Common::VReg rs1) { Itype(Common::VectorOpCode::VSOP_IMM, 0b001, rd, rs1, shift | 0b000000010000); }
    void vsrai_rs(Common::Bit<4> shift, Common::VReg rd, Common::VReg rs1) { Itype(Common::VectorOpCode::VSOP_IMM, 0b001, rd, rs1, shift | 0b000000100000); }

    // VTST
    void vteq(Common::Reg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VTST, 0b000, 0x0, rd, rs1, rs2); }
    void vtne(Common::Reg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VTST, 0b010, 0x0, rd, rs1, rs2); }
    void vtlt(Common::Reg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VTST, 0b100, 0x0, rd, rs1, rs2); }
    void vtge(Common::Reg rd, Common::VReg rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VTST, 0b110, 0x0, rd, rs1, rs2); }

    // VSEL
    void vsel(Common::VReg rd, const Common::Reg& rs1, Common::VReg rs2) { Rtype(Common::VectorOpCode::VSEL, 0b000, 0b0000000, rd, rs1, rs2); }

    // VLUI
    void vlui(Common::VReg rd, uint32_t imm) { Utype(Common::VectorOpCode::VLUI, rd, imm); }

    // VMOV
    void vfill(Common::VReg rd, Common::Reg rs) { Itype(Common::VectorOpCode::VMOV, 0b000, rd, rs, 0); }
    void vextract(Common::Reg rd, Common::VReg rs, Common::Bit<5> lane) { Itype(Common::VectorOpCode::VMOV, 0b001, rd, rs, lane); }

    // VSPC
    void vrng(Common::VReg rd) { Rtype(Common::VectorOpCode::VSPC, 0b000, 0x0, rd, 0, 0); }
    void vandadd(Common::Bit<4> shift, Common::VReg rd, Common::VReg rs1, Common::Reg rs2) { Rtype(Common::VectorOpCode::VSPC, 0b001, shift, rd, rs1, rs2); }

    // VLOAD
    void vloadv(Common::VReg rd, Common::Reg addr, int imm = 0) { Itype(Common::VectorOpCode::VLOAD, 0b000, rd, addr, imm); }
    void vloadl(Common::VReg rd, Common::VReg addr, int imm = 0) { Itype(Common::VectorOpCode::VLOAD, 0b010, rd, addr, imm); }
    void vloadr0(Common::Reg addr, int imm = 0) { Itype(Common::VectorOpCode::VLOAD, 0b001, 0, addr, imm); }
    void vloadr1(Common::Reg addr, int imm = 0) { Itype(Common::VectorOpCode::VLOAD, 0b101, 0, addr, imm); }

    // VSTORE
    void vstore(Common::VReg rs, Common::Reg addr, int imm = 0) { Stype(Common::VectorOpCode::VSTORE, 0b000, addr, rs, imm); }
    void vstorel(Common::VReg rs, Common::VReg addr, int imm = 0) { Stype(Common::VectorOpCode::VSTORE, 0b010, addr, rs, imm); }

private:
    //----------------------------------------------------------------------------
    // Jmp
    //----------------------------------------------------------------------------
    class Jmp
    {
        enum class Type
        {
            JAL,
            JALR,
            BRANCH,
        };

    public:
        // JAL
        Jmp(uint32_t from, Common::Reg rd)
        :   m_Type(Type::JAL), m_From(from), m_Encoded((static_cast<uint32_t>(rd) << 7) | addQuadrant(Common::StandardOpCode::JAL))
        {
        }

        // JALR
        Jmp(uint32_t from, Common::Reg rd, Common::Reg src)
        :   m_Type(Type::JALR), m_From(from),
            m_Encoded((static_cast<uint32_t>(src) << 15) | (static_cast<uint32_t>(rd) << 7) | addQuadrant(Common::StandardOpCode::JALR))
        {
        }

        // BRANCH
        Jmp(uint32_t from, Common::Bit<7> opcode, uint32_t funct3, Common::Reg src1, Common::Reg src2)
        :   m_Type(Type::BRANCH), m_From(from),
            m_Encoded((static_cast<uint32_t>(src2) << 20) | (static_cast<uint32_t>(src1) << 15) | (funct3 << 12) | opcode)
        {
        }

        Jmp(const Jmp& other, uint32_t offset)
        :   m_Type(other.m_Type), m_From(other.m_From + offset), m_Encoded(other.m_Encoded)
        {
        }

        uint32_t encode(uint32_t addr) const;
        uint32_t getFrom() const { return m_From; };

    private:
        // Address of the jmp mnemonic
        Type m_Type;
        uint32_t m_From;
        uint32_t m_Encoded;
    };

    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    void reset()
    {
        m_LabelAddresses.clear();
        m_LabelJumps.clear();
    }

    void append4B(uint32_t code) { m_Code.push_back(code); }


    // **TODO**  add code base address
    uint32_t getCurr() const { return static_cast<uint32_t>(m_Code.size()) * 4; }

    void opJmp(Label label, const Jmp& jmp)
    {
        // Append placeholder and add jump to map
        append4B(0);
        m_LabelJumps.emplace(label, jmp);
    }
    uint32_t enc2(uint32_t a, uint32_t b) const { return (a << 7) | (b << 15); }
    uint32_t enc3(uint32_t a, uint32_t b, uint32_t c) const { return enc2(a, b) | (c << 20); }

    void Rtype(Common::Bit<7> opcode, Common::Bit<3> funct3, Common::Bit<7> funct7, 
               Common::Bit<5> rd, Common::Bit<5> rs1, Common::Bit<5> rs2)
    {
        uint32_t v = (funct7 << 25) | (funct3 << 12) | opcode | enc3(rd, rs1, rs2);
        append4B(v);
    }
    void Itype(Common::Bit<7> opcode,Common:: Bit<3> funct3, 
               Common::Bit<5> rd, Common::Bit<5> rs1, int imm)
    {
        if (!Common::inSBit(imm, 12)) {
            throw Error(AssemblerError::IMM_IS_TOO_BIG);
        }
        uint32_t v = (imm << 20) | (funct3 << 12) | opcode | enc2(rd, rs1);
        append4B(v);
    }
    void Stype(Common::Bit<7> opcode, Common::Bit<3> funct3,
               Common::Bit<5> rs1, Common::Bit<5> rs2, int imm)
    {
        if (!Common::inSBit(imm, 12)) {
            throw Error(AssemblerError::IMM_IS_TOO_BIG);
        }
        uint32_t v = ((imm >> 5) << 25) | (funct3 << 12) | opcode | enc3(imm & Common::mask(5), rs1, rs2);
        append4B(v);
    }
    void Utype(Common::Bit<7> opcode, Common::Bit<5> rd, uint32_t imm)
    {
        if (imm >= (1u << 20)) {
            throw Error(AssemblerError::IMM_IS_TOO_BIG);
        }
        uint32_t v = (imm << 12) | opcode | (rd << 7);
        append4B(v);
    }
    void opShift(Common::Bit<7> pre, Common::Bit<3> funct3, Common::Bit<7> opcode, 
                 Common::Bit<5> rd, Common::Bit<5> rs1, uint32_t shamt, int range = 0)
    {
        if (range == 0) {
            range = 5;
        }
        if (shamt >= (1u << range)) {
            throw Error(AssemblerError::IMM_IS_TOO_BIG);
        }
        uint32_t v = (pre << 25) | (funct3 << 12) | opcode | enc3(rd, rs1, shamt);
        append4B(v);
    }

    void opCSR(Common::Bit<32> baseValue, Common::Bit<12> csr, Common::Bit<5> rs1_uimm, Common::Bit<5> rd)
    {
        /*
            31 .. 20 | 19 .. 15 | 14 .. 12 | 11 .. 7 | 6 .. 0
               csr     rs1_uimm     func3       rd     opcode

            func3 and opcode must be encoded in the baseValue
        */
        uint32_t v = (csr << 20) | (rs1_uimm << 15) | (rd << 7);
        v |= baseValue; // force-encode base value
        append4B(v);
    }

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<uint32_t> m_Code;
    std::unordered_map<Label, uint32_t> m_LabelAddresses;
    std::unordered_multimap<Label, Jmp> m_LabelJumps;
};
}   // namespace FeNN::Assembler