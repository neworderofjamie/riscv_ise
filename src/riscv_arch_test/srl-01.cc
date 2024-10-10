#include "srl-01.h"

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
3735928559, 3735928559, 3735928559, };
}

//----------------------------------------------------------------------------
// ArchTest::srl_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &ArchTest::srl_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
std::vector<uint32_t> ArchTest::srl_01::generateCode(bool simulate)
{
    CodeGenerator c;
    
    // Ready flag is always stored in first word, zero it
    c.sw(Reg::X0, Reg::X0);
    
    
    c.li(Reg::X1, 20);
    // rs1 != rs2  and rs1 != rd and rs2 != rd, rs1==x10, rs2==x4, rd==x7, rs1_val < 0 and rs2_val == 0, rs1_val == -9
    // opcode: srl ; op1:x10; op2:x4; dest:x7; op1val:-0x9;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0x9));
    c.li(Reg::X4, MASK_XLEN(0x0));
    c.srl(Reg::X7, Reg::X10, Reg::X4);
    c.sw(Reg::X7, Reg::X1, 0);

    // rs1 == rd != rs2, rs1==x15, rs2==x0, rd==x15, rs2_val == 15, rs1_val < 0 and rs2_val > 0 and rs2_val < xlen
    // opcode: srl ; op1:x15; op2:x0; dest:x15; op1val:-0x6;  op2val:0x0
    c.li(Reg::X15, MASK_XLEN(-0x6));
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.srl(Reg::X15, Reg::X15, Reg::X0);
    c.sw(Reg::X15, Reg::X1, 4);

    // rs2 == rd != rs1, rs1==x2, rs2==x3, rd==x3, rs2_val == 23, rs1_val == -2049
    // opcode: srl ; op1:x2; op2:x3; dest:x3; op1val:-0x801;  op2val:0x17
    c.li(Reg::X2, MASK_XLEN(-0x801));
    c.li(Reg::X3, MASK_XLEN(0x17));
    c.srl(Reg::X3, Reg::X2, Reg::X3);
    c.sw(Reg::X3, Reg::X1, 8);

    // rs1 == rs2 != rd, rs1==x5, rs2==x5, rd==x12, rs2_val == 27, rs1_val == -1048577
    // opcode: srl ; op1:x5; op2:x5; dest:x12; op1val:-0x100001;  op2val:-0x100001
    c.li(Reg::X5, MASK_XLEN(-0x100001));
    c.li(Reg::X5, MASK_XLEN(-0x100001));
    c.srl(Reg::X12, Reg::X5, Reg::X5);
    c.sw(Reg::X12, Reg::X1, 12);

    // rs1 == rs2 == rd, rs1==x14, rs2==x14, rd==x14, rs2_val == 29, rs1_val == -16777217
    // opcode: srl ; op1:x14; op2:x14; dest:x14; op1val:-0x1000001;  op2val:-0x1000001
    c.li(Reg::X14, MASK_XLEN(-0x1000001));
    c.li(Reg::X14, MASK_XLEN(-0x1000001));
    c.srl(Reg::X14, Reg::X14, Reg::X14);
    c.sw(Reg::X14, Reg::X1, 16);

    // rs1==x12, rs2==x10, rd==x2, rs2_val == 30, rs1_val == -1073741825
    // opcode: srl ; op1:x12; op2:x10; dest:x2; op1val:-0x40000001;  op2val:0x1e
    c.li(Reg::X12, MASK_XLEN(-0x40000001));
    c.li(Reg::X10, MASK_XLEN(0x1e));
    c.srl(Reg::X2, Reg::X12, Reg::X10);
    c.sw(Reg::X2, Reg::X1, 20);

    // rs1==x11, rs2==x15, rd==x9, rs1_val == 2147483647, rs1_val > 0 and rs2_val > 0 and rs2_val < xlen, rs1_val == (2**(xlen-1)-1) and rs2_val >= 0 and rs2_val < xlen
    // opcode: srl ; op1:x11; op2:x15; dest:x9; op1val:0x7fffffff;  op2val:0x1b
    c.li(Reg::X11, MASK_XLEN(0x7fffffff));
    c.li(Reg::X15, MASK_XLEN(0x1b));
    c.srl(Reg::X9, Reg::X11, Reg::X15);
    c.sw(Reg::X9, Reg::X1, 24);

    // rs1==x4, rs2==x6, rd==x10, rs1_val == -536870913, 
    // opcode: srl ; op1:x4; op2:x6; dest:x10; op1val:-0x20000001;  op2val:0x13
    c.li(Reg::X4, MASK_XLEN(-0x20000001));
    c.li(Reg::X6, MASK_XLEN(0x13));
    c.srl(Reg::X10, Reg::X4, Reg::X6);
    c.sw(Reg::X10, Reg::X1, 28);

    c.li(Reg::X10, 52);
    // rs1==x0, rs2==x9, rd==x8, rs1_val == -268435457, 
    // opcode: srl ; op1:x0; op2:x9; dest:x8; op1val:0x0;  op2val:0xe
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.li(Reg::X9, MASK_XLEN(0xe));
    c.srl(Reg::X8, Reg::X0, Reg::X9);
    c.sw(Reg::X8, Reg::X10, 0);

    // rs1==x1, rs2==x7, rd==x11, rs1_val == -134217729, 
    // opcode: srl ; op1:x1; op2:x7; dest:x11; op1val:-0x8000001;  op2val:0x13
    c.li(Reg::X1, MASK_XLEN(-0x8000001));
    c.li(Reg::X7, MASK_XLEN(0x13));
    c.srl(Reg::X11, Reg::X1, Reg::X7);
    c.sw(Reg::X11, Reg::X10, 4);

    // rs1==x7, rs2==x12, rd==x5, rs1_val == -67108865, 
    // opcode: srl ; op1:x7; op2:x12; dest:x5; op1val:-0x4000001;  op2val:0x1b
    c.li(Reg::X7, MASK_XLEN(-0x4000001));
    c.li(Reg::X12, MASK_XLEN(0x1b));
    c.srl(Reg::X5, Reg::X7, Reg::X12);
    c.sw(Reg::X5, Reg::X10, 8);

    // rs1==x6, rs2==x13, rd==x1, rs1_val == -33554433, 
    // opcode: srl ; op1:x6; op2:x13; dest:x1; op1val:-0x2000001;  op2val:0x6
    c.li(Reg::X6, MASK_XLEN(-0x2000001));
    c.li(Reg::X13, MASK_XLEN(0x6));
    c.srl(Reg::X1, Reg::X6, Reg::X13);
    c.sw(Reg::X1, Reg::X10, 12);

    // rs1==x9, rs2==x8, rd==x4, rs1_val == -8388609, 
    // opcode: srl ; op1:x9; op2:x8; dest:x4; op1val:-0x800001;  op2val:0xe
    c.li(Reg::X9, MASK_XLEN(-0x800001));
    c.li(Reg::X8, MASK_XLEN(0xe));
    c.srl(Reg::X4, Reg::X9, Reg::X8);
    c.sw(Reg::X4, Reg::X10, 16);

    // rs1==x3, rs2==x11, rd==x6, rs1_val == -4194305, rs2_val == 8
    // opcode: srl ; op1:x3; op2:x11; dest:x6; op1val:-0x400001;  op2val:0x8
    c.li(Reg::X3, MASK_XLEN(-0x400001));
    c.li(Reg::X11, MASK_XLEN(0x8));
    c.srl(Reg::X6, Reg::X3, Reg::X11);
    c.sw(Reg::X6, Reg::X10, 20);

    // rs1==x13, rs2==x1, rd==x0, rs1_val == -2097153, rs2_val == 4
    // opcode: srl ; op1:x13; op2:x1; dest:x0; op1val:-0x200001;  op2val:0x4
    c.li(Reg::X13, MASK_XLEN(-0x200001));
    c.li(Reg::X1, MASK_XLEN(0x4));
    c.srl(Reg::X0, Reg::X13, Reg::X1);
    c.sw(Reg::X0, Reg::X10, 24);

    // rs1==x8, rs2==x2, rd==x13, rs1_val == -524289, 
    // opcode: srl ; op1:x8; op2:x2; dest:x13; op1val:-0x80001;  op2val:0x1d
    c.li(Reg::X8, MASK_XLEN(-0x80001));
    c.li(Reg::X2, MASK_XLEN(0x1d));
    c.srl(Reg::X13, Reg::X8, Reg::X2);
    c.sw(Reg::X13, Reg::X10, 28);

    c.li(Reg::X1, 84);
    // rs1_val == -262145, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x40001;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0x40001));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 0);

    // rs1_val == -131073, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x20001;  op2val:0x8
    c.li(Reg::X10, MASK_XLEN(-0x20001));
    c.li(Reg::X11, MASK_XLEN(0x8));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 4);

    // rs1_val == -65537, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x10001;  op2val:0x1e
    c.li(Reg::X10, MASK_XLEN(-0x10001));
    c.li(Reg::X11, MASK_XLEN(0x1e));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 8);

    // rs1_val == -32769, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x8001;  op2val:0x1d
    c.li(Reg::X10, MASK_XLEN(-0x8001));
    c.li(Reg::X11, MASK_XLEN(0x1d));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 12);

    // rs1_val == -16385, rs2_val == 10
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x4001;  op2val:0xa
    c.li(Reg::X10, MASK_XLEN(-0x4001));
    c.li(Reg::X11, MASK_XLEN(0xa));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 16);

    // rs1_val == -8193, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x2001;  op2val:0xe
    c.li(Reg::X10, MASK_XLEN(-0x2001));
    c.li(Reg::X11, MASK_XLEN(0xe));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 20);

    // rs1_val == -4097, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x1001;  op2val:0x1f
    c.li(Reg::X10, MASK_XLEN(-0x1001));
    c.li(Reg::X11, MASK_XLEN(0x1f));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 24);

    // rs1_val == -1025, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x401;  op2val:0x1e
    c.li(Reg::X10, MASK_XLEN(-0x401));
    c.li(Reg::X11, MASK_XLEN(0x1e));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 28);

    // rs1_val == -513, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x201;  op2val:0x11
    c.li(Reg::X10, MASK_XLEN(-0x201));
    c.li(Reg::X11, MASK_XLEN(0x11));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 32);

    // rs1_val == -257, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x101;  op2val:0x1d
    c.li(Reg::X10, MASK_XLEN(-0x101));
    c.li(Reg::X11, MASK_XLEN(0x1d));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 36);

    // rs1_val == -129, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x81;  op2val:0xd
    c.li(Reg::X10, MASK_XLEN(-0x81));
    c.li(Reg::X11, MASK_XLEN(0xd));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 40);

    // rs1_val == -65, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x41;  op2val:0x9
    c.li(Reg::X10, MASK_XLEN(-0x41));
    c.li(Reg::X11, MASK_XLEN(0x9));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 44);

    // rs1_val == -33, rs2_val == 21
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x21;  op2val:0x15
    c.li(Reg::X10, MASK_XLEN(-0x21));
    c.li(Reg::X11, MASK_XLEN(0x15));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 48);

    // rs1_val == -17, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x11;  op2val:0xe
    c.li(Reg::X10, MASK_XLEN(-0x11));
    c.li(Reg::X11, MASK_XLEN(0xe));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 52);

    // rs1_val == -5, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x5;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0x5));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 56);

    // rs1_val == -3, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x3;  op2val:0x1f
    c.li(Reg::X10, MASK_XLEN(-0x3));
    c.li(Reg::X11, MASK_XLEN(0x1f));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 60);

    // rs1_val == -2, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x2;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0x2));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 64);

    // rs2_val == 16, rs1_val == 524288
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x80000;  op2val:0x10
    c.li(Reg::X10, MASK_XLEN(0x80000));
    c.li(Reg::X11, MASK_XLEN(0x10));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 68);

    // rs2_val == 2, rs1_val==46341
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 72);

    // rs2_val == 1, rs1_val == 268435456
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x10000000;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x10000000));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 76);

    // rs1_val == -2147483648, rs1_val == (-2**(xlen-1)) and rs2_val >= 0 and rs2_val < xlen
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x80000000;  op2val:0x17
    c.li(Reg::X10, MASK_XLEN(-0x80000000));
    c.li(Reg::X11, MASK_XLEN(0x17));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 80);

    // rs1_val == 1073741824, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x40000000;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x40000000));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 84);

    // rs1_val == 536870912, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x20000000;  op2val:0xa
    c.li(Reg::X10, MASK_XLEN(0x20000000));
    c.li(Reg::X11, MASK_XLEN(0xa));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 88);

    // rs1_val == 134217728, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x8000000;  op2val:0x11
    c.li(Reg::X10, MASK_XLEN(0x8000000));
    c.li(Reg::X11, MASK_XLEN(0x11));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 92);

    // rs1_val == 67108864, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x4000000;  op2val:0xe
    c.li(Reg::X10, MASK_XLEN(0x4000000));
    c.li(Reg::X11, MASK_XLEN(0xe));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 96);

    // rs1_val == 33554432, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x2000000;  op2val:0x12
    c.li(Reg::X10, MASK_XLEN(0x2000000));
    c.li(Reg::X11, MASK_XLEN(0x12));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 100);

    // rs1_val == 16777216, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x1000000;  op2val:0xa
    c.li(Reg::X10, MASK_XLEN(0x1000000));
    c.li(Reg::X11, MASK_XLEN(0xa));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 104);

    // rs1_val == 8388608, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x800000;  op2val:0x13
    c.li(Reg::X10, MASK_XLEN(0x800000));
    c.li(Reg::X11, MASK_XLEN(0x13));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 108);

    // rs1_val == 4194304, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x400000;  op2val:0xb
    c.li(Reg::X10, MASK_XLEN(0x400000));
    c.li(Reg::X11, MASK_XLEN(0xb));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 112);

    // rs1_val == 2097152, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x200000;  op2val:0x8
    c.li(Reg::X10, MASK_XLEN(0x200000));
    c.li(Reg::X11, MASK_XLEN(0x8));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 116);

    // rs1_val == 1048576, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x100000;  op2val:0xe
    c.li(Reg::X10, MASK_XLEN(0x100000));
    c.li(Reg::X11, MASK_XLEN(0xe));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 120);

    // rs1_val == 262144, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x40000;  op2val:0x11
    c.li(Reg::X10, MASK_XLEN(0x40000));
    c.li(Reg::X11, MASK_XLEN(0x11));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 124);

    // rs1_val == 131072, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x20000;  op2val:0xe
    c.li(Reg::X10, MASK_XLEN(0x20000));
    c.li(Reg::X11, MASK_XLEN(0xe));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 128);

    // rs1_val == 65536, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x13
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x13));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 132);

    // rs1_val == 32768, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x8000;  op2val:0xf
    c.li(Reg::X10, MASK_XLEN(0x8000));
    c.li(Reg::X11, MASK_XLEN(0xf));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 136);

    // rs1_val == 16384, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x4000;  op2val:0xe
    c.li(Reg::X10, MASK_XLEN(0x4000));
    c.li(Reg::X11, MASK_XLEN(0xe));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 140);

    // rs1_val == 8192, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x2000;  op2val:0x8
    c.li(Reg::X10, MASK_XLEN(0x2000));
    c.li(Reg::X11, MASK_XLEN(0x8));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 144);

    // rs1_val == 4096, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x1000;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x1000));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 148);

    // rs1_val == 2048, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x800;  op2val:0x1b
    c.li(Reg::X10, MASK_XLEN(0x800));
    c.li(Reg::X11, MASK_XLEN(0x1b));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 152);

    // rs1_val == 1024, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x400;  op2val:0x13
    c.li(Reg::X10, MASK_XLEN(0x400));
    c.li(Reg::X11, MASK_XLEN(0x13));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 156);

    // rs1_val == 512, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x200;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x200));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 160);

    // rs1_val == 256, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x100;  op2val:0x1e
    c.li(Reg::X10, MASK_XLEN(0x100));
    c.li(Reg::X11, MASK_XLEN(0x1e));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 164);

    // rs1_val == 128, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x80;  op2val:0xa
    c.li(Reg::X10, MASK_XLEN(0x80));
    c.li(Reg::X11, MASK_XLEN(0xa));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 168);

    // rs1_val == 64, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x40;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x40));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 172);

    // rs1_val == 32, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x20;  op2val:0xd
    c.li(Reg::X10, MASK_XLEN(0x20));
    c.li(Reg::X11, MASK_XLEN(0xd));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 176);

    // rs1_val == 16, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x10;  op2val:0x11
    c.li(Reg::X10, MASK_XLEN(0x10));
    c.li(Reg::X11, MASK_XLEN(0x11));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 180);

    // rs1_val == 8, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x8;  op2val:0x12
    c.li(Reg::X10, MASK_XLEN(0x8));
    c.li(Reg::X11, MASK_XLEN(0x12));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 184);

    // rs1_val == 4, rs1_val==4
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x10
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x10));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 188);

    // rs1_val == 2, rs1_val==2, rs1_val == rs2_val and rs2_val > 0 and rs2_val < xlen
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 192);

    // rs1_val == 1, rs1_val == 1 and rs2_val >= 0 and rs2_val < xlen
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x1f
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x1f));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 196);

    // rs1_val==-46339, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 200);

    // rs1_val==1717986919, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xe
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xe));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 204);

    // rs1_val==858993460, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x9
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x9));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 208);

    // rs1_val==6, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xd
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xd));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 212);

    // rs1_val==-1431655765, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xe
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xe));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 216);

    // rs1_val==1431655766, rs1_val > 0 and rs2_val == 0
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 220);

    // rs1_val==46339, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xd
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xd));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 224);

    // rs1_val==3, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xf
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xf));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 228);

    // rs1_val == -1431655766, rs1_val==-1431655766
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 232);

    // rs1_val == 1431655765, rs1_val==1431655765
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x7
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x7));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 236);

    // rs1_val == 0 and rs2_val >= 0 and rs2_val < xlen, rs1_val==0
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x7
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x7));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 240);

    // rs1_val==1717986917, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x1b
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x1b));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 244);

    // rs1_val==858993458, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x11
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x11));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 248);

    // rs1_val==1431655764, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x13
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x13));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 252);

    // rs1_val==46340, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x12
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x12));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 256);

    // rs1_val==-46340, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 260);

    // rs1_val==1717986918, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x1b
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x1b));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 264);

    // rs1_val==858993459, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x15
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x15));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 268);

    // rs1_val==5, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x10
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x10));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 272);

    // rs2_val == 27, rs1_val == -1048577
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x100001;  op2val:0x1b
    c.li(Reg::X10, MASK_XLEN(-0x100001));
    c.li(Reg::X11, MASK_XLEN(0x1b));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 276);

    // rs2_val == 29, rs1_val == -16777217
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x1000001;  op2val:0x1d
    c.li(Reg::X10, MASK_XLEN(-0x1000001));
    c.li(Reg::X11, MASK_XLEN(0x1d));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 280);

    // rs1_val == -268435457, 
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x10000001;  op2val:0xe
    c.li(Reg::X10, MASK_XLEN(-0x10000001));
    c.li(Reg::X11, MASK_XLEN(0xe));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 284);

    // rs1_val == -2097153, rs2_val == 4
    // opcode: srl ; op1:x10; op2:x11; dest:x12; op1val:-0x200001;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0x200001));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.srl(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 288);


        
    
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