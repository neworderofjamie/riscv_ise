#include "sub-01.h"

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
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, };
}

//----------------------------------------------------------------------------
// ArchTest::sub_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &ArchTest::sub_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
CodeGenerator ArchTest::sub_01::generateCode()
{
    CodeGenerator c;
    
    c.li(Reg::X1, 16);
    // rs2 == rd != rs1, rs1==x24, rs2==x26, rd==x26, rs1_val > 0 and rs2_val > 0, rs1_val != rs2_val, rs1_val==1431655764 and rs2_val==6
    // opcode: sub ; op1:x24; op2:x26; dest:x26; op1val:0x55555554;  op2val:0x6
    c.li(Reg::X24, MASK_XLEN(0x55555554));
    c.li(Reg::X26, MASK_XLEN(0x6));
    c.sub(Reg::X26, Reg::X24, Reg::X26);
    c.sw(Reg::X26, Reg::X1, 0);

    // rs1 == rs2 != rd, rs1==x17, rs2==x17, rd==x23, rs1_val > 0 and rs2_val < 0, rs1_val == 33554432
    // opcode: sub ; op1:x17; op2:x17; dest:x23; op1val:0x2000000;  op2val:0x2000000
    c.li(Reg::X17, MASK_XLEN(0x2000000));
    c.li(Reg::X17, MASK_XLEN(0x2000000));
    c.sub(Reg::X23, Reg::X17, Reg::X17);
    c.sw(Reg::X23, Reg::X1, 4);

    // rs1 == rs2 == rd, rs1==x16, rs2==x16, rd==x16, rs1_val < 0 and rs2_val < 0, rs2_val == -129
    // opcode: sub ; op1:x16; op2:x16; dest:x16; op1val:-0x7;  op2val:-0x7
    c.li(Reg::X16, MASK_XLEN(-0x7));
    c.li(Reg::X16, MASK_XLEN(-0x7));
    c.sub(Reg::X16, Reg::X16, Reg::X16);
    c.sw(Reg::X16, Reg::X1, 8);

    // rs1 == rd != rs2, rs1==x31, rs2==x19, rd==x31, rs1_val < 0 and rs2_val > 0, rs1_val == -3
    // opcode: sub ; op1:x31; op2:x19; dest:x31; op1val:-0x3;  op2val:0x66666665
    c.li(Reg::X31, MASK_XLEN(-0x3));
    c.li(Reg::X19, MASK_XLEN(0x66666665));
    c.sub(Reg::X31, Reg::X31, Reg::X19);
    c.sw(Reg::X31, Reg::X1, 12);

    // rs1 != rs2  and rs1 != rd and rs2 != rd, rs1==x23, rs2==x14, rd==x8, rs1_val == rs2_val, rs2_val == 524288, rs1_val == 524288
    // opcode: sub ; op1:x23; op2:x14; dest:x8; op1val:0x80000;  op2val:0x80000
    c.li(Reg::X23, MASK_XLEN(0x80000));
    c.li(Reg::X14, MASK_XLEN(0x80000));
    c.sub(Reg::X8, Reg::X23, Reg::X14);
    c.sw(Reg::X8, Reg::X1, 16);

    // rs1==x13, rs2==x24, rd==x18, rs2_val == (-2**(xlen-1)), rs1_val == -67108865, rs2_val == -2147483648
    // opcode: sub ; op1:x13; op2:x24; dest:x18; op1val:-0x4000001;  op2val:-0x80000000
    c.li(Reg::X13, MASK_XLEN(-0x4000001));
    c.li(Reg::X24, MASK_XLEN(-0x80000000));
    c.sub(Reg::X18, Reg::X13, Reg::X24);
    c.sw(Reg::X18, Reg::X1, 20);

    // rs1==x12, rs2==x4, rd==x0, rs2_val == 0, rs1_val == 32
    // opcode: sub ; op1:x12; op2:x4; dest:x0; op1val:0x20;  op2val:0x0
    c.li(Reg::X12, MASK_XLEN(0x20));
    c.li(Reg::X4, MASK_XLEN(0x0));
    c.sub(Reg::X0, Reg::X12, Reg::X4);
    c.sw(Reg::X0, Reg::X1, 24);

    // rs1==x22, rs2==x9, rd==x10, rs2_val == (2**(xlen-1)-1), rs1_val == -536870913, rs2_val == 2147483647
    // opcode: sub ; op1:x22; op2:x9; dest:x10; op1val:-0x20000001;  op2val:0x7fffffff
    c.li(Reg::X22, MASK_XLEN(-0x20000001));
    c.li(Reg::X9, MASK_XLEN(0x7fffffff));
    c.sub(Reg::X10, Reg::X22, Reg::X9);
    c.sw(Reg::X10, Reg::X1, 28);

    // rs1==x10, rs2==x27, rd==x25, rs2_val == 1, rs1_val == 65536
    // opcode: sub ; op1:x10; op2:x27; dest:x25; op1val:0x10000;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X27, MASK_XLEN(0x1));
    c.sub(Reg::X25, Reg::X10, Reg::X27);
    c.sw(Reg::X25, Reg::X1, 32);

    // rs1==x8, rs2==x3, rd==x14, rs1_val == (-2**(xlen-1)), rs1_val == -2147483648
    // opcode: sub ; op1:x8; op2:x3; dest:x14; op1val:-0x80000000;  op2val:-0x40000000
    c.li(Reg::X8, MASK_XLEN(-0x80000000));
    c.li(Reg::X3, MASK_XLEN(-0x40000000));
    c.sub(Reg::X14, Reg::X8, Reg::X3);
    c.sw(Reg::X14, Reg::X1, 36);

    // rs1==x25, rs2==x30, rd==x29, rs1_val == 0, rs2_val == 2097152
    // opcode: sub ; op1:x25; op2:x30; dest:x29; op1val:0x0;  op2val:0x200000
    c.li(Reg::X25, MASK_XLEN(0x0));
    c.li(Reg::X30, MASK_XLEN(0x200000));
    c.sub(Reg::X29, Reg::X25, Reg::X30);
    c.sw(Reg::X29, Reg::X1, 40);

    // rs1==x18, rs2==x8, rd==x15, rs1_val == (2**(xlen-1)-1), rs2_val == 8192, rs1_val == 2147483647
    // opcode: sub ; op1:x18; op2:x8; dest:x15; op1val:0x7fffffff;  op2val:0x2000
    c.li(Reg::X18, MASK_XLEN(0x7fffffff));
    c.li(Reg::X8, MASK_XLEN(0x2000));
    c.sub(Reg::X15, Reg::X18, Reg::X8);
    c.sw(Reg::X15, Reg::X1, 44);

    // rs1==x14, rs2==x15, rd==x3, rs1_val == 1, rs2_val == 16
    // opcode: sub ; op1:x14; op2:x15; dest:x3; op1val:0x1;  op2val:0x10
    c.li(Reg::X14, MASK_XLEN(0x1));
    c.li(Reg::X15, MASK_XLEN(0x10));
    c.sub(Reg::X3, Reg::X14, Reg::X15);
    c.sw(Reg::X3, Reg::X1, 48);

    // rs1==x26, rs2==x29, rd==x13, rs2_val == 2, 
    // opcode: sub ; op1:x26; op2:x29; dest:x13; op1val:-0x7;  op2val:0x2
    c.li(Reg::X26, MASK_XLEN(-0x7));
    c.li(Reg::X29, MASK_XLEN(0x2));
    c.sub(Reg::X13, Reg::X26, Reg::X29);
    c.sw(Reg::X13, Reg::X1, 52);

    // rs1==x21, rs2==x31, rd==x19, rs2_val == 4, rs1_val==2 and rs2_val==4, rs1_val == 2
    // opcode: sub ; op1:x21; op2:x31; dest:x19; op1val:0x2;  op2val:0x4
    c.li(Reg::X21, MASK_XLEN(0x2));
    c.li(Reg::X31, MASK_XLEN(0x4));
    c.sub(Reg::X19, Reg::X21, Reg::X31);
    c.sw(Reg::X19, Reg::X1, 56);

    // rs1==x30, rs2==x5, rd==x11, rs2_val == 8, rs1_val == -16777217
    // opcode: sub ; op1:x30; op2:x5; dest:x11; op1val:-0x1000001;  op2val:0x8
    c.li(Reg::X30, MASK_XLEN(-0x1000001));
    c.li(Reg::X5, MASK_XLEN(0x8));
    c.sub(Reg::X11, Reg::X30, Reg::X5);
    c.sw(Reg::X11, Reg::X1, 60);

    // rs1==x28, rs2==x7, rd==x30, rs2_val == 32, rs1_val == -1431655766
    // opcode: sub ; op1:x28; op2:x7; dest:x30; op1val:-0x55555556;  op2val:0x20
    c.li(Reg::X28, MASK_XLEN(-0x55555556));
    c.li(Reg::X7, MASK_XLEN(0x20));
    c.sub(Reg::X30, Reg::X28, Reg::X7);
    c.sw(Reg::X30, Reg::X1, 64);

    // rs1==x9, rs2==x10, rd==x7, rs2_val == 64, rs1_val == -65
    // opcode: sub ; op1:x9; op2:x10; dest:x7; op1val:-0x41;  op2val:0x40
    c.li(Reg::X9, MASK_XLEN(-0x41));
    c.li(Reg::X10, MASK_XLEN(0x40));
    c.sub(Reg::X7, Reg::X9, Reg::X10);
    c.sw(Reg::X7, Reg::X1, 68);

    // rs1==x0, rs2==x18, rd==x17, rs2_val == 128, 
    // opcode: sub ; op1:x0; op2:x18; dest:x17; op1val:0x0;  op2val:0x80
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.li(Reg::X18, MASK_XLEN(0x80));
    c.sub(Reg::X17, Reg::X0, Reg::X18);
    c.sw(Reg::X17, Reg::X1, 72);

    // rs1==x2, rs2==x12, rd==x27, rs2_val == 256, 
    // opcode: sub ; op1:x2; op2:x12; dest:x27; op1val:-0x40000000;  op2val:0x100
    c.li(Reg::X2, MASK_XLEN(-0x40000000));
    c.li(Reg::X12, MASK_XLEN(0x100));
    c.sub(Reg::X27, Reg::X2, Reg::X12);
    c.sw(Reg::X27, Reg::X1, 76);

    c.li(Reg::X8, 96);
    // rs1==x4, rs2==x28, rd==x20, rs2_val == 512, 
    // opcode: sub ; op1:x4; op2:x28; dest:x20; op1val:-0xb503;  op2val:0x200
    c.li(Reg::X4, MASK_XLEN(-0xb503));
    c.li(Reg::X28, MASK_XLEN(0x200));
    c.sub(Reg::X20, Reg::X4, Reg::X28);
    c.sw(Reg::X20, Reg::X8, 0);

    // rs1==x15, rs2==x1, rd==x6, rs2_val == 1024, 
    // opcode: sub ; op1:x15; op2:x1; dest:x6; op1val:0x0;  op2val:0x400
    c.li(Reg::X15, MASK_XLEN(0x0));
    c.li(Reg::X1, MASK_XLEN(0x400));
    c.sub(Reg::X6, Reg::X15, Reg::X1);
    c.sw(Reg::X6, Reg::X8, 4);

    // rs1==x5, rs2==x0, rd==x9, rs2_val == 2048, 
    // opcode: sub ; op1:x5; op2:x0; dest:x9; op1val:-0x80000000;  op2val:0x0
    c.li(Reg::X5, MASK_XLEN(-0x80000000));
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.sub(Reg::X9, Reg::X5, Reg::X0);
    c.sw(Reg::X9, Reg::X8, 8);

    // rs1==x7, rs2==x11, rd==x4, rs2_val == 4096, rs1_val == 131072
    // opcode: sub ; op1:x7; op2:x11; dest:x4; op1val:0x20000;  op2val:0x1000
    c.li(Reg::X7, MASK_XLEN(0x20000));
    c.li(Reg::X11, MASK_XLEN(0x1000));
    c.sub(Reg::X4, Reg::X7, Reg::X11);
    c.sw(Reg::X4, Reg::X8, 12);

    // rs1==x19, rs2==x6, rd==x1, rs2_val == 16384, rs1_val == -268435457
    // opcode: sub ; op1:x19; op2:x6; dest:x1; op1val:-0x10000001;  op2val:0x4000
    c.li(Reg::X19, MASK_XLEN(-0x10000001));
    c.li(Reg::X6, MASK_XLEN(0x4000));
    c.sub(Reg::X1, Reg::X19, Reg::X6);
    c.sw(Reg::X1, Reg::X8, 16);

    // rs1==x6, rs2==x20, rd==x5, rs2_val == 32768, 
    // opcode: sub ; op1:x6; op2:x20; dest:x5; op1val:0x7fffffff;  op2val:0x8000
    c.li(Reg::X6, MASK_XLEN(0x7fffffff));
    c.li(Reg::X20, MASK_XLEN(0x8000));
    c.sub(Reg::X5, Reg::X6, Reg::X20);
    c.sw(Reg::X5, Reg::X8, 20);

    // rs1==x29, rs2==x22, rd==x2, rs2_val == 65536, 
    // opcode: sub ; op1:x29; op2:x22; dest:x2; op1val:-0xb503;  op2val:0x10000
    c.li(Reg::X29, MASK_XLEN(-0xb503));
    c.li(Reg::X22, MASK_XLEN(0x10000));
    c.sub(Reg::X2, Reg::X29, Reg::X22);
    c.sw(Reg::X2, Reg::X8, 24);

    // rs1==x3, rs2==x13, rd==x21, rs2_val == 131072, rs1_val == -8388609
    // opcode: sub ; op1:x3; op2:x13; dest:x21; op1val:-0x800001;  op2val:0x20000
    c.li(Reg::X3, MASK_XLEN(-0x800001));
    c.li(Reg::X13, MASK_XLEN(0x20000));
    c.sub(Reg::X21, Reg::X3, Reg::X13);
    c.sw(Reg::X21, Reg::X8, 28);

    // rs1==x1, rs2==x21, rd==x28, rs2_val == 262144, rs1_val == 256
    // opcode: sub ; op1:x1; op2:x21; dest:x28; op1val:0x100;  op2val:0x40000
    c.li(Reg::X1, MASK_XLEN(0x100));
    c.li(Reg::X21, MASK_XLEN(0x40000));
    c.sub(Reg::X28, Reg::X1, Reg::X21);
    c.sw(Reg::X28, Reg::X8, 32);

    // rs1==x20, rs2==x23, rd==x12, rs2_val == 1048576, 
    // opcode: sub ; op1:x20; op2:x23; dest:x12; op1val:0x100;  op2val:0x100000
    c.li(Reg::X20, MASK_XLEN(0x100));
    c.li(Reg::X23, MASK_XLEN(0x100000));
    c.sub(Reg::X12, Reg::X20, Reg::X23);
    c.sw(Reg::X12, Reg::X8, 36);

    // rs1==x27, rs2==x25, rd==x22, rs2_val == 4194304, 
    // opcode: sub ; op1:x27; op2:x25; dest:x22; op1val:-0x4;  op2val:0x400000
    c.li(Reg::X27, MASK_XLEN(-0x4));
    c.li(Reg::X25, MASK_XLEN(0x400000));
    c.sub(Reg::X22, Reg::X27, Reg::X25);
    c.sw(Reg::X22, Reg::X8, 40);

    // rs1==x11, rs2==x2, rd==x24, rs2_val == 8388608, 
    // opcode: sub ; op1:x11; op2:x2; dest:x24; op1val:0x66666665;  op2val:0x800000
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.li(Reg::X2, MASK_XLEN(0x800000));
    c.sub(Reg::X24, Reg::X11, Reg::X2);
    c.sw(Reg::X24, Reg::X8, 44);

    // rs2_val == 16777216, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3fffffff;  op2val:0x1000000
    c.li(Reg::X10, MASK_XLEN(0x3fffffff));
    c.li(Reg::X11, MASK_XLEN(0x1000000));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 48);

    // rs2_val == 33554432, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x2000000
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x2000000));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 52);

    // rs2_val == 67108864, rs1_val == -32769
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x8001;  op2val:0x4000000
    c.li(Reg::X10, MASK_XLEN(-0x8001));
    c.li(Reg::X11, MASK_XLEN(0x4000000));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 56);

    // rs2_val == 134217728, rs1_val == 4
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x8000000
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x8000000));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 60);

    // rs2_val == 268435456, rs1_val == -4097
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x1001;  op2val:0x10000000
    c.li(Reg::X10, MASK_XLEN(-0x1001));
    c.li(Reg::X11, MASK_XLEN(0x10000000));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 64);

    // rs2_val == 536870912, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x1001;  op2val:0x20000000
    c.li(Reg::X10, MASK_XLEN(-0x1001));
    c.li(Reg::X11, MASK_XLEN(0x20000000));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 68);

    // rs2_val == 1073741824, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x80000;  op2val:0x40000000
    c.li(Reg::X10, MASK_XLEN(0x80000));
    c.li(Reg::X11, MASK_XLEN(0x40000000));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 72);

    // rs2_val == -2, rs1_val == 1431655765
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x2
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 76);

    // rs2_val == -3, rs1_val == -65537
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x10001;  op2val:-0x3
    c.li(Reg::X10, MASK_XLEN(-0x10001));
    c.li(Reg::X11, MASK_XLEN(-0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 80);

    // rs2_val == -5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x100;  op2val:-0x5
    c.li(Reg::X10, MASK_XLEN(0x100));
    c.li(Reg::X11, MASK_XLEN(-0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 84);

    // rs2_val == -9, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2000000;  op2val:-0x9
    c.li(Reg::X10, MASK_XLEN(0x2000000));
    c.li(Reg::X11, MASK_XLEN(-0x9));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 88);

    // rs2_val == -17, rs1_val == 268435456
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x10000000;  op2val:-0x11
    c.li(Reg::X10, MASK_XLEN(0x10000000));
    c.li(Reg::X11, MASK_XLEN(-0x11));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 92);

    // rs2_val == -33, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x21
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0x21));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 96);

    // rs2_val == -65, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x41
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0x41));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 100);

    // rs2_val == -257, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x101
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0x101));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 104);

    // rs2_val == -513, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x201
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0x201));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 108);

    // rs2_val == -1025, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3fffffff;  op2val:-0x401
    c.li(Reg::X10, MASK_XLEN(0x3fffffff));
    c.li(Reg::X11, MASK_XLEN(-0x401));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 112);

    // rs2_val == -2049, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x801
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x801));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 116);

    // rs2_val == -4097, rs1_val == -33
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x21;  op2val:-0x1001
    c.li(Reg::X10, MASK_XLEN(-0x21));
    c.li(Reg::X11, MASK_XLEN(-0x1001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 120);

    // rs2_val == -8193, rs1_val == -1025
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x401;  op2val:-0x2001
    c.li(Reg::X10, MASK_XLEN(-0x401));
    c.li(Reg::X11, MASK_XLEN(-0x2001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 124);

    // rs2_val == -16385, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x10001;  op2val:-0x4001
    c.li(Reg::X10, MASK_XLEN(-0x10001));
    c.li(Reg::X11, MASK_XLEN(-0x4001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 128);

    // rs2_val == -32769, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x8001
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0x8001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 132);

    // rs2_val == -65537, rs1_val == -2049
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x801;  op2val:-0x10001
    c.li(Reg::X10, MASK_XLEN(-0x801));
    c.li(Reg::X11, MASK_XLEN(-0x10001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 136);

    // rs2_val == -131073, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x800001;  op2val:-0x20001
    c.li(Reg::X10, MASK_XLEN(-0x800001));
    c.li(Reg::X11, MASK_XLEN(-0x20001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 140);

    // rs2_val == -262145, rs1_val == -257
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x101;  op2val:-0x40001
    c.li(Reg::X10, MASK_XLEN(-0x101));
    c.li(Reg::X11, MASK_XLEN(-0x40001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 144);

    // rs2_val == -524289, rs1_val == -134217729
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x8000001;  op2val:-0x80001
    c.li(Reg::X10, MASK_XLEN(-0x8000001));
    c.li(Reg::X11, MASK_XLEN(-0x80001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 148);

    // rs2_val == -1048577, rs1_val == -4194305
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x400001;  op2val:-0x100001
    c.li(Reg::X10, MASK_XLEN(-0x400001));
    c.li(Reg::X11, MASK_XLEN(-0x100001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 152);

    // rs2_val == -2097153, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x3;  op2val:-0x200001
    c.li(Reg::X10, MASK_XLEN(-0x3));
    c.li(Reg::X11, MASK_XLEN(-0x200001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 156);

    // rs2_val == -4194305, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x8000001;  op2val:-0x400001
    c.li(Reg::X10, MASK_XLEN(-0x8000001));
    c.li(Reg::X11, MASK_XLEN(-0x400001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 160);

    // rs2_val == -8388609, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x8000001;  op2val:-0x800001
    c.li(Reg::X10, MASK_XLEN(-0x8000001));
    c.li(Reg::X11, MASK_XLEN(-0x800001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 164);

    // rs2_val == -16777217, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x1000001
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0x1000001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 168);

    // rs2_val == -33554433, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x2000001
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x2000001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 172);

    // rs2_val == -67108865, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x4000001
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0x4000001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 176);

    // rs2_val == -134217729, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x8000001
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0x8000001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 180);

    // rs2_val == -268435457, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x10000001
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x10000001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 184);

    // rs2_val == -536870913, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x20000001
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0x20000001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 188);

    // rs2_val == -1073741825, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x40000001
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(-0x40000001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 192);

    // rs2_val == 1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x100;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x100));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 196);

    // rs2_val == -1431655766, rs1_val == 262144
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x40000;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x40000));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 200);

    // rs1_val == 8, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x8;  op2val:-0x1001
    c.li(Reg::X10, MASK_XLEN(0x8));
    c.li(Reg::X11, MASK_XLEN(-0x1001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 204);

    // rs1_val == 16, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x10;  op2val:-0x40001
    c.li(Reg::X10, MASK_XLEN(0x10));
    c.li(Reg::X11, MASK_XLEN(-0x40001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 208);

    // rs1_val == 64, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x40;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x40));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 212);

    // rs1_val == 128, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x80;  op2val:-0x2001
    c.li(Reg::X10, MASK_XLEN(0x80));
    c.li(Reg::X11, MASK_XLEN(-0x2001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 216);

    // rs1_val == 512, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x200;  op2val:-0x8001
    c.li(Reg::X10, MASK_XLEN(0x200));
    c.li(Reg::X11, MASK_XLEN(-0x8001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 220);

    // rs1_val == 1024, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x400;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x400));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 224);

    // rs1_val == 2048, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x800;  op2val:-0x200001
    c.li(Reg::X10, MASK_XLEN(0x800));
    c.li(Reg::X11, MASK_XLEN(-0x200001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 228);

    // rs1_val == 4096, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x1000;  op2val:0x10
    c.li(Reg::X10, MASK_XLEN(0x1000));
    c.li(Reg::X11, MASK_XLEN(0x10));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 232);

    // rs1_val == 8192, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2000;  op2val:0x800000
    c.li(Reg::X10, MASK_XLEN(0x2000));
    c.li(Reg::X11, MASK_XLEN(0x800000));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 236);

    // rs1_val == 16384, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4000;  op2val:0x20000
    c.li(Reg::X10, MASK_XLEN(0x4000));
    c.li(Reg::X11, MASK_XLEN(0x20000));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 240);

    // rs1_val == 32768, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x8000;  op2val:-0x1
    c.li(Reg::X10, MASK_XLEN(0x8000));
    c.li(Reg::X11, MASK_XLEN(-0x1));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 244);

    // rs1_val == 1048576, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x100000;  op2val:0x8
    c.li(Reg::X10, MASK_XLEN(0x100000));
    c.li(Reg::X11, MASK_XLEN(0x8));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 248);

    // rs1_val == 2097152, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x200000;  op2val:-0x11
    c.li(Reg::X10, MASK_XLEN(0x200000));
    c.li(Reg::X11, MASK_XLEN(-0x11));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 252);

    // rs1_val == 4194304, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x400000;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x400000));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 256);

    // rs1_val == 8388608, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x800000;  op2val:-0x21
    c.li(Reg::X10, MASK_XLEN(0x800000));
    c.li(Reg::X11, MASK_XLEN(-0x21));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 260);

    // rs1_val == 16777216, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x1000000;  op2val:-0x20001
    c.li(Reg::X10, MASK_XLEN(0x1000000));
    c.li(Reg::X11, MASK_XLEN(-0x20001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 264);

    // rs1_val == 67108864, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4000000;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x4000000));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 268);

    // rs1_val == 134217728, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x8000000;  op2val:0x8
    c.li(Reg::X10, MASK_XLEN(0x8000000));
    c.li(Reg::X11, MASK_XLEN(0x8));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 272);

    // rs1_val == 536870912, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x20000000;  op2val:-0x1000001
    c.li(Reg::X10, MASK_XLEN(0x20000000));
    c.li(Reg::X11, MASK_XLEN(-0x1000001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 276);

    // rs1_val == 1073741824, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x40000000;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x40000000));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 280);

    // rs1_val == -2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x2;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 284);

    // rs1_val == -5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x5;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0x5));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 288);

    // rs1_val == -9, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x9;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0x9));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 292);

    // rs1_val == -17, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x11;  op2val:-0x20001
    c.li(Reg::X10, MASK_XLEN(-0x11));
    c.li(Reg::X11, MASK_XLEN(-0x20001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 296);

    // rs1_val == -129, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x81;  op2val:0x400
    c.li(Reg::X10, MASK_XLEN(-0x81));
    c.li(Reg::X11, MASK_XLEN(0x400));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 300);

    // rs1_val == -513, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x201;  op2val:0x800000
    c.li(Reg::X10, MASK_XLEN(-0x201));
    c.li(Reg::X11, MASK_XLEN(0x800000));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 304);

    // rs1_val == -8193, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x2001;  op2val:-0x2
    c.li(Reg::X10, MASK_XLEN(-0x2001));
    c.li(Reg::X11, MASK_XLEN(-0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 308);

    // rs1_val == -16385, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x4001;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0x4001));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 312);

    // rs1_val == -131073, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x20001;  op2val:-0x80001
    c.li(Reg::X10, MASK_XLEN(-0x20001));
    c.li(Reg::X11, MASK_XLEN(-0x80001));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 316);

    // rs1_val == -262145, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x40001;  op2val:0x10
    c.li(Reg::X10, MASK_XLEN(-0x40001));
    c.li(Reg::X11, MASK_XLEN(0x10));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 320);

    // rs1_val == -524289, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x80001;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0x80001));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 324);

    // rs1_val == -1048577, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x100001;  op2val:0x10
    c.li(Reg::X10, MASK_XLEN(-0x100001));
    c.li(Reg::X11, MASK_XLEN(0x10));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 328);

    // rs1_val == -2097153, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x200001;  op2val:-0x7
    c.li(Reg::X10, MASK_XLEN(-0x200001));
    c.li(Reg::X11, MASK_XLEN(-0x7));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 332);

    // rs1_val == -33554433, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x2000001;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(-0x2000001));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 336);

    // rs1_val == -1073741825, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x40000001;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0x40000001));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 340);

    // rs1_val==3 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 344);

    // rs1_val==3 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 348);

    // rs1_val==3 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 352);

    // rs1_val==3 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 356);

    // rs1_val==3 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 360);

    // rs1_val==3 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 364);

    // rs1_val==3 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 368);

    // rs1_val==3 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 372);

    // rs1_val==3 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 376);

    // rs1_val==3 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 380);

    // rs1_val==3 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 384);

    // rs1_val==3 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 388);

    // rs1_val==3 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 392);

    // rs1_val==3 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 396);

    // rs1_val==3 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 400);

    // rs1_val==3 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 404);

    // rs1_val==3 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 408);

    // rs1_val==3 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 412);

    // rs1_val==3 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 416);

    // rs1_val==3 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 420);

    // rs1_val==3 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 424);

    // rs1_val==3 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 428);

    // rs1_val==1431655765 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 432);

    // rs1_val==1431655765 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 436);

    // rs1_val==1431655765 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 440);

    // rs1_val==1431655765 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 444);

    // rs1_val==1431655765 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 448);

    // rs1_val==1431655765 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 452);

    // rs1_val==1431655765 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 456);

    // rs1_val==1431655765 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 460);

    // rs1_val==1431655765 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 464);

    // rs1_val==1431655765 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 468);

    // rs1_val==1431655765 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 472);

    // rs1_val==1431655765 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 476);

    // rs1_val==1431655765 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 480);

    // rs1_val==1431655765 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 484);

    // rs1_val==1431655765 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 488);

    // rs1_val==1431655765 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 492);

    // rs1_val==1431655765 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 496);

    // rs1_val==1431655765 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 500);

    // rs1_val==1431655765 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 504);

    // rs1_val==1431655765 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 508);

    // rs1_val==1431655765 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 512);

    // rs1_val==1431655765 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 516);

    // rs1_val==-1431655766 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 520);

    // rs1_val==-1431655766 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 524);

    // rs1_val==-1431655766 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 528);

    // rs1_val==-1431655766 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 532);

    // rs1_val==-1431655766 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 536);

    // rs1_val==-1431655766 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 540);

    // rs1_val==-1431655766 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 544);

    // rs1_val==-1431655766 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 548);

    // rs1_val==-1431655766 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 552);

    // rs1_val==-1431655766 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 556);

    // rs1_val==-1431655766 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 560);

    // rs1_val==-1431655766 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 564);

    // rs1_val==-1431655766 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 568);

    // rs1_val==-1431655766 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 572);

    // rs1_val==-1431655766 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 576);

    // rs1_val==-1431655766 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 580);

    // rs1_val==-1431655766 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 584);

    // rs1_val==-1431655766 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 588);

    // rs1_val==-1431655766 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 592);

    // rs1_val==-1431655766 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 596);

    // rs1_val==-1431655766 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 600);

    // rs1_val==-1431655766 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 604);

    // rs1_val==5 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 608);

    // rs1_val==5 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 612);

    // rs1_val==5 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 616);

    // rs1_val==5 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 620);

    // rs1_val==5 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 624);

    // rs1_val==5 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 628);

    // rs1_val==5 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 632);

    // rs1_val==5 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 636);

    // rs1_val==5 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 640);

    // rs1_val==5 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 644);

    // rs1_val==5 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 648);

    // rs1_val==5 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 652);

    // rs1_val==5 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 656);

    // rs1_val==5 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 660);

    // rs1_val==5 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 664);

    // rs1_val==5 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 668);

    // rs1_val==5 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 672);

    // rs1_val==5 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 676);

    // rs1_val==5 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 680);

    // rs1_val==5 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 684);

    // rs1_val==5 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 688);

    // rs1_val==5 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 692);

    // rs1_val==858993459 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 696);

    // rs1_val==858993459 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 700);

    // rs1_val==858993459 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 704);

    // rs1_val==858993459 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 708);

    // rs1_val==858993459 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 712);

    // rs1_val==858993459 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 716);

    // rs1_val==858993459 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 720);

    // rs1_val==858993459 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 724);

    // rs1_val==858993459 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 728);

    // rs1_val==858993459 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 732);

    // rs1_val==858993459 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 736);

    // rs1_val==858993459 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 740);

    // rs1_val==858993459 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 744);

    // rs1_val==858993459 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 748);

    // rs1_val==858993459 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 752);

    // rs1_val==858993459 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 756);

    // rs1_val==858993459 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 760);

    // rs1_val==858993459 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 764);

    // rs1_val==858993459 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 768);

    // rs1_val==858993459 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 772);

    // rs1_val==858993459 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 776);

    // rs1_val==858993459 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 780);

    // rs1_val==1717986918 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 784);

    // rs1_val==1717986918 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 788);

    // rs1_val==1717986918 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 792);

    // rs1_val==1717986918 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 796);

    // rs1_val==1717986918 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 800);

    // rs1_val==1717986918 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 804);

    // rs1_val==1717986918 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 808);

    // rs1_val==1717986918 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 812);

    // rs1_val==1717986918 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 816);

    // rs1_val==1717986918 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 820);

    // rs1_val==1717986918 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 824);

    // rs1_val==1717986918 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 828);

    // rs1_val==1717986918 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 832);

    // rs1_val==1717986918 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 836);

    // rs1_val==1717986918 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 840);

    // rs1_val==1717986918 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 844);

    // rs1_val==1717986918 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 848);

    // rs1_val==1717986918 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 852);

    // rs1_val==1717986918 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 856);

    // rs1_val==1717986918 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 860);

    // rs1_val==1717986918 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 864);

    // rs1_val==1717986918 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 868);

    // rs1_val==-46340 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 872);

    // rs1_val==-46340 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 876);

    // rs1_val==-46340 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 880);

    // rs1_val==-46340 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 884);

    // rs1_val==-46340 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 888);

    // rs1_val==-46340 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 892);

    // rs1_val==-46340 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 896);

    // rs1_val==-46340 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 900);

    // rs1_val==-46340 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 904);

    // rs1_val==-46340 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 908);

    // rs1_val==-46340 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 912);

    // rs1_val==-46340 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 916);

    // rs1_val==-46340 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 920);

    // rs1_val==-46340 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 924);

    // rs1_val==-46340 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 928);

    // rs1_val==-46340 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 932);

    // rs1_val==-46340 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 936);

    // rs1_val==-46340 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 940);

    // rs1_val==-46340 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 944);

    // rs1_val==-46340 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 948);

    // rs1_val==-46340 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 952);

    // rs1_val==-46340 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 956);

    // rs1_val==46340 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 960);

    // rs1_val==46340 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 964);

    // rs1_val==46340 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 968);

    // rs1_val==46340 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 972);

    // rs1_val==46340 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 976);

    // rs1_val==46340 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 980);

    // rs1_val==46340 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 984);

    // rs1_val==46340 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 988);

    // rs1_val==46340 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 992);

    // rs1_val==46340 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 996);

    // rs1_val==46340 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1000);

    // rs1_val==46340 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1004);

    // rs1_val==46340 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1008);

    // rs1_val==46340 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1012);

    // rs1_val==46340 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1016);

    // rs1_val==46340 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1020);

    // rs1_val==46340 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1024);

    // rs1_val==46340 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1028);

    // rs1_val==46340 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1032);

    // rs1_val==46340 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1036);

    // rs1_val==46340 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1040);

    // rs1_val==46340 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1044);

    // rs1_val==2 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1048);

    // rs1_val==2 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1052);

    // rs1_val==2 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1056);

    // rs1_val==2 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1060);

    // rs1_val==2 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1064);

    // rs1_val==2 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1068);

    // rs1_val==2 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1072);

    // rs1_val==2 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1076);

    // rs1_val==2 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1080);

    // rs1_val==2 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1084);

    // rs1_val==2 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1088);

    // rs1_val==2 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1092);

    // rs1_val==2 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1096);

    // rs1_val==2 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1100);

    // rs1_val==2 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1104);

    // rs1_val==2 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1108);

    // rs1_val==2 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1112);

    // rs1_val==2 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1116);

    // rs1_val==2 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1120);

    // rs1_val==2 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1124);

    // rs1_val==2 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1128);

    // rs1_val==1431655764 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1132);

    // rs1_val==1431655764 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1136);

    // rs1_val==1431655764 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1140);

    // rs1_val==1431655764 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1144);

    // rs1_val==1431655764 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1148);

    // rs1_val==1431655764 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1152);

    // rs1_val==1431655764 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1156);

    // rs1_val==1431655764 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1160);

    // rs1_val==1431655764 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1164);

    // rs1_val==1431655764 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1168);

    // rs1_val==1431655764 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1172);

    // rs1_val==1431655764 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1176);

    // rs1_val==1431655764 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1180);

    // rs1_val==1431655764 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1184);

    // rs1_val==1431655764 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1188);

    // rs1_val==1431655764 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1192);

    // rs1_val==1431655764 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1196);

    // rs1_val==1431655764 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1200);

    // rs1_val==1431655764 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1204);

    // rs1_val==1431655764 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1208);

    // rs1_val==1431655764 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1212);

    // rs1_val==0 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1216);

    // rs1_val==0 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1220);

    // rs1_val==0 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1224);

    // rs1_val==0 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1228);

    // rs1_val==0 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1232);

    // rs1_val==0 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1236);

    // rs1_val==0 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1240);

    // rs1_val==0 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1244);

    // rs1_val==0 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1248);

    // rs1_val==-1431655765 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1252);

    // rs1_val==-1431655765 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1256);

    // rs1_val==-1431655765 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1260);

    // rs1_val==-1431655765 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1264);

    // rs1_val==-1431655765 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1268);

    // rs1_val==-1431655765 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1272);

    // rs1_val==6 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1276);

    // rs1_val==6 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1280);

    // rs1_val==6 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1284);

    // rs1_val==6 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1288);

    // rs1_val==6 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1292);

    // rs1_val==6 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1296);

    // rs1_val==6 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1300);

    // rs1_val==6 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1304);

    // rs1_val==6 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1308);

    // rs1_val==6 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1312);

    // rs1_val==6 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1316);

    // rs1_val==6 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1320);

    // rs1_val==6 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1324);

    // rs1_val==6 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1328);

    // rs1_val==6 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1332);

    // rs1_val==6 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1336);

    // rs1_val==6 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1340);

    // rs1_val==6 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1344);

    // rs1_val==6 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1348);

    // rs1_val==6 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1352);

    // rs1_val==6 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1356);

    // rs1_val==6 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1360);

    // rs1_val==858993460 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1364);

    // rs1_val==858993460 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1368);

    // rs1_val==858993460 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1372);

    // rs1_val==858993460 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1376);

    // rs1_val==858993460 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1380);

    // rs1_val==858993460 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1384);

    // rs1_val==858993460 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1388);

    // rs1_val==858993460 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1392);

    // rs1_val==858993460 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1396);

    // rs1_val==858993460 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1400);

    // rs1_val==858993460 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1404);

    // rs1_val==858993460 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1408);

    // rs1_val==858993460 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1412);

    // rs1_val==858993460 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1416);

    // rs1_val==858993460 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1420);

    // rs1_val==858993460 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1424);

    // rs1_val==858993460 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1428);

    // rs1_val==858993460 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1432);

    // rs1_val==858993460 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1436);

    // rs1_val==858993460 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1440);

    // rs1_val==858993460 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1444);

    // rs1_val==858993460 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1448);

    // rs1_val==1717986919 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1452);

    // rs1_val==1717986919 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1456);

    // rs1_val==1717986919 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1460);

    // rs1_val==1717986919 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1464);

    // rs1_val==1717986919 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1468);

    // rs1_val==1717986919 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1472);

    // rs1_val==1717986919 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1476);

    // rs1_val==1717986919 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1480);

    // rs1_val==1717986919 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1484);

    // rs1_val==1717986919 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1488);

    // rs1_val==1717986919 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1492);

    // rs1_val==1717986919 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1496);

    // rs1_val==1717986919 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1500);

    // rs1_val==1717986919 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1504);

    // rs1_val==1717986919 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1508);

    // rs1_val==1717986919 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1512);

    // rs1_val==1717986919 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1516);

    // rs1_val==1717986919 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1520);

    // rs1_val==1717986919 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1524);

    // rs1_val==1717986919 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1528);

    // rs1_val==1717986919 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1532);

    // rs1_val==1717986919 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1536);

    // rs1_val==-46339 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1540);

    // rs1_val==-46339 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1544);

    // rs1_val==-46339 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1548);

    // rs1_val==-46339 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1552);

    // rs1_val==-46339 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1556);

    // rs1_val==-46339 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1560);

    // rs1_val==-46339 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1564);

    // rs1_val==-46339 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1568);

    // rs1_val==-46339 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1572);

    // rs1_val==-46339 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1576);

    // rs1_val==-46339 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1580);

    // rs1_val==-46339 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1584);

    // rs1_val==-46339 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1588);

    // rs1_val==-46339 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1592);

    // rs1_val==-46339 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1596);

    // rs1_val==-46339 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1600);

    // rs1_val==-46339 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1604);

    // rs1_val==-46339 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1608);

    // rs1_val==-46339 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1612);

    // rs1_val==-46339 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1616);

    // rs1_val==-46339 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1620);

    // rs1_val==-46339 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1624);

    // rs1_val==46341 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1628);

    // rs1_val==46341 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1632);

    // rs1_val==46341 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1636);

    // rs1_val==46341 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1640);

    // rs1_val==46341 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1644);

    // rs1_val==46341 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1648);

    // rs1_val==46341 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1652);

    // rs1_val==46341 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1656);

    // rs1_val==46341 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1660);

    // rs1_val==46341 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1664);

    // rs1_val==46341 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1668);

    // rs1_val==46341 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1672);

    // rs1_val==46341 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1676);

    // rs1_val==46341 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1680);

    // rs1_val==46341 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1684);

    // rs1_val==46341 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1688);

    // rs1_val==46341 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1692);

    // rs1_val==46341 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1696);

    // rs1_val==46341 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1700);

    // rs1_val==46341 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1704);

    // rs1_val==46341 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1708);

    // rs1_val==46341 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1712);

    // rs1_val==0 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1716);

    // rs1_val==0 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1720);

    // rs1_val==0 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1724);

    // rs1_val==0 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1728);

    // rs1_val==0 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1732);

    // rs1_val==0 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1736);

    // rs1_val==0 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1740);

    // rs1_val==0 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1744);

    // rs1_val==0 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1748);

    // rs1_val==0 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1752);

    // rs1_val==0 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1756);

    // rs1_val==0 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1760);

    // rs1_val==0 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1764);

    // rs1_val==4 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1768);

    // rs1_val==4 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1772);

    // rs1_val==4 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1776);

    // rs1_val==4 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1780);

    // rs1_val==4 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1784);

    // rs1_val==4 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1788);

    // rs1_val==4 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1792);

    // rs1_val==4 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1796);

    // rs1_val==4 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1800);

    // rs1_val==4 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1804);

    // rs1_val==4 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1808);

    // rs1_val==4 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1812);

    // rs1_val==4 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1816);

    // rs1_val==4 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1820);

    // rs1_val==4 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1824);

    // rs1_val==4 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1828);

    // rs1_val==4 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1832);

    // rs1_val==4 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1836);

    // rs1_val==4 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1840);

    // rs1_val==4 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1844);

    // rs1_val==4 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1848);

    // rs1_val==4 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1852);

    // rs1_val==858993458 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1856);

    // rs1_val==858993458 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1860);

    // rs1_val==858993458 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1864);

    // rs1_val==858993458 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1868);

    // rs1_val==858993458 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1872);

    // rs1_val==858993458 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1876);

    // rs1_val==858993458 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1880);

    // rs1_val==858993458 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1884);

    // rs1_val==858993458 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1888);

    // rs1_val==858993458 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1892);

    // rs1_val==858993458 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1896);

    // rs1_val==858993458 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1900);

    // rs1_val==858993458 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1904);

    // rs1_val==858993458 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1908);

    // rs1_val==858993458 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1912);

    // rs1_val==858993458 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1916);

    // rs1_val==858993458 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1920);

    // rs1_val==858993458 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1924);

    // rs1_val==858993458 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1928);

    // rs1_val==858993458 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1932);

    // rs1_val==858993458 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1936);

    // rs1_val==858993458 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1940);

    // rs1_val==1717986917 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1944);

    // rs1_val==1717986917 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1948);

    // rs1_val==1717986917 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1952);

    // rs1_val==1717986917 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1956);

    // rs1_val==1717986917 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1960);

    // rs1_val==1717986917 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1964);

    // rs1_val==1717986917 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1968);

    // rs1_val==1717986917 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1972);

    // rs1_val==1717986917 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1976);

    // rs1_val==1717986917 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1980);

    // rs1_val==1717986917 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1984);

    // rs1_val==1717986917 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1988);

    // rs1_val==1717986917 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1992);

    // rs1_val==1717986917 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 1996);

    // rs1_val==1717986917 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 2000);

    // rs1_val==1717986917 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 2004);

    // rs1_val==1717986917 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 2008);

    // rs1_val==1717986917 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 2012);

    // rs1_val==1717986917 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 2016);

    // rs1_val==1717986917 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 2020);

    // rs1_val==1717986917 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 2024);

    // rs1_val==1717986917 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 2028);

    // rs1_val==46339 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 2032);

    // rs1_val==46339 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 2036);

    // rs1_val==46339 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 2040);

    // rs1_val==46339 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 2044);

    c.li(Reg::X8, 2144);
    // rs1_val==46339 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 0);

    // rs1_val==46339 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 4);

    // rs1_val==46339 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 8);

    // rs1_val==46339 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 12);

    // rs1_val==46339 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 16);

    // rs1_val==46339 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 20);

    // rs1_val==46339 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 24);

    // rs1_val==46339 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 28);

    // rs1_val==46339 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 32);

    // rs1_val==46339 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 36);

    // rs1_val==46339 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 40);

    // rs1_val==46339 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 44);

    // rs1_val==46339 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 48);

    // rs1_val==46339 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 52);

    // rs1_val==46339 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 56);

    // rs1_val==46339 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 60);

    // rs1_val==46339 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 64);

    // rs1_val==46339 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 68);

    // rs1_val==1431655766 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 72);

    // rs1_val==1431655766 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 76);

    // rs1_val==1431655766 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 80);

    // rs1_val==1431655766 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 84);

    // rs1_val==1431655766 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 88);

    // rs1_val==1431655766 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 92);

    // rs1_val==1431655766 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 96);

    // rs1_val==1431655766 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 100);

    // rs1_val==1431655766 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 104);

    // rs1_val==1431655766 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 108);

    // rs1_val==1431655766 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 112);

    // rs1_val==1431655766 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 116);

    // rs1_val==1431655766 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 120);

    // rs1_val==1431655766 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 124);

    // rs1_val==1431655766 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 128);

    // rs1_val==1431655766 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 132);

    // rs1_val==1431655766 and rs2_val==-1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 136);

    // rs1_val==1431655766 and rs2_val==6, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 140);

    // rs1_val==1431655766 and rs2_val==858993460, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 144);

    // rs1_val==1431655766 and rs2_val==1717986919, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 148);

    // rs1_val==1431655766 and rs2_val==-46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 152);

    // rs1_val==1431655766 and rs2_val==46341, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 156);

    // rs1_val==-1431655765 and rs2_val==3, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 160);

    // rs1_val==-1431655765 and rs2_val==1431655765, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 164);

    // rs1_val==-1431655765 and rs2_val==-1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 168);

    // rs1_val==-1431655765 and rs2_val==5, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 172);

    // rs1_val==-1431655765 and rs2_val==858993459, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 176);

    // rs1_val==-1431655765 and rs2_val==1717986918, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 180);

    // rs1_val==-1431655765 and rs2_val==-46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 184);

    // rs1_val==-1431655765 and rs2_val==46340, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 188);

    // rs1_val==-1431655765 and rs2_val==2, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 192);

    // rs1_val==-1431655765 and rs2_val==1431655764, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 196);

    // rs1_val==-1431655765 and rs2_val==0, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 200);

    // rs1_val==-1431655765 and rs2_val==4, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 204);

    // rs1_val==-1431655765 and rs2_val==858993458, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 208);

    // rs1_val==-1431655765 and rs2_val==1717986917, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 212);

    // rs1_val==-1431655765 and rs2_val==46339, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 216);

    // rs1_val==-1431655765 and rs2_val==1431655766, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 220);

    // rs1_val < 0 and rs2_val < 0, rs2_val == -129
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x7;  op2val:-0x81
    c.li(Reg::X10, MASK_XLEN(-0x7));
    c.li(Reg::X11, MASK_XLEN(-0x81));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 224);

    // rs2_val == 0, rs1_val == 32
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x20;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x20));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 228);

    // rs2_val == 128, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x9;  op2val:0x80
    c.li(Reg::X10, MASK_XLEN(0x9));
    c.li(Reg::X11, MASK_XLEN(0x80));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 232);

    // rs2_val == 2048, 
    // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x80000000;  op2val:0x800
    c.li(Reg::X10, MASK_XLEN(-0x80000000));
    c.li(Reg::X11, MASK_XLEN(0x800));
    c.sub(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X8, 236);


        
    // End
    c.ecall();
    return c;
}