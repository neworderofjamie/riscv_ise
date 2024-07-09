/*
    Copyright (C), 2023, MITSUNARI Shigeo
    Copyright (C), 2023, KNS Group LLC (YADRO)
    Licensed under the 3-Clause BSD License
    You may obtain a copy of the License at https://opensource.org/license/bsd-3-clause/
*/
void add(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 0, 0x0, rd, rs1, rs2); }
void sub(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 0, 0x20, rd, rs1, rs2); }
void sll(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 1, 0x0, rd, rs1, rs2); }
void slt(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 2, 0x0, rd, rs1, rs2); }
void sltu(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 3, 0x0, rd, rs1, rs2); }
void xor_(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 4, 0x0, rd, rs1, rs2); }
void srl(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 5, 0x0, rd, rs1, rs2); }
void sra(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 5, 0x20, rd, rs1, rs2); }
void or_(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 6, 0x0, rd, rs1, rs2); }
void and_(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 7, 0x0, rd, rs1, rs2); }
void mul(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 0, 0x1, rd, rs1, rs2); }
void mulh(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 1, 0x1, rd, rs1, rs2); }
void mulhsu(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 2, 0x1, rd, rs1, rs2); }
void mulhu(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 3, 0x1, rd, rs1, rs2); }
void div(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 4, 0x1, rd, rs1, rs2); }
void divu(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 5, 0x1, rd, rs1, rs2); }
void rem(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 6, 0x1, rd, rs1, rs2); }
void remu(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(0x33, 7, 0x1, rd, rs1, rs2); }
void addi(const Reg& rd, const Reg& rs1, int imm) { Itype(0x13, 0, rd, rs1, imm); }
void slti(const Reg& rd, const Reg& rs1, int imm) { Itype(0x13, 2, rd, rs1, imm); }
void sltiu(const Reg& rd, const Reg& rs1, int imm) { Itype(0x13, 3, rd, rs1, imm); }
void xori(const Reg& rd, const Reg& rs1, int imm) { Itype(0x13, 4, rd, rs1, imm); }
void ori(const Reg& rd, const Reg& rs1, int imm) { Itype(0x13, 6, rd, rs1, imm); }
void andi(const Reg& rd, const Reg& rs1, int imm) { Itype(0x13, 7, rd, rs1, imm); }
// load-op rd, imm(addr); rd = addr[imm];
void jalr(const Reg& rd, const Reg& addr, int imm = 0) { Itype(0x67, 0, rd, addr, imm); }
void lb(const Reg& rd, const Reg& addr, int imm = 0) { Itype(0x3, 0, rd, addr, imm); }
void lh(const Reg& rd, const Reg& addr, int imm = 0) { Itype(0x3, 1, rd, addr, imm); }
void lw(const Reg& rd, const Reg& addr, int imm = 0) { Itype(0x3, 2, rd, addr, imm); }
void lbu(const Reg& rd, const Reg& addr, int imm = 0) { Itype(0x3, 4, rd, addr, imm); }
void lhu(const Reg& rd, const Reg& addr, int imm = 0) { Itype(0x3, 5, rd, addr, imm); }
void lwu(const Reg& rd, const Reg& addr, int imm = 0) { Itype(0x3, 6, rd, addr, imm); }
void ld(const Reg& rd, const Reg& addr, int imm = 0) { Itype(0x3, 3, rd, addr, imm); }
void auipc(const Reg& rd, uint32_t imm) { Utype(0x17, rd, imm); }
void lui(const Reg& rd, uint32_t imm) { Utype(0x37, rd, imm); }
void slli(const Reg& rd, const Reg& rs1, uint32_t shamt) { opShift(0x0, 1, 0x13, rd, rs1, shamt); }
void srli(const Reg& rd, const Reg& rs1, uint32_t shamt) { opShift(0x0, 5, 0x13, rd, rs1, shamt); }
void srai(const Reg& rd, const Reg& rs1, uint32_t shamt) { opShift(0x20, 5, 0x13, rd, rs1, shamt); }
void fence_rw_rw() { append4B(0x330000f); }
void fence_tso() { append4B(0x8330000f); }
void fence_rw_w() { append4B(0x310000f); }
void fence_r_rw() { append4B(0x230000f); }
void fence_r_r() { append4B(0x220000f); }
void fence_w_w() { append4B(0x110000f); }
void fence_i() { append4B(0x100f); }
void ecall() { append4B(0x73); }
void ebreak() { append4B(0x00100073); }
// store-op rs, imm(addr) ; addr[imm] = rs;
void sb(const Reg& rs, const Reg& addr, int imm = 0) { Stype(0x23, 0, addr, rs, imm); }
void sh(const Reg& rs, const Reg& addr, int imm = 0) { Stype(0x23, 1, addr, rs, imm); }
void sw(const Reg& rs, const Reg& addr, int imm = 0) { Stype(0x23, 2, addr, rs, imm); }
void sd(const Reg& rs, const Reg& addr, int imm = 0) { Stype(0x23, 3, addr, rs, imm); }
void beq(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), 0x63, 0, rs1, rs2); opJmp(label, jmp); }
void bne(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), 0x63, 1, rs1, rs2); opJmp(label, jmp); }
void blt(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), 0x63, 4, rs1, rs2); opJmp(label, jmp); }
void bge(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), 0x63, 5, rs1, rs2); opJmp(label, jmp); }
void bltu(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), 0x63, 6, rs1, rs2); opJmp(label, jmp); }
void bgeu(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), 0x63, 7, rs1, rs2); opJmp(label, jmp); }
void beqz(const Reg& rs, const Label& label) { beq(rs, x0, label); }
void bnez(const Reg& rs, const Label& label) { bne(rs, x0, label); }
void blez(const Reg& rs, const Label& label) { bge(x0, rs, label); }
void bgez(const Reg& rs, const Label& label) { bge(rs, x0, label); }
void bltz(const Reg& rs, const Label& label) { blt(rs, x0, label); }
void bgtz(const Reg& rs, const Label& label) { blt(x0, rs, label); }
void bgt(const Reg& rs, const Reg& rt, const Label& label) { blt(rt, rs, label); }
void ble(const Reg& rs, const Reg& rt, const Label& label) { bge(rt, rs, label); }
void bgtu(const Reg& rs, const Reg& rt, const Label& label) { bltu(rt, rs, label); }
void bleu(const Reg& rs, const Reg& rt, const Label& label) { bgeu(rt, rs, label); }
void csrrw(const Reg& rd, CSR csr, const Reg& rs1) { opCSR(0x1073, csr, rs1, rd); }
void csrrs(const Reg& rd, CSR csr, const Reg& rs1) { opCSR(0x2073, csr, rs1, rd); }
void csrrc(const Reg& rd, CSR csr, const Reg& rs1) { opCSR(0x3073, csr, rs1, rd); }
void csrrwi(const Reg& rd, CSR csr, uint32_t imm) { opCSR(0x5073, csr, imm, rd); }
void csrrsi(const Reg& rd, CSR csr, uint32_t imm) { opCSR(0x6073, csr, imm, rd); }
void csrrci(const Reg& rd, CSR csr, uint32_t imm) { opCSR(0x7073, csr, imm, rd); }

void nop() { addi(x0, x0, 0); }
void li(const Reg& rd, int imm)
{
    int H, L;
    if (!local::split32bit(&H, &L, imm)) {
        addi(rd, x0, imm);
        return;
    }
    lui(rd, H);
#ifdef XBYAK_RISCV64
    addiw(rd, rd, L);
#else
    addi(rd, rd, L);
#endif
}
void mv(const Reg& rd, const Reg& rs) { addi(rd, rs, 0); }
void not_(const Reg& rd, const Reg& rs) { xori(rd, rs, -1); }
void neg(const Reg& rd, const Reg& rs) { sub(rd, x0, rs); }
void sext_b(const Reg& rd, const Reg& rs) { slli(rd, rs, 24); srai(rd, rd, 24); }
void sext_h(const Reg& rd, const Reg& rs) { slli(rd, rs, 16); srai(rd, rd, 16); }
void zext_b(const Reg& rd, const Reg& rs) { andi(rd, rs, 255); }
void zext_h(const Reg& rd, const Reg& rs) { slli(rd, rs, 16); srli(rd, rd, 16); }
void zext_w(const Reg& rd, const Reg& rs) { slli(rd, rs, 0); srli(rd, rd, 0); }
void seqz(const Reg& rd, const Reg& rs) { sltiu(rd, rs, 1); }
void snez(const Reg& rd, const Reg& rs) { sltu(rd, x0, rs); }
void sltz(const Reg& rd, const Reg& rs) { slt(rd, rs, x0); }
void sgtz(const Reg& rd, const Reg& rs) { slt(rd, x0, rs); }
void fence() { append4B(0x0ff0000f); }
void j_(const Label& label) { jal(x0, label); }
void jal(const Reg& rd, const Label& label) { Jmp jmp(getCurr(), 0x6f, rd); opJmp(label, jmp); }
void jr(const Reg& rs) { jalr(x0, rs, 0); }
void jalr(const Reg& rs) { jalr(x1, rs, 0); }
void ret() { jalr(x0, x1); }

void csrr(const Reg& rd, CSR csr) { csrrs(rd, csr, x0); }
void csrw(CSR csr, const Reg& rs) { csrrw(x0, csr, rs); }
void csrs(CSR csr, const Reg& rs) { csrrs(x0, csr, rs); }
void csrc(CSR csr, const Reg& rs) { csrrc(x0, csr, rs); }
void csrwi(CSR csr, uint32_t imm) { csrrwi(x0, csr, imm); }
void csrsi(CSR csr, uint32_t imm) { csrrsi(x0, csr, imm); }
void csrci(CSR csr, uint32_t imm) { csrrci(x0, csr, imm); }

// FeNN vector processor

// VSOP
// Rtype(Bit7 opcode, Bit3 funct3, Bit7 funct7, Bit5 rd, Bit5 rs1, Bit5 rs2)
void vadd(const VReg &rd, const VReg &rs1, const VReg &rs2){ Rtype(0x2, 0x0, 0x0, rd, rs1, rs2); }
void vsub(const VReg &rd, const VReg &rs1, const VReg &rs2){ Rtype(0x2, 0x2, 0x0, rd, rs1, rs2); }
void vmul(Bit4 shift, const VReg &rd, const VReg &rs1, const VReg &rs2){ Rtype(0x2, 0x4, shift.v, rd, rs1, rs2); }

// VTST
void vseq(const Reg &rd, const VReg &rs1, const VReg &rs2){ Rtype(0x2, 0x0, 0x2, rd, rs1, rs2); }
void vsni(const Reg &rd, const VReg &rs1, const VReg &rs2){ Rtype(0x2, 0x2, 0x2, rd, rs1, rs2); }
void vslt(const Reg &rd, const VReg &rs1, const VReg &rs2){ Rtype(0x2, 0x4, 0x2, rd, rs1, rs2); }
void vsge(const Reg &rd, const VReg &rs1, const VReg &rs2){ Rtype(0x2, 0x6, 0x2, rd, rs1, rs2); }

// VSEL
void vsel(const VReg &rd, const Reg &rs1, const VReg &rs2){ Rtype(0x2, 0x0, 0x4, rd, rs1, rs2); }

// VLUI
void vlui(const VReg& rd, uint32_t imm){ Utype(0x6, rd, imm); }

// VLOAD
void vloadv(const VReg &rd, const Reg &addr, int imm = 0){ Itype(0x12, 0x0, rd, addr, imm); }
void vloads(const VReg &rd, const Reg &addr, int imm = 0){ Itype(0x12, 0x4, rd, addr, imm); }

// VSTORE
void vstore(const VReg &rs, const Reg &addr, int imm = 0){ Stype(0x16, 0x0, addr, rs, imm); }
