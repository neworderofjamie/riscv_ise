#include "slli-01.h"

// Standard C includes
#include <cassert>
#include <cstring>

// RISC-V ISE includes
#include "ise/riscv.h"

#define MASK_XLEN(val)  val&0xFFFFFFFF // shortens 64b values to 32b

#define BIT(addr, bit) (((addr)>>(bit))&1)
#define IMMSZ      12
#define IMMSGN            (IMMSZ -1)
#define IMMMSK     ( (1 << IMMSZ)-1)
#define SEXT_IMM(x)  ((x & IMMMSK) | (-BIT((x), IMMSGN)<< IMMSZ))

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
const std::vector<uint32_t> g_ScalarData{

0, 3133065982, 3133065982, 3133065982, 3133065982, 3735928559, 3735928559,
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
3735928559, 3735928559, };
}

//----------------------------------------------------------------------------
// ArchTest::slli_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &ArchTest::slli_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
std::vector<uint32_t> ArchTest::slli_01::generateCode(bool simulate)
{
    CodeGenerator c;
    
    // Ready flag is always stored in first word, zero it
    c.sw(Reg::X0, Reg::X0);
    
    
    c.li(Reg::X3, 20);
    // rs1 != rd, rs1==x17, rd==x27, rs1_val < 0 and imm_val > 0 and imm_val < xlen, rs1_val == -1073741825, imm_val == 29
    // opcode: slli ; op1:x17; dest:x27; op1val:-0x40000001;  immval:0x1d
    c.li(Reg::X17, MASK_XLEN(-0x40000001));
    c.slli(Reg::X27, Reg::X17, SEXT_IMM(0x1d));
    c.sw(Reg::X27, Reg::X3, 0);

    // rs1 == rd, rs1==x26, rd==x26, rs1_val > 0 and imm_val > 0 and imm_val < xlen, rs1_val==1717986918, imm_val == 15
    // opcode: slli ; op1:x26; dest:x26; op1val:0x66666666;  immval:0xf
    c.li(Reg::X26, MASK_XLEN(0x66666666));
    c.slli(Reg::X26, Reg::X26, SEXT_IMM(0xf));
    c.sw(Reg::X26, Reg::X3, 4);

    // rs1==x22, rd==x11, rs1_val < 0 and imm_val == 0, rs1_val == -65537
    // opcode: slli ; op1:x22; dest:x11; op1val:-0x10001;  immval:0x0
    c.li(Reg::X22, MASK_XLEN(-0x10001));
    c.slli(Reg::X11, Reg::X22, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X3, 8);

    // rs1==x15, rd==x6, rs1_val > 0 and imm_val == 0, rs1_val==4, rs1_val == 4
    // opcode: slli ; op1:x15; dest:x6; op1val:0x4;  immval:0x0
    c.li(Reg::X15, MASK_XLEN(0x4));
    c.slli(Reg::X6, Reg::X15, SEXT_IMM(0x0));
    c.sw(Reg::X6, Reg::X3, 12);

    // rs1==x9, rd==x16, rs1_val < 0 and imm_val == (xlen-1), rs1_val == -4194305
    // opcode: slli ; op1:x9; dest:x16; op1val:-0x400001;  immval:0x1f
    c.li(Reg::X9, MASK_XLEN(-0x400001));
    c.slli(Reg::X16, Reg::X9, SEXT_IMM(0x1f));
    c.sw(Reg::X16, Reg::X3, 16);

    // rs1==x11, rd==x20, rs1_val > 0 and imm_val == (xlen-1), 
    // opcode: slli ; op1:x11; dest:x20; op1val:0x4;  immval:0x1f
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.slli(Reg::X20, Reg::X11, SEXT_IMM(0x1f));
    c.sw(Reg::X20, Reg::X3, 20);

    // rs1==x1, rd==x19, rs1_val == imm_val and imm_val > 0 and imm_val < xlen, imm_val == 8, rs1_val == 8
    // opcode: slli ; op1:x1; dest:x19; op1val:0x8;  immval:0x8
    c.li(Reg::X1, MASK_XLEN(0x8));
    c.slli(Reg::X19, Reg::X1, SEXT_IMM(0x8));
    c.sw(Reg::X19, Reg::X3, 24);

    // rs1==x19, rd==x25, rs1_val == (-2**(xlen-1)) and imm_val >= 0 and imm_val < xlen, rs1_val == -2147483648, imm_val == 16
    // opcode: slli ; op1:x19; dest:x25; op1val:-0x80000000;  immval:0x10
    c.li(Reg::X19, MASK_XLEN(-0x80000000));
    c.slli(Reg::X25, Reg::X19, SEXT_IMM(0x10));
    c.sw(Reg::X25, Reg::X3, 28);

    // rs1==x8, rd==x12, rs1_val == 0 and imm_val >= 0 and imm_val < xlen, rs1_val==0
    // opcode: slli ; op1:x8; dest:x12; op1val:0x0;  immval:0xc
    c.li(Reg::X8, MASK_XLEN(0x0));
    c.slli(Reg::X12, Reg::X8, SEXT_IMM(0xc));
    c.sw(Reg::X12, Reg::X3, 32);

    // rs1==x27, rd==x30, rs1_val == (2**(xlen-1)-1) and imm_val >= 0 and imm_val < xlen, rs1_val == 2147483647
    // opcode: slli ; op1:x27; dest:x30; op1val:0x7fffffff;  immval:0x8
    c.li(Reg::X27, MASK_XLEN(0x7fffffff));
    c.slli(Reg::X30, Reg::X27, SEXT_IMM(0x8));
    c.sw(Reg::X30, Reg::X3, 36);

    // rs1==x2, rd==x4, rs1_val == 1 and imm_val >= 0 and imm_val < xlen, imm_val == 1, rs1_val == 1
    // opcode: slli ; op1:x2; dest:x4; op1val:0x1;  immval:0x1
    c.li(Reg::X2, MASK_XLEN(0x1));
    c.slli(Reg::X4, Reg::X2, SEXT_IMM(0x1));
    c.sw(Reg::X4, Reg::X3, 40);

    // rs1==x31, rd==x14, rs1_val == 2, rs1_val==2
    // opcode: slli ; op1:x31; dest:x14; op1val:0x2;  immval:0x6
    c.li(Reg::X31, MASK_XLEN(0x2));
    c.slli(Reg::X14, Reg::X31, SEXT_IMM(0x6));
    c.sw(Reg::X14, Reg::X3, 44);

    // rs1==x24, rd==x17, rs1_val == 16, 
    // opcode: slli ; op1:x24; dest:x17; op1val:0x10;  immval:0xe
    c.li(Reg::X24, MASK_XLEN(0x10));
    c.slli(Reg::X17, Reg::X24, SEXT_IMM(0xe));
    c.sw(Reg::X17, Reg::X3, 48);

    // rs1==x4, rd==x10, rs1_val == 32, 
    // opcode: slli ; op1:x4; dest:x10; op1val:0x20;  immval:0x3
    c.li(Reg::X4, MASK_XLEN(0x20));
    c.slli(Reg::X10, Reg::X4, SEXT_IMM(0x3));
    c.sw(Reg::X10, Reg::X3, 52);

    // rs1==x18, rd==x2, rs1_val == 64, imm_val == 21
    // opcode: slli ; op1:x18; dest:x2; op1val:0x40;  immval:0x15
    c.li(Reg::X18, MASK_XLEN(0x40));
    c.slli(Reg::X2, Reg::X18, SEXT_IMM(0x15));
    c.sw(Reg::X2, Reg::X3, 56);

    // rs1==x5, rd==x23, rs1_val == 128, 
    // opcode: slli ; op1:x5; dest:x23; op1val:0x80;  immval:0x15
    c.li(Reg::X5, MASK_XLEN(0x80));
    c.slli(Reg::X23, Reg::X5, SEXT_IMM(0x15));
    c.sw(Reg::X23, Reg::X3, 60);

    // rs1==x13, rd==x8, rs1_val == 256, 
    // opcode: slli ; op1:x13; dest:x8; op1val:0x100;  immval:0x1
    c.li(Reg::X13, MASK_XLEN(0x100));
    c.slli(Reg::X8, Reg::X13, SEXT_IMM(0x1));
    c.sw(Reg::X8, Reg::X3, 64);

    // rs1==x20, rd==x0, rs1_val == 512, 
    // opcode: slli ; op1:x20; dest:x0; op1val:0x200;  immval:0x0
    c.li(Reg::X20, MASK_XLEN(0x200));
    c.slli(Reg::X0, Reg::X20, SEXT_IMM(0x0));
    c.sw(Reg::X0, Reg::X3, 68);

    // rs1==x16, rd==x9, rs1_val == 1024, imm_val == 2
    // opcode: slli ; op1:x16; dest:x9; op1val:0x400;  immval:0x2
    c.li(Reg::X16, MASK_XLEN(0x400));
    c.slli(Reg::X9, Reg::X16, SEXT_IMM(0x2));
    c.sw(Reg::X9, Reg::X3, 72);

    // rs1==x21, rd==x5, rs1_val == 2048, 
    // opcode: slli ; op1:x21; dest:x5; op1val:0x800;  immval:0x13
    c.li(Reg::X21, MASK_XLEN(0x800));
    c.slli(Reg::X5, Reg::X21, SEXT_IMM(0x13));
    c.sw(Reg::X5, Reg::X3, 76);

    // rs1==x23, rd==x1, rs1_val == 4096, 
    // opcode: slli ; op1:x23; dest:x1; op1val:0x1000;  immval:0x7
    c.li(Reg::X23, MASK_XLEN(0x1000));
    c.slli(Reg::X1, Reg::X23, SEXT_IMM(0x7));
    c.sw(Reg::X1, Reg::X3, 80);

    // rs1==x12, rd==x18, rs1_val == 8192, 
    // opcode: slli ; op1:x12; dest:x18; op1val:0x2000;  immval:0x10
    c.li(Reg::X12, MASK_XLEN(0x2000));
    c.slli(Reg::X18, Reg::X12, SEXT_IMM(0x10));
    c.sw(Reg::X18, Reg::X3, 84);

    // rs1==x29, rd==x15, rs1_val == 16384, 
    // opcode: slli ; op1:x29; dest:x15; op1val:0x4000;  immval:0xb
    c.li(Reg::X29, MASK_XLEN(0x4000));
    c.slli(Reg::X15, Reg::X29, SEXT_IMM(0xb));
    c.sw(Reg::X15, Reg::X3, 88);

    c.li(Reg::X1, 112);
    // rs1==x3, rd==x21, rs1_val == 32768, imm_val == 23
    // opcode: slli ; op1:x3; dest:x21; op1val:0x8000;  immval:0x17
    c.li(Reg::X3, MASK_XLEN(0x8000));
    c.slli(Reg::X21, Reg::X3, SEXT_IMM(0x17));
    c.sw(Reg::X21, Reg::X1, 0);

    // rs1==x0, rd==x31, rs1_val == 65536, 
    // opcode: slli ; op1:x0; dest:x31; op1val:0x0;  immval:0x1
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.slli(Reg::X31, Reg::X0, SEXT_IMM(0x1));
    c.sw(Reg::X31, Reg::X1, 4);

    // rs1==x14, rd==x3, rs1_val == 131072, imm_val == 27
    // opcode: slli ; op1:x14; dest:x3; op1val:0x20000;  immval:0x1b
    c.li(Reg::X14, MASK_XLEN(0x20000));
    c.slli(Reg::X3, Reg::X14, SEXT_IMM(0x1b));
    c.sw(Reg::X3, Reg::X1, 8);

    // rs1==x25, rd==x24, rs1_val == 262144, 
    // opcode: slli ; op1:x25; dest:x24; op1val:0x40000;  immval:0x1f
    c.li(Reg::X25, MASK_XLEN(0x40000));
    c.slli(Reg::X24, Reg::X25, SEXT_IMM(0x1f));
    c.sw(Reg::X24, Reg::X1, 12);

    // rs1==x30, rd==x29, rs1_val == 524288, 
    // opcode: slli ; op1:x30; dest:x29; op1val:0x80000;  immval:0xf
    c.li(Reg::X30, MASK_XLEN(0x80000));
    c.slli(Reg::X29, Reg::X30, SEXT_IMM(0xf));
    c.sw(Reg::X29, Reg::X1, 16);

    // rs1==x28, rd==x13, rs1_val == 1048576, 
    // opcode: slli ; op1:x28; dest:x13; op1val:0x100000;  immval:0x17
    c.li(Reg::X28, MASK_XLEN(0x100000));
    c.slli(Reg::X13, Reg::X28, SEXT_IMM(0x17));
    c.sw(Reg::X13, Reg::X1, 20);

    // rs1==x10, rd==x7, rs1_val == 2097152, 
    // opcode: slli ; op1:x10; dest:x7; op1val:0x200000;  immval:0x7
    c.li(Reg::X10, MASK_XLEN(0x200000));
    c.slli(Reg::X7, Reg::X10, SEXT_IMM(0x7));
    c.sw(Reg::X7, Reg::X1, 24);

    // rs1==x7, rd==x22, rs1_val == 4194304, 
    // opcode: slli ; op1:x7; dest:x22; op1val:0x400000;  immval:0x1d
    c.li(Reg::X7, MASK_XLEN(0x400000));
    c.slli(Reg::X22, Reg::X7, SEXT_IMM(0x1d));
    c.sw(Reg::X22, Reg::X1, 28);

    // rs1==x6, rd==x28, rs1_val == 8388608, 
    // opcode: slli ; op1:x6; dest:x28; op1val:0x800000;  immval:0x2
    c.li(Reg::X6, MASK_XLEN(0x800000));
    c.slli(Reg::X28, Reg::X6, SEXT_IMM(0x2));
    c.sw(Reg::X28, Reg::X1, 32);

    // rs1_val == 16777216, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x1000000;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x1000000));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 36);

    // rs1_val == 33554432, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x2000000;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x2000000));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X1, 40);

    // rs1_val == 67108864, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x4000000;  immval:0x11
    c.li(Reg::X10, MASK_XLEN(0x4000000));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x11));
    c.sw(Reg::X11, Reg::X1, 44);

    // rs1_val == 134217728, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x8000000;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x8000000));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 48);

    // rs1_val == 268435456, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x10000000;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x10000000));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 52);

    // rs1_val == 536870912, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x20000000;  immval:0xc
    c.li(Reg::X10, MASK_XLEN(0x20000000));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xc));
    c.sw(Reg::X11, Reg::X1, 56);

    // rs1_val == 1073741824, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x40000000;  immval:0xb
    c.li(Reg::X10, MASK_XLEN(0x40000000));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xb));
    c.sw(Reg::X11, Reg::X1, 60);

    // rs1_val == -2, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x2;  immval:0xe
    c.li(Reg::X10, MASK_XLEN(-0x2));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xe));
    c.sw(Reg::X11, Reg::X1, 64);

    // rs1_val == -3, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x3;  immval:0x1
    c.li(Reg::X10, MASK_XLEN(-0x3));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x1));
    c.sw(Reg::X11, Reg::X1, 68);

    // rs1_val == -5, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x5;  immval:0x7
    c.li(Reg::X10, MASK_XLEN(-0x5));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x7));
    c.sw(Reg::X11, Reg::X1, 72);

    // rs1_val == -9, imm_val == 10
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x9;  immval:0xa
    c.li(Reg::X10, MASK_XLEN(-0x9));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xa));
    c.sw(Reg::X11, Reg::X1, 76);

    // rs1_val == -17, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x11;  immval:0x11
    c.li(Reg::X10, MASK_XLEN(-0x11));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x11));
    c.sw(Reg::X11, Reg::X1, 80);

    // rs1_val == -33, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x21;  immval:0xe
    c.li(Reg::X10, MASK_XLEN(-0x21));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xe));
    c.sw(Reg::X11, Reg::X1, 84);

    // rs1_val == -65, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x41;  immval:0x13
    c.li(Reg::X10, MASK_XLEN(-0x41));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x13));
    c.sw(Reg::X11, Reg::X1, 88);

    // rs1_val == -129, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x81;  immval:0x10
    c.li(Reg::X10, MASK_XLEN(-0x81));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x10));
    c.sw(Reg::X11, Reg::X1, 92);

    // rs1_val == -257, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x101;  immval:0xb
    c.li(Reg::X10, MASK_XLEN(-0x101));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xb));
    c.sw(Reg::X11, Reg::X1, 96);

    // rs1_val == -513, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x201;  immval:0x11
    c.li(Reg::X10, MASK_XLEN(-0x201));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x11));
    c.sw(Reg::X11, Reg::X1, 100);

    // rs1_val == -1025, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x401;  immval:0x7
    c.li(Reg::X10, MASK_XLEN(-0x401));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x7));
    c.sw(Reg::X11, Reg::X1, 104);

    // rs1_val == -2049, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x801;  immval:0xf
    c.li(Reg::X10, MASK_XLEN(-0x801));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xf));
    c.sw(Reg::X11, Reg::X1, 108);

    // rs1_val == -4097, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x1001;  immval:0xa
    c.li(Reg::X10, MASK_XLEN(-0x1001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xa));
    c.sw(Reg::X11, Reg::X1, 112);

    // rs1_val == -8193, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x2001;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0x2001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 116);

    // rs1_val == -16385, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x4001;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0x4001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X1, 120);

    // rs1_val == -32769, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x8001;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0x8001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 124);

    // rs1_val == -131073, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x20001;  immval:0x1d
    c.li(Reg::X10, MASK_XLEN(-0x20001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x1d));
    c.sw(Reg::X11, Reg::X1, 128);

    // rs1_val == -262145, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x40001;  immval:0x1d
    c.li(Reg::X10, MASK_XLEN(-0x40001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x1d));
    c.sw(Reg::X11, Reg::X1, 132);

    // rs1_val == -524289, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x80001;  immval:0xc
    c.li(Reg::X10, MASK_XLEN(-0x80001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xc));
    c.sw(Reg::X11, Reg::X1, 136);

    // rs1_val == -2097153, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x200001;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0x200001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 140);

    // rs1_val == -8388609, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x800001;  immval:0x13
    c.li(Reg::X10, MASK_XLEN(-0x800001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x13));
    c.sw(Reg::X11, Reg::X1, 144);

    // rs1_val == -16777217, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x1000001;  immval:0x11
    c.li(Reg::X10, MASK_XLEN(-0x1000001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x11));
    c.sw(Reg::X11, Reg::X1, 148);

    // rs1_val == -33554433, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x2000001;  immval:0xf
    c.li(Reg::X10, MASK_XLEN(-0x2000001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xf));
    c.sw(Reg::X11, Reg::X1, 152);

    // rs1_val == -67108865, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x4000001;  immval:0xd
    c.li(Reg::X10, MASK_XLEN(-0x4000001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xd));
    c.sw(Reg::X11, Reg::X1, 156);

    // rs1_val == -134217729, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x8000001;  immval:0xe
    c.li(Reg::X10, MASK_XLEN(-0x8000001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xe));
    c.sw(Reg::X11, Reg::X1, 160);

    // rs1_val == -268435457, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x10000001;  immval:0x7
    c.li(Reg::X10, MASK_XLEN(-0x10000001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x7));
    c.sw(Reg::X11, Reg::X1, 164);

    // rs1_val == -536870913, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x20000001;  immval:0xb
    c.li(Reg::X10, MASK_XLEN(-0x20000001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xb));
    c.sw(Reg::X11, Reg::X1, 168);

    // rs1_val == 1431655765, rs1_val==1431655765
    // opcode: slli ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x11
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x11));
    c.sw(Reg::X11, Reg::X1, 172);

    // rs1_val == -1431655766, rs1_val==-1431655766
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x17
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x17));
    c.sw(Reg::X11, Reg::X1, 176);

    // rs1_val==3, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x3;  immval:0x13
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x13));
    c.sw(Reg::X11, Reg::X1, 180);

    // rs1_val==5, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x5;  immval:0x1
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x1));
    c.sw(Reg::X11, Reg::X1, 184);

    // rs1_val==858993459, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 188);

    // rs1_val==-46340, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 192);

    // rs1_val==1717986919, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x17
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x17));
    c.sw(Reg::X11, Reg::X1, 196);

    // rs1_val==-46339, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X1, 200);

    // rs1_val==46341, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0xb505;  immval:0x1f
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x1f));
    c.sw(Reg::X11, Reg::X1, 204);

    // imm_val == 4, rs1_val==858993458
    // opcode: slli ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X1, 208);

    // imm_val == 30, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x6;  immval:0x1e
    c.li(Reg::X10, MASK_XLEN(-0x6));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x1e));
    c.sw(Reg::X11, Reg::X1, 212);

    // rs1_val==46340, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0xb504;  immval:0x7
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x7));
    c.sw(Reg::X11, Reg::X1, 216);

    // rs1_val == -1048577, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x100001;  immval:0x8
    c.li(Reg::X10, MASK_XLEN(-0x100001));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x8));
    c.sw(Reg::X11, Reg::X1, 220);

    // rs1_val==1431655764, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X1, 224);

    // rs1_val==1717986917, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x10
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x10));
    c.sw(Reg::X11, Reg::X1, 228);

    // rs1_val==46339, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0xb503;  immval:0x10
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x10));
    c.sw(Reg::X11, Reg::X1, 232);

    // rs1_val==1431655766, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x17
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x17));
    c.sw(Reg::X11, Reg::X1, 236);

    // rs1_val==-1431655765, 
    // opcode: slli ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0xe
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0xe));
    c.sw(Reg::X11, Reg::X1, 240);

    // rs1_val==6, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x6;  immval:0x1e
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x1e));
    c.sw(Reg::X11, Reg::X1, 244);

    // rs1_val==858993460, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x1d
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x1d));
    c.sw(Reg::X11, Reg::X1, 248);

    // rs1_val == 512, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x200;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x200));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X1, 252);

    // rs1_val == 65536, 
    // opcode: slli ; op1:x10; dest:x11; op1val:0x10000;  immval:0x1
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.slli(Reg::X11, Reg::X10, SEXT_IMM(0x1));
    c.sw(Reg::X11, Reg::X1, 256);


        
    
    // If we're simulating, add ecall
    if(simulate) {
        c.ecall();
    }
    // Otherwise
    else {
        // Set ready flag 
        c.li(Reg::X1, 1);
        c.sw(Reg::X1, Reg::X0);
        
        // Infinite loop
        Label spinLoop;
        c.L(spinLoop);
        {
            c.j_(spinLoop);
        }
    }
    return c.getCode();
}