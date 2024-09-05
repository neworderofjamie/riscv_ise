#include "xor-01.h"

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
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, };
}

//----------------------------------------------------------------------------
// xor_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &xor_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
CodeGenerator xor_01::generateCode()
{
    CodeGenerator c;
    
    c.li(Reg::X7, 16);
    // rs2 == rd != rs1, rs1==x27, rs2==x24, rd==x24, rs1_val > 0 and rs2_val > 0, rs1_val != rs2_val, rs1_val==1717986917 and rs2_val==3
    // opcode: xor ; op1:x27; op2:x24; dest:x24; op1val:0x66666665;  op2val:0x3
    c.li(Reg::X27, MASK_XLEN(0x66666665));
    c.li(Reg::X24, MASK_XLEN(0x3));
    c.xor_(Reg::X24, Reg::X27, Reg::X24);
    c.sw(Reg::X24, Reg::X7, 0);

    // rs1 == rs2 != rd, rs1==x13, rs2==x13, rd==x10, rs1_val > 0 and rs2_val < 0, 
    // opcode: xor ; op1:x13; op2:x13; dest:x10; op1val:0x5;  op2val:0x5
    c.li(Reg::X13, MASK_XLEN(0x5));
    c.li(Reg::X13, MASK_XLEN(0x5));
    c.xor_(Reg::X10, Reg::X13, Reg::X13);
    c.sw(Reg::X10, Reg::X7, 4);

    // rs1 == rs2 == rd, rs1==x23, rs2==x23, rd==x23, rs1_val < 0 and rs2_val < 0, rs1_val == -16385, rs2_val == -65537
    // opcode: xor ; op1:x23; op2:x23; dest:x23; op1val:-0x4001;  op2val:-0x4001
    c.li(Reg::X23, MASK_XLEN(-0x4001));
    c.li(Reg::X23, MASK_XLEN(-0x4001));
    c.xor_(Reg::X23, Reg::X23, Reg::X23);
    c.sw(Reg::X23, Reg::X7, 8);

    // rs1 == rd != rs2, rs1==x28, rs2==x14, rd==x28, rs1_val < 0 and rs2_val > 0, rs2_val == 8, rs1_val == -65
    // opcode: xor ; op1:x28; op2:x14; dest:x28; op1val:-0x41;  op2val:0x8
    c.li(Reg::X28, MASK_XLEN(-0x41));
    c.li(Reg::X14, MASK_XLEN(0x8));
    c.xor_(Reg::X28, Reg::X28, Reg::X14);
    c.sw(Reg::X28, Reg::X7, 12);

    // rs1 != rs2  and rs1 != rd and rs2 != rd, rs1==x1, rs2==x2, rd==x18, rs1_val == rs2_val, 
    // opcode: xor ; op1:x1; op2:x2; dest:x18; op1val:-0x1;  op2val:-0x1
    c.li(Reg::X1, MASK_XLEN(-0x1));
    c.li(Reg::X2, MASK_XLEN(-0x1));
    c.xor_(Reg::X18, Reg::X1, Reg::X2);
    c.sw(Reg::X18, Reg::X7, 16);

    // rs1==x5, rs2==x22, rd==x19, rs2_val == (-2**(xlen-1)), rs1_val == 4194304, rs2_val == -2147483648
    // opcode: xor ; op1:x5; op2:x22; dest:x19; op1val:0x400000;  op2val:-0x80000000
    c.li(Reg::X5, MASK_XLEN(0x400000));
    c.li(Reg::X22, MASK_XLEN(-0x80000000));
    c.xor_(Reg::X19, Reg::X5, Reg::X22);
    c.sw(Reg::X19, Reg::X7, 20);

    // rs1==x26, rs2==x12, rd==x13, rs2_val == 0, rs1_val == -17
    // opcode: xor ; op1:x26; op2:x12; dest:x13; op1val:-0x11;  op2val:0x0
    c.li(Reg::X26, MASK_XLEN(-0x11));
    c.li(Reg::X12, MASK_XLEN(0x0));
    c.xor_(Reg::X13, Reg::X26, Reg::X12);
    c.sw(Reg::X13, Reg::X7, 24);

    // rs1==x12, rs2==x11, rd==x4, rs2_val == (2**(xlen-1)-1), rs2_val == 2147483647, rs1_val == -1431655766
    // opcode: xor ; op1:x12; op2:x11; dest:x4; op1val:-0x55555556;  op2val:0x7fffffff
    c.li(Reg::X12, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x7fffffff));
    c.xor_(Reg::X4, Reg::X12, Reg::X11);
    c.sw(Reg::X4, Reg::X7, 28);

    // rs1==x19, rs2==x30, rd==x17, rs2_val == 1, rs1_val == 1
    // opcode: xor ; op1:x19; op2:x30; dest:x17; op1val:0x1;  op2val:0x1
    c.li(Reg::X19, MASK_XLEN(0x1));
    c.li(Reg::X30, MASK_XLEN(0x1));
    c.xor_(Reg::X17, Reg::X19, Reg::X30);
    c.sw(Reg::X17, Reg::X7, 32);

    // rs1==x11, rs2==x1, rd==x3, rs1_val == (-2**(xlen-1)), rs2_val == -268435457, rs1_val == -2147483648
    // opcode: xor ; op1:x11; op2:x1; dest:x3; op1val:-0x80000000;  op2val:-0x10000001
    c.li(Reg::X11, MASK_XLEN(-0x80000000));
    c.li(Reg::X1, MASK_XLEN(-0x10000001));
    c.xor_(Reg::X3, Reg::X11, Reg::X1);
    c.sw(Reg::X3, Reg::X7, 36);

    // rs1==x24, rs2==x29, rd==x8, rs1_val == 0, rs1_val==0 and rs2_val==-46340
    // opcode: xor ; op1:x24; op2:x29; dest:x8; op1val:0x0;  op2val:-0xb504
    c.li(Reg::X24, MASK_XLEN(0x0));
    c.li(Reg::X29, MASK_XLEN(-0xb504));
    c.xor_(Reg::X8, Reg::X24, Reg::X29);
    c.sw(Reg::X8, Reg::X7, 40);

    // rs1==x0, rs2==x18, rd==x9, rs1_val == (2**(xlen-1)-1), rs2_val == 4096, rs1_val == 2147483647
    // opcode: xor ; op1:x0; op2:x18; dest:x9; op1val:0x0;  op2val:0x1000
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.li(Reg::X18, MASK_XLEN(0x1000));
    c.xor_(Reg::X9, Reg::X0, Reg::X18);
    c.sw(Reg::X9, Reg::X7, 44);

    // rs1==x10, rs2==x6, rd==x26, rs2_val == 2, rs1_val == 524288
    // opcode: xor ; op1:x10; op2:x6; dest:x26; op1val:0x80000;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x80000));
    c.li(Reg::X6, MASK_XLEN(0x2));
    c.xor_(Reg::X26, Reg::X10, Reg::X6);
    c.sw(Reg::X26, Reg::X7, 48);

    // rs1==x22, rs2==x31, rd==x30, rs2_val == 4, rs1_val == -33
    // opcode: xor ; op1:x22; op2:x31; dest:x30; op1val:-0x21;  op2val:0x4
    c.li(Reg::X22, MASK_XLEN(-0x21));
    c.li(Reg::X31, MASK_XLEN(0x4));
    c.xor_(Reg::X30, Reg::X22, Reg::X31);
    c.sw(Reg::X30, Reg::X7, 52);

    // rs1==x8, rs2==x0, rd==x16, rs2_val == 16, 
    // opcode: xor ; op1:x8; op2:x0; dest:x16; op1val:0x6;  op2val:0x0
    c.li(Reg::X8, MASK_XLEN(0x6));
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.xor_(Reg::X16, Reg::X8, Reg::X0);
    c.sw(Reg::X16, Reg::X7, 56);

    // rs1==x16, rs2==x27, rd==x15, rs2_val == 32, 
    // opcode: xor ; op1:x16; op2:x27; dest:x15; op1val:0x5;  op2val:0x20
    c.li(Reg::X16, MASK_XLEN(0x5));
    c.li(Reg::X27, MASK_XLEN(0x20));
    c.xor_(Reg::X15, Reg::X16, Reg::X27);
    c.sw(Reg::X15, Reg::X7, 60);

    // rs1==x3, rs2==x26, rd==x31, rs2_val == 64, rs1_val == -2097153
    // opcode: xor ; op1:x3; op2:x26; dest:x31; op1val:-0x200001;  op2val:0x40
    c.li(Reg::X3, MASK_XLEN(-0x200001));
    c.li(Reg::X26, MASK_XLEN(0x40));
    c.xor_(Reg::X31, Reg::X3, Reg::X26);
    c.sw(Reg::X31, Reg::X7, 64);

    // rs1==x9, rs2==x25, rd==x14, rs2_val == 128, rs1_val == -32769
    // opcode: xor ; op1:x9; op2:x25; dest:x14; op1val:-0x8001;  op2val:0x80
    c.li(Reg::X9, MASK_XLEN(-0x8001));
    c.li(Reg::X25, MASK_XLEN(0x80));
    c.xor_(Reg::X14, Reg::X9, Reg::X25);
    c.sw(Reg::X14, Reg::X7, 68);

    c.li(Reg::X13, 88);
    // rs1==x30, rs2==x10, rd==x6, rs2_val == 256, 
    // opcode: xor ; op1:x30; op2:x10; dest:x6; op1val:0x55555556;  op2val:0x100
    c.li(Reg::X30, MASK_XLEN(0x55555556));
    c.li(Reg::X10, MASK_XLEN(0x100));
    c.xor_(Reg::X6, Reg::X30, Reg::X10);
    c.sw(Reg::X6, Reg::X13, 0);

    // rs1==x2, rs2==x9, rd==x7, rs2_val == 512, 
    // opcode: xor ; op1:x2; op2:x9; dest:x7; op1val:-0x4001;  op2val:0x200
    c.li(Reg::X2, MASK_XLEN(-0x4001));
    c.li(Reg::X9, MASK_XLEN(0x200));
    c.xor_(Reg::X7, Reg::X2, Reg::X9);
    c.sw(Reg::X7, Reg::X13, 4);

    // rs1==x6, rs2==x17, rd==x0, rs2_val == 1024, rs1_val == 128
    // opcode: xor ; op1:x6; op2:x17; dest:x0; op1val:0x80;  op2val:0x400
    c.li(Reg::X6, MASK_XLEN(0x80));
    c.li(Reg::X17, MASK_XLEN(0x400));
    c.xor_(Reg::X0, Reg::X6, Reg::X17);
    c.sw(Reg::X0, Reg::X13, 8);

    // rs1==x15, rs2==x5, rd==x2, rs2_val == 2048, 
    // opcode: xor ; op1:x15; op2:x5; dest:x2; op1val:-0x8001;  op2val:0x800
    c.li(Reg::X15, MASK_XLEN(-0x8001));
    c.li(Reg::X5, MASK_XLEN(0x800));
    c.xor_(Reg::X2, Reg::X15, Reg::X5);
    c.sw(Reg::X2, Reg::X13, 12);

    // rs1==x7, rs2==x15, rd==x22, rs2_val == 8192, 
    // opcode: xor ; op1:x7; op2:x15; dest:x22; op1val:0x6;  op2val:0x2000
    c.li(Reg::X7, MASK_XLEN(0x6));
    c.li(Reg::X15, MASK_XLEN(0x2000));
    c.xor_(Reg::X22, Reg::X7, Reg::X15);
    c.sw(Reg::X22, Reg::X13, 16);

    // rs1==x14, rs2==x16, rd==x20, rs2_val == 16384, rs1_val == -9
    // opcode: xor ; op1:x14; op2:x16; dest:x20; op1val:-0x9;  op2val:0x4000
    c.li(Reg::X14, MASK_XLEN(-0x9));
    c.li(Reg::X16, MASK_XLEN(0x4000));
    c.xor_(Reg::X20, Reg::X14, Reg::X16);
    c.sw(Reg::X20, Reg::X13, 20);

    // rs1==x29, rs2==x20, rd==x1, rs2_val == 32768, rs1_val == 32768
    // opcode: xor ; op1:x29; op2:x20; dest:x1; op1val:0x8000;  op2val:0x8000
    c.li(Reg::X29, MASK_XLEN(0x8000));
    c.li(Reg::X20, MASK_XLEN(0x8000));
    c.xor_(Reg::X1, Reg::X29, Reg::X20);
    c.sw(Reg::X1, Reg::X13, 24);

    // rs1==x18, rs2==x8, rd==x12, rs2_val == 65536, rs1_val == 4
    // opcode: xor ; op1:x18; op2:x8; dest:x12; op1val:0x4;  op2val:0x10000
    c.li(Reg::X18, MASK_XLEN(0x4));
    c.li(Reg::X8, MASK_XLEN(0x10000));
    c.xor_(Reg::X12, Reg::X18, Reg::X8);
    c.sw(Reg::X12, Reg::X13, 28);

    // rs1==x21, rs2==x28, rd==x5, rs2_val == 131072, 
    // opcode: xor ; op1:x21; op2:x28; dest:x5; op1val:0x33333332;  op2val:0x20000
    c.li(Reg::X21, MASK_XLEN(0x33333332));
    c.li(Reg::X28, MASK_XLEN(0x20000));
    c.xor_(Reg::X5, Reg::X21, Reg::X28);
    c.sw(Reg::X5, Reg::X13, 32);

    // rs1==x17, rs2==x4, rd==x11, rs2_val == 262144, rs1_val == -67108865
    // opcode: xor ; op1:x17; op2:x4; dest:x11; op1val:-0x4000001;  op2val:0x40000
    c.li(Reg::X17, MASK_XLEN(-0x4000001));
    c.li(Reg::X4, MASK_XLEN(0x40000));
    c.xor_(Reg::X11, Reg::X17, Reg::X4);
    c.sw(Reg::X11, Reg::X13, 36);

    // rs1==x4, rs2==x21, rd==x27, rs2_val == 524288, 
    // opcode: xor ; op1:x4; op2:x21; dest:x27; op1val:0x7;  op2val:0x80000
    c.li(Reg::X4, MASK_XLEN(0x7));
    c.li(Reg::X21, MASK_XLEN(0x80000));
    c.xor_(Reg::X27, Reg::X4, Reg::X21);
    c.sw(Reg::X27, Reg::X13, 40);

    // rs1==x31, rs2==x3, rd==x29, rs2_val == 1048576, 
    // opcode: xor ; op1:x31; op2:x3; dest:x29; op1val:-0x1;  op2val:0x100000
    c.li(Reg::X31, MASK_XLEN(-0x1));
    c.li(Reg::X3, MASK_XLEN(0x100000));
    c.xor_(Reg::X29, Reg::X31, Reg::X3);
    c.sw(Reg::X29, Reg::X13, 44);

    // rs1==x25, rs2==x7, rd==x21, rs2_val == 2097152, 
    // opcode: xor ; op1:x25; op2:x7; dest:x21; op1val:0xb505;  op2val:0x200000
    c.li(Reg::X25, MASK_XLEN(0xb505));
    c.li(Reg::X7, MASK_XLEN(0x200000));
    c.xor_(Reg::X21, Reg::X25, Reg::X7);
    c.sw(Reg::X21, Reg::X13, 48);

    // rs1==x20, rs2==x19, rd==x25, rs2_val == 4194304, 
    // opcode: xor ; op1:x20; op2:x19; dest:x25; op1val:0xb505;  op2val:0x400000
    c.li(Reg::X20, MASK_XLEN(0xb505));
    c.li(Reg::X19, MASK_XLEN(0x400000));
    c.xor_(Reg::X25, Reg::X20, Reg::X19);
    c.sw(Reg::X25, Reg::X13, 52);

    // rs2_val == 8388608, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x800000
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x800000));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 56);

    // rs2_val == 16777216, rs1_val == -268435457
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x10000001;  op2val:0x1000000
    c.li(Reg::X10, MASK_XLEN(-0x10000001));
    c.li(Reg::X11, MASK_XLEN(0x1000000));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 60);

    // rs2_val == 33554432, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x2000000
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x2000000));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 64);

    // rs2_val == 67108864, rs1_val == -8388609
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x800001;  op2val:0x4000000
    c.li(Reg::X10, MASK_XLEN(-0x800001));
    c.li(Reg::X11, MASK_XLEN(0x4000000));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 68);

    // rs2_val == 134217728, rs1_val == 4096
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x1000;  op2val:0x8000000
    c.li(Reg::X10, MASK_XLEN(0x1000));
    c.li(Reg::X11, MASK_XLEN(0x8000000));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 72);

    // rs2_val == 268435456, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x9;  op2val:0x10000000
    c.li(Reg::X10, MASK_XLEN(-0x9));
    c.li(Reg::X11, MASK_XLEN(0x10000000));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 76);

    // rs2_val == 536870912, rs1_val == -33554433
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x2000001;  op2val:0x20000000
    c.li(Reg::X10, MASK_XLEN(-0x2000001));
    c.li(Reg::X11, MASK_XLEN(0x20000000));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 80);

    // rs2_val == 1073741824, rs1_val == -131073
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x20001;  op2val:0x40000000
    c.li(Reg::X10, MASK_XLEN(-0x20001));
    c.li(Reg::X11, MASK_XLEN(0x40000000));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 84);

    // rs2_val == -2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x2000001;  op2val:-0x2
    c.li(Reg::X10, MASK_XLEN(-0x2000001));
    c.li(Reg::X11, MASK_XLEN(-0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 88);

    // rs2_val == -3, rs1_val == 1073741824
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x40000000;  op2val:-0x3
    c.li(Reg::X10, MASK_XLEN(0x40000000));
    c.li(Reg::X11, MASK_XLEN(-0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 92);

    // rs2_val == -5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x5
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 96);

    // rs2_val == -9, rs1_val == 33554432
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2000000;  op2val:-0x9
    c.li(Reg::X10, MASK_XLEN(0x2000000));
    c.li(Reg::X11, MASK_XLEN(-0x9));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 100);

    // rs2_val == -17, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x21;  op2val:-0x11
    c.li(Reg::X10, MASK_XLEN(-0x21));
    c.li(Reg::X11, MASK_XLEN(-0x11));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 104);

    // rs2_val == -33, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x21
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0x21));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 108);

    // rs2_val == -65, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x41
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0x41));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 112);

    // rs2_val == -129, rs1_val == 131072
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x20000;  op2val:-0x81
    c.li(Reg::X10, MASK_XLEN(0x20000));
    c.li(Reg::X11, MASK_XLEN(-0x81));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 116);

    // rs2_val == -257, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x10000001;  op2val:-0x101
    c.li(Reg::X10, MASK_XLEN(-0x10000001));
    c.li(Reg::X11, MASK_XLEN(-0x101));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 120);

    // rs2_val == -513, rs1_val == -5
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x5;  op2val:-0x201
    c.li(Reg::X10, MASK_XLEN(-0x5));
    c.li(Reg::X11, MASK_XLEN(-0x201));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 124);

    // rs2_val == -1025, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x11;  op2val:-0x401
    c.li(Reg::X10, MASK_XLEN(-0x11));
    c.li(Reg::X11, MASK_XLEN(-0x401));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 128);

    // rs2_val == -2049, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x801
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x801));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 132);

    // rs2_val == -4097, rs1_val == 2
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x1001
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0x1001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 136);

    // rs2_val == -8193, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x7;  op2val:-0x2001
    c.li(Reg::X10, MASK_XLEN(-0x7));
    c.li(Reg::X11, MASK_XLEN(-0x2001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 140);

    // rs2_val == -16385, rs1_val == -16777217
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x1000001;  op2val:-0x4001
    c.li(Reg::X10, MASK_XLEN(-0x1000001));
    c.li(Reg::X11, MASK_XLEN(-0x4001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 144);

    // rs2_val == -32769, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x8001
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0x8001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 148);

    // rs2_val == -131073, rs1_val == 16384
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4000;  op2val:-0x20001
    c.li(Reg::X10, MASK_XLEN(0x4000));
    c.li(Reg::X11, MASK_XLEN(-0x20001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 152);

    // rs2_val == -262145, rs1_val == 262144
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x40000;  op2val:-0x40001
    c.li(Reg::X10, MASK_XLEN(0x40000));
    c.li(Reg::X11, MASK_XLEN(-0x40001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 156);

    // rs2_val == -524289, rs1_val == 268435456
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x10000000;  op2val:-0x80001
    c.li(Reg::X10, MASK_XLEN(0x10000000));
    c.li(Reg::X11, MASK_XLEN(-0x80001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 160);

    // rs2_val == -1048577, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x80000000;  op2val:-0x100001
    c.li(Reg::X10, MASK_XLEN(-0x80000000));
    c.li(Reg::X11, MASK_XLEN(-0x100001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 164);

    // rs2_val == -2097153, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x8000;  op2val:-0x200001
    c.li(Reg::X10, MASK_XLEN(0x8000));
    c.li(Reg::X11, MASK_XLEN(-0x200001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 168);

    // rs2_val == -4194305, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x80000000;  op2val:-0x400001
    c.li(Reg::X10, MASK_XLEN(-0x80000000));
    c.li(Reg::X11, MASK_XLEN(-0x400001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 172);

    // rs2_val == -8388609, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x800001
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0x800001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 176);

    // rs2_val == -16777217, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x40000000;  op2val:-0x1000001
    c.li(Reg::X10, MASK_XLEN(-0x40000000));
    c.li(Reg::X11, MASK_XLEN(-0x1000001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 180);

    // rs2_val == -33554433, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x2000001;  op2val:-0x2000001
    c.li(Reg::X10, MASK_XLEN(-0x2000001));
    c.li(Reg::X11, MASK_XLEN(-0x2000001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 184);

    // rs2_val == -67108865, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:-0x4000001
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(-0x4000001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 188);

    // rs2_val == -134217729, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x8000001
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0x8000001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 192);

    // rs2_val == -536870913, rs1_val == -524289
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x80001;  op2val:-0x20000001
    c.li(Reg::X10, MASK_XLEN(-0x80001));
    c.li(Reg::X11, MASK_XLEN(-0x20000001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 196);

    // rs2_val == -1073741825, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x40000001
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x40000001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 200);

    // rs2_val == 1431655765, rs1_val==1431655764 and rs2_val==1431655765
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 204);

    // rs2_val == -1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2000000;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x2000000));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 208);

    // rs1_val == 8, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x8;  op2val:-0x4
    c.li(Reg::X10, MASK_XLEN(0x8));
    c.li(Reg::X11, MASK_XLEN(-0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 212);

    // rs1_val == 16, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x10;  op2val:0x10
    c.li(Reg::X10, MASK_XLEN(0x10));
    c.li(Reg::X11, MASK_XLEN(0x10));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 216);

    // rs1_val == 32, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x20;  op2val:0x40
    c.li(Reg::X10, MASK_XLEN(0x20));
    c.li(Reg::X11, MASK_XLEN(0x40));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 220);

    // rs1_val == 64, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x40;  op2val:-0x21
    c.li(Reg::X10, MASK_XLEN(0x40));
    c.li(Reg::X11, MASK_XLEN(-0x21));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 224);

    // rs1_val == 256, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x100;  op2val:-0x400001
    c.li(Reg::X10, MASK_XLEN(0x100));
    c.li(Reg::X11, MASK_XLEN(-0x400001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 228);

    // rs1_val == 512, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x200;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x200));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 232);

    // rs1_val == 1024, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x400;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x400));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 236);

    // rs1_val == 2048, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x800;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x800));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 240);

    // rs1_val == 8192, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2000;  op2val:-0x100001
    c.li(Reg::X10, MASK_XLEN(0x2000));
    c.li(Reg::X11, MASK_XLEN(-0x100001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 244);

    // rs1_val == 65536, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 248);

    // rs1_val == 1048576, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x100000;  op2val:-0x9
    c.li(Reg::X10, MASK_XLEN(0x100000));
    c.li(Reg::X11, MASK_XLEN(-0x9));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 252);

    // rs1_val == 2097152, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x200000;  op2val:-0x4001
    c.li(Reg::X10, MASK_XLEN(0x200000));
    c.li(Reg::X11, MASK_XLEN(-0x4001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 256);

    // rs1_val == 8388608, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x800000;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x800000));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 260);

    // rs1_val == 16777216, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x1000000;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x1000000));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 264);

    // rs1_val == 67108864, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4000000;  op2val:0x400000
    c.li(Reg::X10, MASK_XLEN(0x4000000));
    c.li(Reg::X11, MASK_XLEN(0x400000));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 268);

    // rs1_val == 134217728, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x8000000;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x8000000));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 272);

    // rs1_val == 536870912, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x20000000;  op2val:0x400
    c.li(Reg::X10, MASK_XLEN(0x20000000));
    c.li(Reg::X11, MASK_XLEN(0x400));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 276);

    // rs1_val == -2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x2;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0x2));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 280);

    // rs1_val == -3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x3;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0x3));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 284);

    // rs1_val == -129, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x81;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0x81));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 288);

    // rs1_val == -257, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x101;  op2val:0x1000
    c.li(Reg::X10, MASK_XLEN(-0x101));
    c.li(Reg::X11, MASK_XLEN(0x1000));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 292);

    // rs1_val == -513, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x201;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0x201));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 296);

    // rs1_val == -1025, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x401;  op2val:-0x10000001
    c.li(Reg::X10, MASK_XLEN(-0x401));
    c.li(Reg::X11, MASK_XLEN(-0x10000001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 300);

    // rs1_val == -2049, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x801;  op2val:0x2000000
    c.li(Reg::X10, MASK_XLEN(-0x801));
    c.li(Reg::X11, MASK_XLEN(0x2000000));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 304);

    // rs1_val == -4097, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x1001;  op2val:-0x2
    c.li(Reg::X10, MASK_XLEN(-0x1001));
    c.li(Reg::X11, MASK_XLEN(-0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 308);

    // rs1_val == -8193, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x2001;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0x2001));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 312);

    // rs1_val == -65537, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x10001;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(-0x10001));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 316);

    // rs1_val == -262145, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x40001;  op2val:-0x80001
    c.li(Reg::X10, MASK_XLEN(-0x40001));
    c.li(Reg::X11, MASK_XLEN(-0x80001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 320);

    // rs1_val == -1048577, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x100001;  op2val:0x20
    c.li(Reg::X10, MASK_XLEN(-0x100001));
    c.li(Reg::X11, MASK_XLEN(0x20));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 324);

    // rs1_val == -4194305, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x400001;  op2val:-0xa
    c.li(Reg::X10, MASK_XLEN(-0x400001));
    c.li(Reg::X11, MASK_XLEN(-0xa));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 328);

    // rs1_val == -134217729, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x8000001;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0x8000001));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 332);

    // rs1_val == -536870913, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x20000001;  op2val:0x1000000
    c.li(Reg::X10, MASK_XLEN(-0x20000001));
    c.li(Reg::X11, MASK_XLEN(0x1000000));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 336);

    // rs1_val == -1073741825, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x40000001;  op2val:-0x10000001
    c.li(Reg::X10, MASK_XLEN(-0x40000001));
    c.li(Reg::X11, MASK_XLEN(-0x10000001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 340);

    // rs1_val == 1431655765, rs1_val==1431655765 and rs2_val==858993460
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 344);

    // rs1_val==3 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 348);

    // rs1_val==3 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 352);

    // rs1_val==3 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 356);

    // rs1_val==3 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 360);

    // rs1_val==3 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 364);

    // rs1_val==3 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 368);

    // rs1_val==3 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 372);

    // rs1_val==3 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 376);

    // rs1_val==3 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 380);

    // rs1_val==3 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 384);

    // rs1_val==3 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 388);

    // rs1_val==3 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 392);

    // rs1_val==3 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 396);

    // rs1_val==3 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 400);

    // rs1_val==3 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 404);

    // rs1_val==3 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 408);

    // rs1_val==3 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 412);

    // rs1_val==3 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 416);

    // rs1_val==3 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 420);

    // rs1_val==3 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 424);

    // rs1_val==3 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 428);

    // rs1_val==3 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 432);

    // rs1_val==1431655765 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 436);

    // rs1_val==1431655765 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 440);

    // rs1_val==1431655765 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 444);

    // rs1_val==1431655765 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 448);

    // rs1_val==1431655765 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 452);

    // rs1_val==1431655765 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 456);

    // rs1_val==1431655765 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 460);

    // rs1_val==1431655765 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 464);

    // rs1_val==1431655765 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 468);

    // rs1_val==1431655765 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 472);

    // rs1_val==1431655765 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 476);

    // rs1_val==1431655765 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 480);

    // rs1_val==1431655765 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 484);

    // rs1_val==1431655765 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 488);

    // rs1_val==1431655765 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 492);

    // rs1_val==1431655765 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 496);

    // rs1_val==1431655765 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 500);

    // rs1_val==1431655765 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 504);

    // rs1_val==1431655765 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 508);

    // rs1_val==1431655765 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 512);

    // rs1_val==1431655765 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 516);

    // rs1_val==-1431655766 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 520);

    // rs1_val==-1431655766 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 524);

    // rs1_val==-1431655766 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 528);

    // rs1_val==-1431655766 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 532);

    // rs1_val==-1431655766 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 536);

    // rs1_val==-1431655766 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 540);

    // rs1_val==-1431655766 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 544);

    // rs1_val==-1431655766 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 548);

    // rs1_val==-1431655766 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 552);

    // rs1_val==-1431655766 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 556);

    // rs1_val==-1431655766 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 560);

    // rs1_val==-1431655766 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 564);

    // rs1_val==-1431655766 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 568);

    // rs1_val==-1431655766 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 572);

    // rs1_val==-1431655766 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 576);

    // rs1_val==-1431655766 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 580);

    // rs1_val==-1431655766 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 584);

    // rs1_val==-1431655766 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 588);

    // rs1_val==-1431655766 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 592);

    // rs1_val==-1431655766 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 596);

    // rs1_val==-1431655766 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 600);

    // rs1_val==-1431655766 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 604);

    // rs1_val==5 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 608);

    // rs1_val==5 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 612);

    // rs1_val==5 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 616);

    // rs1_val==5 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 620);

    // rs1_val==5 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 624);

    // rs1_val==5 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 628);

    // rs1_val==5 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 632);

    // rs1_val==5 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 636);

    // rs1_val==5 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 640);

    // rs1_val==5 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 644);

    // rs1_val==5 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 648);

    // rs1_val==5 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 652);

    // rs1_val==5 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 656);

    // rs1_val==5 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 660);

    // rs1_val==5 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 664);

    // rs1_val==5 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 668);

    // rs1_val==5 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 672);

    // rs1_val==5 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 676);

    // rs1_val==5 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 680);

    // rs1_val==5 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 684);

    // rs1_val==5 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 688);

    // rs1_val==5 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 692);

    // rs1_val==858993459 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 696);

    // rs1_val==858993459 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 700);

    // rs1_val==858993459 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 704);

    // rs1_val==858993459 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 708);

    // rs1_val==858993459 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 712);

    // rs1_val==858993459 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 716);

    // rs1_val==858993459 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 720);

    // rs1_val==858993459 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 724);

    // rs1_val==858993459 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 728);

    // rs1_val==858993459 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 732);

    // rs1_val==858993459 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 736);

    // rs1_val==858993459 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 740);

    // rs1_val==858993459 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 744);

    // rs1_val==858993459 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 748);

    // rs1_val==858993459 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 752);

    // rs1_val==858993459 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 756);

    // rs1_val==858993459 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 760);

    // rs1_val==858993459 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 764);

    // rs1_val==858993459 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 768);

    // rs1_val==858993459 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 772);

    // rs1_val==858993459 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 776);

    // rs1_val==858993459 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 780);

    // rs1_val==1717986918 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 784);

    // rs1_val==1717986918 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 788);

    // rs1_val==1717986918 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 792);

    // rs1_val==1717986918 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 796);

    // rs1_val==1717986918 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 800);

    // rs1_val==1717986918 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 804);

    // rs1_val==1717986918 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 808);

    // rs1_val==1717986918 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 812);

    // rs1_val==1717986918 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 816);

    // rs1_val==1717986918 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 820);

    // rs1_val==1717986918 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 824);

    // rs1_val==1717986918 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 828);

    // rs1_val==1717986918 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 832);

    // rs1_val==1717986918 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 836);

    // rs1_val==1717986918 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 840);

    // rs1_val==1717986918 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 844);

    // rs1_val==1717986918 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 848);

    // rs1_val==1717986918 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 852);

    // rs1_val==1717986918 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 856);

    // rs1_val==1717986918 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 860);

    // rs1_val==1717986918 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 864);

    // rs1_val==1717986918 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 868);

    // rs1_val==-46340 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 872);

    // rs1_val==-46340 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 876);

    // rs1_val==-46340 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 880);

    // rs1_val==-46340 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 884);

    // rs1_val==-46340 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 888);

    // rs1_val==-46340 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 892);

    // rs1_val==-46340 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 896);

    // rs1_val==-46340 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 900);

    // rs1_val==-46340 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 904);

    // rs1_val==-46340 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 908);

    // rs1_val==-46340 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 912);

    // rs1_val==-46340 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 916);

    // rs1_val==-46340 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 920);

    // rs1_val==-46340 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 924);

    // rs1_val==-46340 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 928);

    // rs1_val==-46340 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 932);

    // rs1_val==-46340 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 936);

    // rs1_val==-46340 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 940);

    // rs1_val==-46340 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 944);

    // rs1_val==-46340 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 948);

    // rs1_val==-46340 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 952);

    // rs1_val==-46340 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 956);

    // rs1_val==46340 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 960);

    // rs1_val==46340 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 964);

    // rs1_val==46340 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 968);

    // rs1_val==46340 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 972);

    // rs1_val==46340 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 976);

    // rs1_val==46340 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 980);

    // rs1_val==46340 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 984);

    // rs1_val==46340 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 988);

    // rs1_val==46340 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 992);

    // rs1_val==46340 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 996);

    // rs1_val==46340 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1000);

    // rs1_val==46340 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1004);

    // rs1_val==46340 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1008);

    // rs1_val==46340 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1012);

    // rs1_val==46340 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1016);

    // rs1_val==46340 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1020);

    // rs1_val==46340 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1024);

    // rs1_val==46340 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1028);

    // rs1_val==46340 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1032);

    // rs1_val==46340 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1036);

    // rs1_val==46340 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1040);

    // rs1_val==46340 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1044);

    // rs1_val==2 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1048);

    // rs1_val==2 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1052);

    // rs1_val==2 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1056);

    // rs1_val==2 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1060);

    // rs1_val==2 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1064);

    // rs1_val==2 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1068);

    // rs1_val==2 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1072);

    // rs1_val==2 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1076);

    // rs1_val==2 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1080);

    // rs1_val==2 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1084);

    // rs1_val==2 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1088);

    // rs1_val==2 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1092);

    // rs1_val==2 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1096);

    // rs1_val==2 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1100);

    // rs1_val==2 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1104);

    // rs1_val==2 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1108);

    // rs1_val==2 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1112);

    // rs1_val==2 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1116);

    // rs1_val==2 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1120);

    // rs1_val==2 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1124);

    // rs1_val==2 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1128);

    // rs1_val==2 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1132);

    // rs1_val==1431655764 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1136);

    // rs1_val==1431655764 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1140);

    // rs1_val==1431655764 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1144);

    // rs1_val==1431655764 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1148);

    // rs1_val==1431655764 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1152);

    // rs1_val==1431655764 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1156);

    // rs1_val==1431655764 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1160);

    // rs1_val==1431655764 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1164);

    // rs1_val==1431655764 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1168);

    // rs1_val==1431655764 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1172);

    // rs1_val==1431655764 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1176);

    // rs1_val==1431655764 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1180);

    // rs1_val==1431655764 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1184);

    // rs1_val==1431655764 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1188);

    // rs1_val==1431655764 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1192);

    // rs1_val==1431655764 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1196);

    // rs1_val==1431655764 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1200);

    // rs1_val==1431655764 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1204);

    // rs1_val==1431655764 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1208);

    // rs1_val==1431655764 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1212);

    // rs1_val==1431655764 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1216);

    // rs1_val==0 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1220);

    // rs1_val==0 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1224);

    // rs1_val==0 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1228);

    // rs1_val==0 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1232);

    // rs1_val==0 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1236);

    // rs1_val==0 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1240);

    // rs1_val==0 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1244);

    // rs1_val==-1431655765 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1248);

    // rs1_val==-1431655765 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1252);

    // rs1_val==-1431655765 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1256);

    // rs1_val==-1431655765 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1260);

    // rs1_val==-1431655765 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1264);

    // rs1_val==-1431655765 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1268);

    // rs1_val==6 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1272);

    // rs1_val==6 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1276);

    // rs1_val==6 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1280);

    // rs1_val==6 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1284);

    // rs1_val==6 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1288);

    // rs1_val==6 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1292);

    // rs1_val==6 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1296);

    // rs1_val==6 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1300);

    // rs1_val==6 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1304);

    // rs1_val==6 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1308);

    // rs1_val==6 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1312);

    // rs1_val==6 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1316);

    // rs1_val==6 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1320);

    // rs1_val==6 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1324);

    // rs1_val==6 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1328);

    // rs1_val==6 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1332);

    // rs1_val==6 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1336);

    // rs1_val==6 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1340);

    // rs1_val==6 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1344);

    // rs1_val==6 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1348);

    // rs1_val==6 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1352);

    // rs1_val==6 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1356);

    // rs1_val==858993460 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1360);

    // rs1_val==858993460 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1364);

    // rs1_val==858993460 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1368);

    // rs1_val==858993460 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1372);

    // rs1_val==858993460 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1376);

    // rs1_val==858993460 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1380);

    // rs1_val==858993460 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1384);

    // rs1_val==858993460 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1388);

    // rs1_val==858993460 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1392);

    // rs1_val==858993460 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1396);

    // rs1_val==858993460 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1400);

    // rs1_val==858993460 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1404);

    // rs1_val==858993460 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1408);

    // rs1_val==858993460 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1412);

    // rs1_val==858993460 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1416);

    // rs1_val==858993460 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1420);

    // rs1_val==858993460 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1424);

    // rs1_val==858993460 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1428);

    // rs1_val==858993460 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1432);

    // rs1_val==858993460 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1436);

    // rs1_val==858993460 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1440);

    // rs1_val==858993460 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1444);

    // rs1_val==1717986919 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1448);

    // rs1_val==1717986919 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1452);

    // rs1_val==1717986919 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1456);

    // rs1_val==1717986919 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1460);

    // rs1_val==1717986919 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1464);

    // rs1_val==1717986919 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1468);

    // rs1_val==1717986919 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1472);

    // rs1_val==1717986919 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1476);

    // rs1_val==1717986919 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1480);

    // rs1_val==1717986919 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1484);

    // rs1_val==1717986919 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1488);

    // rs1_val==1717986919 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1492);

    // rs1_val==1717986919 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1496);

    // rs1_val==1717986919 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1500);

    // rs1_val==1717986919 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1504);

    // rs1_val==1717986919 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1508);

    // rs1_val==1717986919 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1512);

    // rs1_val==1717986919 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1516);

    // rs1_val==1717986919 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1520);

    // rs1_val==1717986919 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1524);

    // rs1_val==1717986919 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1528);

    // rs1_val==1717986919 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1532);

    // rs1_val==-46339 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1536);

    // rs1_val==-46339 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1540);

    // rs1_val==-46339 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1544);

    // rs1_val==-46339 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1548);

    // rs1_val==-46339 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1552);

    // rs1_val==-46339 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1556);

    // rs1_val==-46339 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1560);

    // rs1_val==-46339 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1564);

    // rs1_val==-46339 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1568);

    // rs1_val==-46339 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1572);

    // rs1_val==-46339 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1576);

    // rs1_val==-46339 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1580);

    // rs1_val==-46339 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1584);

    // rs1_val==-46339 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1588);

    // rs1_val==-46339 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1592);

    // rs1_val==-46339 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1596);

    // rs1_val==-46339 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1600);

    // rs1_val==-46339 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1604);

    // rs1_val==-46339 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1608);

    // rs1_val==-46339 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1612);

    // rs1_val==-46339 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1616);

    // rs1_val==-46339 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1620);

    // rs1_val==46341 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1624);

    // rs1_val==46341 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1628);

    // rs1_val==46341 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1632);

    // rs1_val==46341 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1636);

    // rs1_val==46341 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1640);

    // rs1_val==46341 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1644);

    // rs1_val==46341 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1648);

    // rs1_val==46341 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1652);

    // rs1_val==46341 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1656);

    // rs1_val==46341 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1660);

    // rs1_val==46341 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1664);

    // rs1_val==46341 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1668);

    // rs1_val==46341 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1672);

    // rs1_val==46341 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1676);

    // rs1_val==46341 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1680);

    // rs1_val==46341 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1684);

    // rs1_val==46341 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1688);

    // rs1_val==46341 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1692);

    // rs1_val==46341 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1696);

    // rs1_val==46341 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1700);

    // rs1_val==46341 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1704);

    // rs1_val==46341 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1708);

    // rs1_val==0 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1712);

    // rs1_val==0 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1716);

    // rs1_val==0 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1720);

    // rs1_val==0 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1724);

    // rs1_val==0 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1728);

    // rs1_val==0 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1732);

    // rs1_val==0 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1736);

    // rs1_val==0 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1740);

    // rs1_val==0 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1744);

    // rs1_val==0 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1748);

    // rs1_val==0 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1752);

    // rs1_val==0 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1756);

    // rs1_val==0 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1760);

    // rs1_val==0 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1764);

    // rs1_val==4 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1768);

    // rs1_val==4 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1772);

    // rs1_val==4 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1776);

    // rs1_val==4 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1780);

    // rs1_val==4 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1784);

    // rs1_val==4 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1788);

    // rs1_val==4 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1792);

    // rs1_val==4 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1796);

    // rs1_val==4 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1800);

    // rs1_val==4 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1804);

    // rs1_val==4 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1808);

    // rs1_val==4 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1812);

    // rs1_val==4 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1816);

    // rs1_val==4 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1820);

    // rs1_val==4 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1824);

    // rs1_val==4 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1828);

    // rs1_val==4 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1832);

    // rs1_val==4 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1836);

    // rs1_val==4 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1840);

    // rs1_val==4 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1844);

    // rs1_val==4 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1848);

    // rs1_val==4 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1852);

    // rs1_val==858993458 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1856);

    // rs1_val==858993458 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1860);

    // rs1_val==858993458 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1864);

    // rs1_val==858993458 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1868);

    // rs1_val==858993458 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1872);

    // rs1_val==858993458 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1876);

    // rs1_val==858993458 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1880);

    // rs1_val==858993458 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1884);

    // rs1_val==858993458 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1888);

    // rs1_val==858993458 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1892);

    // rs1_val==858993458 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1896);

    // rs1_val==858993458 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1900);

    // rs1_val==858993458 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1904);

    // rs1_val==858993458 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1908);

    // rs1_val==858993458 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1912);

    // rs1_val==858993458 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1916);

    // rs1_val==858993458 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1920);

    // rs1_val==858993458 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1924);

    // rs1_val==858993458 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1928);

    // rs1_val==858993458 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1932);

    // rs1_val==858993458 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1936);

    // rs1_val==858993458 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1940);

    // rs1_val==1717986917 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1944);

    // rs1_val==1717986917 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1948);

    // rs1_val==1717986917 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1952);

    // rs1_val==1717986917 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1956);

    // rs1_val==1717986917 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1960);

    // rs1_val==1717986917 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1964);

    // rs1_val==1717986917 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1968);

    // rs1_val==1717986917 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1972);

    // rs1_val==1717986917 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1976);

    // rs1_val==1717986917 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1980);

    // rs1_val==1717986917 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1984);

    // rs1_val==1717986917 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1988);

    // rs1_val==1717986917 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1992);

    // rs1_val==1717986917 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 1996);

    // rs1_val==1717986917 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 2000);

    // rs1_val==1717986917 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 2004);

    // rs1_val==1717986917 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 2008);

    // rs1_val==1717986917 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 2012);

    // rs1_val==1717986917 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 2016);

    // rs1_val==1717986917 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 2020);

    // rs1_val==1717986917 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 2024);

    // rs1_val==46339 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 2028);

    // rs1_val==46339 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 2032);

    // rs1_val==46339 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 2036);

    // rs1_val==46339 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 2040);

    // rs1_val==46339 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 2044);

    c.li(Reg::X13, 2136);
    // rs1_val==46339 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 0);

    // rs1_val==46339 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 4);

    // rs1_val==46339 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 8);

    // rs1_val==46339 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 12);

    // rs1_val==46339 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 16);

    // rs1_val==46339 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 20);

    // rs1_val==46339 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 24);

    // rs1_val==46339 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 28);

    // rs1_val==46339 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 32);

    // rs1_val==46339 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 36);

    // rs1_val==46339 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 40);

    // rs1_val==46339 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 44);

    // rs1_val==46339 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 48);

    // rs1_val==46339 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 52);

    // rs1_val==46339 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 56);

    // rs1_val==46339 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 60);

    // rs1_val==46339 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 64);

    // rs1_val==1431655766 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 68);

    // rs1_val==1431655766 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 72);

    // rs1_val==1431655766 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 76);

    // rs1_val==1431655766 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 80);

    // rs1_val==1431655766 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 84);

    // rs1_val==1431655766 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 88);

    // rs1_val==1431655766 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 92);

    // rs1_val==1431655766 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 96);

    // rs1_val==1431655766 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 100);

    // rs1_val==1431655766 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 104);

    // rs1_val==1431655766 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 108);

    // rs1_val==1431655766 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 112);

    // rs1_val==1431655766 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 116);

    // rs1_val==1431655766 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 120);

    // rs1_val==1431655766 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 124);

    // rs1_val==1431655766 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 128);

    // rs1_val==1431655766 and rs2_val==-1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 132);

    // rs1_val==1431655766 and rs2_val==6, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 136);

    // rs1_val==1431655766 and rs2_val==858993460, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 140);

    // rs1_val==1431655766 and rs2_val==1717986919, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 144);

    // rs1_val==1431655766 and rs2_val==-46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 148);

    // rs1_val==1431655766 and rs2_val==46341, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 152);

    // rs1_val==-1431655765 and rs2_val==3, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 156);

    // rs1_val==-1431655765 and rs2_val==1431655765, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 160);

    // rs1_val==-1431655765 and rs2_val==-1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 164);

    // rs1_val==-1431655765 and rs2_val==5, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 168);

    // rs1_val==-1431655765 and rs2_val==858993459, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 172);

    // rs1_val==-1431655765 and rs2_val==1717986918, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 176);

    // rs1_val==-1431655765 and rs2_val==-46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 180);

    // rs1_val==-1431655765 and rs2_val==46340, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 184);

    // rs1_val==-1431655765 and rs2_val==2, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 188);

    // rs1_val==-1431655765 and rs2_val==1431655764, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 192);

    // rs1_val==-1431655765 and rs2_val==0, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 196);

    // rs1_val==-1431655765 and rs2_val==4, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 200);

    // rs1_val==-1431655765 and rs2_val==858993458, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 204);

    // rs1_val==-1431655765 and rs2_val==1717986917, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 208);

    // rs1_val==-1431655765 and rs2_val==46339, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 212);

    // rs1_val==-1431655765 and rs2_val==1431655766, 
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 216);

    // rs1_val < 0 and rs2_val < 0, rs1_val == -16385, rs2_val == -65537
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:-0x4001;  op2val:-0x10001
    c.li(Reg::X10, MASK_XLEN(-0x4001));
    c.li(Reg::X11, MASK_XLEN(-0x10001));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 220);

    // rs1_val == (2**(xlen-1)-1), rs2_val == 4096, rs1_val == 2147483647
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x7fffffff;  op2val:0x1000
    c.li(Reg::X10, MASK_XLEN(0x7fffffff));
    c.li(Reg::X11, MASK_XLEN(0x1000));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 224);

    // rs2_val == 1024, rs1_val == 128
    // opcode: xor ; op1:x10; op2:x11; dest:x12; op1val:0x80;  op2val:0x400
    c.li(Reg::X10, MASK_XLEN(0x80));
    c.li(Reg::X11, MASK_XLEN(0x400));
    c.xor_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X13, 228);


        
    // End
    c.ecall();
    return c;
}