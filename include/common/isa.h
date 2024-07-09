#pragma once

// Standard C++ includes
#include <tuple>

// Standard C include
#include <cassert>
#include <cstdint>

inline constexpr uint32_t mask(uint32_t n)
{
    assert (n <= 32);
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

// Opcode in the standard encoding quadrant (11)
enum class StandardOpCode : uint32_t
{
    LOAD    = 0b00000,
    STORE   = 0b01000,
    BRANCH  = 0b11000,
    JALR    = 0b11001,
    JAL     = 0b11011,
    OP_IMM  = 0b00100,
    OP      = 0b01100,
    LUI     = 0b01101,
    AUIPC   = 0b00101,
};

static constexpr uint32_t standardQuadrant = 0b11;
static constexpr uint32_t vectorQuadrant = 0b00;

enum class VectorOpCode : uint32_t
{
    VSOP    = 0b01100,
    VTST    = 0b00010,
    VSEL    = 0b01010,
    VLUI    = 0b01101,
    VLOAD   = 0b00000,
    VSTORE  = 0b01000,
};


inline uint32_t addQuadrant(StandardOpCode op)
{
    return (static_cast<uint32_t>(op) << 2) | standardQuadrant;
}

inline uint32_t addQuadrant(VectorOpCode op)
{
    return (static_cast<uint32_t>(op) << 2) | vectorQuadrant;
}

// Standard RISC-V registers
enum class Reg : uint32_t
{
    X0,
    X1,
    X2,
    X3,
    X4,
    X5,
    X6,
    X7,
    X8,
    X9,
    X10,
    X11,
    X12,
    X13,
    X14,
    X15,
    X16,
    X17,
    X18,
    X19,
    X20,
    X21,
    X22,
    X23,
    X24,
    X25,
    X26,
    X27,
    X28,
    X29,
    X30,
    X31,
};

enum class VReg : uint32_t
{
    V0,
    V1,
    V2,
    V3,
    V4,
    V5,
    V6,
    V7,
    V8,
    V9,
    V10,
    V11,
    V12,
    V13,
    V14,
    V15,
    V16,
    V17,
    V18,
    V19,
    V20,
    V21,
    V22,
    V23,
    V24,
    V25,
    V26,
    V27,
    V28,
    V29,
    V30,
    V31,
};

// Control and Status Register
enum class CSR : uint32_t 
{
};

// funct7 rs2 rs1 funct3 rd
inline std::tuple<uint32_t, uint32_t, uint32_t, uint32_t, uint32_t> decodeRType(uint32_t inst)
{
    const uint32_t funct7 = inst >> 25;
    const uint32_t rs2 = (inst >> 20) & 0x1f;
    const uint32_t rs1 = (inst >> 15) & 0x1f;
    const uint32_t funct3 = (inst >> 12) & 7;
    const uint32_t rd = (inst >> 7) & 0x1f;
    return std::make_tuple(funct7, rs2, rs1, funct3, rd);
}

// imm[11:0] rs1 funct3 rd
inline std::tuple<int32_t, uint32_t, uint32_t, uint32_t> decodeIType(uint32_t inst)
{
    const int32_t imm = (int32_t)(inst >> 20);
    const uint32_t rs1 = (inst >> 15) & 0x1f;
    const uint32_t funct3 = (inst >> 12) & 7;
    const uint32_t rd = (inst >> 7) & 0x1f;
    return std::make_tuple(imm, rs1, funct3, rd);
}

inline std::tuple<int32_t, uint32_t, uint32_t, uint32_t> decodeSType(uint32_t inst)
{
    const uint32_t rs2 = (inst >> 20) & 0x1f;
    const uint32_t rs1 = (inst >> 15) & 0x1f;
    const uint32_t funct3 = (inst >> 12) & 7;
    const int32_t imm = ((((inst >> 7) & 0x1f) | ((inst >> (25 - 5)) & 0xfe0)) << 20) >> 20;

    return std::make_tuple(imm, rs2, rs1, funct3);
}

// imm[12] imm[10:5] rs2 rs1 funct3 imm[4:1] imm[11]
inline std::tuple<int32_t, uint32_t, uint32_t, uint32_t> decodeBType(uint32_t inst)
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
inline std::tuple<int32_t, uint32_t> decodeUType(uint32_t inst)
{
    const int32_t imm =  (int32_t)(inst & 0xfffff000);
    const uint32_t rd = (inst >> 7) & 0x1f;
    return std::make_tuple(imm, rd);
}

template<size_t N>
class Bit {
public:
    Bit(uint32_t v) : v(v)
    {
        assert(inBit(v, N));
    }
    
    template<size_t M>
    Bit(Bit<M> b) : Bit(b.operator uint32_t())
    {
    }
    
    Bit(StandardOpCode s) : Bit(addQuadrant(s))
    {
    }
    
    Bit(VectorOpCode s) : Bit(addQuadrant(s))
    {
    }
    
    Bit(Reg r) : Bit(static_cast<uint32_t>(r))
    {
    }
    
    Bit(VReg v) : Bit(static_cast<uint32_t>(v))
    {
    }
    
    Bit(CSR csr) : Bit(static_cast<uint32_t>(csr))
    {
    }

    operator uint32_t() const{ return v; }
private:
    uint32_t v;
};
