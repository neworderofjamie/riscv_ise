#include "srai-01.h"

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
3735928559, };
}

//----------------------------------------------------------------------------
// srai_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &srai_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
CodeGenerator srai_01::generateCode()
{
    CodeGenerator c;
    
    c.li(Reg::X4, 16);
    // rs1 != rd, rs1==x31, rd==x25, rs1_val < 0 and imm_val > 0 and imm_val < xlen, rs1_val == -9
    // opcode: srai ; op1:x31; dest:x25; op1val:-0x9;  immval:0x9
    c.li(Reg::X31, MASK_XLEN(-0x9));
    c.srai(Reg::X25, Reg::X31, SEXT_IMM(0x9));
    c.sw(Reg::X25, Reg::X4, 0);

    // rs1 == rd, rs1==x10, rd==x10, rs1_val > 0 and imm_val > 0 and imm_val < xlen, rs1_val==5, imm_val == 1
    // opcode: srai ; op1:x10; dest:x10; op1val:0x5;  immval:0x1
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.srai(Reg::X10, Reg::X10, SEXT_IMM(0x1));
    c.sw(Reg::X10, Reg::X4, 4);

    // rs1==x8, rd==x28, rs1_val < 0 and imm_val == 0, rs1_val == -16777217
    // opcode: srai ; op1:x8; dest:x28; op1val:-0x1000001;  immval:0x0
    c.li(Reg::X8, MASK_XLEN(-0x1000001));
    c.srai(Reg::X28, Reg::X8, SEXT_IMM(0x0));
    c.sw(Reg::X28, Reg::X4, 8);

    // rs1==x17, rd==x5, rs1_val > 0 and imm_val == 0, rs1_val == 1048576
    // opcode: srai ; op1:x17; dest:x5; op1val:0x100000;  immval:0x0
    c.li(Reg::X17, MASK_XLEN(0x100000));
    c.srai(Reg::X5, Reg::X17, SEXT_IMM(0x0));
    c.sw(Reg::X5, Reg::X4, 12);

    // rs1==x23, rd==x27, rs1_val < 0 and imm_val == (xlen-1), rs1_val == -131073
    // opcode: srai ; op1:x23; dest:x27; op1val:-0x20001;  immval:0x1f
    c.li(Reg::X23, MASK_XLEN(-0x20001));
    c.srai(Reg::X27, Reg::X23, SEXT_IMM(0x1f));
    c.sw(Reg::X27, Reg::X4, 16);

    // rs1==x13, rd==x20, rs1_val > 0 and imm_val == (xlen-1), rs1_val == 1 and imm_val >= 0 and imm_val < xlen, rs1_val == 1
    // opcode: srai ; op1:x13; dest:x20; op1val:0x1;  immval:0x1f
    c.li(Reg::X13, MASK_XLEN(0x1));
    c.srai(Reg::X20, Reg::X13, SEXT_IMM(0x1f));
    c.sw(Reg::X20, Reg::X4, 20);

    // rs1==x22, rd==x11, rs1_val == imm_val and imm_val > 0 and imm_val < xlen, rs1_val==4, imm_val == 4, rs1_val == 4
    // opcode: srai ; op1:x22; dest:x11; op1val:0x4;  immval:0x4
    c.li(Reg::X22, MASK_XLEN(0x4));
    c.srai(Reg::X11, Reg::X22, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 24);

    // rs1==x7, rd==x30, rs1_val == (-2**(xlen-1)) and imm_val >= 0 and imm_val < xlen, rs1_val == -2147483648
    // opcode: srai ; op1:x7; dest:x30; op1val:-0x80000000;  immval:0x0
    c.li(Reg::X7, MASK_XLEN(-0x80000000));
    c.srai(Reg::X30, Reg::X7, SEXT_IMM(0x0));
    c.sw(Reg::X30, Reg::X4, 28);

    // rs1==x18, rd==x14, rs1_val == 0 and imm_val >= 0 and imm_val < xlen, rs1_val==0
    // opcode: srai ; op1:x18; dest:x14; op1val:0x0;  immval:0xe
    c.li(Reg::X18, MASK_XLEN(0x0));
    c.srai(Reg::X14, Reg::X18, SEXT_IMM(0xe));
    c.sw(Reg::X14, Reg::X4, 32);

    // rs1==x3, rd==x19, rs1_val == (2**(xlen-1)-1) and imm_val >= 0 and imm_val < xlen, rs1_val == 2147483647
    // opcode: srai ; op1:x3; dest:x19; op1val:0x7fffffff;  immval:0x1f
    c.li(Reg::X3, MASK_XLEN(0x7fffffff));
    c.srai(Reg::X19, Reg::X3, SEXT_IMM(0x1f));
    c.sw(Reg::X19, Reg::X4, 36);

    // rs1==x25, rd==x29, rs1_val == 2, rs1_val==2
    // opcode: srai ; op1:x25; dest:x29; op1val:0x2;  immval:0x11
    c.li(Reg::X25, MASK_XLEN(0x2));
    c.srai(Reg::X29, Reg::X25, SEXT_IMM(0x11));
    c.sw(Reg::X29, Reg::X4, 40);

    // rs1==x30, rd==x3, rs1_val == 8, 
    // opcode: srai ; op1:x30; dest:x3; op1val:0x8;  immval:0x11
    c.li(Reg::X30, MASK_XLEN(0x8));
    c.srai(Reg::X3, Reg::X30, SEXT_IMM(0x11));
    c.sw(Reg::X3, Reg::X4, 44);

    // rs1==x2, rd==x22, rs1_val == 16, 
    // opcode: srai ; op1:x2; dest:x22; op1val:0x10;  immval:0x12
    c.li(Reg::X2, MASK_XLEN(0x10));
    c.srai(Reg::X22, Reg::X2, SEXT_IMM(0x12));
    c.sw(Reg::X22, Reg::X4, 48);

    // rs1==x12, rd==x2, rs1_val == 32, 
    // opcode: srai ; op1:x12; dest:x2; op1val:0x20;  immval:0xd
    c.li(Reg::X12, MASK_XLEN(0x20));
    c.srai(Reg::X2, Reg::X12, SEXT_IMM(0xd));
    c.sw(Reg::X2, Reg::X4, 52);

    // rs1==x1, rd==x12, rs1_val == 64, imm_val == 23
    // opcode: srai ; op1:x1; dest:x12; op1val:0x40;  immval:0x17
    c.li(Reg::X1, MASK_XLEN(0x40));
    c.srai(Reg::X12, Reg::X1, SEXT_IMM(0x17));
    c.sw(Reg::X12, Reg::X4, 56);

    // rs1==x20, rd==x24, rs1_val == 128, 
    // opcode: srai ; op1:x20; dest:x24; op1val:0x80;  immval:0x9
    c.li(Reg::X20, MASK_XLEN(0x80));
    c.srai(Reg::X24, Reg::X20, SEXT_IMM(0x9));
    c.sw(Reg::X24, Reg::X4, 60);

    // rs1==x11, rd==x0, rs1_val == 256, imm_val == 16
    // opcode: srai ; op1:x11; dest:x0; op1val:0x100;  immval:0x10
    c.li(Reg::X11, MASK_XLEN(0x100));
    c.srai(Reg::X0, Reg::X11, SEXT_IMM(0x10));
    c.sw(Reg::X0, Reg::X4, 64);

    // rs1==x26, rd==x8, rs1_val == 512, 
    // opcode: srai ; op1:x26; dest:x8; op1val:0x200;  immval:0x9
    c.li(Reg::X26, MASK_XLEN(0x200));
    c.srai(Reg::X8, Reg::X26, SEXT_IMM(0x9));
    c.sw(Reg::X8, Reg::X4, 68);

    // rs1==x9, rd==x17, rs1_val == 1024, 
    // opcode: srai ; op1:x9; dest:x17; op1val:0x400;  immval:0x11
    c.li(Reg::X9, MASK_XLEN(0x400));
    c.srai(Reg::X17, Reg::X9, SEXT_IMM(0x11));
    c.sw(Reg::X17, Reg::X4, 72);

    // rs1==x16, rd==x23, rs1_val == 2048, imm_val == 27
    // opcode: srai ; op1:x16; dest:x23; op1val:0x800;  immval:0x1b
    c.li(Reg::X16, MASK_XLEN(0x800));
    c.srai(Reg::X23, Reg::X16, SEXT_IMM(0x1b));
    c.sw(Reg::X23, Reg::X4, 76);

    // rs1==x14, rd==x13, rs1_val == 4096, 
    // opcode: srai ; op1:x14; dest:x13; op1val:0x1000;  immval:0x5
    c.li(Reg::X14, MASK_XLEN(0x1000));
    c.srai(Reg::X13, Reg::X14, SEXT_IMM(0x5));
    c.sw(Reg::X13, Reg::X4, 80);

    // rs1==x21, rd==x7, rs1_val == 8192, 
    // opcode: srai ; op1:x21; dest:x7; op1val:0x2000;  immval:0x1
    c.li(Reg::X21, MASK_XLEN(0x2000));
    c.srai(Reg::X7, Reg::X21, SEXT_IMM(0x1));
    c.sw(Reg::X7, Reg::X4, 84);

    c.li(Reg::X2, 104);
    // rs1==x5, rd==x18, rs1_val == 16384, 
    // opcode: srai ; op1:x5; dest:x18; op1val:0x4000;  immval:0x5
    c.li(Reg::X5, MASK_XLEN(0x4000));
    c.srai(Reg::X18, Reg::X5, SEXT_IMM(0x5));
    c.sw(Reg::X18, Reg::X2, 0);

    // rs1==x0, rd==x26, rs1_val == 32768, 
    // opcode: srai ; op1:x0; dest:x26; op1val:0x0;  immval:0x11
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.srai(Reg::X26, Reg::X0, SEXT_IMM(0x11));
    c.sw(Reg::X26, Reg::X2, 4);

    // rs1==x24, rd==x4, rs1_val == 65536, 
    // opcode: srai ; op1:x24; dest:x4; op1val:0x10000;  immval:0xd
    c.li(Reg::X24, MASK_XLEN(0x10000));
    c.srai(Reg::X4, Reg::X24, SEXT_IMM(0xd));
    c.sw(Reg::X4, Reg::X2, 8);

    // rs1==x15, rd==x9, rs1_val == 131072, 
    // opcode: srai ; op1:x15; dest:x9; op1val:0x20000;  immval:0x12
    c.li(Reg::X15, MASK_XLEN(0x20000));
    c.srai(Reg::X9, Reg::X15, SEXT_IMM(0x12));
    c.sw(Reg::X9, Reg::X2, 12);

    // rs1==x19, rd==x6, rs1_val == 262144, 
    // opcode: srai ; op1:x19; dest:x6; op1val:0x40000;  immval:0x1f
    c.li(Reg::X19, MASK_XLEN(0x40000));
    c.srai(Reg::X6, Reg::X19, SEXT_IMM(0x1f));
    c.sw(Reg::X6, Reg::X2, 16);

    // rs1==x28, rd==x21, rs1_val == 524288, imm_val == 21
    // opcode: srai ; op1:x28; dest:x21; op1val:0x80000;  immval:0x15
    c.li(Reg::X28, MASK_XLEN(0x80000));
    c.srai(Reg::X21, Reg::X28, SEXT_IMM(0x15));
    c.sw(Reg::X21, Reg::X2, 20);

    // rs1==x27, rd==x31, rs1_val == 2097152, 
    // opcode: srai ; op1:x27; dest:x31; op1val:0x200000;  immval:0x17
    c.li(Reg::X27, MASK_XLEN(0x200000));
    c.srai(Reg::X31, Reg::X27, SEXT_IMM(0x17));
    c.sw(Reg::X31, Reg::X2, 24);

    // rs1==x6, rd==x1, rs1_val == 4194304, 
    // opcode: srai ; op1:x6; dest:x1; op1val:0x400000;  immval:0x12
    c.li(Reg::X6, MASK_XLEN(0x400000));
    c.srai(Reg::X1, Reg::X6, SEXT_IMM(0x12));
    c.sw(Reg::X1, Reg::X2, 28);

    // rs1==x4, rd==x15, rs1_val == 8388608, 
    // opcode: srai ; op1:x4; dest:x15; op1val:0x800000;  immval:0xb
    c.li(Reg::X4, MASK_XLEN(0x800000));
    c.srai(Reg::X15, Reg::X4, SEXT_IMM(0xb));
    c.sw(Reg::X15, Reg::X2, 32);

    // rs1==x29, rd==x16, rs1_val == 16777216, 
    // opcode: srai ; op1:x29; dest:x16; op1val:0x1000000;  immval:0x1f
    c.li(Reg::X29, MASK_XLEN(0x1000000));
    c.srai(Reg::X16, Reg::X29, SEXT_IMM(0x1f));
    c.sw(Reg::X16, Reg::X2, 36);

    // rs1_val == 33554432, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x2000000;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x2000000));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X2, 40);

    // rs1_val == 67108864, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x4000000;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x4000000));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X2, 44);

    // rs1_val == 134217728, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x8000000;  immval:0x7
    c.li(Reg::X10, MASK_XLEN(0x8000000));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x7));
    c.sw(Reg::X11, Reg::X2, 48);

    // rs1_val == 268435456, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x10000000;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x10000000));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X2, 52);

    // rs1_val == 536870912, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x20000000;  immval:0xc
    c.li(Reg::X10, MASK_XLEN(0x20000000));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xc));
    c.sw(Reg::X11, Reg::X2, 56);

    // rs1_val == 1073741824, imm_val == 8
    // opcode: srai ; op1:x10; dest:x11; op1val:0x40000000;  immval:0x8
    c.li(Reg::X10, MASK_XLEN(0x40000000));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x8));
    c.sw(Reg::X11, Reg::X2, 60);

    // rs1_val == -2, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x2;  immval:0x1
    c.li(Reg::X10, MASK_XLEN(-0x2));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x1));
    c.sw(Reg::X11, Reg::X2, 64);

    // rs1_val == -3, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x3;  immval:0x10
    c.li(Reg::X10, MASK_XLEN(-0x3));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x10));
    c.sw(Reg::X11, Reg::X2, 68);

    // rs1_val == -5, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x5;  immval:0x12
    c.li(Reg::X10, MASK_XLEN(-0x5));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x12));
    c.sw(Reg::X11, Reg::X2, 72);

    // rs1_val == -17, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x11;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0x11));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X2, 76);

    // rs1_val == -33, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x21;  immval:0x8
    c.li(Reg::X10, MASK_XLEN(-0x21));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x8));
    c.sw(Reg::X11, Reg::X2, 80);

    // rs1_val == -65, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x41;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0x41));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X2, 84);

    // rs1_val == -129, imm_val == 15
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x81;  immval:0xf
    c.li(Reg::X10, MASK_XLEN(-0x81));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xf));
    c.sw(Reg::X11, Reg::X2, 88);

    // rs1_val == -257, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x101;  immval:0xb
    c.li(Reg::X10, MASK_XLEN(-0x101));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xb));
    c.sw(Reg::X11, Reg::X2, 92);

    // rs1_val == -513, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x201;  immval:0xd
    c.li(Reg::X10, MASK_XLEN(-0x201));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xd));
    c.sw(Reg::X11, Reg::X2, 96);

    // rs1_val == -1025, imm_val == 30
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x401;  immval:0x1e
    c.li(Reg::X10, MASK_XLEN(-0x401));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x1e));
    c.sw(Reg::X11, Reg::X2, 100);

    // rs1_val == -2049, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x801;  immval:0xd
    c.li(Reg::X10, MASK_XLEN(-0x801));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xd));
    c.sw(Reg::X11, Reg::X2, 104);

    // rs1_val == -4097, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x1001;  immval:0xf
    c.li(Reg::X10, MASK_XLEN(-0x1001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xf));
    c.sw(Reg::X11, Reg::X2, 108);

    // rs1_val == -8193, imm_val == 29
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x2001;  immval:0x1d
    c.li(Reg::X10, MASK_XLEN(-0x2001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x1d));
    c.sw(Reg::X11, Reg::X2, 112);

    // rs1_val == -16385, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x4001;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0x4001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X2, 116);

    // rs1_val == -32769, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x8001;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0x8001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X2, 120);

    // rs1_val == -65537, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x10001;  immval:0xd
    c.li(Reg::X10, MASK_XLEN(-0x10001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xd));
    c.sw(Reg::X11, Reg::X2, 124);

    // rs1_val == -262145, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x40001;  immval:0x11
    c.li(Reg::X10, MASK_XLEN(-0x40001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x11));
    c.sw(Reg::X11, Reg::X2, 128);

    // rs1_val == -524289, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x80001;  immval:0x9
    c.li(Reg::X10, MASK_XLEN(-0x80001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x9));
    c.sw(Reg::X11, Reg::X2, 132);

    // rs1_val == -1048577, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x100001;  immval:0x1
    c.li(Reg::X10, MASK_XLEN(-0x100001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x1));
    c.sw(Reg::X11, Reg::X2, 136);

    // rs1_val == -2097153, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x200001;  immval:0x11
    c.li(Reg::X10, MASK_XLEN(-0x200001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x11));
    c.sw(Reg::X11, Reg::X2, 140);

    // rs1_val == -4194305, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x400001;  immval:0x11
    c.li(Reg::X10, MASK_XLEN(-0x400001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x11));
    c.sw(Reg::X11, Reg::X2, 144);

    // rs1_val == -8388609, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x800001;  immval:0xe
    c.li(Reg::X10, MASK_XLEN(-0x800001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xe));
    c.sw(Reg::X11, Reg::X2, 148);

    // rs1_val == -33554433, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x2000001;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0x2000001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X2, 152);

    // rs1_val == -67108865, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x4000001;  immval:0x12
    c.li(Reg::X10, MASK_XLEN(-0x4000001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x12));
    c.sw(Reg::X11, Reg::X2, 156);

    // rs1_val == -134217729, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x8000001;  immval:0xd
    c.li(Reg::X10, MASK_XLEN(-0x8000001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xd));
    c.sw(Reg::X11, Reg::X2, 160);

    // rs1_val == -268435457, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x10000001;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0x10000001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X2, 164);

    // rs1_val == -536870913, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x20000001;  immval:0x7
    c.li(Reg::X10, MASK_XLEN(-0x20000001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x7));
    c.sw(Reg::X11, Reg::X2, 168);

    // rs1_val == -1073741825, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x40000001;  immval:0x13
    c.li(Reg::X10, MASK_XLEN(-0x40000001));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x13));
    c.sw(Reg::X11, Reg::X2, 172);

    // rs1_val == 1431655765, rs1_val==1431655765
    // opcode: srai ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x15
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x15));
    c.sw(Reg::X11, Reg::X2, 176);

    // rs1_val == -1431655766, rs1_val==-1431655766
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0xf
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xf));
    c.sw(Reg::X11, Reg::X2, 180);

    // rs1_val==3, imm_val == 10
    // opcode: srai ; op1:x10; dest:x11; op1val:0x3;  immval:0xa
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xa));
    c.sw(Reg::X11, Reg::X2, 184);

    // rs1_val==858993459, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X2, 188);

    // rs1_val==1717986919, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x66666667;  immval:0xa
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xa));
    c.sw(Reg::X11, Reg::X2, 192);

    // rs1_val==-46339, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x1
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x1));
    c.sw(Reg::X11, Reg::X2, 196);

    // rs1_val==46341, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0xb505;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X2, 200);

    // imm_val == 2, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X2, 204);

    // rs1_val==1717986918, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x1b
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x1b));
    c.sw(Reg::X11, Reg::X2, 208);

    // rs1_val==-46340, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0xb504;  immval:0xc
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xc));
    c.sw(Reg::X11, Reg::X2, 212);

    // rs1_val==46340, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0xb504;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X2, 216);

    // rs1_val==1431655764, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X2, 220);

    // rs1_val==858993458, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x1e
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x1e));
    c.sw(Reg::X11, Reg::X2, 224);

    // rs1_val==1717986917, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x1f
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x1f));
    c.sw(Reg::X11, Reg::X2, 228);

    // rs1_val==46339, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0xb503;  immval:0x17
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x17));
    c.sw(Reg::X11, Reg::X2, 232);

    // rs1_val==1431655766, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x1b
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x1b));
    c.sw(Reg::X11, Reg::X2, 236);

    // rs1_val==-1431655765, 
    // opcode: srai ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0xa
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xa));
    c.sw(Reg::X11, Reg::X2, 240);

    // rs1_val==6, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x6;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X2, 244);

    // rs1_val==858993460, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x33333334;  immval:0xc
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0xc));
    c.sw(Reg::X11, Reg::X2, 248);

    // rs1_val == 256, imm_val == 16
    // opcode: srai ; op1:x10; dest:x11; op1val:0x100;  immval:0x10
    c.li(Reg::X10, MASK_XLEN(0x100));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x10));
    c.sw(Reg::X11, Reg::X2, 252);

    // rs1_val == 32768, 
    // opcode: srai ; op1:x10; dest:x11; op1val:0x8000;  immval:0x11
    c.li(Reg::X10, MASK_XLEN(0x8000));
    c.srai(Reg::X11, Reg::X10, SEXT_IMM(0x11));
    c.sw(Reg::X11, Reg::X2, 256);


        
    // End
    c.ecall();
    return c;
}