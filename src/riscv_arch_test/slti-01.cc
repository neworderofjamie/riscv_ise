#include "slti-01.h"

// Standard C includes
#include <cassert>
#include <cstring>

// RISC-V assembler includes
#include "assembler/xbyak_riscv.hpp"

// RISC-V ISE includes
#include "ise/riscv.h"

#define MASK_XLEN(val)  val&0xFFFFFFFF // shortens 64b values to 32b

#define BIT(addr, bit) (((addr)>>(bit))&1)
#define IMMSZ      12
#define IMMSGN            (IMMSZ -1)
#define IMMMSK     ( (1 << IMMSZ)-1)
#define SEXT_IMM(x)  ((x & IMMMSK) | (-BIT((x), IMMSGN)<< IMMSZ))

using namespace Xbyak_riscv;

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
const std::vector<uint32_t> g_ScalarData{

3133065982, 3133065982, 3133065982, 3133065982, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559, };
}

//----------------------------------------------------------------------------
// slti_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &slti_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
CodeGenerator slti_01::generateCode()
{
    CodeGenerator c;
    
    c.li(Reg::X2, 16);
    // rs1 != rd, rs1==x25, rd==x12, imm_val == (-2**(12-1)), imm_val == -2048, rs1_val != imm_val, rs1_val == -129, rs1_val < 0 and imm_val < 0
    // opcode: slti ; op1:x25; dest:x12; op1val:-0x81;  immval:-0x800
    c.li(Reg::X25, MASK_XLEN(-0x81));
    c.slti(Reg::X12, Reg::X25, SEXT_IMM(-0x800));
    c.sw(Reg::X12, Reg::X2, 0);

    // rs1 == rd, rs1==x5, rd==x5, imm_val == 0, rs1_val == -4097
    // opcode: slti ; op1:x5; dest:x5; op1val:-0x1001;  immval:0x0
    c.li(Reg::X5, MASK_XLEN(-0x1001));
    c.slti(Reg::X5, Reg::X5, SEXT_IMM(0x0));
    c.sw(Reg::X5, Reg::X2, 4);

    // rs1==x4, rd==x28, imm_val == (2**(12-1)-1), rs1_val < 0 and imm_val > 0, imm_val == 2047
    // opcode: slti ; op1:x4; dest:x28; op1val:-0x40000000;  immval:0x7ff
    c.li(Reg::X4, MASK_XLEN(-0x40000000));
    c.slti(Reg::X28, Reg::X4, SEXT_IMM(0x7ff));
    c.sw(Reg::X28, Reg::X2, 8);

    // rs1==x31, rd==x15, imm_val == 1, rs1_val == -17
    // opcode: slti ; op1:x31; dest:x15; op1val:-0x11;  immval:0x1
    c.li(Reg::X31, MASK_XLEN(-0x11));
    c.slti(Reg::X15, Reg::X31, SEXT_IMM(0x1));
    c.sw(Reg::X15, Reg::X2, 12);

    // rs1==x1, rd==x13, rs1_val == (-2**(xlen-1)), rs1_val == -2147483648
    // opcode: slti ; op1:x1; dest:x13; op1val:-0x80000000;  immval:0x3
    c.li(Reg::X1, MASK_XLEN(-0x80000000));
    c.slti(Reg::X13, Reg::X1, SEXT_IMM(0x3));
    c.sw(Reg::X13, Reg::X2, 16);

    // rs1==x15, rd==x1, rs1_val == 0, rs1_val==0 and imm_val==2, imm_val == 2
    // opcode: slti ; op1:x15; dest:x1; op1val:0x0;  immval:0x2
    c.li(Reg::X15, MASK_XLEN(0x0));
    c.slti(Reg::X1, Reg::X15, SEXT_IMM(0x2));
    c.sw(Reg::X1, Reg::X2, 20);

    // rs1==x16, rd==x9, rs1_val == (2**(xlen-1)-1), rs1_val > 0 and imm_val < 0, rs1_val == 2147483647
    // opcode: slti ; op1:x16; dest:x9; op1val:0x7fffffff;  immval:-0x8
    c.li(Reg::X16, MASK_XLEN(0x7fffffff));
    c.slti(Reg::X9, Reg::X16, SEXT_IMM(-0x8));
    c.sw(Reg::X9, Reg::X2, 24);

    // rs1==x11, rd==x31, rs1_val == 1, 
    // opcode: slti ; op1:x11; dest:x31; op1val:0x1;  immval:-0x400
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.slti(Reg::X31, Reg::X11, SEXT_IMM(-0x400));
    c.sw(Reg::X31, Reg::X2, 28);

    // rs1==x14, rd==x27, rs1_val == imm_val, imm_val == 16, rs1_val > 0 and imm_val > 0, rs1_val == 16
    // opcode: slti ; op1:x14; dest:x27; op1val:0x10;  immval:0x10
    c.li(Reg::X14, MASK_XLEN(0x10));
    c.slti(Reg::X27, Reg::X14, SEXT_IMM(0x10));
    c.sw(Reg::X27, Reg::X2, 32);

    // rs1==x12, rd==x26, imm_val == 4, rs1_val==858993460 and imm_val==4
    // opcode: slti ; op1:x12; dest:x26; op1val:0x33333334;  immval:0x4
    c.li(Reg::X12, MASK_XLEN(0x33333334));
    c.slti(Reg::X26, Reg::X12, SEXT_IMM(0x4));
    c.sw(Reg::X26, Reg::X2, 36);

    // rs1==x17, rd==x4, imm_val == 8, 
    // opcode: slti ; op1:x17; dest:x4; op1val:0x3fffffff;  immval:0x8
    c.li(Reg::X17, MASK_XLEN(0x3fffffff));
    c.slti(Reg::X4, Reg::X17, SEXT_IMM(0x8));
    c.sw(Reg::X4, Reg::X2, 40);

    // rs1==x18, rd==x10, imm_val == 32, rs1_val == -8193
    // opcode: slti ; op1:x18; dest:x10; op1val:-0x2001;  immval:0x20
    c.li(Reg::X18, MASK_XLEN(-0x2001));
    c.slti(Reg::X10, Reg::X18, SEXT_IMM(0x20));
    c.sw(Reg::X10, Reg::X2, 44);

    // rs1==x27, rd==x21, imm_val == 64, 
    // opcode: slti ; op1:x27; dest:x21; op1val:0x3;  immval:0x40
    c.li(Reg::X27, MASK_XLEN(0x3));
    c.slti(Reg::X21, Reg::X27, SEXT_IMM(0x40));
    c.sw(Reg::X21, Reg::X2, 48);

    // rs1==x3, rd==x8, imm_val == 128, 
    // opcode: slti ; op1:x3; dest:x8; op1val:0x55555554;  immval:0x80
    c.li(Reg::X3, MASK_XLEN(0x55555554));
    c.slti(Reg::X8, Reg::X3, SEXT_IMM(0x80));
    c.sw(Reg::X8, Reg::X2, 52);

    // rs1==x7, rd==x0, imm_val == 256, 
    // opcode: slti ; op1:x7; dest:x0; op1val:0x55555554;  immval:0x100
    c.li(Reg::X7, MASK_XLEN(0x55555554));
    c.slti(Reg::X0, Reg::X7, SEXT_IMM(0x100));
    c.sw(Reg::X0, Reg::X2, 56);

    // rs1==x22, rd==x24, imm_val == 512, rs1_val == -1431655766
    // opcode: slti ; op1:x22; dest:x24; op1val:-0x55555556;  immval:0x200
    c.li(Reg::X22, MASK_XLEN(-0x55555556));
    c.slti(Reg::X24, Reg::X22, SEXT_IMM(0x200));
    c.sw(Reg::X24, Reg::X2, 60);

    // rs1==x24, rd==x18, imm_val == 1024, rs1_val == 16384
    // opcode: slti ; op1:x24; dest:x18; op1val:0x4000;  immval:0x400
    c.li(Reg::X24, MASK_XLEN(0x4000));
    c.slti(Reg::X18, Reg::X24, SEXT_IMM(0x400));
    c.sw(Reg::X18, Reg::X2, 64);

    // rs1==x6, rd==x25, imm_val == -2, rs1_val == -1025
    // opcode: slti ; op1:x6; dest:x25; op1val:-0x401;  immval:-0x2
    c.li(Reg::X6, MASK_XLEN(-0x401));
    c.slti(Reg::X25, Reg::X6, SEXT_IMM(-0x2));
    c.sw(Reg::X25, Reg::X2, 68);

    // rs1==x21, rd==x23, imm_val == -3, 
    // opcode: slti ; op1:x21; dest:x23; op1val:0x66666667;  immval:-0x3
    c.li(Reg::X21, MASK_XLEN(0x66666667));
    c.slti(Reg::X23, Reg::X21, SEXT_IMM(-0x3));
    c.sw(Reg::X23, Reg::X2, 72);

    // rs1==x0, rd==x7, imm_val == -5, rs1_val == -1048577
    // opcode: slti ; op1:x0; dest:x7; op1val:0x0;  immval:-0x5
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.slti(Reg::X7, Reg::X0, SEXT_IMM(-0x5));
    c.sw(Reg::X7, Reg::X2, 76);

    // rs1==x10, rd==x20, imm_val == -9, 
    // opcode: slti ; op1:x10; dest:x20; op1val:-0xb504;  immval:-0x9
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X20, Reg::X10, SEXT_IMM(-0x9));
    c.sw(Reg::X20, Reg::X2, 80);

    // rs1==x26, rd==x16, imm_val == -17, 
    // opcode: slti ; op1:x26; dest:x16; op1val:0x3fffffff;  immval:-0x11
    c.li(Reg::X26, MASK_XLEN(0x3fffffff));
    c.slti(Reg::X16, Reg::X26, SEXT_IMM(-0x11));
    c.sw(Reg::X16, Reg::X2, 84);

    // rs1==x20, rd==x29, imm_val == -33, rs1_val == -524289
    // opcode: slti ; op1:x20; dest:x29; op1val:-0x80001;  immval:-0x21
    c.li(Reg::X20, MASK_XLEN(-0x80001));
    c.slti(Reg::X29, Reg::X20, SEXT_IMM(-0x21));
    c.sw(Reg::X29, Reg::X2, 88);

    c.li(Reg::X1, 108);
    // rs1==x13, rd==x3, imm_val == -65, rs1_val == -268435457
    // opcode: slti ; op1:x13; dest:x3; op1val:-0x10000001;  immval:-0x41
    c.li(Reg::X13, MASK_XLEN(-0x10000001));
    c.slti(Reg::X3, Reg::X13, SEXT_IMM(-0x41));
    c.sw(Reg::X3, Reg::X1, 0);

    // rs1==x29, rd==x14, imm_val == -129, rs1_val == 8388608
    // opcode: slti ; op1:x29; dest:x14; op1val:0x800000;  immval:-0x81
    c.li(Reg::X29, MASK_XLEN(0x800000));
    c.slti(Reg::X14, Reg::X29, SEXT_IMM(-0x81));
    c.sw(Reg::X14, Reg::X1, 4);

    // rs1==x23, rd==x2, imm_val == -257, 
    // opcode: slti ; op1:x23; dest:x2; op1val:-0x10000001;  immval:-0x101
    c.li(Reg::X23, MASK_XLEN(-0x10000001));
    c.slti(Reg::X2, Reg::X23, SEXT_IMM(-0x101));
    c.sw(Reg::X2, Reg::X1, 8);

    // rs1==x2, rd==x22, imm_val == -513, 
    // opcode: slti ; op1:x2; dest:x22; op1val:0x0;  immval:-0x201
    c.li(Reg::X2, MASK_XLEN(0x0));
    c.slti(Reg::X22, Reg::X2, SEXT_IMM(-0x201));
    c.sw(Reg::X22, Reg::X1, 12);

    // rs1==x30, rd==x19, imm_val == -1025, rs1_val == -262145
    // opcode: slti ; op1:x30; dest:x19; op1val:-0x40001;  immval:-0x401
    c.li(Reg::X30, MASK_XLEN(-0x40001));
    c.slti(Reg::X19, Reg::X30, SEXT_IMM(-0x401));
    c.sw(Reg::X19, Reg::X1, 16);

    // rs1==x19, rd==x11, imm_val == 1365, rs1_val == 4, rs1_val==4 and imm_val==1365
    // opcode: slti ; op1:x19; dest:x11; op1val:0x4;  immval:0x555
    c.li(Reg::X19, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X19, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 20);

    // rs1==x8, rd==x30, imm_val == -1366, 
    // opcode: slti ; op1:x8; dest:x30; op1val:-0x40000000;  immval:-0x556
    c.li(Reg::X8, MASK_XLEN(-0x40000000));
    c.slti(Reg::X30, Reg::X8, SEXT_IMM(-0x556));
    c.sw(Reg::X30, Reg::X1, 24);

    // rs1==x9, rd==x17, rs1_val == 2, 
    // opcode: slti ; op1:x9; dest:x17; op1val:0x2;  immval:0x7ff
    c.li(Reg::X9, MASK_XLEN(0x2));
    c.slti(Reg::X17, Reg::X9, SEXT_IMM(0x7ff));
    c.sw(Reg::X17, Reg::X1, 28);

    // rs1==x28, rd==x6, rs1_val == 8, 
    // opcode: slti ; op1:x28; dest:x6; op1val:0x8;  immval:-0x555
    c.li(Reg::X28, MASK_XLEN(0x8));
    c.slti(Reg::X6, Reg::X28, SEXT_IMM(-0x555));
    c.sw(Reg::X6, Reg::X1, 32);

    // rs1_val == 32, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x20;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x20));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 36);

    // rs1_val == 64, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x40;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x40));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 40);

    // rs1_val == 128, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x80;  immval:0x200
    c.li(Reg::X10, MASK_XLEN(0x80));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x200));
    c.sw(Reg::X11, Reg::X1, 44);

    // rs1_val == 256, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x100;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x100));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 48);

    // rs1_val == 512, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x200;  immval:-0x3
    c.li(Reg::X10, MASK_XLEN(0x200));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x3));
    c.sw(Reg::X11, Reg::X1, 52);

    // rs1_val == 1024, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x400;  immval:-0x101
    c.li(Reg::X10, MASK_XLEN(0x400));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x101));
    c.sw(Reg::X11, Reg::X1, 56);

    // rs1_val == 2048, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x800;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x800));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 60);

    // rs1_val == 4096, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x1000;  immval:-0x5
    c.li(Reg::X10, MASK_XLEN(0x1000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x5));
    c.sw(Reg::X11, Reg::X1, 64);

    // rs1_val == 8192, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2000;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x2000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 68);

    // rs1_val == 32768, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x8000;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x8000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 72);

    // rs1_val == 65536, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x10000;  immval:-0x1
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x1));
    c.sw(Reg::X11, Reg::X1, 76);

    // rs1_val == 131072, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x20000;  immval:-0x2
    c.li(Reg::X10, MASK_XLEN(0x20000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2));
    c.sw(Reg::X11, Reg::X1, 80);

    // rs1_val == 262144, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x40000;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x40000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 84);

    // rs1_val == 524288, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x80000;  immval:-0x41
    c.li(Reg::X10, MASK_XLEN(0x80000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x41));
    c.sw(Reg::X11, Reg::X1, 88);

    // rs1_val == 1048576, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x100000;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x100000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 92);

    // rs1_val == 2097152, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x200000;  immval:-0x11
    c.li(Reg::X10, MASK_XLEN(0x200000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x11));
    c.sw(Reg::X11, Reg::X1, 96);

    // rs1_val == 4194304, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x400000;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x400000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 100);

    // rs1_val == 16777216, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x1000000;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x1000000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 104);

    // rs1_val == 33554432, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2000000;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x2000000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 108);

    // rs1_val == 67108864, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4000000;  immval:-0x41
    c.li(Reg::X10, MASK_XLEN(0x4000000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x41));
    c.sw(Reg::X11, Reg::X1, 112);

    // rs1_val == 134217728, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x8000000;  immval:0x7ff
    c.li(Reg::X10, MASK_XLEN(0x8000000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x7ff));
    c.sw(Reg::X11, Reg::X1, 116);

    // rs1_val == 268435456, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x10000000;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x10000000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 120);

    // rs1_val == 536870912, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x20000000;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x20000000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 124);

    // rs1_val == 1073741824, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x40000000;  immval:-0x11
    c.li(Reg::X10, MASK_XLEN(0x40000000));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x11));
    c.sw(Reg::X11, Reg::X1, 128);

    // rs1_val == -2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x2;  immval:0x1
    c.li(Reg::X10, MASK_XLEN(-0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x1));
    c.sw(Reg::X11, Reg::X1, 132);

    // rs1_val == -3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x3;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 136);

    // rs1_val == -5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x5;  immval:0x100
    c.li(Reg::X10, MASK_XLEN(-0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x100));
    c.sw(Reg::X11, Reg::X1, 140);

    // rs1_val == -9, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x9;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0x9));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 144);

    // rs1_val == -33, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x21;  immval:0x1
    c.li(Reg::X10, MASK_XLEN(-0x21));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x1));
    c.sw(Reg::X11, Reg::X1, 148);

    // rs1_val == -65, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x41;  immval:-0x4
    c.li(Reg::X10, MASK_XLEN(-0x41));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x4));
    c.sw(Reg::X11, Reg::X1, 152);

    // rs1_val == -257, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x101;  immval:-0x800
    c.li(Reg::X10, MASK_XLEN(-0x101));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x800));
    c.sw(Reg::X11, Reg::X1, 156);

    // rs1_val == -513, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x201;  immval:0x9
    c.li(Reg::X10, MASK_XLEN(-0x201));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x9));
    c.sw(Reg::X11, Reg::X1, 160);

    // rs1_val == -2049, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x801;  immval:-0x8
    c.li(Reg::X10, MASK_XLEN(-0x801));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x8));
    c.sw(Reg::X11, Reg::X1, 164);

    // rs1_val == -16385, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x4001;  immval:0x400
    c.li(Reg::X10, MASK_XLEN(-0x4001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x400));
    c.sw(Reg::X11, Reg::X1, 168);

    // rs1_val == -32769, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x8001;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(-0x8001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 172);

    // rs1_val == -65537, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x10001;  immval:-0x101
    c.li(Reg::X10, MASK_XLEN(-0x10001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x101));
    c.sw(Reg::X11, Reg::X1, 176);

    // rs1_val == -131073, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x20001;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0x20001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 180);

    // rs1_val == -2097153, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x200001;  immval:-0x6
    c.li(Reg::X10, MASK_XLEN(-0x200001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x6));
    c.sw(Reg::X11, Reg::X1, 184);

    // rs1_val == -4194305, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x400001;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(-0x400001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 188);

    // rs1_val == -8388609, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x800001;  immval:0x7
    c.li(Reg::X10, MASK_XLEN(-0x800001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x7));
    c.sw(Reg::X11, Reg::X1, 192);

    // rs1_val == -16777217, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x1000001;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0x1000001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 196);

    // rs1_val == -33554433, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x2000001;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(-0x2000001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 200);

    // rs1_val == -67108865, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x4000001;  immval:-0x4
    c.li(Reg::X10, MASK_XLEN(-0x4000001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x4));
    c.sw(Reg::X11, Reg::X1, 204);

    // rs1_val == -134217729, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x8000001;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0x8000001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 208);

    // rs1_val == -536870913, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x20000001;  immval:0x10
    c.li(Reg::X10, MASK_XLEN(-0x20000001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x10));
    c.sw(Reg::X11, Reg::X1, 212);

    // rs1_val == -1073741825, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x40000001;  immval:-0x101
    c.li(Reg::X10, MASK_XLEN(-0x40000001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x101));
    c.sw(Reg::X11, Reg::X1, 216);

    // rs1_val == 1431655765, rs1_val==1431655765 and imm_val==0
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 220);

    // rs1_val==3 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 224);

    // rs1_val==3 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 228);

    // rs1_val==3 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 232);

    // rs1_val==3 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 236);

    // rs1_val==3 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 240);

    // rs1_val==3 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 244);

    // rs1_val==3 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 248);

    // rs1_val==3 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 252);

    // rs1_val==3 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 256);

    // rs1_val==3 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 260);

    // rs1_val==3 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 264);

    // rs1_val==3 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 268);

    // rs1_val==3 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 272);

    // rs1_val==3 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 276);

    // rs1_val==3 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 280);

    // rs1_val==3 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 284);

    // rs1_val==3 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 288);

    // rs1_val==3 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 292);

    // rs1_val==3 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 296);

    // rs1_val==3 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 300);

    // rs1_val==3 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 304);

    // rs1_val==3 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x3;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 308);

    // rs1_val==1431655765 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 312);

    // rs1_val==1431655765 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 316);

    // rs1_val==1431655765 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 320);

    // rs1_val==1431655765 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 324);

    // rs1_val==1431655765 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 328);

    // rs1_val==1431655765 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 332);

    // rs1_val==1431655765 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 336);

    // rs1_val==1431655765 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 340);

    // rs1_val==1431655765 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 344);

    // rs1_val==1431655765 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 348);

    // rs1_val==1431655765 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 352);

    // rs1_val==1431655765 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 356);

    // rs1_val==1431655765 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 360);

    // rs1_val==1431655765 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 364);

    // rs1_val==1431655765 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 368);

    // rs1_val==1431655765 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 372);

    // rs1_val==1431655765 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 376);

    // rs1_val==1431655765 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 380);

    // rs1_val==1431655765 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 384);

    // rs1_val==1431655765 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 388);

    // rs1_val==1431655765 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 392);

    // rs1_val==-1431655766 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 396);

    // rs1_val==-1431655766 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 400);

    // rs1_val==-1431655766 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 404);

    // rs1_val==-1431655766 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 408);

    // rs1_val==-1431655766 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 412);

    // rs1_val==-1431655766 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 416);

    // rs1_val==-1431655766 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 420);

    // rs1_val==-1431655766 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 424);

    // rs1_val==-1431655766 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 428);

    // rs1_val==-1431655766 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 432);

    // rs1_val==-1431655766 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 436);

    // rs1_val==-1431655766 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 440);

    // rs1_val==-1431655766 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 444);

    // rs1_val==-1431655766 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 448);

    // rs1_val==-1431655766 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 452);

    // rs1_val==-1431655766 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 456);

    // rs1_val==-1431655766 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 460);

    // rs1_val==-1431655766 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 464);

    // rs1_val==-1431655766 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 468);

    // rs1_val==-1431655766 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 472);

    // rs1_val==-1431655766 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 476);

    // rs1_val==-1431655766 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 480);

    // rs1_val==5 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 484);

    // rs1_val==5 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 488);

    // rs1_val==5 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 492);

    // rs1_val==5 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 496);

    // rs1_val==5 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 500);

    // rs1_val==5 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 504);

    // rs1_val==5 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 508);

    // rs1_val==5 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 512);

    // rs1_val==5 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 516);

    // rs1_val==5 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 520);

    // rs1_val==5 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 524);

    // rs1_val==5 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 528);

    // rs1_val==5 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 532);

    // rs1_val==5 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 536);

    // rs1_val==5 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 540);

    // rs1_val==5 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 544);

    // rs1_val==5 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 548);

    // rs1_val==5 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 552);

    // rs1_val==5 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 556);

    // rs1_val==5 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 560);

    // rs1_val==5 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 564);

    // rs1_val==5 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x5;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 568);

    // rs1_val==858993459 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 572);

    // rs1_val==858993459 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 576);

    // rs1_val==858993459 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 580);

    // rs1_val==858993459 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 584);

    // rs1_val==858993459 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 588);

    // rs1_val==858993459 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 592);

    // rs1_val==858993459 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 596);

    // rs1_val==858993459 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 600);

    // rs1_val==858993459 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 604);

    // rs1_val==858993459 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 608);

    // rs1_val==858993459 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 612);

    // rs1_val==858993459 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 616);

    // rs1_val==858993459 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 620);

    // rs1_val==858993459 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 624);

    // rs1_val==858993459 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 628);

    // rs1_val==858993459 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 632);

    // rs1_val==858993459 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 636);

    // rs1_val==858993459 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 640);

    // rs1_val==858993459 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 644);

    // rs1_val==858993459 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 648);

    // rs1_val==858993459 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 652);

    // rs1_val==858993459 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 656);

    // rs1_val==1717986918 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 660);

    // rs1_val==1717986918 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 664);

    // rs1_val==1717986918 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 668);

    // rs1_val==1717986918 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 672);

    // rs1_val==1717986918 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 676);

    // rs1_val==1717986918 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 680);

    // rs1_val==1717986918 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 684);

    // rs1_val==1717986918 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 688);

    // rs1_val==1717986918 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 692);

    // rs1_val==1717986918 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 696);

    // rs1_val==1717986918 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 700);

    // rs1_val==1717986918 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 704);

    // rs1_val==1717986918 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 708);

    // rs1_val==1717986918 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 712);

    // rs1_val==1717986918 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 716);

    // rs1_val==1717986918 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 720);

    // rs1_val==1717986918 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 724);

    // rs1_val==1717986918 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 728);

    // rs1_val==1717986918 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 732);

    // rs1_val==1717986918 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 736);

    // rs1_val==1717986918 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 740);

    // rs1_val==1717986918 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 744);

    // rs1_val==-46340 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 748);

    // rs1_val==-46340 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 752);

    // rs1_val==-46340 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 756);

    // rs1_val==-46340 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 760);

    // rs1_val==-46340 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 764);

    // rs1_val==-46340 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 768);

    // rs1_val==-46340 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 772);

    // rs1_val==-46340 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 776);

    // rs1_val==-46340 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 780);

    // rs1_val==-46340 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 784);

    // rs1_val==-46340 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 788);

    // rs1_val==-46340 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 792);

    // rs1_val==-46340 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 796);

    // rs1_val==-46340 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 800);

    // rs1_val==-46340 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 804);

    // rs1_val==-46340 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 808);

    // rs1_val==-46340 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 812);

    // rs1_val==-46340 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 816);

    // rs1_val==-46340 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 820);

    // rs1_val==-46340 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 824);

    // rs1_val==-46340 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 828);

    // rs1_val==-46340 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 832);

    // rs1_val==46340 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 836);

    // rs1_val==46340 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 840);

    // rs1_val==46340 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 844);

    // rs1_val==46340 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 848);

    // rs1_val==46340 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 852);

    // rs1_val==46340 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 856);

    // rs1_val==46340 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 860);

    // rs1_val==46340 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 864);

    // rs1_val==46340 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 868);

    // rs1_val==46340 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 872);

    // rs1_val==46340 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 876);

    // rs1_val==46340 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 880);

    // rs1_val==46340 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 884);

    // rs1_val==46340 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 888);

    // rs1_val==46340 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 892);

    // rs1_val==46340 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 896);

    // rs1_val==46340 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 900);

    // rs1_val==46340 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 904);

    // rs1_val==46340 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 908);

    // rs1_val==46340 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 912);

    // rs1_val==46340 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 916);

    // rs1_val==46340 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 920);

    // rs1_val==2 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 924);

    // rs1_val==2 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 928);

    // rs1_val==2 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 932);

    // rs1_val==2 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 936);

    // rs1_val==2 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 940);

    // rs1_val==2 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 944);

    // rs1_val==2 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 948);

    // rs1_val==2 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 952);

    // rs1_val==2 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 956);

    // rs1_val==2 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 960);

    // rs1_val==2 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 964);

    // rs1_val==2 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 968);

    // rs1_val==2 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 972);

    // rs1_val==2 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 976);

    // rs1_val==2 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 980);

    // rs1_val==2 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 984);

    // rs1_val==2 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 988);

    // rs1_val==2 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 992);

    // rs1_val==2 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 996);

    // rs1_val==2 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 1000);

    // rs1_val==2 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 1004);

    // rs1_val==2 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x2;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 1008);

    // rs1_val==1431655764 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 1012);

    // rs1_val==1431655764 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 1016);

    // rs1_val==1431655764 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 1020);

    // rs1_val==1431655764 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 1024);

    // rs1_val==1431655764 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 1028);

    // rs1_val==1431655764 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 1032);

    // rs1_val==1431655764 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 1036);

    // rs1_val==1431655764 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 1040);

    // rs1_val==1431655764 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 1044);

    // rs1_val==1431655764 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 1048);

    // rs1_val==1431655764 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 1052);

    // rs1_val==1431655764 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 1056);

    // rs1_val==1431655764 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 1060);

    // rs1_val==1431655764 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 1064);

    // rs1_val==1431655764 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 1068);

    // rs1_val==1431655764 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 1072);

    // rs1_val==1431655764 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 1076);

    // rs1_val==1431655764 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 1080);

    // rs1_val==1431655764 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 1084);

    // rs1_val==1431655764 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 1088);

    // rs1_val==1431655764 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 1092);

    // rs1_val==1431655764 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 1096);

    // rs1_val==0 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 1100);

    // rs1_val==0 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 1104);

    // rs1_val==0 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 1108);

    // rs1_val==0 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 1112);

    // rs1_val==0 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 1116);

    // rs1_val==0 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 1120);

    // rs1_val==858993460 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 1124);

    // rs1_val==858993460 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 1128);

    // rs1_val==858993460 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 1132);

    // rs1_val==858993460 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 1136);

    // rs1_val==858993460 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 1140);

    // rs1_val==858993460 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 1144);

    // rs1_val==858993460 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 1148);

    // rs1_val==858993460 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 1152);

    // rs1_val==858993460 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 1156);

    // rs1_val==858993460 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 1160);

    // rs1_val==1717986919 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 1164);

    // rs1_val==1717986919 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 1168);

    // rs1_val==1717986919 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 1172);

    // rs1_val==1717986919 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 1176);

    // rs1_val==1717986919 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 1180);

    // rs1_val==1717986919 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 1184);

    // rs1_val==1717986919 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 1188);

    // rs1_val==1717986919 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 1192);

    // rs1_val==1717986919 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 1196);

    // rs1_val==1717986919 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 1200);

    // rs1_val==1717986919 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 1204);

    // rs1_val==1717986919 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 1208);

    // rs1_val==1717986919 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 1212);

    // rs1_val==1717986919 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 1216);

    // rs1_val==1717986919 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 1220);

    // rs1_val==1717986919 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 1224);

    // rs1_val==1717986919 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 1228);

    // rs1_val==1717986919 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 1232);

    // rs1_val==1717986919 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 1236);

    // rs1_val==1717986919 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 1240);

    // rs1_val==1717986919 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 1244);

    // rs1_val==1717986919 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 1248);

    // rs1_val==-46339 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 1252);

    // rs1_val==-46339 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 1256);

    // rs1_val==-46339 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 1260);

    // rs1_val==-46339 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 1264);

    // rs1_val==-46339 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 1268);

    // rs1_val==-46339 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 1272);

    // rs1_val==-46339 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 1276);

    // rs1_val==-46339 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 1280);

    // rs1_val==-46339 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 1284);

    // rs1_val==-46339 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 1288);

    // rs1_val==-46339 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 1292);

    // rs1_val==-46339 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 1296);

    // rs1_val==-46339 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 1300);

    // rs1_val==-46339 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 1304);

    // rs1_val==-46339 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 1308);

    // rs1_val==-46339 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 1312);

    // rs1_val==-46339 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 1316);

    // rs1_val==-46339 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 1320);

    // rs1_val==-46339 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 1324);

    // rs1_val==-46339 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 1328);

    // rs1_val==-46339 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 1332);

    // rs1_val==-46339 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 1336);

    // rs1_val==46341 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 1340);

    // rs1_val==46341 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 1344);

    // rs1_val==46341 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 1348);

    // rs1_val==46341 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 1352);

    // rs1_val==46341 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 1356);

    // rs1_val==46341 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 1360);

    // rs1_val==46341 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 1364);

    // rs1_val==46341 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 1368);

    // rs1_val==46341 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 1372);

    // rs1_val==46341 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 1376);

    // rs1_val==46341 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 1380);

    // rs1_val==46341 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 1384);

    // rs1_val==46341 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 1388);

    // rs1_val==46341 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 1392);

    // rs1_val==46341 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 1396);

    // rs1_val==46341 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 1400);

    // rs1_val==46341 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 1404);

    // rs1_val==46341 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 1408);

    // rs1_val==46341 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 1412);

    // rs1_val==46341 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 1416);

    // rs1_val==46341 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 1420);

    // rs1_val==46341 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 1424);

    // rs1_val==0 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 1428);

    // rs1_val==0 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 1432);

    // rs1_val==0 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 1436);

    // rs1_val==0 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 1440);

    // rs1_val==0 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 1444);

    // rs1_val==0 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 1448);

    // rs1_val==0 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 1452);

    // rs1_val==0 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 1456);

    // rs1_val==0 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 1460);

    // rs1_val==0 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 1464);

    // rs1_val==0 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 1468);

    // rs1_val==0 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 1472);

    // rs1_val==0 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 1476);

    // rs1_val==0 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 1480);

    // rs1_val==0 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x0;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 1484);

    // rs1_val==4 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 1488);

    // rs1_val==4 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 1492);

    // rs1_val==4 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 1496);

    // rs1_val==4 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 1500);

    // rs1_val==4 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 1504);

    // rs1_val==4 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 1508);

    // rs1_val==4 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 1512);

    // rs1_val==4 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 1516);

    // rs1_val==4 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 1520);

    // rs1_val==4 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 1524);

    // rs1_val==4 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 1528);

    // rs1_val==4 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 1532);

    // rs1_val==4 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 1536);

    // rs1_val==4 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 1540);

    // rs1_val==4 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 1544);

    // rs1_val==4 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 1548);

    // rs1_val==4 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 1552);

    // rs1_val==4 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 1556);

    // rs1_val==4 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 1560);

    // rs1_val==4 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 1564);

    // rs1_val==4 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x4;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 1568);

    // rs1_val==858993458 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 1572);

    // rs1_val==858993458 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 1576);

    // rs1_val==858993458 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 1580);

    // rs1_val==858993458 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 1584);

    // rs1_val==858993458 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 1588);

    // rs1_val==858993458 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 1592);

    // rs1_val==858993458 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 1596);

    // rs1_val==858993458 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 1600);

    // rs1_val==858993458 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 1604);

    // rs1_val==858993458 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 1608);

    // rs1_val==858993458 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 1612);

    // rs1_val==858993458 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 1616);

    // rs1_val==858993458 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 1620);

    // rs1_val==858993458 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 1624);

    // rs1_val==858993458 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 1628);

    // rs1_val==858993458 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 1632);

    // rs1_val==858993458 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 1636);

    // rs1_val==858993458 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 1640);

    // rs1_val==858993458 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 1644);

    // rs1_val==858993458 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 1648);

    // rs1_val==858993458 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 1652);

    // rs1_val==858993458 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 1656);

    // rs1_val==1717986917 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 1660);

    // rs1_val==1717986917 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 1664);

    // rs1_val==1717986917 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 1668);

    // rs1_val==1717986917 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 1672);

    // rs1_val==1717986917 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 1676);

    // rs1_val==1717986917 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 1680);

    // rs1_val==1717986917 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 1684);

    // rs1_val==1717986917 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 1688);

    // rs1_val==1717986917 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 1692);

    // rs1_val==1717986917 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 1696);

    // rs1_val==1717986917 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 1700);

    // rs1_val==1717986917 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 1704);

    // rs1_val==1717986917 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 1708);

    // rs1_val==1717986917 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 1712);

    // rs1_val==1717986917 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 1716);

    // rs1_val==1717986917 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 1720);

    // rs1_val==1717986917 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 1724);

    // rs1_val==1717986917 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 1728);

    // rs1_val==1717986917 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 1732);

    // rs1_val==1717986917 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 1736);

    // rs1_val==1717986917 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 1740);

    // rs1_val==1717986917 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 1744);

    // rs1_val==46339 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 1748);

    // rs1_val==46339 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 1752);

    // rs1_val==46339 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 1756);

    // rs1_val==46339 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 1760);

    // rs1_val==46339 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 1764);

    // rs1_val==46339 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 1768);

    // rs1_val==46339 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 1772);

    // rs1_val==46339 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 1776);

    // rs1_val==46339 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 1780);

    // rs1_val==46339 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 1784);

    // rs1_val==46339 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 1788);

    // rs1_val==46339 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 1792);

    // rs1_val==46339 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 1796);

    // rs1_val==46339 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 1800);

    // rs1_val==46339 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 1804);

    // rs1_val==46339 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 1808);

    // rs1_val==46339 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 1812);

    // rs1_val==46339 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 1816);

    // rs1_val==46339 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 1820);

    // rs1_val==46339 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 1824);

    // rs1_val==46339 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 1828);

    // rs1_val==46339 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 1832);

    // rs1_val==1431655766 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 1836);

    // rs1_val==1431655766 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 1840);

    // rs1_val==1431655766 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 1844);

    // rs1_val==1431655766 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 1848);

    // rs1_val==1431655766 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 1852);

    // rs1_val==1431655766 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 1856);

    // rs1_val==1431655766 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 1860);

    // rs1_val==1431655766 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 1864);

    // rs1_val==1431655766 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 1868);

    // rs1_val==1431655766 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 1872);

    // rs1_val==1431655766 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 1876);

    // rs1_val==1431655766 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 1880);

    // rs1_val==1431655766 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 1884);

    // rs1_val==1431655766 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 1888);

    // rs1_val==1431655766 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 1892);

    // rs1_val==1431655766 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 1896);

    // rs1_val==1431655766 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 1900);

    // rs1_val==1431655766 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 1904);

    // rs1_val==1431655766 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 1908);

    // rs1_val==1431655766 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 1912);

    // rs1_val==1431655766 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 1916);

    // rs1_val==1431655766 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 1920);

    // rs1_val==-1431655765 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 1924);

    // rs1_val==-1431655765 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 1928);

    // rs1_val==-1431655765 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 1932);

    // rs1_val==-1431655765 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 1936);

    // rs1_val==-1431655765 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 1940);

    // rs1_val==-1431655765 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 1944);

    // rs1_val==-1431655765 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 1948);

    // rs1_val==-1431655765 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 1952);

    // rs1_val==-1431655765 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 1956);

    // rs1_val==-1431655765 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 1960);

    // rs1_val==-1431655765 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 1964);

    // rs1_val==-1431655765 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 1968);

    // rs1_val==-1431655765 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 1972);

    // rs1_val==-1431655765 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 1976);

    // rs1_val==-1431655765 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 1980);

    // rs1_val==-1431655765 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 1984);

    // rs1_val==-1431655765 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 1988);

    // rs1_val==-1431655765 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 1992);

    // rs1_val==-1431655765 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 1996);

    // rs1_val==-1431655765 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 2000);

    // rs1_val==-1431655765 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 2004);

    // rs1_val==-1431655765 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 2008);

    // rs1_val==6 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 2012);

    // rs1_val==6 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 2016);

    // rs1_val==6 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 2020);

    // rs1_val==6 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 2024);

    // rs1_val==6 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 2028);

    // rs1_val==6 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 2032);

    // rs1_val==6 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 2036);

    // rs1_val==6 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 2040);

    // rs1_val==6 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 2044);

    c.li(Reg::X1, 2156);
    // rs1_val==6 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 0);

    // rs1_val==6 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 4);

    // rs1_val==6 and imm_val==4, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 8);

    // rs1_val==6 and imm_val==818, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X1, 12);

    // rs1_val==6 and imm_val==1637, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X1, 16);

    // rs1_val==6 and imm_val==44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X1, 20);

    // rs1_val==6 and imm_val==1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X1, 24);

    // rs1_val==6 and imm_val==-1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X1, 28);

    // rs1_val==6 and imm_val==6, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 32);

    // rs1_val==6 and imm_val==820, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X1, 36);

    // rs1_val==6 and imm_val==1639, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X1, 40);

    // rs1_val==6 and imm_val==-44, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X1, 44);

    // rs1_val==6 and imm_val==46, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x6;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X1, 48);

    // rs1_val==858993460 and imm_val==3, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 52);

    // rs1_val==858993460 and imm_val==1365, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X1, 56);

    // rs1_val==858993460 and imm_val==-1366, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X1, 60);

    // rs1_val==858993460 and imm_val==5, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 64);

    // rs1_val==858993460 and imm_val==819, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X1, 68);

    // rs1_val==858993460 and imm_val==1638, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X1, 72);

    // rs1_val==858993460 and imm_val==-45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X1, 76);

    // rs1_val==858993460 and imm_val==45, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X1, 80);

    // rs1_val==858993460 and imm_val==2, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 84);

    // rs1_val==858993460 and imm_val==1364, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X1, 88);

    // rs1_val==858993460 and imm_val==0, 
    // opcode: slti ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 92);

    // imm_val == -5, rs1_val == -1048577
    // opcode: slti ; op1:x10; dest:x11; op1val:-0x100001;  immval:-0x5
    c.li(Reg::X10, MASK_XLEN(-0x100001));
    c.slti(Reg::X11, Reg::X10, SEXT_IMM(-0x5));
    c.sw(Reg::X11, Reg::X1, 96);


        
    // End
    c.ecall();
    return c;
}