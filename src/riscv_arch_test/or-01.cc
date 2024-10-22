#include "or-01.h"

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
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
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
// ArchTest::or_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &ArchTest::or_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
std::vector<uint32_t> ArchTest::or_01::generateCode(bool simulate)
{
    CodeGenerator c;
    
    // Ready flag is always stored in first word, zero it
    c.sw(Reg::X0, Reg::X0);
    
    
    c.li(Reg::X3, 20);
    // rs2 == rd != rs1, rs1==x8, rs2==x26, rd==x26, rs1_val > 0 and rs2_val > 0, rs2_val == 16, rs1_val == 1048576, rs1_val != rs2_val
    // opcode: or ; op1:x8; op2:x26; dest:x26; op1val:0x100000;  op2val:0x10
    c.li(Reg::X8, MASK_XLEN(0x100000));
    c.li(Reg::X26, MASK_XLEN(0x10));
    c.or_(Reg::X26, Reg::X8, Reg::X26);
    c.sw(Reg::X26, Reg::X3, 0);

    // rs1 == rs2 != rd, rs1==x6, rs2==x6, rd==x17, rs1_val > 0 and rs2_val < 0, rs2_val == -2049, rs1_val == 2
    // opcode: or ; op1:x6; op2:x6; dest:x17; op1val:0x2;  op2val:0x2
    c.li(Reg::X6, MASK_XLEN(0x2));
    c.li(Reg::X6, MASK_XLEN(0x2));
    c.or_(Reg::X17, Reg::X6, Reg::X6);
    c.sw(Reg::X17, Reg::X3, 4);

    // rs1 == rs2 == rd, rs1==x31, rs2==x31, rd==x31, rs1_val < 0 and rs2_val < 0, rs1_val == -268435457, rs2_val == -1025
    // opcode: or ; op1:x31; op2:x31; dest:x31; op1val:-0x10000001;  op2val:-0x10000001
    c.li(Reg::X31, MASK_XLEN(-0x10000001));
    c.li(Reg::X31, MASK_XLEN(-0x10000001));
    c.or_(Reg::X31, Reg::X31, Reg::X31);
    c.sw(Reg::X31, Reg::X3, 8);

    // rs1 == rd != rs2, rs1==x27, rs2==x29, rd==x27, rs1_val < 0 and rs2_val > 0, rs1_val == -2049, rs2_val == 4194304
    // opcode: or ; op1:x27; op2:x29; dest:x27; op1val:-0x801;  op2val:0x400000
    c.li(Reg::X27, MASK_XLEN(-0x801));
    c.li(Reg::X29, MASK_XLEN(0x400000));
    c.or_(Reg::X27, Reg::X27, Reg::X29);
    c.sw(Reg::X27, Reg::X3, 12);

    // rs1 != rs2  and rs1 != rd and rs2 != rd, rs1==x30, rs2==x19, rd==x18, rs1_val == rs2_val, rs2_val == -1048577, rs1_val == -1048577
    // opcode: or ; op1:x30; op2:x19; dest:x18; op1val:-0x100001;  op2val:-0x100001
    c.li(Reg::X30, MASK_XLEN(-0x100001));
    c.li(Reg::X19, MASK_XLEN(-0x100001));
    c.or_(Reg::X18, Reg::X30, Reg::X19);
    c.sw(Reg::X18, Reg::X3, 16);

    // rs1==x21, rs2==x14, rd==x9, rs2_val == (-2**(xlen-1)), rs2_val == -2147483648, rs1_val == 131072
    // opcode: or ; op1:x21; op2:x14; dest:x9; op1val:0x20000;  op2val:-0x80000000
    c.li(Reg::X21, MASK_XLEN(0x20000));
    c.li(Reg::X14, MASK_XLEN(-0x80000000));
    c.or_(Reg::X9, Reg::X21, Reg::X14);
    c.sw(Reg::X9, Reg::X3, 20);

    // rs1==x26, rs2==x24, rd==x4, rs2_val == 0, rs1_val == -8193
    // opcode: or ; op1:x26; op2:x24; dest:x4; op1val:-0x2001;  op2val:0x0
    c.li(Reg::X26, MASK_XLEN(-0x2001));
    c.li(Reg::X24, MASK_XLEN(0x0));
    c.or_(Reg::X4, Reg::X26, Reg::X24);
    c.sw(Reg::X4, Reg::X3, 24);

    // rs1==x9, rs2==x8, rd==x30, rs2_val == (2**(xlen-1)-1), rs1_val == 0, rs2_val == 2147483647
    // opcode: or ; op1:x9; op2:x8; dest:x30; op1val:0x0;  op2val:0x7fffffff
    c.li(Reg::X9, MASK_XLEN(0x0));
    c.li(Reg::X8, MASK_XLEN(0x7fffffff));
    c.or_(Reg::X30, Reg::X9, Reg::X8);
    c.sw(Reg::X30, Reg::X3, 28);

    // rs1==x23, rs2==x7, rd==x8, rs2_val == 1, rs1_val == -8388609
    // opcode: or ; op1:x23; op2:x7; dest:x8; op1val:-0x800001;  op2val:0x1
    c.li(Reg::X23, MASK_XLEN(-0x800001));
    c.li(Reg::X7, MASK_XLEN(0x1));
    c.or_(Reg::X8, Reg::X23, Reg::X7);
    c.sw(Reg::X8, Reg::X3, 32);

    // rs1==x12, rs2==x0, rd==x22, rs1_val == (-2**(xlen-1)), rs1_val == -2147483648
    // opcode: or ; op1:x12; op2:x0; dest:x22; op1val:-0x80000000;  op2val:0x0
    c.li(Reg::X12, MASK_XLEN(-0x80000000));
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.or_(Reg::X22, Reg::X12, Reg::X0);
    c.sw(Reg::X22, Reg::X3, 36);

    // rs1==x10, rs2==x30, rd==x28, rs1_val == (2**(xlen-1)-1), rs2_val == 64, rs1_val == 2147483647
    // opcode: or ; op1:x10; op2:x30; dest:x28; op1val:0x7fffffff;  op2val:0x40
    c.li(Reg::X10, MASK_XLEN(0x7fffffff));
    c.li(Reg::X30, MASK_XLEN(0x40));
    c.or_(Reg::X28, Reg::X10, Reg::X30);
    c.sw(Reg::X28, Reg::X3, 40);

    // rs1==x18, rs2==x21, rd==x16, rs1_val == 1, 
    // opcode: or ; op1:x18; op2:x21; dest:x16; op1val:0x1;  op2val:0x55555554
    c.li(Reg::X18, MASK_XLEN(0x1));
    c.li(Reg::X21, MASK_XLEN(0x55555554));
    c.or_(Reg::X16, Reg::X18, Reg::X21);
    c.sw(Reg::X16, Reg::X3, 44);

    // rs1==x14, rs2==x17, rd==x12, rs2_val == 2, rs1_val == 4096
    // opcode: or ; op1:x14; op2:x17; dest:x12; op1val:0x1000;  op2val:0x2
    c.li(Reg::X14, MASK_XLEN(0x1000));
    c.li(Reg::X17, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X14, Reg::X17);
    c.sw(Reg::X12, Reg::X3, 48);

    // rs1==x19, rs2==x16, rd==x15, rs2_val == 4, 
    // opcode: or ; op1:x19; op2:x16; dest:x15; op1val:-0x800001;  op2val:0x4
    c.li(Reg::X19, MASK_XLEN(-0x800001));
    c.li(Reg::X16, MASK_XLEN(0x4));
    c.or_(Reg::X15, Reg::X19, Reg::X16);
    c.sw(Reg::X15, Reg::X3, 52);

    // rs1==x4, rs2==x2, rd==x7, rs2_val == 8, rs1_val == -1025
    // opcode: or ; op1:x4; op2:x2; dest:x7; op1val:-0x401;  op2val:0x8
    c.li(Reg::X4, MASK_XLEN(-0x401));
    c.li(Reg::X2, MASK_XLEN(0x8));
    c.or_(Reg::X7, Reg::X4, Reg::X2);
    c.sw(Reg::X7, Reg::X3, 56);

    // rs1==x2, rs2==x22, rd==x11, rs2_val == 32, 
    // opcode: or ; op1:x2; op2:x22; dest:x11; op1val:0x7fffffff;  op2val:0x20
    c.li(Reg::X2, MASK_XLEN(0x7fffffff));
    c.li(Reg::X22, MASK_XLEN(0x20));
    c.or_(Reg::X11, Reg::X2, Reg::X22);
    c.sw(Reg::X11, Reg::X3, 60);

    // rs1==x28, rs2==x15, rd==x25, rs2_val == 128, rs1_val == -513
    // opcode: or ; op1:x28; op2:x15; dest:x25; op1val:-0x201;  op2val:0x80
    c.li(Reg::X28, MASK_XLEN(-0x201));
    c.li(Reg::X15, MASK_XLEN(0x80));
    c.or_(Reg::X25, Reg::X28, Reg::X15);
    c.sw(Reg::X25, Reg::X3, 64);

    // rs1==x25, rs2==x1, rd==x6, rs2_val == 256, 
    // opcode: or ; op1:x25; op2:x1; dest:x6; op1val:0xb504;  op2val:0x100
    c.li(Reg::X25, MASK_XLEN(0xb504));
    c.li(Reg::X1, MASK_XLEN(0x100));
    c.or_(Reg::X6, Reg::X25, Reg::X1);
    c.sw(Reg::X6, Reg::X3, 68);

    // rs1==x17, rs2==x10, rd==x20, rs2_val == 512, rs1_val == 4
    // opcode: or ; op1:x17; op2:x10; dest:x20; op1val:0x4;  op2val:0x200
    c.li(Reg::X17, MASK_XLEN(0x4));
    c.li(Reg::X10, MASK_XLEN(0x200));
    c.or_(Reg::X20, Reg::X17, Reg::X10);
    c.sw(Reg::X20, Reg::X3, 72);

    c.li(Reg::X6, 96);
    // rs1==x20, rs2==x23, rd==x5, rs2_val == 1024, 
    // opcode: or ; op1:x20; op2:x23; dest:x5; op1val:-0x100001;  op2val:0x400
    c.li(Reg::X20, MASK_XLEN(-0x100001));
    c.li(Reg::X23, MASK_XLEN(0x400));
    c.or_(Reg::X5, Reg::X20, Reg::X23);
    c.sw(Reg::X5, Reg::X6, 0);

    // rs1==x11, rs2==x12, rd==x2, rs2_val == 2048, 
    // opcode: or ; op1:x11; op2:x12; dest:x2; op1val:0x0;  op2val:0x800
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.li(Reg::X12, MASK_XLEN(0x800));
    c.or_(Reg::X2, Reg::X11, Reg::X12);
    c.sw(Reg::X2, Reg::X6, 4);

    // rs1==x13, rs2==x20, rd==x1, rs2_val == 4096, rs1_val == -67108865
    // opcode: or ; op1:x13; op2:x20; dest:x1; op1val:-0x4000001;  op2val:0x1000
    c.li(Reg::X13, MASK_XLEN(-0x4000001));
    c.li(Reg::X20, MASK_XLEN(0x1000));
    c.or_(Reg::X1, Reg::X13, Reg::X20);
    c.sw(Reg::X1, Reg::X6, 8);

    // rs1==x15, rs2==x27, rd==x3, rs2_val == 8192, 
    // opcode: or ; op1:x15; op2:x27; dest:x3; op1val:0x4;  op2val:0x2000
    c.li(Reg::X15, MASK_XLEN(0x4));
    c.li(Reg::X27, MASK_XLEN(0x2000));
    c.or_(Reg::X3, Reg::X15, Reg::X27);
    c.sw(Reg::X3, Reg::X6, 12);

    // rs1==x22, rs2==x4, rd==x21, rs2_val == 16384, rs1_val == -65537
    // opcode: or ; op1:x22; op2:x4; dest:x21; op1val:-0x10001;  op2val:0x4000
    c.li(Reg::X22, MASK_XLEN(-0x10001));
    c.li(Reg::X4, MASK_XLEN(0x4000));
    c.or_(Reg::X21, Reg::X22, Reg::X4);
    c.sw(Reg::X21, Reg::X6, 16);

    // rs1==x7, rs2==x13, rd==x19, rs2_val == 32768, rs1_val == 524288
    // opcode: or ; op1:x7; op2:x13; dest:x19; op1val:0x80000;  op2val:0x8000
    c.li(Reg::X7, MASK_XLEN(0x80000));
    c.li(Reg::X13, MASK_XLEN(0x8000));
    c.or_(Reg::X19, Reg::X7, Reg::X13);
    c.sw(Reg::X19, Reg::X6, 20);

    // rs1==x24, rs2==x5, rd==x14, rs2_val == 65536, 
    // opcode: or ; op1:x24; op2:x5; dest:x14; op1val:-0x4;  op2val:0x10000
    c.li(Reg::X24, MASK_XLEN(-0x4));
    c.li(Reg::X5, MASK_XLEN(0x10000));
    c.or_(Reg::X14, Reg::X24, Reg::X5);
    c.sw(Reg::X14, Reg::X6, 24);

    // rs1==x16, rs2==x18, rd==x10, rs2_val == 131072, rs1_val == 1431655765
    // opcode: or ; op1:x16; op2:x18; dest:x10; op1val:0x55555555;  op2val:0x20000
    c.li(Reg::X16, MASK_XLEN(0x55555555));
    c.li(Reg::X18, MASK_XLEN(0x20000));
    c.or_(Reg::X10, Reg::X16, Reg::X18);
    c.sw(Reg::X10, Reg::X6, 28);

    // rs1==x1, rs2==x9, rd==x23, rs2_val == 262144, 
    // opcode: or ; op1:x1; op2:x9; dest:x23; op1val:-0xb504;  op2val:0x40000
    c.li(Reg::X1, MASK_XLEN(-0xb504));
    c.li(Reg::X9, MASK_XLEN(0x40000));
    c.or_(Reg::X23, Reg::X1, Reg::X9);
    c.sw(Reg::X23, Reg::X6, 32);

    // rs1==x3, rs2==x11, rd==x29, rs2_val == 524288, 
    // opcode: or ; op1:x3; op2:x11; dest:x29; op1val:0x4;  op2val:0x80000
    c.li(Reg::X3, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x80000));
    c.or_(Reg::X29, Reg::X3, Reg::X11);
    c.sw(Reg::X29, Reg::X6, 36);

    // rs1==x29, rs2==x28, rd==x13, rs2_val == 1048576, rs1_val == -4097
    // opcode: or ; op1:x29; op2:x28; dest:x13; op1val:-0x1001;  op2val:0x100000
    c.li(Reg::X29, MASK_XLEN(-0x1001));
    c.li(Reg::X28, MASK_XLEN(0x100000));
    c.or_(Reg::X13, Reg::X29, Reg::X28);
    c.sw(Reg::X13, Reg::X6, 40);

    // rs1==x5, rs2==x3, rd==x0, rs2_val == 2097152, 
    // opcode: or ; op1:x5; op2:x3; dest:x0; op1val:0x33333332;  op2val:0x200000
    c.li(Reg::X5, MASK_XLEN(0x33333332));
    c.li(Reg::X3, MASK_XLEN(0x200000));
    c.or_(Reg::X0, Reg::X5, Reg::X3);
    c.sw(Reg::X0, Reg::X6, 44);

    // rs1==x0, rs2==x25, rd==x24, rs2_val == 8388608, 
    // opcode: or ; op1:x0; op2:x25; dest:x24; op1val:0x0;  op2val:0x800000
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.li(Reg::X25, MASK_XLEN(0x800000));
    c.or_(Reg::X24, Reg::X0, Reg::X25);
    c.sw(Reg::X24, Reg::X6, 48);

    // rs2_val == 16777216, rs1_val == 64
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x40;  op2val:0x1000000
    c.li(Reg::X10, MASK_XLEN(0x40));
    c.li(Reg::X11, MASK_XLEN(0x1000000));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 52);

    // rs2_val == 33554432, rs1_val == -9
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x9;  op2val:0x2000000
    c.li(Reg::X10, MASK_XLEN(-0x9));
    c.li(Reg::X11, MASK_XLEN(0x2000000));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 56);

    // rs2_val == 67108864, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x4000000
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x4000000));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 60);

    // rs2_val == 134217728, rs1_val == 16777216
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x1000000;  op2val:0x8000000
    c.li(Reg::X10, MASK_XLEN(0x1000000));
    c.li(Reg::X11, MASK_XLEN(0x8000000));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 64);

    // rs2_val == 268435456, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x6;  op2val:0x10000000
    c.li(Reg::X10, MASK_XLEN(-0x6));
    c.li(Reg::X11, MASK_XLEN(0x10000000));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 68);

    // rs2_val == 536870912, rs1_val == -1431655766
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x20000000
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x20000000));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 72);

    // rs2_val == 1073741824, rs1_val == 8388608
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x800000;  op2val:0x40000000
    c.li(Reg::X10, MASK_XLEN(0x800000));
    c.li(Reg::X11, MASK_XLEN(0x40000000));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 76);

    // rs2_val == -2, rs1_val == 32
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x20;  op2val:-0x2
    c.li(Reg::X10, MASK_XLEN(0x20));
    c.li(Reg::X11, MASK_XLEN(-0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 80);

    // rs2_val == -3, rs1_val == 268435456
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x10000000;  op2val:-0x3
    c.li(Reg::X10, MASK_XLEN(0x10000000));
    c.li(Reg::X11, MASK_XLEN(-0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 84);

    // rs2_val == -5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:-0x5
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(-0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 88);

    // rs2_val == -9, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x80000;  op2val:-0x9
    c.li(Reg::X10, MASK_XLEN(0x80000));
    c.li(Reg::X11, MASK_XLEN(-0x9));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 92);

    // rs2_val == -17, rs1_val == 8
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x8;  op2val:-0x11
    c.li(Reg::X10, MASK_XLEN(0x8));
    c.li(Reg::X11, MASK_XLEN(-0x11));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 96);

    // rs2_val == -33, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x21
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x21));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 100);

    // rs2_val == -65, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x20000;  op2val:-0x41
    c.li(Reg::X10, MASK_XLEN(0x20000));
    c.li(Reg::X11, MASK_XLEN(-0x41));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 104);

    // rs2_val == -129, rs1_val == -2097153
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x200001;  op2val:-0x81
    c.li(Reg::X10, MASK_XLEN(-0x200001));
    c.li(Reg::X11, MASK_XLEN(-0x81));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 108);

    // rs2_val == -257, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x101
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0x101));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 112);

    // rs2_val == -513, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x7fffffff;  op2val:-0x201
    c.li(Reg::X10, MASK_XLEN(0x7fffffff));
    c.li(Reg::X11, MASK_XLEN(-0x201));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 116);

    // rs2_val == -4097, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x1001
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0x1001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 120);

    // rs2_val == -8193, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x80000000;  op2val:-0x2001
    c.li(Reg::X10, MASK_XLEN(-0x80000000));
    c.li(Reg::X11, MASK_XLEN(-0x2001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 124);

    // rs2_val == -16385, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x4001
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(-0x4001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 128);

    // rs2_val == -32769, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x800001;  op2val:-0x8001
    c.li(Reg::X10, MASK_XLEN(-0x800001));
    c.li(Reg::X11, MASK_XLEN(-0x8001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 132);

    // rs2_val == -65537, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x9;  op2val:-0x10001
    c.li(Reg::X10, MASK_XLEN(-0x9));
    c.li(Reg::X11, MASK_XLEN(-0x10001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 136);

    // rs2_val == -131073, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x20001
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x20001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 140);

    // rs2_val == -262145, rs1_val == -33
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x21;  op2val:-0x40001
    c.li(Reg::X10, MASK_XLEN(-0x21));
    c.li(Reg::X11, MASK_XLEN(-0x40001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 144);

    // rs2_val == -524289, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x80001
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x80001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 148);

    // rs2_val == -2097153, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x200001
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x200001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 152);

    // rs2_val == -4194305, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x400001
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0x400001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 156);

    // rs2_val == -8388609, rs1_val == -257
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x101;  op2val:-0x800001
    c.li(Reg::X10, MASK_XLEN(-0x101));
    c.li(Reg::X11, MASK_XLEN(-0x800001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 160);

    // rs2_val == -16777217, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x1000001
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0x1000001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 164);

    // rs2_val == -33554433, rs1_val == -32769
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x8001;  op2val:-0x2000001
    c.li(Reg::X10, MASK_XLEN(-0x8001));
    c.li(Reg::X11, MASK_XLEN(-0x2000001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 168);

    // rs2_val == -67108865, rs1_val == 65536
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:-0x4000001
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(-0x4000001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 172);

    // rs2_val == -134217729, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x8000001
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(-0x8000001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 176);

    // rs2_val == -268435457, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x40000000;  op2val:-0x10000001
    c.li(Reg::X10, MASK_XLEN(-0x40000000));
    c.li(Reg::X11, MASK_XLEN(-0x10000001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 180);

    // rs2_val == -536870913, rs1_val == 512
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x200;  op2val:-0x20000001
    c.li(Reg::X10, MASK_XLEN(0x200));
    c.li(Reg::X11, MASK_XLEN(-0x20000001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 184);

    // rs2_val == -1073741825, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x40000001
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x40000001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 188);

    // rs2_val == 1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x4;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(-0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 192);

    // rs2_val == -1431655766, rs1_val == -536870913
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x20000001;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x20000001));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 196);

    // rs1_val == 16, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x10;  op2val:-0x7
    c.li(Reg::X10, MASK_XLEN(0x10));
    c.li(Reg::X11, MASK_XLEN(-0x7));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 200);

    // rs1_val == 128, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x80;  op2val:-0x7
    c.li(Reg::X10, MASK_XLEN(0x80));
    c.li(Reg::X11, MASK_XLEN(-0x7));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 204);

    // rs1_val == 256, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x100;  op2val:-0x4000001
    c.li(Reg::X10, MASK_XLEN(0x100));
    c.li(Reg::X11, MASK_XLEN(-0x4000001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 208);

    // rs1_val == 1024, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x400;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x400));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 212);

    // rs1_val == 2048, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x800;  op2val:-0x200001
    c.li(Reg::X10, MASK_XLEN(0x800));
    c.li(Reg::X11, MASK_XLEN(-0x200001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 216);

    // rs1_val == 8192, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2000;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x2000));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 220);

    // rs1_val == 16384, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4000;  op2val:-0x6
    c.li(Reg::X10, MASK_XLEN(0x4000));
    c.li(Reg::X11, MASK_XLEN(-0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 224);

    // rs1_val == 32768, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x8000;  op2val:0x800
    c.li(Reg::X10, MASK_XLEN(0x8000));
    c.li(Reg::X11, MASK_XLEN(0x800));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 228);

    // rs1_val == 262144, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x40000;  op2val:-0x1000001
    c.li(Reg::X10, MASK_XLEN(0x40000));
    c.li(Reg::X11, MASK_XLEN(-0x1000001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 232);

    // rs1_val == 2097152, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x200000;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x200000));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 236);

    // rs1_val == 4194304, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x400000;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x400000));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 240);

    // rs1_val == 33554432, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2000000;  op2val:0x3fffffff
    c.li(Reg::X10, MASK_XLEN(0x2000000));
    c.li(Reg::X11, MASK_XLEN(0x3fffffff));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 244);

    // rs1_val == 67108864, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4000000;  op2val:-0x80001
    c.li(Reg::X10, MASK_XLEN(0x4000000));
    c.li(Reg::X11, MASK_XLEN(-0x80001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 248);

    // rs1_val == 134217728, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x8000000;  op2val:0x20
    c.li(Reg::X10, MASK_XLEN(0x8000000));
    c.li(Reg::X11, MASK_XLEN(0x20));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 252);

    // rs1_val == 536870912, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x20000000;  op2val:-0x2001
    c.li(Reg::X10, MASK_XLEN(0x20000000));
    c.li(Reg::X11, MASK_XLEN(-0x2001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 256);

    // rs1_val == 1073741824, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x40000000;  op2val:0x8
    c.li(Reg::X10, MASK_XLEN(0x40000000));
    c.li(Reg::X11, MASK_XLEN(0x8));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 260);

    // rs1_val == -2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x2;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0x2));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 264);

    // rs1_val == -3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x3;  op2val:-0x8
    c.li(Reg::X10, MASK_XLEN(-0x3));
    c.li(Reg::X11, MASK_XLEN(-0x8));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 268);

    // rs1_val == -5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x5;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(-0x5));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 272);

    // rs1_val == -17, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x11;  op2val:-0x400001
    c.li(Reg::X10, MASK_XLEN(-0x11));
    c.li(Reg::X11, MASK_XLEN(-0x400001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 276);

    // rs1_val == -65, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x41;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0x41));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 280);

    // rs1_val == -129, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x81;  op2val:0x800000
    c.li(Reg::X10, MASK_XLEN(-0x81));
    c.li(Reg::X11, MASK_XLEN(0x800000));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 284);

    // rs1_val == -16385, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x4001;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0x4001));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 288);

    // rs1_val == -131073, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x20001;  op2val:-0x4001
    c.li(Reg::X10, MASK_XLEN(-0x20001));
    c.li(Reg::X11, MASK_XLEN(-0x4001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 292);

    // rs1_val == -262145, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x40001;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x40001));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 296);

    // rs1_val == -524289, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x80001;  op2val:-0x41
    c.li(Reg::X10, MASK_XLEN(-0x80001));
    c.li(Reg::X11, MASK_XLEN(-0x41));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 300);

    // rs1_val == -4194305, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x400001;  op2val:-0x101
    c.li(Reg::X10, MASK_XLEN(-0x400001));
    c.li(Reg::X11, MASK_XLEN(-0x101));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 304);

    // rs1_val == -16777217, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x1000001;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0x1000001));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 308);

    // rs1_val == -33554433, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x2000001;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0x2000001));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 312);

    // rs1_val == -134217729, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x8000001;  op2val:-0x5
    c.li(Reg::X10, MASK_XLEN(-0x8000001));
    c.li(Reg::X11, MASK_XLEN(-0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 316);

    // rs1_val == -1073741825, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x40000001;  op2val:-0x40000001
    c.li(Reg::X10, MASK_XLEN(-0x40000001));
    c.li(Reg::X11, MASK_XLEN(-0x40000001));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 320);

    // rs1_val==3 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 324);

    // rs1_val==3 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 328);

    // rs1_val==3 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 332);

    // rs1_val==3 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 336);

    // rs1_val==3 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 340);

    // rs1_val==3 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 344);

    // rs1_val==3 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 348);

    // rs1_val==3 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 352);

    // rs1_val==3 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 356);

    // rs1_val==3 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 360);

    // rs1_val==3 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 364);

    // rs1_val==3 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 368);

    // rs1_val==3 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 372);

    // rs1_val==3 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 376);

    // rs1_val==3 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 380);

    // rs1_val==3 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 384);

    // rs1_val==3 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 388);

    // rs1_val==3 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 392);

    // rs1_val==3 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 396);

    // rs1_val==3 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 400);

    // rs1_val==3 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 404);

    // rs1_val==3 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 408);

    // rs1_val==1431655765 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 412);

    // rs1_val==1431655765 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 416);

    // rs1_val==1431655765 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 420);

    // rs1_val==1431655765 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 424);

    // rs1_val==1431655765 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 428);

    // rs1_val==1431655765 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 432);

    // rs1_val==1431655765 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 436);

    // rs1_val==1431655765 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 440);

    // rs1_val==1431655765 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 444);

    // rs1_val==1431655765 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 448);

    // rs1_val==1431655765 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 452);

    // rs1_val==1431655765 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 456);

    // rs1_val==1431655765 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 460);

    // rs1_val==1431655765 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 464);

    // rs1_val==1431655765 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 468);

    // rs1_val==1431655765 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 472);

    // rs1_val==1431655765 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 476);

    // rs1_val==1431655765 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 480);

    // rs1_val==1431655765 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 484);

    // rs1_val==1431655765 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 488);

    // rs1_val==1431655765 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 492);

    // rs1_val==1431655765 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 496);

    // rs1_val==-1431655766 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 500);

    // rs1_val==-1431655766 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 504);

    // rs1_val==-1431655766 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 508);

    // rs1_val==-1431655766 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 512);

    // rs1_val==-1431655766 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 516);

    // rs1_val==-1431655766 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 520);

    // rs1_val==-1431655766 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 524);

    // rs1_val==-1431655766 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 528);

    // rs1_val==-1431655766 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 532);

    // rs1_val==-1431655766 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 536);

    // rs1_val==-1431655766 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 540);

    // rs1_val==-1431655766 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 544);

    // rs1_val==-1431655766 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 548);

    // rs1_val==-1431655766 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 552);

    // rs1_val==-1431655766 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 556);

    // rs1_val==-1431655766 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 560);

    // rs1_val==-1431655766 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 564);

    // rs1_val==-1431655766 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 568);

    // rs1_val==-1431655766 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 572);

    // rs1_val==-1431655766 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 576);

    // rs1_val==-1431655766 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 580);

    // rs1_val==-1431655766 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 584);

    // rs1_val==5 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 588);

    // rs1_val==5 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 592);

    // rs1_val==5 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 596);

    // rs1_val==5 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 600);

    // rs1_val==5 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 604);

    // rs1_val==5 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 608);

    // rs1_val==5 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 612);

    // rs1_val==5 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 616);

    // rs1_val==5 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 620);

    // rs1_val==5 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 624);

    // rs1_val==5 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 628);

    // rs1_val==5 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 632);

    // rs1_val==5 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 636);

    // rs1_val==5 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 640);

    // rs1_val==5 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 644);

    // rs1_val==5 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 648);

    // rs1_val==5 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 652);

    // rs1_val==5 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 656);

    // rs1_val==5 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 660);

    // rs1_val==5 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 664);

    // rs1_val==5 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 668);

    // rs1_val==5 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 672);

    // rs1_val==858993459 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 676);

    // rs1_val==858993459 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 680);

    // rs1_val==858993459 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 684);

    // rs1_val==858993459 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 688);

    // rs1_val==858993459 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 692);

    // rs1_val==858993459 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 696);

    // rs1_val==858993459 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 700);

    // rs1_val==858993459 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 704);

    // rs1_val==858993459 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 708);

    // rs1_val==858993459 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 712);

    // rs1_val==858993459 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 716);

    // rs1_val==858993459 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 720);

    // rs1_val==858993459 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 724);

    // rs1_val==858993459 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 728);

    // rs1_val==858993459 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 732);

    // rs1_val==858993459 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 736);

    // rs1_val==858993459 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 740);

    // rs1_val==858993459 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 744);

    // rs1_val==858993459 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 748);

    // rs1_val==858993459 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 752);

    // rs1_val==858993459 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 756);

    // rs1_val==858993459 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 760);

    // rs1_val==1717986918 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 764);

    // rs1_val==1717986918 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 768);

    // rs1_val==1717986918 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 772);

    // rs1_val==1717986918 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 776);

    // rs1_val==1717986918 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 780);

    // rs1_val==1717986918 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 784);

    // rs1_val==1717986918 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 788);

    // rs1_val==1717986918 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 792);

    // rs1_val==1717986918 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 796);

    // rs1_val==1717986918 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 800);

    // rs1_val==1717986918 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 804);

    // rs1_val==1717986918 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 808);

    // rs1_val==1717986918 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 812);

    // rs1_val==1717986918 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 816);

    // rs1_val==1717986918 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 820);

    // rs1_val==1717986918 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 824);

    // rs1_val==1717986918 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 828);

    // rs1_val==1717986918 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 832);

    // rs1_val==1717986918 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 836);

    // rs1_val==1717986918 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 840);

    // rs1_val==1717986918 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 844);

    // rs1_val==1717986918 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 848);

    // rs1_val==-46340 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 852);

    // rs1_val==-46340 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 856);

    // rs1_val==-46340 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 860);

    // rs1_val==-46340 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 864);

    // rs1_val==-46340 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 868);

    // rs1_val==-46340 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 872);

    // rs1_val==-46340 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 876);

    // rs1_val==-46340 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 880);

    // rs1_val==-46340 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 884);

    // rs1_val==-46340 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 888);

    // rs1_val==-46340 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 892);

    // rs1_val==-46340 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 896);

    // rs1_val==-46340 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 900);

    // rs1_val==-46340 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 904);

    // rs1_val==-46340 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 908);

    // rs1_val==-46340 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 912);

    // rs1_val==-46340 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 916);

    // rs1_val==-46340 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 920);

    // rs1_val==-46340 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 924);

    // rs1_val==-46340 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 928);

    // rs1_val==-46340 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 932);

    // rs1_val==-46340 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 936);

    // rs1_val==46340 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 940);

    // rs1_val==46340 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 944);

    // rs1_val==46340 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 948);

    // rs1_val==46340 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 952);

    // rs1_val==46340 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 956);

    // rs1_val==46340 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 960);

    // rs1_val==46340 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 964);

    // rs1_val==46340 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 968);

    // rs1_val==46340 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 972);

    // rs1_val==46340 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 976);

    // rs1_val==46340 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 980);

    // rs1_val==46340 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 984);

    // rs1_val==46340 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 988);

    // rs1_val==46340 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 992);

    // rs1_val==46340 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 996);

    // rs1_val==46340 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1000);

    // rs1_val==46340 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1004);

    // rs1_val==46340 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1008);

    // rs1_val==46340 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1012);

    // rs1_val==46340 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1016);

    // rs1_val==46340 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1020);

    // rs1_val==46340 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1024);

    // rs1_val==2 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1028);

    // rs1_val==2 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1032);

    // rs1_val==2 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1036);

    // rs1_val==2 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1040);

    // rs1_val==2 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1044);

    // rs1_val==2 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1048);

    // rs1_val==2 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1052);

    // rs1_val==2 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1056);

    // rs1_val==2 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1060);

    // rs1_val==2 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1064);

    // rs1_val==2 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1068);

    // rs1_val==2 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1072);

    // rs1_val==2 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1076);

    // rs1_val==2 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1080);

    // rs1_val==2 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1084);

    // rs1_val==2 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1088);

    // rs1_val==2 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1092);

    // rs1_val==2 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1096);

    // rs1_val==2 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1100);

    // rs1_val==2 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1104);

    // rs1_val==2 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1108);

    // rs1_val==2 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1112);

    // rs1_val==1431655764 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1116);

    // rs1_val==1431655764 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1120);

    // rs1_val==1431655764 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1124);

    // rs1_val==1431655764 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1128);

    // rs1_val==1431655764 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1132);

    // rs1_val==1431655764 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1136);

    // rs1_val==1431655764 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1140);

    // rs1_val==1431655764 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1144);

    // rs1_val==1431655764 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1148);

    // rs1_val==1431655764 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1152);

    // rs1_val==1431655764 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1156);

    // rs1_val==1431655764 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1160);

    // rs1_val==1431655764 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1164);

    // rs1_val==1431655764 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1168);

    // rs1_val==1431655764 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1172);

    // rs1_val==1431655764 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1176);

    // rs1_val==1431655764 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1180);

    // rs1_val==1431655764 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1184);

    // rs1_val==1431655764 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1188);

    // rs1_val==1431655764 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1192);

    // rs1_val==1431655764 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1196);

    // rs1_val==1431655764 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1200);

    // rs1_val==0 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1204);

    // rs1_val==0 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1208);

    // rs1_val==0 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1212);

    // rs1_val==0 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1216);

    // rs1_val==0 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1220);

    // rs1_val==0 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1224);

    // rs1_val==0 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1228);

    // rs1_val==0 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1232);

    // rs1_val==0 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1236);

    // rs1_val==-1431655765 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1240);

    // rs1_val==-1431655765 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1244);

    // rs1_val==-1431655765 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1248);

    // rs1_val==-1431655765 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1252);

    // rs1_val==-1431655765 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1256);

    // rs1_val==-1431655765 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1260);

    // rs1_val==6 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1264);

    // rs1_val==6 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1268);

    // rs1_val==6 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1272);

    // rs1_val==6 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1276);

    // rs1_val==6 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1280);

    // rs1_val==6 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1284);

    // rs1_val==6 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1288);

    // rs1_val==6 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1292);

    // rs1_val==6 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1296);

    // rs1_val==6 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1300);

    // rs1_val==6 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1304);

    // rs1_val==6 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1308);

    // rs1_val==6 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1312);

    // rs1_val==6 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1316);

    // rs1_val==6 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1320);

    // rs1_val==6 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1324);

    // rs1_val==6 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1328);

    // rs1_val==6 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1332);

    // rs1_val==6 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1336);

    // rs1_val==6 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1340);

    // rs1_val==6 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1344);

    // rs1_val==6 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1348);

    // rs1_val==858993460 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1352);

    // rs1_val==858993460 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1356);

    // rs1_val==858993460 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1360);

    // rs1_val==858993460 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1364);

    // rs1_val==858993460 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1368);

    // rs1_val==858993460 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1372);

    // rs1_val==858993460 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1376);

    // rs1_val==858993460 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1380);

    // rs1_val==858993460 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1384);

    // rs1_val==858993460 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1388);

    // rs1_val==858993460 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1392);

    // rs1_val==858993460 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1396);

    // rs1_val==858993460 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1400);

    // rs1_val==858993460 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1404);

    // rs1_val==858993460 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1408);

    // rs1_val==858993460 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1412);

    // rs1_val==858993460 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1416);

    // rs1_val==858993460 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1420);

    // rs1_val==858993460 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1424);

    // rs1_val==858993460 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1428);

    // rs1_val==858993460 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1432);

    // rs1_val==858993460 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1436);

    // rs1_val==1717986919 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1440);

    // rs1_val==1717986919 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1444);

    // rs1_val==1717986919 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1448);

    // rs1_val==1717986919 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1452);

    // rs1_val==1717986919 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1456);

    // rs1_val==1717986919 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1460);

    // rs1_val==1717986919 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1464);

    // rs1_val==1717986919 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1468);

    // rs1_val==1717986919 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1472);

    // rs1_val==1717986919 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1476);

    // rs1_val==1717986919 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1480);

    // rs1_val==1717986919 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1484);

    // rs1_val==1717986919 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1488);

    // rs1_val==1717986919 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1492);

    // rs1_val==1717986919 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1496);

    // rs1_val==1717986919 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1500);

    // rs1_val==1717986919 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1504);

    // rs1_val==1717986919 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1508);

    // rs1_val==1717986919 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1512);

    // rs1_val==1717986919 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1516);

    // rs1_val==1717986919 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1520);

    // rs1_val==1717986919 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1524);

    // rs1_val==-46339 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1528);

    // rs1_val==-46339 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1532);

    // rs1_val==-46339 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1536);

    // rs1_val==-46339 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1540);

    // rs1_val==-46339 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1544);

    // rs1_val==-46339 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1548);

    // rs1_val==-46339 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1552);

    // rs1_val==-46339 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1556);

    // rs1_val==-46339 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1560);

    // rs1_val==-46339 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1564);

    // rs1_val==-46339 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1568);

    // rs1_val==-46339 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1572);

    // rs1_val==-46339 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1576);

    // rs1_val==-46339 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1580);

    // rs1_val==-46339 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1584);

    // rs1_val==-46339 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1588);

    // rs1_val==-46339 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1592);

    // rs1_val==-46339 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1596);

    // rs1_val==-46339 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1600);

    // rs1_val==-46339 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1604);

    // rs1_val==-46339 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1608);

    // rs1_val==-46339 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1612);

    // rs1_val==46341 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1616);

    // rs1_val==46341 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1620);

    // rs1_val==46341 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1624);

    // rs1_val==46341 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1628);

    // rs1_val==46341 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1632);

    // rs1_val==46341 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1636);

    // rs1_val==46341 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1640);

    // rs1_val==46341 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1644);

    // rs1_val==46341 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1648);

    // rs1_val==46341 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1652);

    // rs1_val==46341 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1656);

    // rs1_val==46341 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1660);

    // rs1_val==46341 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1664);

    // rs1_val==46341 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1668);

    // rs1_val==46341 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1672);

    // rs1_val==46341 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1676);

    // rs1_val==46341 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1680);

    // rs1_val==46341 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1684);

    // rs1_val==46341 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1688);

    // rs1_val==46341 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1692);

    // rs1_val==46341 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1696);

    // rs1_val==46341 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1700);

    // rs1_val==0 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1704);

    // rs1_val==0 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1708);

    // rs1_val==0 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1712);

    // rs1_val==0 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1716);

    // rs1_val==0 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1720);

    // rs1_val==0 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1724);

    // rs1_val==0 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1728);

    // rs1_val==0 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1732);

    // rs1_val==0 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1736);

    // rs1_val==0 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1740);

    // rs1_val==0 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1744);

    // rs1_val==0 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1748);

    // rs1_val==0 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1752);

    // rs1_val==4 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1756);

    // rs1_val==4 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1760);

    // rs1_val==4 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1764);

    // rs1_val==4 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1768);

    // rs1_val==4 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1772);

    // rs1_val==4 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1776);

    // rs1_val==4 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1780);

    // rs1_val==4 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1784);

    // rs1_val==4 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1788);

    // rs1_val==4 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1792);

    // rs1_val==4 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1796);

    // rs1_val==4 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1800);

    // rs1_val==4 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1804);

    // rs1_val==4 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1808);

    // rs1_val==4 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1812);

    // rs1_val==4 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1816);

    // rs1_val==4 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1820);

    // rs1_val==4 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1824);

    // rs1_val==4 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1828);

    // rs1_val==4 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1832);

    // rs1_val==4 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1836);

    // rs1_val==4 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1840);

    // rs1_val==858993458 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1844);

    // rs1_val==858993458 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1848);

    // rs1_val==858993458 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1852);

    // rs1_val==858993458 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1856);

    // rs1_val==858993458 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1860);

    // rs1_val==858993458 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1864);

    // rs1_val==858993458 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1868);

    // rs1_val==858993458 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1872);

    // rs1_val==858993458 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1876);

    // rs1_val==858993458 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1880);

    // rs1_val==858993458 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1884);

    // rs1_val==858993458 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1888);

    // rs1_val==858993458 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1892);

    // rs1_val==858993458 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1896);

    // rs1_val==858993458 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1900);

    // rs1_val==858993458 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1904);

    // rs1_val==858993458 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1908);

    // rs1_val==858993458 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1912);

    // rs1_val==858993458 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1916);

    // rs1_val==858993458 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1920);

    // rs1_val==858993458 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1924);

    // rs1_val==858993458 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1928);

    // rs1_val==1717986917 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1932);

    // rs1_val==1717986917 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1936);

    // rs1_val==1717986917 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1940);

    // rs1_val==1717986917 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1944);

    // rs1_val==1717986917 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1948);

    // rs1_val==1717986917 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1952);

    // rs1_val==1717986917 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1956);

    // rs1_val==1717986917 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1960);

    // rs1_val==1717986917 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1964);

    // rs1_val==1717986917 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1968);

    // rs1_val==1717986917 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1972);

    // rs1_val==1717986917 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1976);

    // rs1_val==1717986917 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1980);

    // rs1_val==1717986917 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1984);

    // rs1_val==1717986917 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1988);

    // rs1_val==1717986917 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1992);

    // rs1_val==1717986917 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 1996);

    // rs1_val==1717986917 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 2000);

    // rs1_val==1717986917 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 2004);

    // rs1_val==1717986917 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 2008);

    // rs1_val==1717986917 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 2012);

    // rs1_val==1717986917 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 2016);

    // rs1_val==46339 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 2020);

    // rs1_val==46339 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 2024);

    // rs1_val==46339 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 2028);

    // rs1_val==46339 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 2032);

    // rs1_val==46339 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 2036);

    // rs1_val==46339 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 2040);

    // rs1_val==46339 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 2044);

    c.li(Reg::X6, 2144);
    // rs1_val==46339 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 0);

    // rs1_val==46339 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 4);

    // rs1_val==46339 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 8);

    // rs1_val==46339 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 12);

    // rs1_val==46339 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 16);

    // rs1_val==46339 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 20);

    // rs1_val==46339 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 24);

    // rs1_val==46339 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 28);

    // rs1_val==46339 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 32);

    // rs1_val==46339 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 36);

    // rs1_val==46339 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 40);

    // rs1_val==46339 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 44);

    // rs1_val==46339 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 48);

    // rs1_val==46339 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 52);

    // rs1_val==46339 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 56);

    // rs1_val==1431655766 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 60);

    // rs1_val==1431655766 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 64);

    // rs1_val==1431655766 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 68);

    // rs1_val==1431655766 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 72);

    // rs1_val==1431655766 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 76);

    // rs1_val==1431655766 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 80);

    // rs1_val==1431655766 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 84);

    // rs1_val==1431655766 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 88);

    // rs1_val==1431655766 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 92);

    // rs1_val==1431655766 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 96);

    // rs1_val==1431655766 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 100);

    // rs1_val==1431655766 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 104);

    // rs1_val==1431655766 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 108);

    // rs1_val==1431655766 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 112);

    // rs1_val==1431655766 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 116);

    // rs1_val==1431655766 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 120);

    // rs1_val==1431655766 and rs2_val==-1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 124);

    // rs1_val==1431655766 and rs2_val==6, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 128);

    // rs1_val==1431655766 and rs2_val==858993460, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 132);

    // rs1_val==1431655766 and rs2_val==1717986919, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 136);

    // rs1_val==1431655766 and rs2_val==-46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(-0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 140);

    // rs1_val==1431655766 and rs2_val==46341, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 144);

    // rs1_val==-1431655765 and rs2_val==3, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 148);

    // rs1_val==-1431655765 and rs2_val==1431655765, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 152);

    // rs1_val==-1431655765 and rs2_val==-1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 156);

    // rs1_val==-1431655765 and rs2_val==5, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 160);

    // rs1_val==-1431655765 and rs2_val==858993459, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 164);

    // rs1_val==-1431655765 and rs2_val==1717986918, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 168);

    // rs1_val==-1431655765 and rs2_val==-46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb504
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(-0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 172);

    // rs1_val==-1431655765 and rs2_val==46340, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 176);

    // rs1_val==-1431655765 and rs2_val==2, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 180);

    // rs1_val==-1431655765 and rs2_val==1431655764, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 184);

    // rs1_val==-1431655765 and rs2_val==0, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 188);

    // rs1_val==-1431655765 and rs2_val==4, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 192);

    // rs1_val==-1431655765 and rs2_val==858993458, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 196);

    // rs1_val==-1431655765 and rs2_val==1717986917, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 200);

    // rs1_val==-1431655765 and rs2_val==46339, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 204);

    // rs1_val==-1431655765 and rs2_val==1431655766, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 208);

    // rs1_val > 0 and rs2_val < 0, rs2_val == -2049, rs1_val == 2
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x801
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(-0x801));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 212);

    // rs1_val < 0 and rs2_val < 0, rs1_val == -268435457, rs2_val == -1025
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x10000001;  op2val:-0x401
    c.li(Reg::X10, MASK_XLEN(-0x10000001));
    c.li(Reg::X11, MASK_XLEN(-0x401));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 216);

    // rs2_val == 2097152, 
    // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x200000
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x200000));
    c.or_(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X6, 220);


        
    
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