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
#include <iostream>
#include <iomanip>
#include <list>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

// Standard C includes
#include <cassert>
#include <cstdint>

// RISC-V common includes
#include "common/isa.h"

namespace Xbyak_riscv {
enum {
    ERR_NONE,
    ERR_OFFSET_IS_TOO_BIG,
    ERR_CODE_IS_TOO_BIG,
    ERR_IMM_IS_TOO_BIG,
    ERR_INVALID_IMM_OF_JAL,
    ERR_INVALID_IMM_OF_BTYPE,
    ERR_LABEL_IS_NOT_FOUND,
    ERR_LABEL_IS_REDEFINED,
    ERR_LABEL_IS_TOO_FAR,
    ERR_LABEL_IS_NOT_SET_BY_L,
    ERR_LABEL_IS_ALREADY_SET_BY_L,
    ERR_CANT_PROTECT,
    ERR_CANT_ALLOC,
    ERR_BAD_PARAMETER,
    ERR_MUNMAP,
    ERR_INTERNAL,
    ERR_MAX,
};

inline const char *ConvertErrorToString(int err)
{
    static const char *errTbl[] = {
        "none",
        "offset is too big",
        "code is too big",
        "imm is too big",
        "invalid imm of jal",
        "invalid imm of Btype",
        "label is not found",
        "label is redefined",
        "label is too far",
        "label is not set by L",
        "label is already set by L",
        "can't protect",
        "can't alloc",
        "bad parameter",
        "munmap",
        "internal error"
    };
    assert(ERR_MAX == (sizeof(errTbl) / sizeof(*errTbl)));
    return (err < ERR_MAX) ? errTbl[err] : "unknown err";
}

class Error : public std::exception {
    int err_;
public:
    explicit Error(int err) : err_(err)
    {
        std::cout << "Error:" << err << std::endl;
        if (err_ < 0 || err_ > ERR_INTERNAL) {
            err_ = ERR_INTERNAL;
        }
    }
    operator int() const { return err_; }
    const char *what() const noexcept
    {
        return ConvertErrorToString(err_);
    }
};

// dummy functions
inline void ClearError() { }
inline int GetError() { return 0; }

inline const char *ConvertErrorToString(const Error& err)
{
    return err.what();
}

#define XBYAK_RISCV_THROW(err) { throw Error(err); }
#define XBYAK_RISCV_THROW_RET(err, r) { throw Error(err); }


namespace local {

// split x to hi20bits and low12bits
// return false if x in 12-bit signed integer
inline bool split32bit(int *pH, int* pL, int x) {
    if (inSBit(x, 12)) return false;
    int H = (x >> 12) & mask(20);
    int L = x & mask(12);
    if (x & (1 << 11)) {
        H++;
        L = L | (mask(20) << 12);
    }
    *pH = H;
    *pL = L;
    return true;
}

inline uint32_t get20_10to1_11_19to12_z12(uint32_t v) { return ((v & (1<<20)) << 11)| ((v & (1023<<1)) << 20)| ((v & (1<<11)) << 9)| (v & (255<<12)); }
inline uint32_t get12_10to5_z13_4to1_11_z7(uint32_t v) { return ((v & (1<<12)) << 19)| ((v & (63<<5)) << 20)| ((v & (15<<1)) << 7)| ((v & (1<<11)) >> 4); }

} // local

class CodeArray {
    std::vector<uint32_t> code_;
public:

    void resetSize()
    {
        code_.clear();
    }

    void append4B(uint32_t code) { code_.push_back(code); }
    
    void write4B(size_t offset, uint32_t v) { code_.at(offset) = v; }

    // **TODO**  add code base address
    uint32_t getCurr() const{ return static_cast<uint32_t>(code_.size()) * 4; }
    
    const auto &getCode() const{ return code_; }
};

struct Jmp {
    enum Type {
        tJal,
        tBtype,
        tRawAddress,
    } type;
    const uint32_t from; /* address of the jmp mnemonic */
    uint32_t encoded;

    // jal
    Jmp(uint32_t from, Bit<7> opcode, Reg rd)
        : type(tJal)
        , from(from)
        , encoded((static_cast<uint32_t>(rd) << 7) | opcode)
    {
    }
    // B-type
    Jmp(uint32_t from, Bit<7> opcode, uint32_t funct3, Reg src1, Reg src2)
        : type(tBtype)
        , from(from)
        , encoded((static_cast<uint32_t>(src2) << 20) | (static_cast<uint32_t>(src1) << 15) | (funct3 << 12) | opcode)
    {
    }
    // raw address
    explicit Jmp(uint32_t from)
        : type(tRawAddress)
        , from(from)
        , encoded(0)
    {
    }
    
    uint32_t encode(uint32_t addr) const
    {
        if (addr == 0) return 0;
        if (type == tRawAddress) return addr;
        const int imm = addr - from;
        std::cout << "JUMP " << addr << " - " << from << " = " << imm << std::endl;
        if (type == tJal) {
            if (!inSBit(imm, 20)) XBYAK_RISCV_THROW(ERR_INVALID_IMM_OF_JAL)
            return local::get20_10to1_11_19to12_z12(imm) | encoded;
        } else {
            if (!inSBit(imm, 12)) XBYAK_RISCV_THROW(ERR_INVALID_IMM_OF_JAL)
            return local::get12_10to5_z13_4to1_11_z7(imm) | encoded;
        }
    }
    // update jmp address by base->getCurr()
    void update(CodeArray *base) const
    {
        base->write4B(from, encode(base->getCurr()));
    }
    // append jmp opcode with addr
    void appendCode(CodeArray *base, uint32_t addr) const
    {
        base->append4B(encode(addr));
    }
};

class LabelManager;

class Label {
    mutable LabelManager *mgr;
    mutable int id;
    friend class LabelManager;
public:
    Label() : mgr(0), id(0) {}
    Label(const Label& rhs);
    Label& operator=(const Label& rhs);
    ~Label();
    void clear() { mgr = 0; id = 0; }
    int getId() const { return id; }
    uint32_t getAddress() const;
};

class LabelManager {
    // for Label class
    struct ClabelVal {
        ClabelVal(uint32_t addr = 0) : addr(addr), refCount(1) {}
        const uint32_t addr;
        int refCount;
    };
    typedef std::unordered_map<int, ClabelVal> ClabelDefList;
    typedef std::unordered_multimap<int, Jmp> ClabelUndefList;
    typedef std::unordered_set<Label*> LabelPtrList;

    CodeArray *base_;
    mutable int labelId_;
    ClabelDefList clabelDefList_;
    ClabelUndefList clabelUndefList_;
    LabelPtrList labelPtrList_;

    int getId(const Label& label) const
    {
        if (label.id == 0) label.id = labelId_++;
        return label.id;
    }
    void define_inner(ClabelDefList& defList, ClabelUndefList& undefList, int labelId, uint32_t addr)
    {
        // add label
        ClabelDefList::value_type item(labelId, addr);
        std::pair<ClabelDefList::iterator, bool> ret = defList.insert(item);
        if (!ret.second) XBYAK_RISCV_THROW(ERR_LABEL_IS_REDEFINED)
        // search undefined label
        for (;;) {
            ClabelUndefList::iterator itr = undefList.find(labelId);
            if (itr == undefList.end()) break;
            const Jmp& jmp = itr->second;
            jmp.update(base_);
            undefList.erase(itr);
        }
    }
    friend class Label;
    void incRefCount(int id, Label *label)
    {
        clabelDefList_[id].refCount++;
        labelPtrList_.insert(label);
    }
    void decRefCount(int id, Label *label)
    {
        labelPtrList_.erase(label);
        ClabelDefList::iterator i = clabelDefList_.find(id);
        if (i == clabelDefList_.end()) return;
        if (i->second.refCount == 1) {
            clabelDefList_.erase(id);
        } else {
            --i->second.refCount;
        }
    }
    template<class T>
    bool hasUndefinedLabel_inner(const T& list) const
    {
        return !list.empty();
    }
    // detach all labels linked to LabelManager
    void resetLabelPtrList()
    {
        for (LabelPtrList::iterator i = labelPtrList_.begin(), ie = labelPtrList_.end(); i != ie; ++i) {
            (*i)->clear();
        }
        labelPtrList_.clear();
    }
public:
    LabelManager()
    {
        reset();
    }
    ~LabelManager()
    {
        resetLabelPtrList();
    }
    void reset()
    {
        base_ = 0;
        labelId_ = 1;
        clabelDefList_.clear();
        clabelUndefList_.clear();
        resetLabelPtrList();
    }
    void set(CodeArray *base) { base_ = base; }
    void defineClabel(Label& label)
    {
        define_inner(clabelDefList_, clabelUndefList_, getId(label), base_->getCurr());
        label.mgr = this;
        labelPtrList_.insert(&label);
    }
    void assign(Label& dst, const Label& src)
    {
        ClabelDefList::const_iterator i = clabelDefList_.find(src.id);
        if (i == clabelDefList_.end()) XBYAK_RISCV_THROW(ERR_LABEL_IS_NOT_SET_BY_L)
        define_inner(clabelDefList_, clabelUndefList_, dst.id, i->second.addr);
        dst.mgr = this;
        labelPtrList_.insert(&dst);
    }
    // return 0 unless label exists
    uint32_t getAddr(const Label& label) const
    {
        ClabelDefList::const_iterator i = clabelDefList_.find(getId(label));
        if (i == clabelDefList_.end()) return 0;
        return i->second.addr;
    }
    void addUndefinedLabel(const Label& label, const Jmp& jmp)
    {
        clabelUndefList_.insert(ClabelUndefList::value_type(label.id, jmp));
    }
    bool hasUndefClabel() const { return hasUndefinedLabel_inner(clabelUndefList_); }
};

inline Label::Label(const Label& rhs)
{
    id = rhs.id;
    mgr = rhs.mgr;
    if (mgr) mgr->incRefCount(id, this);
}
inline Label& Label::operator=(const Label& rhs)
{
    if (id) XBYAK_RISCV_THROW_RET(ERR_LABEL_IS_ALREADY_SET_BY_L, *this)
    id = rhs.id;
    mgr = rhs.mgr;
    if (mgr) mgr->incRefCount(id, this);
    return *this;
}
inline Label::~Label()
{
    if (id && mgr) mgr->decRefCount(id, this);
}
inline uint32_t Label::getAddress() const
{
    if (mgr == 0) return 0;
    return mgr->getAddr(*this);
}


class CodeGenerator : public CodeArray {
public:
    enum AqRlType {
        T_aq = 2,
        T_rl = 1,
        T_aqrl = 3,
    };
    
private:
    CodeGenerator operator=(const CodeGenerator&) = delete;
    LabelManager labelMgr_;
    
    void opJmp(const Label& label, const Jmp& jmp)
    {
        const uint32_t addr = labelMgr_.getAddr(label);
        jmp.appendCode(this, addr);
        if (addr) return;
        labelMgr_.addUndefinedLabel(label, jmp);
    }
    uint32_t enc2(uint32_t a, uint32_t b) const { return (a<<7) | (b<<15); }
    uint32_t enc3(uint32_t a, uint32_t b, uint32_t c) const { return enc2(a, b) | (c<<20); }

    void Rtype(Bit<7> opcode, Bit<3> funct3, Bit<7> funct7, Bit<5> rd, Bit<5> rs1, Bit<5> rs2)
    {
        uint32_t v = (funct7<<25) | (funct3<<12) | opcode | enc3(rd, rs1, rs2);
        append4B(v);
    }
    void Itype(Bit<7> opcode, Bit<3> funct3, Bit<5> rd, Bit<5> rs1, int imm)
    {
        if (!inSBit(imm, 12)) XBYAK_RISCV_THROW(ERR_IMM_IS_TOO_BIG)
        uint32_t v = (imm<<20) | (funct3<<12) | opcode | enc2(rd, rs1);
        append4B(v);
    }
    void Stype(Bit<7> opcode, Bit<3> funct3, Bit<5> rs1, Bit<5> rs2, int imm)
    {
        if (!inSBit(imm, 12)) XBYAK_RISCV_THROW(ERR_IMM_IS_TOO_BIG)
        uint32_t v = ((imm>>5)<<25) | (funct3<<12) | opcode | enc3(imm & mask(5), rs1, rs2);
        append4B(v);
    }
    void Utype(Bit<7> opcode, Bit<5> rd, uint32_t imm)
    {
        if (imm >= (1u << 20)) XBYAK_RISCV_THROW(ERR_IMM_IS_TOO_BIG)
        uint32_t v = (imm<<12) | opcode | (rd<<7);
        append4B(v);
    }
    void opShift(Bit<7> pre, Bit<3> funct3, Bit<7> opcode, Bit<5> rd, Bit<5> rs1, uint32_t shamt, int range = 0)
    {
        if (range == 0) range = 5;
        if (shamt >= (1u << range)) XBYAK_RISCV_THROW(ERR_IMM_IS_TOO_BIG)
        uint32_t v = (pre<<25) | (funct3<<12) | opcode | enc3(rd, rs1, shamt);
        append4B(v);
    }
    
    void opCSR(Bit<32> baseValue, Bit<12> csr, Bit<5> rs1_uimm, Bit<5> rd)
    {
        /*
            31 .. 20 | 19 .. 15 | 14 .. 12 | 11 .. 7 | 6 .. 0
               csr     rs1_uimm     func3       rd     opcode

            func3 and opcode must be encoded in the baseValue
        */
        uint32_t v = (csr<<20) | (rs1_uimm<<15) | (rd<<7);
        v |= baseValue; // force-encode base value
        append4B(v);
    }

    bool isValiCidx(uint32_t idx) const { return 8 <= idx && idx < 16; }

public:
    void L(Label& label) { labelMgr_.defineClabel(label); }
    Label L() { Label label; L(label); return label; }
    /*
        assign src to dst
        require
        dst : does not used by L()
        src : used by L()
    */
    void assignL(Label& dst, const Label& src) { labelMgr_.assign(dst, src); }
    /*
        put the absolute address of label to buffer
        @note the put size is 4(32-bit), 8(64-bit)
    */
    void putL(const Label &label)
    {
        Jmp jmp(getCurr());
        opJmp(label, jmp);
    }

    // constructor
    CodeGenerator()
    {
        labelMgr_.set(this);
    }
    void reset()
    {
        ClearError();
        resetSize();
        labelMgr_.reset();
        labelMgr_.set(this);
    }

    bool hasUndefinedLabel() const { return labelMgr_.hasUndefClabel(); }

    void add(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 0, 0x0, rd, rs1, rs2); }
    void sub(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 0, 0x20, rd, rs1, rs2); }
    void sll(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 1, 0x0, rd, rs1, rs2); }
    void slt(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 2, 0x0, rd, rs1, rs2); }
    void sltu(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 3, 0x0, rd, rs1, rs2); }
    void xor_(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 4, 0x0, rd, rs1, rs2); }
    void srl(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 5, 0x0, rd, rs1, rs2); }
    void sra(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 5, 0x20, rd, rs1, rs2); }
    void or_(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 6, 0x0, rd, rs1, rs2); }
    void and_(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 7, 0x0, rd, rs1, rs2); }
    void mul(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 0, 0x1, rd, rs1, rs2); }
    void mulh(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 1, 0x1, rd, rs1, rs2); }
    void mulhsu(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 2, 0x1, rd, rs1, rs2); }
    void mulhu(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 3, 0x1, rd, rs1, rs2); }
    void div(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 4, 0x1, rd, rs1, rs2); }
    void divu(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 5, 0x1, rd, rs1, rs2); }
    void rem(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 6, 0x1, rd, rs1, rs2); }
    void remu(const Reg& rd, const Reg& rs1, const Reg& rs2) { Rtype(StandardOpCode::OP, 7, 0x1, rd, rs1, rs2); }
    void addi(const Reg& rd, const Reg& rs1, int imm) { Itype(StandardOpCode::OP_IMM, 0, rd, rs1, imm); }
    void slti(const Reg& rd, const Reg& rs1, int imm) { Itype(StandardOpCode::OP_IMM, 2, rd, rs1, imm); }
    void sltiu(const Reg& rd, const Reg& rs1, int imm) { Itype(StandardOpCode::OP_IMM, 3, rd, rs1, imm); }
    void xori(const Reg& rd, const Reg& rs1, int imm) { Itype(StandardOpCode::OP_IMM, 4, rd, rs1, imm); }
    void ori(const Reg& rd, const Reg& rs1, int imm) { Itype(StandardOpCode::OP_IMM, 6, rd, rs1, imm); }
    void andi(const Reg& rd, const Reg& rs1, int imm) { Itype(StandardOpCode::OP_IMM, 7, rd, rs1, imm); }
    // load-op rd, imm(addr); rd = addr[imm];
    void jalr(const Reg& rd, const Reg& addr, int imm = 0) { Itype(StandardOpCode::JALR, 0, rd, addr, imm); }
    void lb(const Reg& rd, const Reg& addr, int imm = 0) { Itype(StandardOpCode::LOAD, 0, rd, addr, imm); }
    void lh(const Reg& rd, const Reg& addr, int imm = 0) { Itype(StandardOpCode::LOAD, 1, rd, addr, imm); }
    void lw(const Reg& rd, const Reg& addr, int imm = 0) { Itype(StandardOpCode::LOAD, 2, rd, addr, imm); }
    void lbu(const Reg& rd, const Reg& addr, int imm = 0) { Itype(StandardOpCode::LOAD, 4, rd, addr, imm); }
    void lhu(const Reg& rd, const Reg& addr, int imm = 0) { Itype(StandardOpCode::LOAD, 5, rd, addr, imm); }
    void lwu(const Reg& rd, const Reg& addr, int imm = 0) { Itype(StandardOpCode::LOAD, 6, rd, addr, imm); }
    void ld(const Reg& rd, const Reg& addr, int imm = 0) { Itype(StandardOpCode::LOAD, 3, rd, addr, imm); }
    void auipc(const Reg& rd, uint32_t imm) { Utype(StandardOpCode::AUIPC, rd, imm); }
    void lui(const Reg& rd, uint32_t imm) { Utype(StandardOpCode::LUI, rd, imm); }
    void slli(const Reg& rd, const Reg& rs1, uint32_t shamt) { opShift(0x0, 1, StandardOpCode::OP_IMM, rd, rs1, shamt); }
    void srli(const Reg& rd, const Reg& rs1, uint32_t shamt) { opShift(0x0, 5, StandardOpCode::OP_IMM, rd, rs1, shamt); }
    void srai(const Reg& rd, const Reg& rs1, uint32_t shamt) { opShift(0x20, 5, StandardOpCode::OP_IMM, rd, rs1, shamt); }
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
    void sb(const Reg& rs, const Reg& addr, int imm = 0) { Stype(StandardOpCode::STORE, 0, addr, rs, imm); }
    void sh(const Reg& rs, const Reg& addr, int imm = 0) { Stype(StandardOpCode::STORE, 1, addr, rs, imm); }
    void sw(const Reg& rs, const Reg& addr, int imm = 0) { Stype(StandardOpCode::STORE, 2, addr, rs, imm); }
    void sd(const Reg& rs, const Reg& addr, int imm = 0) { Stype(StandardOpCode::STORE, 3, addr, rs, imm); }
    void beq(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), StandardOpCode::BRANCH, 0, rs1, rs2); opJmp(label, jmp); }
    void bne(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), StandardOpCode::BRANCH, 1, rs1, rs2); opJmp(label, jmp); }
    void blt(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), StandardOpCode::BRANCH, 4, rs1, rs2); opJmp(label, jmp); }
    void bge(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), StandardOpCode::BRANCH, 5, rs1, rs2); opJmp(label, jmp); }
    void bltu(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), StandardOpCode::BRANCH, 6, rs1, rs2); opJmp(label, jmp); }
    void bgeu(const Reg& rs1, const Reg& rs2, const Label& label) { Jmp jmp(getCurr(), StandardOpCode::BRANCH, 7, rs1, rs2); opJmp(label, jmp); }
    void beqz(const Reg& rs, const Label& label) { beq(rs, Reg::X0, label); }
    void bnez(const Reg& rs, const Label& label) { bne(rs, Reg::X0, label); }
    void blez(const Reg& rs, const Label& label) { bge(Reg::X0, rs, label); }
    void bgez(const Reg& rs, const Label& label) { bge(rs, Reg::X0, label); }
    void bltz(const Reg& rs, const Label& label) { blt(rs, Reg::X0, label); }
    void bgtz(const Reg& rs, const Label& label) { blt(Reg::X0, rs, label); }
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

    void nop() { addi(Reg::X0, Reg::X0, 0); }
    void li(const Reg& rd, int imm)
    {
        int H, L;
        if (!local::split32bit(&H, &L, imm)) {
            addi(rd, Reg::X0, imm);
            return;
        }
        lui(rd, H);
        addi(rd, rd, L);
    }
    void mv(const Reg& rd, const Reg& rs) { addi(rd, rs, 0); }
    void not_(const Reg& rd, const Reg& rs) { xori(rd, rs, -1); }
    void neg(const Reg& rd, const Reg& rs) { sub(rd, Reg::X0, rs); }
    void sext_b(const Reg& rd, const Reg& rs) { slli(rd, rs, 24); srai(rd, rd, 24); }
    void sext_h(const Reg& rd, const Reg& rs) { slli(rd, rs, 16); srai(rd, rd, 16); }
    void zext_b(const Reg& rd, const Reg& rs) { andi(rd, rs, 255); }
    void zext_h(const Reg& rd, const Reg& rs) { slli(rd, rs, 16); srli(rd, rd, 16); }
    void zext_w(const Reg& rd, const Reg& rs) { slli(rd, rs, 0); srli(rd, rd, 0); }
    void seqz(const Reg& rd, const Reg& rs) { sltiu(rd, rs, 1); }
    void snez(const Reg& rd, const Reg& rs) { sltu(rd, Reg::X0, rs); }
    void sltz(const Reg& rd, const Reg& rs) { slt(rd, rs, Reg::X0); }
    void sgtz(const Reg& rd, const Reg& rs) { slt(rd, Reg::X0, rs); }
    void fence() { append4B(0x0ff0000f); }
    void j_(const Label& label) { jal(Reg::X0, label); }
    void jal(const Reg& rd, const Label& label) { Jmp jmp(getCurr(), 0x6f, rd); opJmp(label, jmp); }
    void jr(const Reg& rs) { jalr(Reg::X0, rs, 0); }
    void jalr(const Reg& rs) { jalr(Reg::X1, rs, 0); }
    void ret() { jalr(Reg::X0, Reg::X1); }

    void csrr(const Reg& rd, CSR csr) { csrrs(rd, csr, Reg::X0); }
    void csrw(CSR csr, const Reg& rs) { csrrw(Reg::X0, csr, rs); }
    void csrs(CSR csr, const Reg& rs) { csrrs(Reg::X0, csr, rs); }
    void csrc(CSR csr, const Reg& rs) { csrrc(Reg::X0, csr, rs); }
    void csrwi(CSR csr, uint32_t imm) { csrrwi(Reg::X0, csr, imm); }
    void csrsi(CSR csr, uint32_t imm) { csrrsi(Reg::X0, csr, imm); }
    void csrci(CSR csr, uint32_t imm) { csrrci(Reg::X0, csr, imm); }

    // FeNN vector processor

    // VSOP
    // Rtype(Bit<7> opcode, Bit<3> funct3, Bit<7> funct7, Bit<5> rd, Bit<5> rs1, Bit<5> rs2)
    void vadd(const VReg &rd, const VReg &rs1, const VReg &rs2){ Rtype(0x2, 0x0, 0x0, rd, rs1, rs2); }
    void vsub(const VReg &rd, const VReg &rs1, const VReg &rs2){ Rtype(0x2, 0x2, 0x0, rd, rs1, rs2); }
    void vmul(Bit<4> shift, const VReg &rd, const VReg &rs1, const VReg &rs2){ Rtype(0x2, 0x4, shift, rd, rs1, rs2); }

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

};

} // Xbyak_riscv

