#include "xori-01.h"

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
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559, };
}

//----------------------------------------------------------------------------
// xori_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &xori_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
CodeGenerator xori_01::generateCode()
{
    CodeGenerator c;
    
    c.li(Reg::X2, 16);
    // rs1 != rd, rs1==x24, rd==x10, imm_val == (-2**(12-1)), imm_val == -2048, rs1_val != imm_val, rs1_val > 0 and imm_val < 0
    // opcode: xori ; op1:x24; dest:x10; op1val:0x33333334;  immval:-0x800
    c.li(Reg::X24, MASK_XLEN(0x33333334));
    c.xori(Reg::X10, Reg::X24, SEXT_IMM(-0x800));
    c.sw(Reg::X10, Reg::X2, 0);

    // rs1 == rd, rs1==x18, rd==x18, imm_val == 0, rs1_val == 4, rs1_val==4 and imm_val==0
    // opcode: xori ; op1:x18; dest:x18; op1val:0x4;  immval:0x0
    c.li(Reg::X18, MASK_XLEN(0x4));
    c.xori(Reg::X18, Reg::X18, SEXT_IMM(0x0));
    c.sw(Reg::X18, Reg::X2, 4);

    // rs1==x15, rd==x24, imm_val == (2**(12-1)-1), rs1_val < 0 and imm_val > 0, imm_val == 2047
    // opcode: xori ; op1:x15; dest:x24; op1val:-0x4;  immval:0x7ff
    c.li(Reg::X15, MASK_XLEN(-0x4));
    c.xori(Reg::X24, Reg::X15, SEXT_IMM(0x7ff));
    c.sw(Reg::X24, Reg::X2, 8);

    // rs1==x11, rd==x20, imm_val == 1, rs1_val > 0 and imm_val > 0, rs1_val == 2
    // opcode: xori ; op1:x11; dest:x20; op1val:0x2;  immval:0x1
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xori(Reg::X20, Reg::X11, SEXT_IMM(0x1));
    c.sw(Reg::X20, Reg::X2, 12);

    // rs1==x7, rd==x21, rs1_val == (-2**(xlen-1)), rs1_val == -2147483648
    // opcode: xori ; op1:x7; dest:x21; op1val:-0x80000000;  immval:0x554
    c.li(Reg::X7, MASK_XLEN(-0x80000000));
    c.xori(Reg::X21, Reg::X7, SEXT_IMM(0x554));
    c.sw(Reg::X21, Reg::X2, 16);

    // rs1==x17, rd==x27, rs1_val == 0, imm_val == -1025
    // opcode: xori ; op1:x17; dest:x27; op1val:0x0;  immval:-0x401
    c.li(Reg::X17, MASK_XLEN(0x0));
    c.xori(Reg::X27, Reg::X17, SEXT_IMM(-0x401));
    c.sw(Reg::X27, Reg::X2, 20);

    // rs1==x22, rd==x1, rs1_val == (2**(xlen-1)-1), rs1_val == 2147483647
    // opcode: xori ; op1:x22; dest:x1; op1val:0x7fffffff;  immval:-0xa
    c.li(Reg::X22, MASK_XLEN(0x7fffffff));
    c.xori(Reg::X1, Reg::X22, SEXT_IMM(-0xa));
    c.sw(Reg::X1, Reg::X2, 24);

    // rs1==x20, rd==x22, rs1_val == 1, imm_val == 4
    // opcode: xori ; op1:x20; dest:x22; op1val:0x1;  immval:0x4
    c.li(Reg::X20, MASK_XLEN(0x1));
    c.xori(Reg::X22, Reg::X20, SEXT_IMM(0x4));
    c.sw(Reg::X22, Reg::X2, 28);

    // rs1==x19, rd==x31, rs1_val == imm_val, imm_val == -513, rs1_val < 0 and imm_val < 0, rs1_val == -513
    // opcode: xori ; op1:x19; dest:x31; op1val:-0x201;  immval:-0x201
    c.li(Reg::X19, MASK_XLEN(-0x201));
    c.xori(Reg::X31, Reg::X19, SEXT_IMM(-0x201));
    c.sw(Reg::X31, Reg::X2, 32);

    // rs1==x9, rd==x5, imm_val == 2, rs1_val == -33
    // opcode: xori ; op1:x9; dest:x5; op1val:-0x21;  immval:0x2
    c.li(Reg::X9, MASK_XLEN(-0x21));
    c.xori(Reg::X5, Reg::X9, SEXT_IMM(0x2));
    c.sw(Reg::X5, Reg::X2, 36);

    // rs1==x28, rd==x29, imm_val == 8, 
    // opcode: xori ; op1:x28; dest:x29; op1val:-0x80000000;  immval:0x8
    c.li(Reg::X28, MASK_XLEN(-0x80000000));
    c.xori(Reg::X29, Reg::X28, SEXT_IMM(0x8));
    c.sw(Reg::X29, Reg::X2, 40);

    // rs1==x30, rd==x4, imm_val == 16, rs1_val == -1073741825
    // opcode: xori ; op1:x30; dest:x4; op1val:-0x40000001;  immval:0x10
    c.li(Reg::X30, MASK_XLEN(-0x40000001));
    c.xori(Reg::X4, Reg::X30, SEXT_IMM(0x10));
    c.sw(Reg::X4, Reg::X2, 44);

    // rs1==x27, rd==x8, imm_val == 32, 
    // opcode: xori ; op1:x27; dest:x8; op1val:0x7fffffff;  immval:0x20
    c.li(Reg::X27, MASK_XLEN(0x7fffffff));
    c.xori(Reg::X8, Reg::X27, SEXT_IMM(0x20));
    c.sw(Reg::X8, Reg::X2, 48);

    // rs1==x3, rd==x25, imm_val == 64, 
    // opcode: xori ; op1:x3; dest:x25; op1val:0x66666666;  immval:0x40
    c.li(Reg::X3, MASK_XLEN(0x66666666));
    c.xori(Reg::X25, Reg::X3, SEXT_IMM(0x40));
    c.sw(Reg::X25, Reg::X2, 52);

    // rs1==x31, rd==x17, imm_val == 128, rs1_val == -524289
    // opcode: xori ; op1:x31; dest:x17; op1val:-0x80001;  immval:0x80
    c.li(Reg::X31, MASK_XLEN(-0x80001));
    c.xori(Reg::X17, Reg::X31, SEXT_IMM(0x80));
    c.sw(Reg::X17, Reg::X2, 56);

    // rs1==x29, rd==x16, imm_val == 256, 
    // opcode: xori ; op1:x29; dest:x16; op1val:-0xb504;  immval:0x100
    c.li(Reg::X29, MASK_XLEN(-0xb504));
    c.xori(Reg::X16, Reg::X29, SEXT_IMM(0x100));
    c.sw(Reg::X16, Reg::X2, 60);

    // rs1==x4, rd==x6, imm_val == 512, 
    // opcode: xori ; op1:x4; dest:x6; op1val:0x0;  immval:0x200
    c.li(Reg::X4, MASK_XLEN(0x0));
    c.xori(Reg::X6, Reg::X4, SEXT_IMM(0x200));
    c.sw(Reg::X6, Reg::X2, 64);

    // rs1==x14, rd==x3, imm_val == 1024, rs1_val == -1048577
    // opcode: xori ; op1:x14; dest:x3; op1val:-0x100001;  immval:0x400
    c.li(Reg::X14, MASK_XLEN(-0x100001));
    c.xori(Reg::X3, Reg::X14, SEXT_IMM(0x400));
    c.sw(Reg::X3, Reg::X2, 68);

    // rs1==x12, rd==x15, imm_val == -2, 
    // opcode: xori ; op1:x12; dest:x15; op1val:-0x7;  immval:-0x2
    c.li(Reg::X12, MASK_XLEN(-0x7));
    c.xori(Reg::X15, Reg::X12, SEXT_IMM(-0x2));
    c.sw(Reg::X15, Reg::X2, 72);

    // rs1==x21, rd==x9, imm_val == -3, 
    // opcode: xori ; op1:x21; dest:x9; op1val:0x5;  immval:-0x3
    c.li(Reg::X21, MASK_XLEN(0x5));
    c.xori(Reg::X9, Reg::X21, SEXT_IMM(-0x3));
    c.sw(Reg::X9, Reg::X2, 76);

    // rs1==x10, rd==x19, imm_val == -5, 
    // opcode: xori ; op1:x10; dest:x19; op1val:-0xb504;  immval:-0x5
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X19, Reg::X10, SEXT_IMM(-0x5));
    c.sw(Reg::X19, Reg::X2, 80);

    // rs1==x16, rd==x26, imm_val == -9, 
    // opcode: xori ; op1:x16; dest:x26; op1val:0x1;  immval:-0x9
    c.li(Reg::X16, MASK_XLEN(0x1));
    c.xori(Reg::X26, Reg::X16, SEXT_IMM(-0x9));
    c.sw(Reg::X26, Reg::X2, 84);

    c.li(Reg::X3, 104);
    // rs1==x26, rd==x30, imm_val == -17, 
    // opcode: xori ; op1:x26; dest:x30; op1val:0x66666667;  immval:-0x11
    c.li(Reg::X26, MASK_XLEN(0x66666667));
    c.xori(Reg::X30, Reg::X26, SEXT_IMM(-0x11));
    c.sw(Reg::X30, Reg::X3, 0);

    // rs1==x1, rd==x23, imm_val == -33, rs1_val == -65
    // opcode: xori ; op1:x1; dest:x23; op1val:-0x41;  immval:-0x21
    c.li(Reg::X1, MASK_XLEN(-0x41));
    c.xori(Reg::X23, Reg::X1, SEXT_IMM(-0x21));
    c.sw(Reg::X23, Reg::X3, 4);

    // rs1==x6, rd==x0, imm_val == -65, 
    // opcode: xori ; op1:x6; dest:x0; op1val:-0x1;  immval:-0x41
    c.li(Reg::X6, MASK_XLEN(-0x1));
    c.xori(Reg::X0, Reg::X6, SEXT_IMM(-0x41));
    c.sw(Reg::X0, Reg::X3, 8);

    // rs1==x2, rd==x7, imm_val == -129, rs1_val == 1431655765
    // opcode: xori ; op1:x2; dest:x7; op1val:0x55555555;  immval:-0x81
    c.li(Reg::X2, MASK_XLEN(0x55555555));
    c.xori(Reg::X7, Reg::X2, SEXT_IMM(-0x81));
    c.sw(Reg::X7, Reg::X3, 12);

    // rs1==x0, rd==x14, imm_val == -257, 
    // opcode: xori ; op1:x0; dest:x14; op1val:0x0;  immval:-0x101
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.xori(Reg::X14, Reg::X0, SEXT_IMM(-0x101));
    c.sw(Reg::X14, Reg::X3, 16);

    // rs1==x8, rd==x13, imm_val == 1365, 
    // opcode: xori ; op1:x8; dest:x13; op1val:0x3fffffff;  immval:0x555
    c.li(Reg::X8, MASK_XLEN(0x3fffffff));
    c.xori(Reg::X13, Reg::X8, SEXT_IMM(0x555));
    c.sw(Reg::X13, Reg::X3, 20);

    // rs1==x25, rd==x12, imm_val == -1366, 
    // opcode: xori ; op1:x25; dest:x12; op1val:-0x7;  immval:-0x556
    c.li(Reg::X25, MASK_XLEN(-0x7));
    c.xori(Reg::X12, Reg::X25, SEXT_IMM(-0x556));
    c.sw(Reg::X12, Reg::X3, 24);

    // rs1==x13, rd==x11, rs1_val == 8, 
    // opcode: xori ; op1:x13; dest:x11; op1val:0x8;  immval:0x0
    c.li(Reg::X13, MASK_XLEN(0x8));
    c.xori(Reg::X11, Reg::X13, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 28);

    // rs1==x23, rd==x2, rs1_val == 16, 
    // opcode: xori ; op1:x23; dest:x2; op1val:0x10;  immval:0x1
    c.li(Reg::X23, MASK_XLEN(0x10));
    c.xori(Reg::X2, Reg::X23, SEXT_IMM(0x1));
    c.sw(Reg::X2, Reg::X3, 32);

    // rs1==x5, rd==x28, rs1_val == 32, 
    // opcode: xori ; op1:x5; dest:x28; op1val:0x20;  immval:0x555
    c.li(Reg::X5, MASK_XLEN(0x20));
    c.xori(Reg::X28, Reg::X5, SEXT_IMM(0x555));
    c.sw(Reg::X28, Reg::X3, 36);

    // rs1_val == 64, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x40;  immval:0x100
    c.li(Reg::X10, MASK_XLEN(0x40));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x100));
    c.sw(Reg::X11, Reg::X3, 40);

    // rs1_val == 128, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x80;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x80));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 44);

    // rs1_val == 256, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x100;  immval:-0x41
    c.li(Reg::X10, MASK_XLEN(0x100));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x41));
    c.sw(Reg::X11, Reg::X3, 48);

    // rs1_val == 512, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x200;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x200));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 52);

    // rs1_val == 1024, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x400;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x400));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 56);

    // rs1_val == 2048, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x800;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x800));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 60);

    // rs1_val == 4096, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x1000;  immval:0x8
    c.li(Reg::X10, MASK_XLEN(0x1000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x8));
    c.sw(Reg::X11, Reg::X3, 64);

    // rs1_val == 8192, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2000;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x2000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 68);

    // rs1_val == 16384, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4000;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x4000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 72);

    // rs1_val == 32768, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x8000;  immval:0x7ff
    c.li(Reg::X10, MASK_XLEN(0x8000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x7ff));
    c.sw(Reg::X11, Reg::X3, 76);

    // rs1_val == 65536, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x10000;  immval:0x40
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x40));
    c.sw(Reg::X11, Reg::X3, 80);

    // rs1_val == 131072, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x20000;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x20000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 84);

    // rs1_val == 262144, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x40000;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x40000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 88);

    // rs1_val == 524288, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x80000;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x80000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 92);

    // rs1_val == 1048576, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x100000;  immval:-0x6
    c.li(Reg::X10, MASK_XLEN(0x100000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x6));
    c.sw(Reg::X11, Reg::X3, 96);

    // rs1_val == 2097152, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x200000;  immval:-0x3
    c.li(Reg::X10, MASK_XLEN(0x200000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x3));
    c.sw(Reg::X11, Reg::X3, 100);

    // rs1_val == 4194304, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x400000;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x400000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 104);

    // rs1_val == 8388608, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x800000;  immval:-0x9
    c.li(Reg::X10, MASK_XLEN(0x800000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x9));
    c.sw(Reg::X11, Reg::X3, 108);

    // rs1_val == 16777216, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x1000000;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x1000000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 112);

    // rs1_val == 33554432, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2000000;  immval:-0x201
    c.li(Reg::X10, MASK_XLEN(0x2000000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x201));
    c.sw(Reg::X11, Reg::X3, 116);

    // rs1_val == 67108864, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4000000;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x4000000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 120);

    // rs1_val == 134217728, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x8000000;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x8000000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 124);

    // rs1_val == 268435456, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x10000000;  immval:-0x41
    c.li(Reg::X10, MASK_XLEN(0x10000000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x41));
    c.sw(Reg::X11, Reg::X3, 128);

    // rs1_val == 536870912, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x20000000;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x20000000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 132);

    // rs1_val == 1073741824, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x40000000;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x40000000));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 136);

    // rs1_val == -2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x2;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 140);

    // rs1_val == -3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x3;  immval:-0x3
    c.li(Reg::X10, MASK_XLEN(-0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x3));
    c.sw(Reg::X11, Reg::X3, 144);

    // rs1_val == -5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x5;  immval:-0x400
    c.li(Reg::X10, MASK_XLEN(-0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x400));
    c.sw(Reg::X11, Reg::X3, 148);

    // rs1_val == -9, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x9;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0x9));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 152);

    // rs1_val == -17, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x11;  immval:0x80
    c.li(Reg::X10, MASK_XLEN(-0x11));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x80));
    c.sw(Reg::X11, Reg::X3, 156);

    // rs1_val == -129, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x81;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0x81));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 160);

    // rs1_val == -257, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x101;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0x101));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 164);

    // rs1_val == -1025, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x401;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0x401));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 168);

    // rs1_val == -2049, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x801;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0x801));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 172);

    // rs1_val == -4097, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x1001;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0x1001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 176);

    // rs1_val == -8193, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x2001;  immval:0x7ff
    c.li(Reg::X10, MASK_XLEN(-0x2001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x7ff));
    c.sw(Reg::X11, Reg::X3, 180);

    // rs1_val == -16385, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x4001;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0x4001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 184);

    // rs1_val == -32769, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x8001;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0x8001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 188);

    // rs1_val == -65537, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x10001;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0x10001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 192);

    // rs1_val == -131073, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x20001;  immval:-0x101
    c.li(Reg::X10, MASK_XLEN(-0x20001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x101));
    c.sw(Reg::X11, Reg::X3, 196);

    // rs1_val == -262145, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x40001;  immval:-0x800
    c.li(Reg::X10, MASK_XLEN(-0x40001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x800));
    c.sw(Reg::X11, Reg::X3, 200);

    // rs1_val == -2097153, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x200001;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0x200001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 204);

    // rs1_val == -4194305, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x400001;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0x400001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 208);

    // rs1_val == -8388609, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x800001;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(-0x800001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 212);

    // rs1_val == -16777217, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x1000001;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(-0x1000001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 216);

    // rs1_val == -33554433, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x2000001;  immval:-0x8
    c.li(Reg::X10, MASK_XLEN(-0x2000001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x8));
    c.sw(Reg::X11, Reg::X3, 220);

    // rs1_val == -67108865, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x4000001;  immval:-0x2
    c.li(Reg::X10, MASK_XLEN(-0x4000001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2));
    c.sw(Reg::X11, Reg::X3, 224);

    // rs1_val == -134217729, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x8000001;  immval:-0x400
    c.li(Reg::X10, MASK_XLEN(-0x8000001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x400));
    c.sw(Reg::X11, Reg::X3, 228);

    // rs1_val == -268435457, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x10000001;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0x10000001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 232);

    // rs1_val == -536870913, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x20000001;  immval:-0x101
    c.li(Reg::X10, MASK_XLEN(-0x20000001));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x101));
    c.sw(Reg::X11, Reg::X3, 236);

    // rs1_val == -1431655766, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x200
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x200));
    c.sw(Reg::X11, Reg::X3, 240);

    // rs1_val==3 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 244);

    // rs1_val==3 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 248);

    // rs1_val==3 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 252);

    // rs1_val==3 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 256);

    // rs1_val==3 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 260);

    // rs1_val==3 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 264);

    // rs1_val==3 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 268);

    // rs1_val==3 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 272);

    // rs1_val==3 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 276);

    // rs1_val==3 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 280);

    // rs1_val==3 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 284);

    // rs1_val==3 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 288);

    // rs1_val==3 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 292);

    // rs1_val==3 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 296);

    // rs1_val==3 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 300);

    // rs1_val==3 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 304);

    // rs1_val==3 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 308);

    // rs1_val==3 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 312);

    // rs1_val==3 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 316);

    // rs1_val==3 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 320);

    // rs1_val==3 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 324);

    // rs1_val==3 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 328);

    // rs1_val==1431655765 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 332);

    // rs1_val==1431655765 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 336);

    // rs1_val==1431655765 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 340);

    // rs1_val==1431655765 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 344);

    // rs1_val==1431655765 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 348);

    // rs1_val==1431655765 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 352);

    // rs1_val==1431655765 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 356);

    // rs1_val==1431655765 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 360);

    // rs1_val==1431655765 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 364);

    // rs1_val==1431655765 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 368);

    // rs1_val==1431655765 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 372);

    // rs1_val==1431655765 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 376);

    // rs1_val==1431655765 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 380);

    // rs1_val==1431655765 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 384);

    // rs1_val==1431655765 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 388);

    // rs1_val==1431655765 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 392);

    // rs1_val==1431655765 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 396);

    // rs1_val==1431655765 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 400);

    // rs1_val==1431655765 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 404);

    // rs1_val==1431655765 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 408);

    // rs1_val==1431655765 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 412);

    // rs1_val==1431655765 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 416);

    // rs1_val==-1431655766 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 420);

    // rs1_val==-1431655766 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 424);

    // rs1_val==-1431655766 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 428);

    // rs1_val==-1431655766 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 432);

    // rs1_val==-1431655766 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 436);

    // rs1_val==-1431655766 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 440);

    // rs1_val==-1431655766 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 444);

    // rs1_val==-1431655766 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 448);

    // rs1_val==-1431655766 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 452);

    // rs1_val==-1431655766 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 456);

    // rs1_val==-1431655766 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 460);

    // rs1_val==-1431655766 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 464);

    // rs1_val==-1431655766 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 468);

    // rs1_val==-1431655766 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 472);

    // rs1_val==-1431655766 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 476);

    // rs1_val==-1431655766 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 480);

    // rs1_val==-1431655766 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 484);

    // rs1_val==-1431655766 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 488);

    // rs1_val==-1431655766 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 492);

    // rs1_val==-1431655766 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 496);

    // rs1_val==-1431655766 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 500);

    // rs1_val==-1431655766 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 504);

    // rs1_val==5 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 508);

    // rs1_val==5 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 512);

    // rs1_val==5 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 516);

    // rs1_val==5 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 520);

    // rs1_val==5 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 524);

    // rs1_val==5 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 528);

    // rs1_val==5 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 532);

    // rs1_val==5 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 536);

    // rs1_val==5 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 540);

    // rs1_val==5 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 544);

    // rs1_val==5 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 548);

    // rs1_val==5 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 552);

    // rs1_val==5 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 556);

    // rs1_val==5 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 560);

    // rs1_val==5 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 564);

    // rs1_val==5 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 568);

    // rs1_val==5 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 572);

    // rs1_val==5 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 576);

    // rs1_val==5 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 580);

    // rs1_val==5 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 584);

    // rs1_val==5 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 588);

    // rs1_val==5 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 592);

    // rs1_val==858993459 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 596);

    // rs1_val==858993459 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 600);

    // rs1_val==858993459 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 604);

    // rs1_val==858993459 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 608);

    // rs1_val==858993459 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 612);

    // rs1_val==858993459 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 616);

    // rs1_val==858993459 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 620);

    // rs1_val==858993459 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 624);

    // rs1_val==858993459 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 628);

    // rs1_val==858993459 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 632);

    // rs1_val==858993459 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 636);

    // rs1_val==858993459 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 640);

    // rs1_val==858993459 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 644);

    // rs1_val==858993459 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 648);

    // rs1_val==858993459 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 652);

    // rs1_val==858993459 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 656);

    // rs1_val==858993459 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 660);

    // rs1_val==858993459 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 664);

    // rs1_val==858993459 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 668);

    // rs1_val==858993459 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 672);

    // rs1_val==858993459 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 676);

    // rs1_val==858993459 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 680);

    // rs1_val==1717986918 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 684);

    // rs1_val==1717986918 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 688);

    // rs1_val==1717986918 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 692);

    // rs1_val==1717986918 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 696);

    // rs1_val==1717986918 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 700);

    // rs1_val==1717986918 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 704);

    // rs1_val==1717986918 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 708);

    // rs1_val==1717986918 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 712);

    // rs1_val==1717986918 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 716);

    // rs1_val==1717986918 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 720);

    // rs1_val==1717986918 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 724);

    // rs1_val==1717986918 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 728);

    // rs1_val==1717986918 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 732);

    // rs1_val==1717986918 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 736);

    // rs1_val==1717986918 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 740);

    // rs1_val==1717986918 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 744);

    // rs1_val==1717986918 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 748);

    // rs1_val==1717986918 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 752);

    // rs1_val==1717986918 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 756);

    // rs1_val==1717986918 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 760);

    // rs1_val==1717986918 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 764);

    // rs1_val==1717986918 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 768);

    // rs1_val==-46340 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 772);

    // rs1_val==-46340 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 776);

    // rs1_val==-46340 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 780);

    // rs1_val==-46340 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 784);

    // rs1_val==-46340 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 788);

    // rs1_val==-46340 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 792);

    // rs1_val==-46340 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 796);

    // rs1_val==-46340 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 800);

    // rs1_val==-46340 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 804);

    // rs1_val==-46340 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 808);

    // rs1_val==-46340 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 812);

    // rs1_val==-46340 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 816);

    // rs1_val==-46340 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 820);

    // rs1_val==-46340 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 824);

    // rs1_val==-46340 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 828);

    // rs1_val==-46340 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 832);

    // rs1_val==-46340 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 836);

    // rs1_val==-46340 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 840);

    // rs1_val==-46340 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 844);

    // rs1_val==-46340 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 848);

    // rs1_val==-46340 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 852);

    // rs1_val==-46340 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 856);

    // rs1_val==46340 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 860);

    // rs1_val==46340 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 864);

    // rs1_val==46340 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 868);

    // rs1_val==46340 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 872);

    // rs1_val==46340 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 876);

    // rs1_val==46340 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 880);

    // rs1_val==46340 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 884);

    // rs1_val==46340 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 888);

    // rs1_val==46340 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 892);

    // rs1_val==46340 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 896);

    // rs1_val==46340 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 900);

    // rs1_val==46340 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 904);

    // rs1_val==46340 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 908);

    // rs1_val==46340 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 912);

    // rs1_val==46340 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 916);

    // rs1_val==46340 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 920);

    // rs1_val==46340 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 924);

    // rs1_val==46340 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 928);

    // rs1_val==46340 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 932);

    // rs1_val==46340 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 936);

    // rs1_val==46340 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 940);

    // rs1_val==46340 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 944);

    // rs1_val==2 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 948);

    // rs1_val==2 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 952);

    // rs1_val==2 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 956);

    // rs1_val==2 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 960);

    // rs1_val==2 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 964);

    // rs1_val==2 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 968);

    // rs1_val==2 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 972);

    // rs1_val==2 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 976);

    // rs1_val==2 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 980);

    // rs1_val==2 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 984);

    // rs1_val==2 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 988);

    // rs1_val==2 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 992);

    // rs1_val==2 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 996);

    // rs1_val==2 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 1000);

    // rs1_val==2 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 1004);

    // rs1_val==2 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 1008);

    // rs1_val==2 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 1012);

    // rs1_val==2 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 1016);

    // rs1_val==2 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 1020);

    // rs1_val==2 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 1024);

    // rs1_val==2 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 1028);

    // rs1_val==2 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 1032);

    // rs1_val==1431655764 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 1036);

    // rs1_val==1431655764 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 1040);

    // rs1_val==1431655764 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 1044);

    // rs1_val==1431655764 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 1048);

    // rs1_val==1431655764 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 1052);

    // rs1_val==1431655764 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 1056);

    // rs1_val==1431655764 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 1060);

    // rs1_val==1431655764 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 1064);

    // rs1_val==1431655764 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 1068);

    // rs1_val==1431655764 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 1072);

    // rs1_val==1431655764 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 1076);

    // rs1_val==1431655764 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 1080);

    // rs1_val==1431655764 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 1084);

    // rs1_val==1431655764 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 1088);

    // rs1_val==1431655764 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 1092);

    // rs1_val==1431655764 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 1096);

    // rs1_val==1431655764 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 1100);

    // rs1_val==1431655764 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 1104);

    // rs1_val==1431655764 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 1108);

    // rs1_val==1431655764 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 1112);

    // rs1_val==1431655764 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 1116);

    // rs1_val==1431655764 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 1120);

    // rs1_val==0 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 1124);

    // rs1_val==0 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 1128);

    // rs1_val==0 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 1132);

    // rs1_val==0 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 1136);

    // rs1_val==0 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 1140);

    // rs1_val==0 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 1144);

    // rs1_val==0 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 1148);

    // rs1_val==0 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 1152);

    // rs1_val==858993460 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 1156);

    // rs1_val==858993460 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 1160);

    // rs1_val==858993460 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 1164);

    // rs1_val==858993460 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 1168);

    // rs1_val==858993460 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 1172);

    // rs1_val==858993460 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 1176);

    // rs1_val==858993460 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 1180);

    // rs1_val==858993460 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 1184);

    // rs1_val==858993460 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 1188);

    // rs1_val==858993460 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 1192);

    // rs1_val==1717986919 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 1196);

    // rs1_val==1717986919 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 1200);

    // rs1_val==1717986919 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 1204);

    // rs1_val==1717986919 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 1208);

    // rs1_val==1717986919 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 1212);

    // rs1_val==1717986919 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 1216);

    // rs1_val==1717986919 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 1220);

    // rs1_val==1717986919 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 1224);

    // rs1_val==1717986919 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 1228);

    // rs1_val==1717986919 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 1232);

    // rs1_val==1717986919 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 1236);

    // rs1_val==1717986919 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 1240);

    // rs1_val==1717986919 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 1244);

    // rs1_val==1717986919 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 1248);

    // rs1_val==1717986919 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 1252);

    // rs1_val==1717986919 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 1256);

    // rs1_val==1717986919 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 1260);

    // rs1_val==1717986919 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 1264);

    // rs1_val==1717986919 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 1268);

    // rs1_val==1717986919 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 1272);

    // rs1_val==1717986919 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 1276);

    // rs1_val==1717986919 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 1280);

    // rs1_val==-46339 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 1284);

    // rs1_val==-46339 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 1288);

    // rs1_val==-46339 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 1292);

    // rs1_val==-46339 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 1296);

    // rs1_val==-46339 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 1300);

    // rs1_val==-46339 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 1304);

    // rs1_val==-46339 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 1308);

    // rs1_val==-46339 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 1312);

    // rs1_val==-46339 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 1316);

    // rs1_val==-46339 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 1320);

    // rs1_val==-46339 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 1324);

    // rs1_val==-46339 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 1328);

    // rs1_val==-46339 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 1332);

    // rs1_val==-46339 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 1336);

    // rs1_val==-46339 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 1340);

    // rs1_val==-46339 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 1344);

    // rs1_val==-46339 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 1348);

    // rs1_val==-46339 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 1352);

    // rs1_val==-46339 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 1356);

    // rs1_val==-46339 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 1360);

    // rs1_val==-46339 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 1364);

    // rs1_val==-46339 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 1368);

    // rs1_val==46341 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 1372);

    // rs1_val==46341 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 1376);

    // rs1_val==46341 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 1380);

    // rs1_val==46341 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 1384);

    // rs1_val==46341 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 1388);

    // rs1_val==46341 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 1392);

    // rs1_val==46341 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 1396);

    // rs1_val==46341 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 1400);

    // rs1_val==46341 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 1404);

    // rs1_val==46341 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 1408);

    // rs1_val==46341 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 1412);

    // rs1_val==46341 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 1416);

    // rs1_val==46341 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 1420);

    // rs1_val==46341 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 1424);

    // rs1_val==46341 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 1428);

    // rs1_val==46341 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 1432);

    // rs1_val==46341 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 1436);

    // rs1_val==46341 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 1440);

    // rs1_val==46341 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 1444);

    // rs1_val==46341 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 1448);

    // rs1_val==46341 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 1452);

    // rs1_val==46341 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 1456);

    // rs1_val==0 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 1460);

    // rs1_val==0 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 1464);

    // rs1_val==0 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 1468);

    // rs1_val==0 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 1472);

    // rs1_val==0 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 1476);

    // rs1_val==0 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 1480);

    // rs1_val==0 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 1484);

    // rs1_val==0 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 1488);

    // rs1_val==0 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 1492);

    // rs1_val==0 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 1496);

    // rs1_val==0 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 1500);

    // rs1_val==0 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 1504);

    // rs1_val==0 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 1508);

    // rs1_val==0 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 1512);

    // rs1_val==4 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 1516);

    // rs1_val==4 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 1520);

    // rs1_val==4 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 1524);

    // rs1_val==4 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 1528);

    // rs1_val==4 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 1532);

    // rs1_val==4 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 1536);

    // rs1_val==4 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 1540);

    // rs1_val==4 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 1544);

    // rs1_val==4 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 1548);

    // rs1_val==4 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 1552);

    // rs1_val==4 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 1556);

    // rs1_val==4 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 1560);

    // rs1_val==4 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 1564);

    // rs1_val==4 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 1568);

    // rs1_val==4 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 1572);

    // rs1_val==4 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 1576);

    // rs1_val==4 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 1580);

    // rs1_val==4 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 1584);

    // rs1_val==4 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 1588);

    // rs1_val==4 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 1592);

    // rs1_val==4 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 1596);

    // rs1_val==858993458 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 1600);

    // rs1_val==858993458 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 1604);

    // rs1_val==858993458 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 1608);

    // rs1_val==858993458 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 1612);

    // rs1_val==858993458 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 1616);

    // rs1_val==858993458 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 1620);

    // rs1_val==858993458 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 1624);

    // rs1_val==858993458 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 1628);

    // rs1_val==858993458 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 1632);

    // rs1_val==858993458 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 1636);

    // rs1_val==858993458 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 1640);

    // rs1_val==858993458 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 1644);

    // rs1_val==858993458 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 1648);

    // rs1_val==858993458 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 1652);

    // rs1_val==858993458 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 1656);

    // rs1_val==858993458 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 1660);

    // rs1_val==858993458 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 1664);

    // rs1_val==858993458 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 1668);

    // rs1_val==858993458 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 1672);

    // rs1_val==858993458 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 1676);

    // rs1_val==858993458 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 1680);

    // rs1_val==858993458 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 1684);

    // rs1_val==1717986917 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 1688);

    // rs1_val==1717986917 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 1692);

    // rs1_val==1717986917 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 1696);

    // rs1_val==1717986917 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 1700);

    // rs1_val==1717986917 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 1704);

    // rs1_val==1717986917 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 1708);

    // rs1_val==1717986917 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 1712);

    // rs1_val==1717986917 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 1716);

    // rs1_val==1717986917 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 1720);

    // rs1_val==1717986917 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 1724);

    // rs1_val==1717986917 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 1728);

    // rs1_val==1717986917 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 1732);

    // rs1_val==1717986917 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 1736);

    // rs1_val==1717986917 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 1740);

    // rs1_val==1717986917 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 1744);

    // rs1_val==1717986917 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 1748);

    // rs1_val==1717986917 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 1752);

    // rs1_val==1717986917 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 1756);

    // rs1_val==1717986917 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 1760);

    // rs1_val==1717986917 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 1764);

    // rs1_val==1717986917 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 1768);

    // rs1_val==1717986917 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 1772);

    // rs1_val==46339 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 1776);

    // rs1_val==46339 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 1780);

    // rs1_val==46339 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 1784);

    // rs1_val==46339 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 1788);

    // rs1_val==46339 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 1792);

    // rs1_val==46339 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 1796);

    // rs1_val==46339 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 1800);

    // rs1_val==46339 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 1804);

    // rs1_val==46339 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 1808);

    // rs1_val==46339 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 1812);

    // rs1_val==46339 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 1816);

    // rs1_val==46339 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 1820);

    // rs1_val==46339 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 1824);

    // rs1_val==46339 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 1828);

    // rs1_val==46339 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 1832);

    // rs1_val==46339 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 1836);

    // rs1_val==46339 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 1840);

    // rs1_val==46339 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 1844);

    // rs1_val==46339 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 1848);

    // rs1_val==46339 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 1852);

    // rs1_val==46339 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 1856);

    // rs1_val==46339 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 1860);

    // rs1_val==1431655766 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 1864);

    // rs1_val==1431655766 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 1868);

    // rs1_val==1431655766 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 1872);

    // rs1_val==1431655766 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 1876);

    // rs1_val==1431655766 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 1880);

    // rs1_val==1431655766 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 1884);

    // rs1_val==1431655766 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 1888);

    // rs1_val==1431655766 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 1892);

    // rs1_val==1431655766 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 1896);

    // rs1_val==1431655766 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 1900);

    // rs1_val==1431655766 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 1904);

    // rs1_val==1431655766 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 1908);

    // rs1_val==1431655766 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 1912);

    // rs1_val==1431655766 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 1916);

    // rs1_val==1431655766 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 1920);

    // rs1_val==1431655766 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 1924);

    // rs1_val==1431655766 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 1928);

    // rs1_val==1431655766 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 1932);

    // rs1_val==1431655766 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 1936);

    // rs1_val==1431655766 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 1940);

    // rs1_val==1431655766 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 1944);

    // rs1_val==1431655766 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 1948);

    // rs1_val==-1431655765 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 1952);

    // rs1_val==-1431655765 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 1956);

    // rs1_val==-1431655765 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 1960);

    // rs1_val==-1431655765 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 1964);

    // rs1_val==-1431655765 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 1968);

    // rs1_val==-1431655765 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 1972);

    // rs1_val==-1431655765 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 1976);

    // rs1_val==-1431655765 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 1980);

    // rs1_val==-1431655765 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 1984);

    // rs1_val==-1431655765 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 1988);

    // rs1_val==-1431655765 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 1992);

    // rs1_val==-1431655765 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 1996);

    // rs1_val==-1431655765 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 2000);

    // rs1_val==-1431655765 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 2004);

    // rs1_val==-1431655765 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 2008);

    // rs1_val==-1431655765 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 2012);

    // rs1_val==-1431655765 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 2016);

    // rs1_val==-1431655765 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 2020);

    // rs1_val==-1431655765 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 2024);

    // rs1_val==-1431655765 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 2028);

    // rs1_val==-1431655765 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 2032);

    // rs1_val==-1431655765 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 2036);

    // rs1_val==6 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 2040);

    // rs1_val==6 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 2044);

    c.li(Reg::X3, 2152);
    // rs1_val==6 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 0);

    // rs1_val==6 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 4);

    // rs1_val==6 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 8);

    // rs1_val==6 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 12);

    // rs1_val==6 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 16);

    // rs1_val==6 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 20);

    // rs1_val==6 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 24);

    // rs1_val==6 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 28);

    // rs1_val==6 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 32);

    // rs1_val==6 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 36);

    // rs1_val==6 and imm_val==818, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X3, 40);

    // rs1_val==6 and imm_val==1637, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X3, 44);

    // rs1_val==6 and imm_val==44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X3, 48);

    // rs1_val==6 and imm_val==1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X3, 52);

    // rs1_val==6 and imm_val==-1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X3, 56);

    // rs1_val==6 and imm_val==6, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X3, 60);

    // rs1_val==6 and imm_val==820, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X3, 64);

    // rs1_val==6 and imm_val==1639, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X3, 68);

    // rs1_val==6 and imm_val==-44, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X3, 72);

    // rs1_val==6 and imm_val==46, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X3, 76);

    // rs1_val==858993460 and imm_val==3, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X3, 80);

    // rs1_val==858993460 and imm_val==1365, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X3, 84);

    // rs1_val==858993460 and imm_val==-1366, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X3, 88);

    // rs1_val==858993460 and imm_val==5, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X3, 92);

    // rs1_val==858993460 and imm_val==819, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X3, 96);

    // rs1_val==858993460 and imm_val==1638, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X3, 100);

    // rs1_val==858993460 and imm_val==-45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X3, 104);

    // rs1_val==858993460 and imm_val==45, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X3, 108);

    // rs1_val==858993460 and imm_val==2, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X3, 112);

    // rs1_val==858993460 and imm_val==1364, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X3, 116);

    // rs1_val==858993460 and imm_val==0, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 120);

    // rs1_val==858993460 and imm_val==4, 
    // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X3, 124);


        
    // End
    c.ecall();
    return c;
}