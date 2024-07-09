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

// RISC-V assembler includes
#include "xbyak_riscv_csr.hpp"

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

inline constexpr uint32_t mask(uint32_t n)
{
    assert(n <= 32);
    return n == 32 ? 0xffffffff : (1u << n) - 1;
}
// is x <= mask(n) ?
inline constexpr bool inBit(uint32_t x, uint32_t n)
{
    return x <= mask(n);
}

// is x a signed n-bit integer?
inline constexpr bool inSBit(int x, int n)
{
    return -(1 << (n-1)) <= x && x < (1 << (n-1));
}

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


namespace local {

// Register Interface
class IReg {
public:
    enum Kind {
        GPR = 1,         // General purpose register
        FReg = 1 << 1,   // Floating-point register
        VECTOR = 1 << 2, // Vector register
    };
protected:
    uint32_t idx_;
    Kind kind_;
public:
    constexpr IReg(uint32_t idx = 0, Kind kind = GPR)
        : idx_(idx), kind_(kind)
    {
        assert(local::inBit(idx, 5));
    }
    constexpr int getIdx() const { return idx_; }
    const char *toString() const
    {
        if (kind_ == GPR) {
            static const char tbl[][4] = {
                "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7",
                "x8", "x9", "x10", "x11", "x12", "x13", "x14", "x15",
                "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23",
                "x24", "x25", "x26", "x27", "x28", "x29", "x30", "x31",
            };
            return tbl[idx_];
        } else if (kind_ == FReg) {
            static const char tbl[][4] = {
                "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",
                "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15",
                "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23",
                "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31",
            };
            return tbl[idx_];
        } else if (kind_ == VECTOR) {
            static const char tbl[][4] = {
                "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
                "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
                "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
                "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31",
            };
            return tbl[idx_];
        }
        XBYAK_RISCV_THROW_RET(ERR_INTERNAL, 0);
    }
    bool operator==(const IReg& rhs) const
    {
        return idx_ == rhs.idx_ && kind_ == rhs.kind_;
    }
    bool operator!=(const IReg& rhs) const { return !operator==(rhs); }

};

} // local

// General Purpose Register
struct Reg : public local::IReg {
    explicit constexpr Reg(int idx = 0) : local::IReg(idx, IReg::Kind::GPR) { }
};

// FeNN vector register
struct VReg : public local::IReg {
    explicit constexpr VReg(int idx = 0) : local::IReg(idx, IReg::Kind::VECTOR) { }
};

static constexpr Reg x0(0), x1(1), x2(2), x3(3), x4(4), x5(5), x6(6), x7(7);
static constexpr Reg x8(8), x9(9), x10(10), x11(11), x12(12), x13(13), x14(14), x15(15);
static constexpr Reg x16(16), x17(17), x18(18), x19(19), x20(20), x21(21), x22(22), x23(23);
static constexpr Reg x24(24), x25(25), x26(26), x27(27), x28(28), x29(29), x30(30), x31(31);

static constexpr VReg v0(0), v1(1), v2(2), v3(3), v4(4), v5(5), v6(6), v7(7);
static constexpr VReg v8(8), v9(9), v10(10), v11(11), v12(12), v13(13), v14(14), v15(15);
static constexpr VReg v16(16), v17(17), v18(18), v19(19), v20(20), v21(21), v22(22), v23(23);
static constexpr VReg v24(24), v25(25), v26(26), v27(27), v28(28), v29(29), v30(30), v31(31);

static constexpr Reg zero(x0);
static constexpr Reg ra(x1);
static constexpr Reg sp(x2);
static constexpr Reg gp(x3);
static constexpr Reg tp(x4);
static constexpr Reg t0(x5);
static constexpr Reg t1(x6);
static constexpr Reg t2(x7);
static constexpr Reg fp(x8);
static constexpr Reg s0(x8);
static constexpr Reg s1(x9);
static constexpr Reg a0(x10), a1(x11), a2(x12), a3(x13), a4(x14), a5(x15), a6(x16), a7(x17);
static constexpr Reg s2(x18), s3(x19), s4(x20), s5(x21), s6(x22), s7(x23), s8(x24), s9(x25);
static constexpr Reg s10(x26), s11(x27);
static constexpr Reg t3(x28), t4(x29), t5(x30), t6(x31);

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
    Jmp(uint32_t from, uint32_t opcode, const Reg& rd)
        : type(tJal)
        , from(from)
        , encoded((rd.getIdx() << 7) | opcode)
    {
    }
    // B-type
    Jmp(uint32_t from, uint32_t opcode, uint32_t funct3, const Reg& src1, const Reg& src2)
        : type(tBtype)
        , from(from)
        , encoded((src2.getIdx() << 20) | (src1.getIdx() << 15) | (funct3 << 12) | opcode)
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
            if (!local::inSBit(imm, 20)) XBYAK_RISCV_THROW(ERR_INVALID_IMM_OF_JAL)
            return local::get20_10to1_11_19to12_z12(imm) | encoded;
        } else {
            if (!local::inSBit(imm, 12)) XBYAK_RISCV_THROW(ERR_INVALID_IMM_OF_JAL)
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

namespace local {

template<size_t n>
struct Bit {
    uint32_t v;
    Bit(uint32_t v)
        : v(v)
    {
        assert(inBit(v, n));
    }
    Bit(const IReg& r)
        : v(r.getIdx())
    {
    }
    /*Bit(VM vm)
        : v(static_cast<uint32_t>(vm))
    {
    }*/
    Bit(CSR csr)
        : v(static_cast<uint32_t>(csr))
    {
    }
    /*Bit(RM rm)
        : v(static_cast<uint32_t>(rm))
    {
    }*/
};

} // local

class CodeGenerator : public CodeArray {
public:
    enum AqRlType {
        T_aq = 2,
        T_rl = 1,
        T_aqrl = 3,
    };
    typedef local::Bit<1> Bit1;
    typedef local::Bit<2> Bit2;
    typedef local::Bit<3> Bit3;
    typedef local::Bit<4> Bit4;
    typedef local::Bit<5> Bit5;
    typedef local::Bit<6> Bit6;
    typedef local::Bit<7> Bit7;
    typedef local::Bit<12> Bit12;
    typedef local::Bit<32> Bit32;
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
    void Rtype(Bit7 opcode, Bit3 funct3, Bit7 funct7, Bit5 rd, Bit5 rs1, Bit5 rs2)
    {
        uint32_t v = (funct7.v<<25) | (funct3.v<<12) | opcode.v | enc3(rd.v, rs1.v, rs2.v);
        append4B(v);
    }
    void Itype(Bit7 opcode, Bit3 funct3, Bit5 rd, Bit5 rs1, int imm)
    {
        if (!local::inSBit(imm, 12)) XBYAK_RISCV_THROW(ERR_IMM_IS_TOO_BIG)
        uint32_t v = (imm<<20) | (funct3.v<<12) | opcode.v | enc2(rd.v, rs1.v);
        append4B(v);
    }
    void Stype(Bit7 opcode, Bit3 funct3, Bit5 rs1, Bit5 rs2, int imm)
    {
        if (!local::inSBit(imm, 12)) XBYAK_RISCV_THROW(ERR_IMM_IS_TOO_BIG)
        uint32_t v = ((imm>>5)<<25) | (funct3.v<<12) | opcode.v | enc3(imm & local::mask(5), rs1.v, rs2.v);
        append4B(v);
    }
    void Utype(Bit7 opcode, Bit5 rd, uint32_t imm)
    {
        if (imm >= (1u << 20)) XBYAK_RISCV_THROW(ERR_IMM_IS_TOO_BIG)
        uint32_t v = (imm<<12) | opcode.v | (rd.v<<7);
        append4B(v);
    }
    void opShift(Bit7 pre, Bit3 funct3, Bit7 opcode, Bit5 rd, Bit5 rs1, uint32_t shamt, int range = 0)
    {
        if (range == 0) range = 5;
        if (shamt >= (1u << range)) XBYAK_RISCV_THROW(ERR_IMM_IS_TOO_BIG)
        uint32_t v = (pre.v<<25) | (funct3.v<<12) | opcode.v | enc3(rd.v, rs1.v, shamt);
        append4B(v);
    }
    
    void opCSR(Bit32 baseValue, Bit12 csr, Bit5 rs1_uimm, Bit5 rd)
    {
        /*
            31 .. 20 | 19 .. 15 | 14 .. 12 | 11 .. 7 | 6 .. 0
               csr     rs1_uimm     func3       rd     opcode

            func3 and opcode must be encoded in the baseValue
        */
        uint32_t v = (csr.v<<20) | (rs1_uimm.v<<15) | (rd.v<<7);
        v |= baseValue.v; // force-encode base value
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
        addi(rd, rd, L);
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

};

} // Xbyak_riscv

