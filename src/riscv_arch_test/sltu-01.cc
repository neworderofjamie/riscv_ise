#include "sltu-01.h"

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
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
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
// sltu_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &sltu_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
CodeGenerator sltu_01::generateCode()
{
    CodeGenerator c;
    
    c.li(Reg::X9, 16);
    // rs2 == rd != rs1, rs1==x0, rs2==x31, rd==x31, rs1_val > 0 and rs2_val > 0, rs2_val == 4294967294, rs1_val != rs2_val and rs1_val > 0 and rs2_val > 0, rs1_val == 2048
    // opcode: sltu ; op1:x0; op2:x31; dest:x31; op1val:0x0;  op2val:0xfffffffe
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.li(Reg::X31, MASK_XLEN(0xfffffffe));
    c.sltu(Reg::X31, Reg::X0, Reg::X31);
    c.sw(Reg::X31, Reg::X9, 0);

    // rs1 == rs2 != rd, rs1==x19, rs2==x19, rd==x5, rs1_val == rs2_val and rs1_val > 0 and rs2_val > 0, rs2_val == 1048576, rs1_val == 1048576
    // opcode: sltu ; op1:x19; op2:x19; dest:x5; op1val:0x100000;  op2val:0x100000
    c.li(Reg::X19, MASK_XLEN(0x100000));
    c.li(Reg::X19, MASK_XLEN(0x100000));
    c.sltu(Reg::X5, Reg::X19, Reg::X19);
    c.sw(Reg::X5, Reg::X9, 4);

    // rs1 == rs2 == rd, rs1==x25, rs2==x25, rd==x25, rs2_val == 0, rs1_val == 1073741824
    // opcode: sltu ; op1:x25; op2:x25; dest:x25; op1val:0x40000000;  op2val:0x40000000
    c.li(Reg::X25, MASK_XLEN(0x40000000));
    c.li(Reg::X25, MASK_XLEN(0x40000000));
    c.sltu(Reg::X25, Reg::X25, Reg::X25);
    c.sw(Reg::X25, Reg::X9, 8);

    // rs1 == rd != rs2, rs1==x14, rs2==x24, rd==x14, rs2_val == (2**(xlen)-1), rs1_val == 4294967294
    // opcode: sltu ; op1:x14; op2:x24; dest:x14; op1val:0xfffffffe;  op2val:0xffffffff
    c.li(Reg::X14, MASK_XLEN(0xfffffffe));
    c.li(Reg::X24, MASK_XLEN(0xffffffff));
    c.sltu(Reg::X14, Reg::X14, Reg::X24);
    c.sw(Reg::X14, Reg::X9, 12);

    // rs1 != rs2  and rs1 != rd and rs2 != rd, rs1==x17, rs2==x13, rd==x12, rs2_val == 1, rs1_val == 1, rs1_val==1 and rs2_val==1
    // opcode: sltu ; op1:x17; op2:x13; dest:x12; op1val:0x1;  op2val:0x1
    c.li(Reg::X17, MASK_XLEN(0x1));
    c.li(Reg::X13, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X17, Reg::X13);
    c.sw(Reg::X12, Reg::X9, 16);

    // rs1==x26, rs2==x18, rd==x24, rs1_val == 0, 
    // opcode: sltu ; op1:x26; op2:x18; dest:x24; op1val:0x0;  op2val:0xb
    c.li(Reg::X26, MASK_XLEN(0x0));
    c.li(Reg::X18, MASK_XLEN(0xb));
    c.sltu(Reg::X24, Reg::X26, Reg::X18);
    c.sw(Reg::X24, Reg::X9, 20);

    // rs1==x5, rs2==x14, rd==x19, rs1_val == (2**(xlen)-1), 
    // opcode: sltu ; op1:x5; op2:x14; dest:x19; op1val:0xffffffff;  op2val:0x0
    c.li(Reg::X5, MASK_XLEN(0xffffffff));
    c.li(Reg::X14, MASK_XLEN(0x0));
    c.sltu(Reg::X19, Reg::X5, Reg::X14);
    c.sw(Reg::X19, Reg::X9, 24);

    // rs1==x3, rs2==x22, rd==x0, rs2_val == 2, rs1_val==4 and rs2_val==2, rs1_val == 4
    // opcode: sltu ; op1:x3; op2:x22; dest:x0; op1val:0x4;  op2val:0x2
    c.li(Reg::X3, MASK_XLEN(0x4));
    c.li(Reg::X22, MASK_XLEN(0x2));
    c.sltu(Reg::X0, Reg::X3, Reg::X22);
    c.sw(Reg::X0, Reg::X9, 28);

    // rs1==x23, rs2==x29, rd==x20, rs2_val == 4, rs1_val == 4160749567
    // opcode: sltu ; op1:x23; op2:x29; dest:x20; op1val:0xf7ffffff;  op2val:0x4
    c.li(Reg::X23, MASK_XLEN(0xf7ffffff));
    c.li(Reg::X29, MASK_XLEN(0x4));
    c.sltu(Reg::X20, Reg::X23, Reg::X29);
    c.sw(Reg::X20, Reg::X9, 32);

    // rs1==x4, rs2==x6, rd==x10, rs2_val == 8, 
    // opcode: sltu ; op1:x4; op2:x6; dest:x10; op1val:0x11;  op2val:0x8
    c.li(Reg::X4, MASK_XLEN(0x11));
    c.li(Reg::X6, MASK_XLEN(0x8));
    c.sltu(Reg::X10, Reg::X4, Reg::X6);
    c.sw(Reg::X10, Reg::X9, 36);

    // rs1==x12, rs2==x17, rd==x1, rs2_val == 16, rs1_val == 2147483647
    // opcode: sltu ; op1:x12; op2:x17; dest:x1; op1val:0x7fffffff;  op2val:0x10
    c.li(Reg::X12, MASK_XLEN(0x7fffffff));
    c.li(Reg::X17, MASK_XLEN(0x10));
    c.sltu(Reg::X1, Reg::X12, Reg::X17);
    c.sw(Reg::X1, Reg::X9, 40);

    // rs1==x30, rs2==x8, rd==x6, rs2_val == 32, rs1_val == 33554432
    // opcode: sltu ; op1:x30; op2:x8; dest:x6; op1val:0x2000000;  op2val:0x20
    c.li(Reg::X30, MASK_XLEN(0x2000000));
    c.li(Reg::X8, MASK_XLEN(0x20));
    c.sltu(Reg::X6, Reg::X30, Reg::X8);
    c.sw(Reg::X6, Reg::X9, 44);

    // rs1==x21, rs2==x16, rd==x3, rs2_val == 64, rs1_val == 4294965247
    // opcode: sltu ; op1:x21; op2:x16; dest:x3; op1val:0xfffff7ff;  op2val:0x40
    c.li(Reg::X21, MASK_XLEN(0xfffff7ff));
    c.li(Reg::X16, MASK_XLEN(0x40));
    c.sltu(Reg::X3, Reg::X21, Reg::X16);
    c.sw(Reg::X3, Reg::X9, 48);

    // rs1==x29, rs2==x26, rd==x17, rs2_val == 128, rs1_val == 1024
    // opcode: sltu ; op1:x29; op2:x26; dest:x17; op1val:0x400;  op2val:0x80
    c.li(Reg::X29, MASK_XLEN(0x400));
    c.li(Reg::X26, MASK_XLEN(0x80));
    c.sltu(Reg::X17, Reg::X29, Reg::X26);
    c.sw(Reg::X17, Reg::X9, 52);

    // rs1==x18, rs2==x10, rd==x28, rs2_val == 256, 
    // opcode: sltu ; op1:x18; op2:x10; dest:x28; op1val:0xd;  op2val:0x100
    c.li(Reg::X18, MASK_XLEN(0xd));
    c.li(Reg::X10, MASK_XLEN(0x100));
    c.sltu(Reg::X28, Reg::X18, Reg::X10);
    c.sw(Reg::X28, Reg::X9, 56);

    // rs1==x2, rs2==x28, rd==x11, rs2_val == 512, 
    // opcode: sltu ; op1:x2; op2:x28; dest:x11; op1val:0x4;  op2val:0x200
    c.li(Reg::X2, MASK_XLEN(0x4));
    c.li(Reg::X28, MASK_XLEN(0x200));
    c.sltu(Reg::X11, Reg::X2, Reg::X28);
    c.sw(Reg::X11, Reg::X9, 60);

    // rs1==x8, rs2==x30, rd==x29, rs2_val == 1024, rs1_val == 4294967231
    // opcode: sltu ; op1:x8; op2:x30; dest:x29; op1val:0xffffffbf;  op2val:0x400
    c.li(Reg::X8, MASK_XLEN(0xffffffbf));
    c.li(Reg::X30, MASK_XLEN(0x400));
    c.sltu(Reg::X29, Reg::X8, Reg::X30);
    c.sw(Reg::X29, Reg::X9, 64);

    // rs1==x22, rs2==x11, rd==x7, rs2_val == 2048, rs1_val == 128
    // opcode: sltu ; op1:x22; op2:x11; dest:x7; op1val:0x80;  op2val:0x800
    c.li(Reg::X22, MASK_XLEN(0x80));
    c.li(Reg::X11, MASK_XLEN(0x800));
    c.sltu(Reg::X7, Reg::X22, Reg::X11);
    c.sw(Reg::X7, Reg::X9, 68);

    c.li(Reg::X14, 88);
    // rs1==x15, rs2==x4, rd==x9, rs2_val == 4096, rs1_val == 2097152
    // opcode: sltu ; op1:x15; op2:x4; dest:x9; op1val:0x200000;  op2val:0x1000
    c.li(Reg::X15, MASK_XLEN(0x200000));
    c.li(Reg::X4, MASK_XLEN(0x1000));
    c.sltu(Reg::X9, Reg::X15, Reg::X4);
    c.sw(Reg::X9, Reg::X14, 0);

    // rs1==x28, rs2==x12, rd==x13, rs2_val == 8192, 
    // opcode: sltu ; op1:x28; op2:x12; dest:x13; op1val:0x80;  op2val:0x2000
    c.li(Reg::X28, MASK_XLEN(0x80));
    c.li(Reg::X12, MASK_XLEN(0x2000));
    c.sltu(Reg::X13, Reg::X28, Reg::X12);
    c.sw(Reg::X13, Reg::X14, 4);

    // rs1==x31, rs2==x20, rd==x26, rs2_val == 16384, rs1_val == 8388608
    // opcode: sltu ; op1:x31; op2:x20; dest:x26; op1val:0x800000;  op2val:0x4000
    c.li(Reg::X31, MASK_XLEN(0x800000));
    c.li(Reg::X20, MASK_XLEN(0x4000));
    c.sltu(Reg::X26, Reg::X31, Reg::X20);
    c.sw(Reg::X26, Reg::X14, 8);

    // rs1==x27, rs2==x5, rd==x16, rs2_val == 32768, rs1_val == 32768
    // opcode: sltu ; op1:x27; op2:x5; dest:x16; op1val:0x8000;  op2val:0x8000
    c.li(Reg::X27, MASK_XLEN(0x8000));
    c.li(Reg::X5, MASK_XLEN(0x8000));
    c.sltu(Reg::X16, Reg::X27, Reg::X5);
    c.sw(Reg::X16, Reg::X14, 12);

    // rs1==x16, rs2==x0, rd==x2, rs2_val == 65536, rs1_val == 3758096383
    // opcode: sltu ; op1:x16; op2:x0; dest:x2; op1val:0xdfffffff;  op2val:0x0
    c.li(Reg::X16, MASK_XLEN(0xdfffffff));
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.sltu(Reg::X2, Reg::X16, Reg::X0);
    c.sw(Reg::X2, Reg::X14, 16);

    // rs1==x11, rs2==x9, rd==x23, rs2_val == 131072, 
    // opcode: sltu ; op1:x11; op2:x9; dest:x23; op1val:0x3;  op2val:0x20000
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.li(Reg::X9, MASK_XLEN(0x20000));
    c.sltu(Reg::X23, Reg::X11, Reg::X9);
    c.sw(Reg::X23, Reg::X14, 20);

    // rs1==x20, rs2==x21, rd==x4, rs2_val == 262144, rs1_val == 2147483648
    // opcode: sltu ; op1:x20; op2:x21; dest:x4; op1val:0x80000000;  op2val:0x40000
    c.li(Reg::X20, MASK_XLEN(0x80000000));
    c.li(Reg::X21, MASK_XLEN(0x40000));
    c.sltu(Reg::X4, Reg::X20, Reg::X21);
    c.sw(Reg::X4, Reg::X14, 24);

    // rs1==x13, rs2==x7, rd==x27, rs2_val == 524288, rs1_val == 536870912
    // opcode: sltu ; op1:x13; op2:x7; dest:x27; op1val:0x20000000;  op2val:0x80000
    c.li(Reg::X13, MASK_XLEN(0x20000000));
    c.li(Reg::X7, MASK_XLEN(0x80000));
    c.sltu(Reg::X27, Reg::X13, Reg::X7);
    c.sw(Reg::X27, Reg::X14, 28);

    // rs1==x7, rs2==x1, rd==x18, rs2_val == 2097152, rs1_val == 8
    // opcode: sltu ; op1:x7; op2:x1; dest:x18; op1val:0x8;  op2val:0x200000
    c.li(Reg::X7, MASK_XLEN(0x8));
    c.li(Reg::X1, MASK_XLEN(0x200000));
    c.sltu(Reg::X18, Reg::X7, Reg::X1);
    c.sw(Reg::X18, Reg::X14, 32);

    // rs1==x1, rs2==x27, rd==x22, rs2_val == 4194304, rs1_val == 4293918719
    // opcode: sltu ; op1:x1; op2:x27; dest:x22; op1val:0xffefffff;  op2val:0x400000
    c.li(Reg::X1, MASK_XLEN(0xffefffff));
    c.li(Reg::X27, MASK_XLEN(0x400000));
    c.sltu(Reg::X22, Reg::X1, Reg::X27);
    c.sw(Reg::X22, Reg::X14, 36);

    // rs1==x9, rs2==x23, rd==x21, rs2_val == 8388608, rs1_val == 2863311530
    // opcode: sltu ; op1:x9; op2:x23; dest:x21; op1val:0xaaaaaaaa;  op2val:0x800000
    c.li(Reg::X9, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X23, MASK_XLEN(0x800000));
    c.sltu(Reg::X21, Reg::X9, Reg::X23);
    c.sw(Reg::X21, Reg::X14, 40);

    // rs1==x6, rs2==x2, rd==x30, rs2_val == 16777216, 
    // opcode: sltu ; op1:x6; op2:x2; dest:x30; op1val:0x66666667;  op2val:0x1000000
    c.li(Reg::X6, MASK_XLEN(0x66666667));
    c.li(Reg::X2, MASK_XLEN(0x1000000));
    c.sltu(Reg::X30, Reg::X6, Reg::X2);
    c.sw(Reg::X30, Reg::X14, 44);

    // rs1==x10, rs2==x3, rd==x15, rs2_val == 33554432, rs1_val == 4227858431
    // opcode: sltu ; op1:x10; op2:x3; dest:x15; op1val:0xfbffffff;  op2val:0x2000000
    c.li(Reg::X10, MASK_XLEN(0xfbffffff));
    c.li(Reg::X3, MASK_XLEN(0x2000000));
    c.sltu(Reg::X15, Reg::X10, Reg::X3);
    c.sw(Reg::X15, Reg::X14, 48);

    // rs1==x24, rs2==x15, rd==x8, rs2_val == 67108864, rs1_val == 256
    // opcode: sltu ; op1:x24; op2:x15; dest:x8; op1val:0x100;  op2val:0x4000000
    c.li(Reg::X24, MASK_XLEN(0x100));
    c.li(Reg::X15, MASK_XLEN(0x4000000));
    c.sltu(Reg::X8, Reg::X24, Reg::X15);
    c.sw(Reg::X8, Reg::X14, 52);

    // rs2_val == 134217728, rs1_val == 4294959103
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffffdfff;  op2val:0x8000000
    c.li(Reg::X10, MASK_XLEN(0xffffdfff));
    c.li(Reg::X11, MASK_XLEN(0x8000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 56);

    // rs2_val == 268435456, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x40000000;  op2val:0x10000000
    c.li(Reg::X10, MASK_XLEN(0x40000000));
    c.li(Reg::X11, MASK_XLEN(0x10000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 60);

    // rs2_val == 536870912, rs1_val == 4278190079
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfeffffff;  op2val:0x20000000
    c.li(Reg::X10, MASK_XLEN(0xfeffffff));
    c.li(Reg::X11, MASK_XLEN(0x20000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 64);

    // rs2_val == 1073741824, rs1_val == 268435456
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000000;  op2val:0x40000000
    c.li(Reg::X10, MASK_XLEN(0x10000000));
    c.li(Reg::X11, MASK_XLEN(0x40000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 68);

    // rs2_val == 2147483648, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x40000000;  op2val:0x80000000
    c.li(Reg::X10, MASK_XLEN(0x40000000));
    c.li(Reg::X11, MASK_XLEN(0x80000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 72);

    // rs2_val == 4294967293, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xfffffffd
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xfffffffd));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 76);

    // rs2_val == 4294967291, rs1_val == 262144
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x40000;  op2val:0xfffffffb
    c.li(Reg::X10, MASK_XLEN(0x40000));
    c.li(Reg::X11, MASK_XLEN(0xfffffffb));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 80);

    // rs2_val == 4294967287, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xfffffff7
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xfffffff7));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 84);

    // rs2_val == 4294967279, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xffffffef
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xffffffef));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 88);

    // rs2_val == 4294967263, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb;  op2val:0xffffffdf
    c.li(Reg::X10, MASK_XLEN(0xb));
    c.li(Reg::X11, MASK_XLEN(0xffffffdf));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 92);

    // rs2_val == 4294967231, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xa;  op2val:0xffffffbf
    c.li(Reg::X10, MASK_XLEN(0xa));
    c.li(Reg::X11, MASK_XLEN(0xffffffbf));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 96);

    // rs2_val == 4294967167, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x80;  op2val:0xffffff7f
    c.li(Reg::X10, MASK_XLEN(0x80));
    c.li(Reg::X11, MASK_XLEN(0xffffff7f));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 100);

    // rs2_val == 4294967039, rs1_val == 4294967287
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffff7;  op2val:0xfffffeff
    c.li(Reg::X10, MASK_XLEN(0xfffffff7));
    c.li(Reg::X11, MASK_XLEN(0xfffffeff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 104);

    // rs2_val == 4294966783, rs1_val == 16777216
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1000000;  op2val:0xfffffdff
    c.li(Reg::X10, MASK_XLEN(0x1000000));
    c.li(Reg::X11, MASK_XLEN(0xfffffdff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 108);

    // rs2_val == 4294966271, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffff7;  op2val:0xfffffbff
    c.li(Reg::X10, MASK_XLEN(0xfffffff7));
    c.li(Reg::X11, MASK_XLEN(0xfffffbff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 112);

    // rs2_val == 4294965247, rs1_val == 8192
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2000;  op2val:0xfffff7ff
    c.li(Reg::X10, MASK_XLEN(0x2000));
    c.li(Reg::X11, MASK_XLEN(0xfffff7ff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 116);

    // rs2_val == 4294963199, rs1_val == 4294443007
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfff7ffff;  op2val:0xffffefff
    c.li(Reg::X10, MASK_XLEN(0xfff7ffff));
    c.li(Reg::X11, MASK_XLEN(0xffffefff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 120);

    // rs2_val == 4294959103, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xffffdfff
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xffffdfff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 124);

    // rs2_val == 4294950911, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xffffbfff
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xffffbfff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 128);

    // rs2_val == 4294934527, rs1_val == 4294836223
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffdffff;  op2val:0xffff7fff
    c.li(Reg::X10, MASK_XLEN(0xfffdffff));
    c.li(Reg::X11, MASK_XLEN(0xffff7fff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 132);

    // rs2_val == 4294901759, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xa;  op2val:0xfffeffff
    c.li(Reg::X10, MASK_XLEN(0xa));
    c.li(Reg::X11, MASK_XLEN(0xfffeffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 136);

    // rs2_val == 4294836223, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfff7ffff;  op2val:0xfffdffff
    c.li(Reg::X10, MASK_XLEN(0xfff7ffff));
    c.li(Reg::X11, MASK_XLEN(0xfffdffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 140);

    // rs2_val == 4294705151, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x8000;  op2val:0xfffbffff
    c.li(Reg::X10, MASK_XLEN(0x8000));
    c.li(Reg::X11, MASK_XLEN(0xfffbffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 144);

    // rs2_val == 4294443007, rs1_val == 64
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x40;  op2val:0xfff7ffff
    c.li(Reg::X10, MASK_XLEN(0x40));
    c.li(Reg::X11, MASK_XLEN(0xfff7ffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 148);

    // rs2_val == 4293918719, rs1_val == 16
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10;  op2val:0xffefffff
    c.li(Reg::X10, MASK_XLEN(0x10));
    c.li(Reg::X11, MASK_XLEN(0xffefffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 152);

    // rs2_val == 4292870143, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xffdfffff
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xffdfffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 156);

    // rs2_val == 4290772991, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xffbfffff
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xffbfffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 160);

    // rs2_val == 4286578687, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xff7fffff
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xff7fffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 164);

    // rs2_val == 4278190079, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xfeffffff
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xfeffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 168);

    // rs2_val == 4261412863, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x800000;  op2val:0xfdffffff
    c.li(Reg::X10, MASK_XLEN(0x800000));
    c.li(Reg::X11, MASK_XLEN(0xfdffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 172);

    // rs2_val == 4227858431, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xfbffffff
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xfbffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 176);

    // rs2_val == 4160749567, rs1_val == 1431655765
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xf7ffffff
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xf7ffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 180);

    // rs2_val == 4026531839, rs1_val == 4292870143
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffdfffff;  op2val:0xefffffff
    c.li(Reg::X10, MASK_XLEN(0xffdfffff));
    c.li(Reg::X11, MASK_XLEN(0xefffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 184);

    // rs2_val == 3758096383, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xdfffffff
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xdfffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 188);

    // rs2_val == 3221225471, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x800;  op2val:0xbfffffff
    c.li(Reg::X10, MASK_XLEN(0x800));
    c.li(Reg::X11, MASK_XLEN(0xbfffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 192);

    // rs2_val == 2147483647, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x800000;  op2val:0x7fffffff
    c.li(Reg::X10, MASK_XLEN(0x800000));
    c.li(Reg::X11, MASK_XLEN(0x7fffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 196);

    // rs2_val == 1431655765, rs1_val == 4290772991
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffbfffff;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xffbfffff));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 200);

    // rs2_val == 2863311530, rs1_val==1431655764 and rs2_val==2863311530
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 204);

    // rs1_val == 2, rs1_val==2 and rs2_val==1717986917
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 208);

    // rs1_val == 32, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x20;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x20));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 212);

    // rs1_val == 512, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x200;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x200));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 216);

    // rs1_val == 4096, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1000;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x1000));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 220);

    // rs1_val == 16384, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4000;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x4000));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 224);

    // rs1_val == 65536, rs1_val==65536 and rs2_val==5
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 228);

    // rs1_val == 131072, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x20000;  op2val:0x20000
    c.li(Reg::X10, MASK_XLEN(0x20000));
    c.li(Reg::X11, MASK_XLEN(0x20000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 232);

    // rs1_val == 524288, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x80000;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x80000));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 236);

    // rs1_val == 4194304, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x400000;  op2val:0x200000
    c.li(Reg::X10, MASK_XLEN(0x400000));
    c.li(Reg::X11, MASK_XLEN(0x200000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 240);

    // rs1_val == 67108864, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4000000;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x4000000));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 244);

    // rs1_val == 134217728, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x8000000;  op2val:0xfffdffff
    c.li(Reg::X10, MASK_XLEN(0x8000000));
    c.li(Reg::X11, MASK_XLEN(0xfffdffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 248);

    // rs1_val == 4294967293, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffffd;  op2val:0x11
    c.li(Reg::X10, MASK_XLEN(0xfffffffd));
    c.li(Reg::X11, MASK_XLEN(0x11));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 252);

    // rs1_val == 4294967291, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffffb;  op2val:0xffffff7f
    c.li(Reg::X10, MASK_XLEN(0xfffffffb));
    c.li(Reg::X11, MASK_XLEN(0xffffff7f));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 256);

    // rs1_val == 4294967279, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffffffef;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xffffffef));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 260);

    // rs1_val == 4294967263, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffffffdf;  op2val:0xfffbffff
    c.li(Reg::X10, MASK_XLEN(0xffffffdf));
    c.li(Reg::X11, MASK_XLEN(0xfffbffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 264);

    // rs1_val == 4294967167, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffffff7f;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xffffff7f));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 268);

    // rs1_val == 4294967039, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffeff;  op2val:0x40000000
    c.li(Reg::X10, MASK_XLEN(0xfffffeff));
    c.li(Reg::X11, MASK_XLEN(0x40000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 272);

    // rs1_val == 4294966783, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffdff;  op2val:0xa
    c.li(Reg::X10, MASK_XLEN(0xfffffdff));
    c.li(Reg::X11, MASK_XLEN(0xa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 276);

    // rs1_val == 4294966271, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffbff;  op2val:0xf7ffffff
    c.li(Reg::X10, MASK_XLEN(0xfffffbff));
    c.li(Reg::X11, MASK_XLEN(0xf7ffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 280);

    // rs1_val == 4294963199, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffffefff;  op2val:0x100000
    c.li(Reg::X10, MASK_XLEN(0xffffefff));
    c.li(Reg::X11, MASK_XLEN(0x100000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 284);

    // rs1_val == 4294950911, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffffbfff;  op2val:0x8000
    c.li(Reg::X10, MASK_XLEN(0xffffbfff));
    c.li(Reg::X11, MASK_XLEN(0x8000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 288);

    // rs1_val == 4294934527, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff7fff;  op2val:0xfffeffff
    c.li(Reg::X10, MASK_XLEN(0xffff7fff));
    c.li(Reg::X11, MASK_XLEN(0xfffeffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 292);

    // rs1_val == 4294901759, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffeffff;  op2val:0xfffff7ff
    c.li(Reg::X10, MASK_XLEN(0xfffeffff));
    c.li(Reg::X11, MASK_XLEN(0xfffff7ff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 296);

    // rs1_val == 4294705151, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffbffff;  op2val:0xfff7ffff
    c.li(Reg::X10, MASK_XLEN(0xfffbffff));
    c.li(Reg::X11, MASK_XLEN(0xfff7ffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 300);

    // rs1_val == 4286578687, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xff7fffff;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xff7fffff));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 304);

    // rs1_val == 4261412863, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfdffffff;  op2val:0xbfffffff
    c.li(Reg::X10, MASK_XLEN(0xfdffffff));
    c.li(Reg::X11, MASK_XLEN(0xbfffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 308);

    // rs1_val == 4026531839, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xefffffff;  op2val:0x800
    c.li(Reg::X10, MASK_XLEN(0xefffffff));
    c.li(Reg::X11, MASK_XLEN(0x800));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 312);

    // rs1_val == 3221225471, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xbfffffff;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xbfffffff));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 316);

    // rs1_val==3 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 320);

    // rs1_val==3 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 324);

    // rs1_val==3 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 328);

    // rs1_val==3 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 332);

    // rs1_val==3 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 336);

    // rs1_val==3 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 340);

    // rs1_val==3 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 344);

    // rs1_val==3 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 348);

    // rs1_val==3 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 352);

    // rs1_val==3 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 356);

    // rs1_val==3 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 360);

    // rs1_val==3 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 364);

    // rs1_val==3 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 368);

    // rs1_val==3 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 372);

    // rs1_val==3 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 376);

    // rs1_val==3 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 380);

    // rs1_val==3 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 384);

    // rs1_val==3 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 388);

    // rs1_val==3 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 392);

    // rs1_val==3 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 396);

    // rs1_val==3 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 400);

    // rs1_val==3 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 404);

    // rs1_val==3 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 408);

    // rs1_val==3 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 412);

    // rs1_val==3 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 416);

    // rs1_val==1431655765 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 420);

    // rs1_val==1431655765 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 424);

    // rs1_val==1431655765 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 428);

    // rs1_val==1431655765 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 432);

    // rs1_val==1431655765 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 436);

    // rs1_val==1431655765 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 440);

    // rs1_val==1431655765 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 444);

    // rs1_val==1431655765 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 448);

    // rs1_val==1431655765 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 452);

    // rs1_val==1431655765 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 456);

    // rs1_val==1431655765 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 460);

    // rs1_val==1431655765 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 464);

    // rs1_val==1431655765 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 468);

    // rs1_val==1431655765 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 472);

    // rs1_val==1431655765 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 476);

    // rs1_val==1431655765 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 480);

    // rs1_val==1431655765 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 484);

    // rs1_val==1431655765 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 488);

    // rs1_val==1431655765 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 492);

    // rs1_val==1431655765 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 496);

    // rs1_val==1431655765 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 500);

    // rs1_val==1431655765 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 504);

    // rs1_val==1431655765 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 508);

    // rs1_val==1431655765 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 512);

    // rs1_val==1431655765 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 516);

    // rs1_val==2863311530 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 520);

    // rs1_val==2863311530 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 524);

    // rs1_val==2863311530 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 528);

    // rs1_val==2863311530 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 532);

    // rs1_val==2863311530 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 536);

    // rs1_val==2863311530 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 540);

    // rs1_val==2863311530 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 544);

    // rs1_val==2863311530 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 548);

    // rs1_val==2863311530 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 552);

    // rs1_val==2863311530 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 556);

    // rs1_val==2863311530 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 560);

    // rs1_val==2863311530 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 564);

    // rs1_val==2863311530 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 568);

    // rs1_val==2863311530 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 572);

    // rs1_val==2863311530 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 576);

    // rs1_val==2863311530 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 580);

    // rs1_val==2863311530 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 584);

    // rs1_val==2863311530 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 588);

    // rs1_val==2863311530 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 592);

    // rs1_val==2863311530 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 596);

    // rs1_val==2863311530 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 600);

    // rs1_val==2863311530 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 604);

    // rs1_val==2863311530 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 608);

    // rs1_val==2863311530 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 612);

    // rs1_val==2863311530 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 616);

    // rs1_val==5 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 620);

    // rs1_val==5 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 624);

    // rs1_val==5 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 628);

    // rs1_val==5 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 632);

    // rs1_val==5 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 636);

    // rs1_val==5 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 640);

    // rs1_val==5 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 644);

    // rs1_val==5 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 648);

    // rs1_val==5 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 652);

    // rs1_val==5 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 656);

    // rs1_val==5 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 660);

    // rs1_val==5 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 664);

    // rs1_val==5 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 668);

    // rs1_val==5 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 672);

    // rs1_val==5 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 676);

    // rs1_val==5 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 680);

    // rs1_val==5 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 684);

    // rs1_val==5 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 688);

    // rs1_val==5 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 692);

    // rs1_val==5 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 696);

    // rs1_val==5 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 700);

    // rs1_val==5 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 704);

    // rs1_val==5 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 708);

    // rs1_val==5 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 712);

    // rs1_val==5 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 716);

    // rs1_val==858993459 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 720);

    // rs1_val==858993459 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 724);

    // rs1_val==858993459 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 728);

    // rs1_val==858993459 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 732);

    // rs1_val==858993459 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 736);

    // rs1_val==858993459 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 740);

    // rs1_val==858993459 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 744);

    // rs1_val==858993459 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 748);

    // rs1_val==858993459 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 752);

    // rs1_val==858993459 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 756);

    // rs1_val==858993459 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 760);

    // rs1_val==858993459 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 764);

    // rs1_val==858993459 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 768);

    // rs1_val==858993459 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 772);

    // rs1_val==858993459 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 776);

    // rs1_val==858993459 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 780);

    // rs1_val==858993459 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 784);

    // rs1_val==858993459 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 788);

    // rs1_val==858993459 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 792);

    // rs1_val==858993459 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 796);

    // rs1_val==858993459 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 800);

    // rs1_val==858993459 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 804);

    // rs1_val==858993459 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 808);

    // rs1_val==858993459 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 812);

    // rs1_val==858993459 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 816);

    // rs1_val==1717986918 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 820);

    // rs1_val==1717986918 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 824);

    // rs1_val==1717986918 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 828);

    // rs1_val==1717986918 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 832);

    // rs1_val==1717986918 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 836);

    // rs1_val==1717986918 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 840);

    // rs1_val==1717986918 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 844);

    // rs1_val==1717986918 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 848);

    // rs1_val==1717986918 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 852);

    // rs1_val==1717986918 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 856);

    // rs1_val==1717986918 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 860);

    // rs1_val==1717986918 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 864);

    // rs1_val==1717986918 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 868);

    // rs1_val==1717986918 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 872);

    // rs1_val==1717986918 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 876);

    // rs1_val==1717986918 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 880);

    // rs1_val==1717986918 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 884);

    // rs1_val==1717986918 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 888);

    // rs1_val==1717986918 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 892);

    // rs1_val==1717986918 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 896);

    // rs1_val==1717986918 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 900);

    // rs1_val==1717986918 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 904);

    // rs1_val==1717986918 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 908);

    // rs1_val==1717986918 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 912);

    // rs1_val==1717986918 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 916);

    // rs1_val==46340 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 920);

    // rs1_val==46340 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 924);

    // rs1_val==46340 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 928);

    // rs1_val==46340 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 932);

    // rs1_val==46340 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 936);

    // rs1_val==46340 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 940);

    // rs1_val==46340 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 944);

    // rs1_val==46340 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 948);

    // rs1_val==46340 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 952);

    // rs1_val==46340 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 956);

    // rs1_val==46340 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 960);

    // rs1_val==46340 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 964);

    // rs1_val==46340 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 968);

    // rs1_val==46340 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 972);

    // rs1_val==46340 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 976);

    // rs1_val==46340 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 980);

    // rs1_val==46340 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 984);

    // rs1_val==46340 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 988);

    // rs1_val==46340 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 992);

    // rs1_val==46340 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 996);

    // rs1_val==46340 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1000);

    // rs1_val==46340 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1004);

    // rs1_val==46340 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1008);

    // rs1_val==46340 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1012);

    // rs1_val==46340 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1016);

    // rs1_val==0 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1020);

    // rs1_val==0 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1024);

    // rs1_val==0 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1028);

    // rs1_val==0 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1032);

    // rs1_val==0 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1036);

    // rs1_val==0 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1040);

    // rs1_val==0 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1044);

    // rs1_val==0 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1048);

    // rs1_val==0 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1052);

    // rs1_val==0 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1056);

    // rs1_val==0 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1060);

    // rs1_val==0 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1064);

    // rs1_val==0 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1068);

    // rs1_val==0 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1072);

    // rs1_val==0 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1076);

    // rs1_val==0 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1080);

    // rs1_val==0 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1084);

    // rs1_val==0 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1088);

    // rs1_val==0 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1092);

    // rs1_val==0 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1096);

    // rs1_val==0 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1100);

    // rs1_val==0 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1104);

    // rs1_val==0 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1108);

    // rs1_val==0 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1112);

    // rs1_val==0 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1116);

    // rs1_val==65535 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1120);

    // rs1_val==65535 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1124);

    // rs1_val==65535 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1128);

    // rs1_val==65535 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1132);

    // rs1_val==65535 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1136);

    // rs1_val==65535 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1140);

    // rs1_val==65535 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1144);

    // rs1_val==65535 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1148);

    // rs1_val==65535 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1152);

    // rs1_val==65535 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1156);

    // rs1_val==65535 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1160);

    // rs1_val==65535 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1164);

    // rs1_val==65535 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1168);

    // rs1_val==65535 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1172);

    // rs1_val==65535 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1176);

    // rs1_val==65535 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1180);

    // rs1_val==65535 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1184);

    // rs1_val==65535 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1188);

    // rs1_val==65535 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1192);

    // rs1_val==65535 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1196);

    // rs1_val==65535 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1200);

    // rs1_val==65535 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1204);

    // rs1_val==65535 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1208);

    // rs1_val==65535 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1212);

    // rs1_val==65535 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1216);

    // rs1_val==2 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1220);

    // rs1_val==2 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1224);

    // rs1_val==2 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1228);

    // rs1_val==2 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1232);

    // rs1_val==2 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1236);

    // rs1_val==2 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1240);

    // rs1_val==2 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1244);

    // rs1_val==2 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1248);

    // rs1_val==2 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1252);

    // rs1_val==2 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1256);

    // rs1_val==2 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1260);

    // rs1_val==2 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1264);

    // rs1_val==2 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1268);

    // rs1_val==2 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1272);

    // rs1_val==2 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1276);

    // rs1_val==2 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1280);

    // rs1_val==2 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1284);

    // rs1_val==2 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1288);

    // rs1_val==2 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1292);

    // rs1_val==2 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1296);

    // rs1_val==2 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1300);

    // rs1_val==2 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1304);

    // rs1_val==2 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1308);

    // rs1_val==2 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1312);

    // rs1_val==1431655764 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1316);

    // rs1_val==1431655764 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1320);

    // rs1_val==1431655764 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1324);

    // rs1_val==1431655764 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1328);

    // rs1_val==1431655764 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1332);

    // rs1_val==1431655764 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1336);

    // rs1_val==1431655764 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1340);

    // rs1_val==1431655764 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1344);

    // rs1_val==1431655764 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1348);

    // rs1_val==1431655764 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1352);

    // rs1_val==1431655764 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1356);

    // rs1_val==1431655764 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1360);

    // rs1_val==1431655764 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1364);

    // rs1_val==1431655764 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1368);

    // rs1_val==1431655764 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1372);

    // rs1_val==1431655764 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1376);

    // rs1_val==1431655764 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1380);

    // rs1_val==1431655764 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1384);

    // rs1_val==1431655764 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1388);

    // rs1_val==1431655764 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1392);

    // rs1_val==1431655764 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1396);

    // rs1_val==1431655764 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1400);

    // rs1_val==1431655764 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1404);

    // rs1_val==1431655764 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1408);

    // rs1_val==2863311529 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1412);

    // rs1_val==2863311529 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1416);

    // rs1_val==2863311529 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1420);

    // rs1_val==2863311529 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1424);

    // rs1_val==2863311529 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1428);

    // rs1_val==2863311529 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1432);

    // rs1_val==2863311529 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1436);

    // rs1_val==2863311529 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1440);

    // rs1_val==2863311529 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1444);

    // rs1_val==2863311529 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1448);

    // rs1_val==2863311529 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1452);

    // rs1_val==2863311529 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1456);

    // rs1_val==2863311529 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1460);

    // rs1_val==2863311529 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1464);

    // rs1_val==2863311529 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1468);

    // rs1_val==2863311529 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1472);

    // rs1_val==2863311529 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1476);

    // rs1_val==2863311529 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1480);

    // rs1_val==2863311529 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1484);

    // rs1_val==2863311529 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1488);

    // rs1_val==2863311529 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1492);

    // rs1_val==2863311529 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1496);

    // rs1_val==2863311529 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1500);

    // rs1_val==2863311529 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1504);

    // rs1_val==2863311529 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1508);

    // rs1_val==4 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1512);

    // rs1_val==4 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1516);

    // rs1_val==4 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1520);

    // rs1_val==4 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1524);

    // rs1_val==4 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1528);

    // rs1_val==4 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1532);

    // rs1_val==4 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1536);

    // rs1_val==4 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1540);

    // rs1_val==4 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1544);

    // rs1_val==4 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1548);

    // rs1_val==4 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1552);

    // rs1_val==4 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1556);

    // rs1_val==4 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1560);

    // rs1_val==4 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1564);

    // rs1_val==4 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1568);

    // rs1_val==4 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1572);

    // rs1_val==4 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1576);

    // rs1_val==4 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1580);

    // rs1_val==4 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1584);

    // rs1_val==4 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1588);

    // rs1_val==4 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1592);

    // rs1_val==4 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1596);

    // rs1_val==4 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1600);

    // rs1_val==4 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1604);

    // rs1_val==858993458 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1608);

    // rs1_val==858993458 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1612);

    // rs1_val==858993458 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1616);

    // rs1_val==858993458 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1620);

    // rs1_val==858993458 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1624);

    // rs1_val==858993458 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1628);

    // rs1_val==858993458 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1632);

    // rs1_val==858993458 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1636);

    // rs1_val==858993458 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1640);

    // rs1_val==858993458 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1644);

    // rs1_val==858993458 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1648);

    // rs1_val==858993458 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1652);

    // rs1_val==858993458 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1656);

    // rs1_val==858993458 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1660);

    // rs1_val==858993458 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1664);

    // rs1_val==858993458 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1668);

    // rs1_val==858993458 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1672);

    // rs1_val==858993458 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1676);

    // rs1_val==858993458 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1680);

    // rs1_val==858993458 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1684);

    // rs1_val==858993458 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1688);

    // rs1_val==858993458 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1692);

    // rs1_val==858993458 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1696);

    // rs1_val==858993458 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1700);

    // rs1_val==858993458 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1704);

    // rs1_val==1717986917 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1708);

    // rs1_val==1717986917 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1712);

    // rs1_val==1717986917 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1716);

    // rs1_val==1717986917 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1720);

    // rs1_val==1717986917 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1724);

    // rs1_val==1717986917 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1728);

    // rs1_val==1717986917 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1732);

    // rs1_val==1717986917 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1736);

    // rs1_val==1717986917 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1740);

    // rs1_val==1717986917 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1744);

    // rs1_val==1717986917 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1748);

    // rs1_val==1717986917 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1752);

    // rs1_val==1717986917 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1756);

    // rs1_val==1717986917 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1760);

    // rs1_val==1717986917 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1764);

    // rs1_val==1717986917 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1768);

    // rs1_val==1717986917 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1772);

    // rs1_val==1717986917 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1776);

    // rs1_val==1717986917 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1780);

    // rs1_val==1717986917 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1784);

    // rs1_val==46339 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1788);

    // rs1_val==46339 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1792);

    // rs1_val==46339 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1796);

    // rs1_val==46339 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1800);

    // rs1_val==46339 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1804);

    // rs1_val==46339 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1808);

    // rs1_val==46339 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1812);

    // rs1_val==46339 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1816);

    // rs1_val==46339 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1820);

    // rs1_val==46339 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1824);

    // rs1_val==46339 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1828);

    // rs1_val==46339 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1832);

    // rs1_val==46339 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1836);

    // rs1_val==46339 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1840);

    // rs1_val==46339 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1844);

    // rs1_val==46339 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1848);

    // rs1_val==46339 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1852);

    // rs1_val==46339 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1856);

    // rs1_val==46339 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1860);

    // rs1_val==46339 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1864);

    // rs1_val==46339 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1868);

    // rs1_val==46339 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1872);

    // rs1_val==46339 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1876);

    // rs1_val==46339 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1880);

    // rs1_val==46339 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1884);

    // rs1_val==65534 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1888);

    // rs1_val==65534 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1892);

    // rs1_val==65534 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1896);

    // rs1_val==65534 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1900);

    // rs1_val==65534 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1904);

    // rs1_val==65534 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1908);

    // rs1_val==65534 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1912);

    // rs1_val==65534 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1916);

    // rs1_val==65534 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1920);

    // rs1_val==65534 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1924);

    // rs1_val==65534 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1928);

    // rs1_val==65534 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1932);

    // rs1_val==65534 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1936);

    // rs1_val==65534 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1940);

    // rs1_val==65534 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1944);

    // rs1_val==65534 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1948);

    // rs1_val==65534 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1952);

    // rs1_val==65534 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1956);

    // rs1_val==65534 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1960);

    // rs1_val==65534 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1964);

    // rs1_val==65534 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1968);

    // rs1_val==65534 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1972);

    // rs1_val==65534 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1976);

    // rs1_val==65534 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1980);

    // rs1_val==65534 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1984);

    // rs1_val==1431655766 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1988);

    // rs1_val==1431655766 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1992);

    // rs1_val==1431655766 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 1996);

    // rs1_val==1431655766 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 2000);

    // rs1_val==1431655766 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 2004);

    // rs1_val==1431655766 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 2008);

    // rs1_val==1431655766 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 2012);

    // rs1_val==1431655766 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 2016);

    // rs1_val==1431655766 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 2020);

    // rs1_val==1431655766 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 2024);

    // rs1_val==1431655766 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 2028);

    // rs1_val==1431655766 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 2032);

    // rs1_val==1431655766 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 2036);

    // rs1_val==1431655766 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 2040);

    // rs1_val==1431655766 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 2044);

    c.li(Reg::X14, 2136);
    // rs1_val==1431655766 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 0);

    // rs1_val==1431655766 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 4);

    // rs1_val==1431655766 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 8);

    // rs1_val==1431655766 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 12);

    // rs1_val==1431655766 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 16);

    // rs1_val==1431655766 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 20);

    // rs1_val==1431655766 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 24);

    // rs1_val==1431655766 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 28);

    // rs1_val==1431655766 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 32);

    // rs1_val==1431655766 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 36);

    // rs1_val==2863311531 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 40);

    // rs1_val==2863311531 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 44);

    // rs1_val==2863311531 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 48);

    // rs1_val==2863311531 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 52);

    // rs1_val==2863311531 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 56);

    // rs1_val==2863311531 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 60);

    // rs1_val==2863311531 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 64);

    // rs1_val==2863311531 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 68);

    // rs1_val==2863311531 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 72);

    // rs1_val==2863311531 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 76);

    // rs1_val==2863311531 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 80);

    // rs1_val==2863311531 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 84);

    // rs1_val==2863311531 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 88);

    // rs1_val==2863311531 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 92);

    // rs1_val==2863311531 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 96);

    // rs1_val==2863311531 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 100);

    // rs1_val==2863311531 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 104);

    // rs1_val==2863311531 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 108);

    // rs1_val==2863311531 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 112);

    // rs1_val==2863311531 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 116);

    // rs1_val==2863311531 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 120);

    // rs1_val==2863311531 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 124);

    // rs1_val==2863311531 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 128);

    // rs1_val==2863311531 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 132);

    // rs1_val==2863311531 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 136);

    // rs1_val==6 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 140);

    // rs1_val==6 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 144);

    // rs1_val==6 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 148);

    // rs1_val==6 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 152);

    // rs1_val==6 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 156);

    // rs1_val==6 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 160);

    // rs1_val==6 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 164);

    // rs1_val==6 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 168);

    // rs1_val==6 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 172);

    // rs1_val==6 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 176);

    // rs1_val==6 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 180);

    // rs1_val==6 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 184);

    // rs1_val==6 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 188);

    // rs1_val==6 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 192);

    // rs1_val==6 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 196);

    // rs1_val==6 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 200);

    // rs1_val==6 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 204);

    // rs1_val==6 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 208);

    // rs1_val==6 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 212);

    // rs1_val==6 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 216);

    // rs1_val==6 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 220);

    // rs1_val==6 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 224);

    // rs1_val==6 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 228);

    // rs1_val==6 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 232);

    // rs1_val==6 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 236);

    // rs1_val==858993460 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 240);

    // rs1_val==1717986917 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 244);

    // rs1_val==858993460 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 248);

    // rs1_val==858993460 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 252);

    // rs1_val==858993460 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 256);

    // rs1_val==858993460 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 260);

    // rs1_val==858993460 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 264);

    // rs1_val==858993460 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 268);

    // rs1_val==858993460 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 272);

    // rs1_val==858993460 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 276);

    // rs1_val==858993460 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 280);

    // rs1_val==858993460 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 284);

    // rs1_val==858993460 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 288);

    // rs1_val==858993460 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 292);

    // rs1_val==858993460 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 296);

    // rs1_val==858993460 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 300);

    // rs1_val==858993460 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 304);

    // rs1_val==858993460 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 308);

    // rs1_val==858993460 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 312);

    // rs1_val==858993460 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 316);

    // rs1_val==858993460 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 320);

    // rs1_val==858993460 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 324);

    // rs1_val==858993460 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 328);

    // rs1_val==858993460 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 332);

    // rs1_val==858993460 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 336);

    // rs1_val==858993460 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 340);

    // rs1_val==1717986919 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 344);

    // rs1_val==1717986919 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 348);

    // rs1_val==1717986919 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 352);

    // rs1_val==1717986919 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 356);

    // rs1_val==1717986919 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 360);

    // rs1_val==1717986919 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 364);

    // rs1_val==1717986919 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 368);

    // rs1_val==1717986919 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 372);

    // rs1_val==1717986919 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 376);

    // rs1_val==1717986919 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 380);

    // rs1_val==1717986919 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 384);

    // rs1_val==1717986919 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 388);

    // rs1_val==1717986919 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 392);

    // rs1_val==1717986919 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 396);

    // rs1_val==1717986919 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 400);

    // rs1_val==1717986919 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 404);

    // rs1_val==1717986919 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 408);

    // rs1_val==1717986919 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 412);

    // rs1_val==1717986919 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 416);

    // rs1_val==1717986919 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 420);

    // rs1_val==1717986919 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 424);

    // rs1_val==1717986919 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 428);

    // rs1_val==1717986919 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 432);

    // rs1_val==1717986919 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 436);

    // rs1_val==1717986919 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 440);

    // rs1_val==46341 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 444);

    // rs1_val==46341 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 448);

    // rs1_val==46341 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 452);

    // rs1_val==46341 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 456);

    // rs1_val==46341 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 460);

    // rs1_val==46341 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 464);

    // rs1_val==46341 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 468);

    // rs1_val==46341 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 472);

    // rs1_val==46341 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 476);

    // rs1_val==46341 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 480);

    // rs1_val==46341 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 484);

    // rs1_val==46341 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 488);

    // rs1_val==46341 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 492);

    // rs1_val==46341 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 496);

    // rs1_val==46341 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 500);

    // rs1_val==46341 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 504);

    // rs1_val==46341 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 508);

    // rs1_val==46341 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 512);

    // rs1_val==46341 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 516);

    // rs1_val==46341 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 520);

    // rs1_val==46341 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 524);

    // rs1_val==46341 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 528);

    // rs1_val==46341 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 532);

    // rs1_val==46341 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 536);

    // rs1_val==46341 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 540);

    // rs1_val==1 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 544);

    // rs1_val==1 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 548);

    // rs1_val==1 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 552);

    // rs1_val==1 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 556);

    // rs1_val==1 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 560);

    // rs1_val==1 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 564);

    // rs1_val==1 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 568);

    // rs1_val==1 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 572);

    // rs1_val==1 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 576);

    // rs1_val==1 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 580);

    // rs1_val==1 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 584);

    // rs1_val==1 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 588);

    // rs1_val==1 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 592);

    // rs1_val==1 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 596);

    // rs1_val==1 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 600);

    // rs1_val==1 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 604);

    // rs1_val==1 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 608);

    // rs1_val==1 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 612);

    // rs1_val==1 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 616);

    // rs1_val==1 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 620);

    // rs1_val==1 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 624);

    // rs1_val==1 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 628);

    // rs1_val==1 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 632);

    // rs1_val==1 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 636);

    // rs1_val==65536 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 640);

    // rs1_val==65536 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 644);

    // rs1_val==65536 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 648);

    // rs1_val==65536 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 652);

    // rs1_val==65536 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 656);

    // rs1_val==65536 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 660);

    // rs1_val==65536 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 664);

    // rs1_val==65536 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 668);

    // rs1_val==65536 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 672);

    // rs1_val==65536 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 676);

    // rs1_val==65536 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 680);

    // rs1_val==65536 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 684);

    // rs1_val==65536 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 688);

    // rs1_val==65536 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 692);

    // rs1_val==65536 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 696);

    // rs1_val==65536 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 700);

    // rs1_val==65536 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 704);

    // rs1_val==65536 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 708);

    // rs1_val==65536 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 712);

    // rs1_val==65536 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 716);

    // rs1_val==65536 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 720);

    // rs1_val==65536 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 724);

    // rs1_val==65536 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 728);

    // rs1_val==65536 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 732);

    // rs1_val==1717986917 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 736);

    // rs1_val==1717986917 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 740);

    // rs1_val==1717986917 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 744);

    // rs1_val==1717986917 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 748);

    // rs1_val > 0 and rs2_val > 0, rs2_val == 4294967294, rs1_val != rs2_val and rs1_val > 0 and rs2_val > 0, rs1_val == 2048
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x800;  op2val:0xfffffffe
    c.li(Reg::X10, MASK_XLEN(0x800));
    c.li(Reg::X11, MASK_XLEN(0xfffffffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 752);

    // rs1_val == rs2_val and rs1_val > 0 and rs2_val > 0, rs2_val == 1048576, rs1_val == 1048576
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x100000;  op2val:0x100000
    c.li(Reg::X10, MASK_XLEN(0x100000));
    c.li(Reg::X11, MASK_XLEN(0x100000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 756);

    // rs2_val == 2, rs1_val==4 and rs2_val==2, rs1_val == 4
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 760);

    // rs2_val == 65536, rs1_val == 3758096383
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xdfffffff;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xdfffffff));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X14, 764);


        
    // End
    c.ecall();
    return c;
}