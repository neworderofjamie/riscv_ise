#include "sltu-01.h"

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
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
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
// ArchTest::sltu_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &ArchTest::sltu_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
std::vector<uint32_t> ArchTest::sltu_01::generateCode(bool simulate)
{
    CodeGenerator c;
    
    // Ready flag is always stored in first word, zero it
    c.sw(Reg::X0, Reg::X0);
    
    
    c.li(Reg::X4, 20);
    // rs1 != rs2  and rs1 != rd and rs2 != rd, rs1==x12, rs2==x2, rd==x7, rs1_val != rs2_val and rs1_val > 0 and rs2_val > 0, rs1_val > 0 and rs2_val > 0, rs2_val == 16384
    // opcode: sltu ; op1:x12; op2:x2; dest:x7; op1val:0xaaaaaaab;  op2val:0x4000
    c.li(Reg::X12, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X2, MASK_XLEN(0x4000));
    c.sltu(Reg::X7, Reg::X12, Reg::X2);
    c.sw(Reg::X7, Reg::X4, 0);

    // rs1 == rd != rs2, rs1==x11, rs2==x7, rd==x11, rs2_val == 2147483647, rs1_val == 262144
    // opcode: sltu ; op1:x11; op2:x7; dest:x11; op1val:0x40000;  op2val:0x7fffffff
    c.li(Reg::X11, MASK_XLEN(0x40000));
    c.li(Reg::X7, MASK_XLEN(0x7fffffff));
    c.sltu(Reg::X11, Reg::X11, Reg::X7);
    c.sw(Reg::X11, Reg::X4, 4);

    // rs2 == rd != rs1, rs1==x6, rs2==x10, rd==x10, rs2_val == 3221225471, rs1_val == 131072
    // opcode: sltu ; op1:x6; op2:x10; dest:x10; op1val:0x20000;  op2val:0xbfffffff
    c.li(Reg::X6, MASK_XLEN(0x20000));
    c.li(Reg::X10, MASK_XLEN(0xbfffffff));
    c.sltu(Reg::X10, Reg::X6, Reg::X10);
    c.sw(Reg::X10, Reg::X4, 8);

    // rs1 == rs2 != rd, rs1==x14, rs2==x14, rd==x3, rs2_val == 3758096383, rs1_val == 4261412863
    // opcode: sltu ; op1:x14; op2:x14; dest:x3; op1val:0xfdffffff;  op2val:0xfdffffff
    c.li(Reg::X14, MASK_XLEN(0xfdffffff));
    c.li(Reg::X14, MASK_XLEN(0xfdffffff));
    c.sltu(Reg::X3, Reg::X14, Reg::X14);
    c.sw(Reg::X3, Reg::X4, 12);

    // rs1 == rs2 == rd, rs1==x5, rs2==x5, rd==x5, rs2_val == 4026531839, rs1_val == 0
    // opcode: sltu ; op1:x5; op2:x5; dest:x5; op1val:0x0;  op2val:0x0
    c.li(Reg::X5, MASK_XLEN(0x0));
    c.li(Reg::X5, MASK_XLEN(0x0));
    c.sltu(Reg::X5, Reg::X5, Reg::X5);
    c.sw(Reg::X5, Reg::X4, 16);

    // rs1==x8, rs2==x1, rd==x0, rs2_val == 4160749567, 
    // opcode: sltu ; op1:x8; op2:x1; dest:x0; op1val:0x0;  op2val:0xf7ffffff
    c.li(Reg::X8, MASK_XLEN(0x0));
    c.li(Reg::X1, MASK_XLEN(0xf7ffffff));
    c.sltu(Reg::X0, Reg::X8, Reg::X1);
    c.sw(Reg::X0, Reg::X4, 20);

    // rs1==x3, rs2==x9, rd==x13, rs2_val == 4227858431, 
    // opcode: sltu ; op1:x3; op2:x9; dest:x13; op1val:0x0;  op2val:0xfbffffff
    c.li(Reg::X3, MASK_XLEN(0x0));
    c.li(Reg::X9, MASK_XLEN(0xfbffffff));
    c.sltu(Reg::X13, Reg::X3, Reg::X9);
    c.sw(Reg::X13, Reg::X4, 24);

    c.li(Reg::X5, 48);
    // rs1==x9, rs2==x3, rd==x2, rs2_val == 4261412863, rs1_val == 4294967167
    // opcode: sltu ; op1:x9; op2:x3; dest:x2; op1val:0xffffff7f;  op2val:0xfdffffff
    c.li(Reg::X9, MASK_XLEN(0xffffff7f));
    c.li(Reg::X3, MASK_XLEN(0xfdffffff));
    c.sltu(Reg::X2, Reg::X9, Reg::X3);
    c.sw(Reg::X2, Reg::X5, 0);

    // rs1==x15, rs2==x13, rd==x6, rs2_val == 4278190079, rs1_val == 4294950911
    // opcode: sltu ; op1:x15; op2:x13; dest:x6; op1val:0xffffbfff;  op2val:0xfeffffff
    c.li(Reg::X15, MASK_XLEN(0xffffbfff));
    c.li(Reg::X13, MASK_XLEN(0xfeffffff));
    c.sltu(Reg::X6, Reg::X15, Reg::X13);
    c.sw(Reg::X6, Reg::X5, 4);

    // rs1==x13, rs2==x0, rd==x12, rs2_val == 4286578687, rs1_val == 4026531839
    // opcode: sltu ; op1:x13; op2:x0; dest:x12; op1val:0xefffffff;  op2val:0x0
    c.li(Reg::X13, MASK_XLEN(0xefffffff));
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X13, Reg::X0);
    c.sw(Reg::X12, Reg::X5, 8);

    // rs1==x1, rs2==x11, rd==x14, rs2_val == 4290772991, rs1_val == 4294901759
    // opcode: sltu ; op1:x1; op2:x11; dest:x14; op1val:0xfffeffff;  op2val:0xffbfffff
    c.li(Reg::X1, MASK_XLEN(0xfffeffff));
    c.li(Reg::X11, MASK_XLEN(0xffbfffff));
    c.sltu(Reg::X14, Reg::X1, Reg::X11);
    c.sw(Reg::X14, Reg::X5, 12);

    // rs1==x2, rs2==x8, rd==x4, rs2_val == 4292870143, rs1_val == 8
    // opcode: sltu ; op1:x2; op2:x8; dest:x4; op1val:0x8;  op2val:0xffdfffff
    c.li(Reg::X2, MASK_XLEN(0x8));
    c.li(Reg::X8, MASK_XLEN(0xffdfffff));
    c.sltu(Reg::X4, Reg::X2, Reg::X8);
    c.sw(Reg::X4, Reg::X5, 16);

    c.li(Reg::X2, 68);
    // rs1==x7, rs2==x4, rd==x8, rs2_val == 4293918719, rs1_val == 2863311530
    // opcode: sltu ; op1:x7; op2:x4; dest:x8; op1val:0xaaaaaaaa;  op2val:0xffefffff
    c.li(Reg::X7, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X4, MASK_XLEN(0xffefffff));
    c.sltu(Reg::X8, Reg::X7, Reg::X4);
    c.sw(Reg::X8, Reg::X2, 0);

    // rs1==x10, rs2==x12, rd==x15, rs2_val == 4294443007, 
    // opcode: sltu ; op1:x10; op2:x12; dest:x15; op1val:0xb504;  op2val:0xfff7ffff
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X12, MASK_XLEN(0xfff7ffff));
    c.sltu(Reg::X15, Reg::X10, Reg::X12);
    c.sw(Reg::X15, Reg::X2, 4);

    // rs1==x4, rs2==x15, rd==x9, rs2_val == 4294705151, rs1_val == 2048
    // opcode: sltu ; op1:x4; op2:x15; dest:x9; op1val:0x800;  op2val:0xfffbffff
    c.li(Reg::X4, MASK_XLEN(0x800));
    c.li(Reg::X15, MASK_XLEN(0xfffbffff));
    c.sltu(Reg::X9, Reg::X4, Reg::X15);
    c.sw(Reg::X9, Reg::X2, 8);

    // rs1==x0, rs2==x6, rd==x1, rs2_val == 4294836223, 
    // opcode: sltu ; op1:x0; op2:x6; dest:x1; op1val:0x0;  op2val:0xfffdffff
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.li(Reg::X6, MASK_XLEN(0xfffdffff));
    c.sltu(Reg::X1, Reg::X0, Reg::X6);
    c.sw(Reg::X1, Reg::X2, 12);

    // rs2_val == 4294901759, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xf;  op2val:0xfffeffff
    c.li(Reg::X10, MASK_XLEN(0xf));
    c.li(Reg::X11, MASK_XLEN(0xfffeffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 16);

    // rs2_val == 4294934527, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xf;  op2val:0xffff7fff
    c.li(Reg::X10, MASK_XLEN(0xf));
    c.li(Reg::X11, MASK_XLEN(0xffff7fff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 20);

    // rs2_val == 4294950911, rs1_val == 1431655765
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xffffbfff
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xffffbfff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 24);

    // rs2_val == 4294959103, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xffffdfff
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xffffdfff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 28);

    // rs2_val == 4294963199, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xffffefff
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xffffefff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 32);

    // rs2_val == 4294965247, rs1_val == 4293918719
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffefffff;  op2val:0xfffff7ff
    c.li(Reg::X10, MASK_XLEN(0xffefffff));
    c.li(Reg::X11, MASK_XLEN(0xfffff7ff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 36);

    // rs2_val == 4294966271, rs1_val == 524288
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x80000;  op2val:0xfffffbff
    c.li(Reg::X10, MASK_XLEN(0x80000));
    c.li(Reg::X11, MASK_XLEN(0xfffffbff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 40);

    // rs2_val == 4294966783, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xfffffdff
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xfffffdff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 44);

    // rs2_val == 4294967039, rs1_val == 4286578687
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xff7fffff;  op2val:0xfffffeff
    c.li(Reg::X10, MASK_XLEN(0xff7fffff));
    c.li(Reg::X11, MASK_XLEN(0xfffffeff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 48);

    // rs2_val == 4294967167, rs1_val == 4294966271
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffbff;  op2val:0xffffff7f
    c.li(Reg::X10, MASK_XLEN(0xfffffbff));
    c.li(Reg::X11, MASK_XLEN(0xffffff7f));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 52);

    // rs2_val == 4294967231, rs1_val == 64
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x40;  op2val:0xffffffbf
    c.li(Reg::X10, MASK_XLEN(0x40));
    c.li(Reg::X11, MASK_XLEN(0xffffffbf));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 56);

    // rs2_val == 4294967263, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x800;  op2val:0xffffffdf
    c.li(Reg::X10, MASK_XLEN(0x800));
    c.li(Reg::X11, MASK_XLEN(0xffffffdf));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 60);

    // rs2_val == 4294967279, rs1_val == 4294965247
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffff7ff;  op2val:0xffffffef
    c.li(Reg::X10, MASK_XLEN(0xfffff7ff));
    c.li(Reg::X11, MASK_XLEN(0xffffffef));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 64);

    // rs2_val == 4294967287, rs1_val == 4194304
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x400000;  op2val:0xfffffff7
    c.li(Reg::X10, MASK_XLEN(0x400000));
    c.li(Reg::X11, MASK_XLEN(0xfffffff7));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 68);

    // rs2_val == 4294967291, rs1_val == rs2_val and rs1_val > 0 and rs2_val > 0, rs1_val == 4294967291
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffffb;  op2val:0xfffffffb
    c.li(Reg::X10, MASK_XLEN(0xfffffffb));
    c.li(Reg::X11, MASK_XLEN(0xfffffffb));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 72);

    // rs2_val == 4294967293, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xfffffffd
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xfffffffd));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 76);

    // rs2_val == 4294967294, rs1_val == 536870912
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x20000000;  op2val:0xfffffffe
    c.li(Reg::X10, MASK_XLEN(0x20000000));
    c.li(Reg::X11, MASK_XLEN(0xfffffffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 80);

    // rs1_val == 2147483647, rs2_val == 536870912
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x7fffffff;  op2val:0x20000000
    c.li(Reg::X10, MASK_XLEN(0x7fffffff));
    c.li(Reg::X11, MASK_XLEN(0x20000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 84);

    // rs1_val == 3221225471, rs2_val == 0
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xbfffffff;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xbfffffff));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 88);

    // rs1_val == 3758096383, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xdfffffff;  op2val:0x12
    c.li(Reg::X10, MASK_XLEN(0xdfffffff));
    c.li(Reg::X11, MASK_XLEN(0x12));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 92);

    // rs1_val == 4160749567, rs2_val == 2863311530
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xf7ffffff;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xf7ffffff));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 96);

    // rs1_val == 4227858431, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfbffffff;  op2val:0x20000000
    c.li(Reg::X10, MASK_XLEN(0xfbffffff));
    c.li(Reg::X11, MASK_XLEN(0x20000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 100);

    // rs1_val == 4278190079, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfeffffff;  op2val:0xfffffdff
    c.li(Reg::X10, MASK_XLEN(0xfeffffff));
    c.li(Reg::X11, MASK_XLEN(0xfffffdff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 104);

    // rs1_val == 4290772991, rs2_val == 65536
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffbfffff;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xffbfffff));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 108);

    // rs1_val == 4292870143, rs2_val == 2097152
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffdfffff;  op2val:0x200000
    c.li(Reg::X10, MASK_XLEN(0xffdfffff));
    c.li(Reg::X11, MASK_XLEN(0x200000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 112);

    // rs1_val == 4294443007, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfff7ffff;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xfff7ffff));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 116);

    // rs1_val == 4294705151, rs2_val == 16
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffbffff;  op2val:0x10
    c.li(Reg::X10, MASK_XLEN(0xfffbffff));
    c.li(Reg::X11, MASK_XLEN(0x10));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 120);

    // rs1_val == 4294836223, rs2_val == 32768
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffdffff;  op2val:0x8000
    c.li(Reg::X10, MASK_XLEN(0xfffdffff));
    c.li(Reg::X11, MASK_XLEN(0x8000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 124);

    // rs1_val == 4294934527, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff7fff;  op2val:0xfffffffd
    c.li(Reg::X10, MASK_XLEN(0xffff7fff));
    c.li(Reg::X11, MASK_XLEN(0xfffffffd));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 128);

    // rs1_val == 4294959103, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffffdfff;  op2val:0x8000
    c.li(Reg::X10, MASK_XLEN(0xffffdfff));
    c.li(Reg::X11, MASK_XLEN(0x8000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 132);

    // rs1_val == 4294963199, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffffefff;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xffffefff));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 136);

    // rs1_val == 4294966783, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffdff;  op2val:0xfffffffb
    c.li(Reg::X10, MASK_XLEN(0xfffffdff));
    c.li(Reg::X11, MASK_XLEN(0xfffffffb));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 140);

    // rs1_val == 4294967039, rs2_val == 4194304
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffeff;  op2val:0x400000
    c.li(Reg::X10, MASK_XLEN(0xfffffeff));
    c.li(Reg::X11, MASK_XLEN(0x400000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 144);

    // rs1_val == 4294967231, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffffffbf;  op2val:0xa
    c.li(Reg::X10, MASK_XLEN(0xffffffbf));
    c.li(Reg::X11, MASK_XLEN(0xa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 148);

    // rs1_val == 4294967263, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffffffdf;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xffffffdf));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 152);

    // rs1_val == 4294967279, rs2_val == 2048
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffffffef;  op2val:0x800
    c.li(Reg::X10, MASK_XLEN(0xffffffef));
    c.li(Reg::X11, MASK_XLEN(0x800));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 156);

    // rs1_val == 4294967287, rs2_val == 33554432
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffff7;  op2val:0x2000000
    c.li(Reg::X10, MASK_XLEN(0xfffffff7));
    c.li(Reg::X11, MASK_XLEN(0x2000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 160);

    // rs1_val == 4294967293, rs2_val == 8388608
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffffd;  op2val:0x800000
    c.li(Reg::X10, MASK_XLEN(0xfffffffd));
    c.li(Reg::X11, MASK_XLEN(0x800000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 164);

    // rs1_val == 4294967294, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffffe;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xfffffffe));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 168);

    // rs2_val == 2147483648, rs1_val == 2
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x80000000
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x80000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 172);

    // rs2_val == 1073741824, rs1_val == 4
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x40000000
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x40000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 176);

    // rs2_val == 268435456, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xd;  op2val:0x10000000
    c.li(Reg::X10, MASK_XLEN(0xd));
    c.li(Reg::X11, MASK_XLEN(0x10000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 180);

    // rs2_val == 134217728, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x8000000
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x8000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 184);

    // rs2_val == 67108864, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x4000000
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x4000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 188);

    // rs2_val == 16777216, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffffe;  op2val:0x1000000
    c.li(Reg::X10, MASK_XLEN(0xfffffffe));
    c.li(Reg::X11, MASK_XLEN(0x1000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 192);

    // rs2_val == 1048576, rs1_val == 16
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10;  op2val:0x100000
    c.li(Reg::X10, MASK_XLEN(0x10));
    c.li(Reg::X11, MASK_XLEN(0x100000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 196);

    // rs2_val == 524288, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffdfffff;  op2val:0x80000
    c.li(Reg::X10, MASK_XLEN(0xffdfffff));
    c.li(Reg::X11, MASK_XLEN(0x80000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 200);

    // rs2_val == 262144, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x40000
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x40000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 204);

    // rs2_val == 131072, rs1_val == 2097152
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x200000;  op2val:0x20000
    c.li(Reg::X10, MASK_XLEN(0x200000));
    c.li(Reg::X11, MASK_XLEN(0x20000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 208);

    // rs2_val == 8192, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffbffff;  op2val:0x2000
    c.li(Reg::X10, MASK_XLEN(0xfffbffff));
    c.li(Reg::X11, MASK_XLEN(0x2000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 212);

    // rs2_val == 4096, rs1_val == 268435456
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000000;  op2val:0x1000
    c.li(Reg::X10, MASK_XLEN(0x10000000));
    c.li(Reg::X11, MASK_XLEN(0x1000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 216);

    // rs2_val == 1024, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x400
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x400));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 220);

    // rs2_val == 512, rs1_val == 1048576
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x100000;  op2val:0x200
    c.li(Reg::X10, MASK_XLEN(0x100000));
    c.li(Reg::X11, MASK_XLEN(0x200));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 224);

    // rs2_val == 256, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x100
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x100));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 228);

    // rs2_val == 128, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffffff7;  op2val:0x80
    c.li(Reg::X10, MASK_XLEN(0xfffffff7));
    c.li(Reg::X11, MASK_XLEN(0x80));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 232);

    // rs2_val == 64, rs1_val == 2147483648
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x80000000;  op2val:0x40
    c.li(Reg::X10, MASK_XLEN(0x80000000));
    c.li(Reg::X11, MASK_XLEN(0x40));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 236);

    // rs2_val == 32, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffffdfff;  op2val:0x20
    c.li(Reg::X10, MASK_XLEN(0xffffdfff));
    c.li(Reg::X11, MASK_XLEN(0x20));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 240);

    // rs2_val == 8, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000000;  op2val:0x8
    c.li(Reg::X10, MASK_XLEN(0x10000000));
    c.li(Reg::X11, MASK_XLEN(0x8));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 244);

    // rs2_val == 4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x8;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x8));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 248);

    // rs2_val == 2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xa;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xa));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 252);

    // rs2_val == 1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xa;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xa));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 256);

    // rs1_val == 1073741824, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x40000000;  op2val:0x2000000
    c.li(Reg::X10, MASK_XLEN(0x40000000));
    c.li(Reg::X11, MASK_XLEN(0x2000000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 260);

    // rs1_val == 134217728, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x8000000;  op2val:0x200
    c.li(Reg::X10, MASK_XLEN(0x8000000));
    c.li(Reg::X11, MASK_XLEN(0x200));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 264);

    // rs1_val == 67108864, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4000000;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x4000000));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 268);

    // rs1_val == 33554432, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2000000;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x2000000));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 272);

    // rs1_val == 16777216, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1000000;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x1000000));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 276);

    // rs1_val == 8388608, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x800000;  op2val:0x80000
    c.li(Reg::X10, MASK_XLEN(0x800000));
    c.li(Reg::X11, MASK_XLEN(0x80000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 280);

    // rs1_val == 65536, rs1_val==65536 and rs2_val==0
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 284);

    // rs1_val == 32768, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x8000;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x8000));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 288);

    // rs1_val == 16384, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4000;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x4000));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 292);

    // rs1_val == 8192, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2000;  op2val:0x20
    c.li(Reg::X10, MASK_XLEN(0x2000));
    c.li(Reg::X11, MASK_XLEN(0x20));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 296);

    // rs1_val == 4096, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1000;  op2val:0xfffffbff
    c.li(Reg::X10, MASK_XLEN(0x1000));
    c.li(Reg::X11, MASK_XLEN(0xfffffbff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 300);

    // rs1_val == 1024, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x400;  op2val:0xbfffffff
    c.li(Reg::X10, MASK_XLEN(0x400));
    c.li(Reg::X11, MASK_XLEN(0xbfffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 304);

    // rs1_val == 512, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x200;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x200));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 308);

    // rs1_val == 256, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x100;  op2val:0xf7ffffff
    c.li(Reg::X10, MASK_XLEN(0x100));
    c.li(Reg::X11, MASK_XLEN(0xf7ffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 312);

    // rs1_val == 128, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x80;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x80));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 316);

    // rs1_val == 32, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x20;  op2val:0xefffffff
    c.li(Reg::X10, MASK_XLEN(0x20));
    c.li(Reg::X11, MASK_XLEN(0xefffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 320);

    // rs1_val == 1, rs1_val==1 and rs2_val==46340
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 324);

    // rs1_val==65536 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 328);

    // rs1_val==65536 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 332);

    // rs1_val==65536 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 336);

    // rs1_val==65536 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 340);

    // rs1_val==65536 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 344);

    // rs1_val==65536 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 348);

    // rs1_val==65536 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 352);

    // rs1_val==65536 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 356);

    // rs1_val==65536 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 360);

    // rs1_val==65536 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 364);

    // rs1_val==65536 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 368);

    // rs1_val==65536 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 372);

    // rs1_val==65536 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 376);

    // rs1_val==65536 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 380);

    // rs1_val==65536 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 384);

    // rs1_val==65536 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 388);

    // rs1_val==65536 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 392);

    // rs1_val==65536 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 396);

    // rs1_val==65536 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 400);

    // rs1_val==65536 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 404);

    // rs1_val==65536 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 408);

    // rs1_val==65536 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 412);

    // rs1_val==65536 and rs2_val==1431655765, rs2_val == 1431655765
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 416);

    // rs1_val==65536 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 420);

    // rs1_val==1 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 424);

    // rs1_val==1 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 428);

    // rs1_val==1 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 432);

    // rs1_val==1 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 436);

    // rs1_val==1 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 440);

    // rs1_val==1 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 444);

    // rs1_val==1 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 448);

    // rs1_val==1 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 452);

    // rs1_val==1 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 456);

    // rs1_val==1 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 460);

    // rs1_val==1 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 464);

    // rs1_val==1 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 468);

    // rs1_val==1 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 472);

    // rs1_val==1 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 476);

    // rs1_val==1 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 480);

    // rs1_val==1 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 484);

    // rs1_val==1 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 488);

    // rs1_val==1 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 492);

    // rs1_val==1 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 496);

    // rs1_val==1 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 500);

    // rs1_val==1 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 504);

    // rs1_val==1 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 508);

    // rs1_val==1 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 512);

    // rs1_val==1 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x1));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 516);

    // rs1_val==46341 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 520);

    // rs1_val==46341 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 524);

    // rs1_val==46341 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 528);

    // rs1_val==46341 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 532);

    // rs1_val==46341 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 536);

    // rs1_val==46341 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 540);

    // rs1_val==46341 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 544);

    // rs1_val==46341 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 548);

    // rs1_val==46341 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 552);

    // rs1_val==46341 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 556);

    // rs1_val==46341 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 560);

    // rs1_val==46341 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 564);

    // rs1_val==46341 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 568);

    // rs1_val==46341 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 572);

    // rs1_val==46341 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 576);

    // rs1_val==46341 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 580);

    // rs1_val==46341 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 584);

    // rs1_val==46341 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 588);

    // rs1_val==46341 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 592);

    // rs1_val==46341 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 596);

    // rs1_val==46341 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 600);

    // rs1_val==46341 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 604);

    // rs1_val==46341 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 608);

    // rs1_val==46341 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 612);

    // rs1_val==46341 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 616);

    // rs1_val==1717986919 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 620);

    // rs1_val==1717986919 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 624);

    // rs1_val==1717986919 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 628);

    // rs1_val==1717986919 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 632);

    // rs1_val==1717986919 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 636);

    // rs1_val==1717986919 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 640);

    // rs1_val==1717986919 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 644);

    // rs1_val==1717986919 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 648);

    // rs1_val==1717986919 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 652);

    // rs1_val==1717986919 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 656);

    // rs1_val==1717986919 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 660);

    // rs1_val==1717986919 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 664);

    // rs1_val==1717986919 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 668);

    // rs1_val==1717986919 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 672);

    // rs1_val==1717986919 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 676);

    // rs1_val==1717986919 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 680);

    // rs1_val==1717986919 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 684);

    // rs1_val==1717986919 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 688);

    // rs1_val==1717986919 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 692);

    // rs1_val==1717986919 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 696);

    // rs1_val==1717986919 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 700);

    // rs1_val==1717986919 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 704);

    // rs1_val==1717986919 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 708);

    // rs1_val==1717986919 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 712);

    // rs1_val==1717986919 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 716);

    // rs1_val==858993460 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 720);

    // rs1_val==858993460 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 724);

    // rs1_val==858993460 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 728);

    // rs1_val==858993460 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 732);

    // rs1_val==858993460 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 736);

    // rs1_val==858993460 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 740);

    // rs1_val==858993460 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 744);

    // rs1_val==858993460 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 748);

    // rs1_val==858993460 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 752);

    // rs1_val==858993460 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 756);

    // rs1_val==858993460 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 760);

    // rs1_val==858993460 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 764);

    // rs1_val==858993460 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 768);

    // rs1_val==858993460 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 772);

    // rs1_val==858993460 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 776);

    // rs1_val==858993460 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 780);

    // rs1_val==858993460 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 784);

    // rs1_val==858993460 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 788);

    // rs1_val==858993460 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 792);

    // rs1_val==858993460 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 796);

    // rs1_val==858993460 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 800);

    // rs1_val==858993460 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 804);

    // rs1_val==858993460 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 808);

    // rs1_val==858993460 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 812);

    // rs1_val==858993460 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 816);

    // rs1_val==6 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 820);

    // rs1_val==6 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 824);

    // rs1_val==6 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 828);

    // rs1_val==6 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 832);

    // rs1_val==6 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 836);

    // rs1_val==6 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 840);

    // rs1_val==6 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 844);

    // rs1_val==6 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 848);

    // rs1_val==6 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 852);

    // rs1_val==6 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 856);

    // rs1_val==6 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 860);

    // rs1_val==6 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 864);

    // rs1_val==6 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 868);

    // rs1_val==6 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 872);

    // rs1_val==6 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 876);

    // rs1_val==6 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 880);

    // rs1_val==6 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 884);

    // rs1_val==6 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 888);

    // rs1_val==6 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 892);

    // rs1_val==6 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 896);

    // rs1_val==6 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 900);

    // rs1_val==6 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 904);

    // rs1_val==6 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 908);

    // rs1_val==6 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 912);

    // rs1_val==6 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 916);

    // rs1_val==2863311531 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 920);

    // rs1_val==2863311531 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 924);

    // rs1_val==2863311531 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 928);

    // rs1_val==2863311531 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 932);

    // rs1_val==2863311531 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 936);

    // rs1_val==2863311531 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 940);

    // rs1_val==2863311531 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 944);

    // rs1_val==2863311531 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 948);

    // rs1_val==2863311531 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 952);

    // rs1_val==2863311531 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 956);

    // rs1_val==2863311531 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 960);

    // rs1_val==2863311531 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 964);

    // rs1_val==2863311531 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 968);

    // rs1_val==2863311531 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 972);

    // rs1_val==2863311531 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 976);

    // rs1_val==2863311531 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 980);

    // rs1_val==2863311531 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 984);

    // rs1_val==2863311531 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 988);

    // rs1_val==2863311531 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 992);

    // rs1_val==2863311531 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 996);

    // rs1_val==2863311531 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1000);

    // rs1_val==2863311531 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1004);

    // rs1_val==2863311531 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1008);

    // rs1_val==2863311531 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1012);

    // rs1_val==2863311531 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaab;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1016);

    // rs1_val==1431655766 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1020);

    // rs1_val==1431655766 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1024);

    // rs1_val==1431655766 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1028);

    // rs1_val==1431655766 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1032);

    // rs1_val==1431655766 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1036);

    // rs1_val==1431655766 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1040);

    // rs1_val==1431655766 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1044);

    // rs1_val==1431655766 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1048);

    // rs1_val==1431655766 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1052);

    // rs1_val==1431655766 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1056);

    // rs1_val==1431655766 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1060);

    // rs1_val==1431655766 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1064);

    // rs1_val==1431655766 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1068);

    // rs1_val==1431655766 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1072);

    // rs1_val==1431655766 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1076);

    // rs1_val==1431655766 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1080);

    // rs1_val==1431655766 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1084);

    // rs1_val==1431655766 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1088);

    // rs1_val==1431655766 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1092);

    // rs1_val==1431655766 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1096);

    // rs1_val==1431655766 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1100);

    // rs1_val==1431655766 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1104);

    // rs1_val==1431655766 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1108);

    // rs1_val==1431655766 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1112);

    // rs1_val==1431655766 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1116);

    // rs1_val==4 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1120);

    // rs1_val==4 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1124);

    // rs1_val==4 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1128);

    // rs1_val==4 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1132);

    // rs1_val==4 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1136);

    // rs1_val==4 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1140);

    // rs1_val==4 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1144);

    // rs1_val==4 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1148);

    // rs1_val==4 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1152);

    // rs1_val==4 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1156);

    // rs1_val==4 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1160);

    // rs1_val==4 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1164);

    // rs1_val==4 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1168);

    // rs1_val==4 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1172);

    // rs1_val==4 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1176);

    // rs1_val==4 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1180);

    // rs1_val==4 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1184);

    // rs1_val==4 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1188);

    // rs1_val==4 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1192);

    // rs1_val==4 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1196);

    // rs1_val==4 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1200);

    // rs1_val==4 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1204);

    // rs1_val==4 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1208);

    // rs1_val==4 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1212);

    // rs1_val==4 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1216);

    // rs1_val==65534 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1220);

    // rs1_val==65534 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1224);

    // rs1_val==65534 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1228);

    // rs1_val==65534 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1232);

    // rs1_val==65534 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1236);

    // rs1_val==65534 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1240);

    // rs1_val==65534 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1244);

    // rs1_val==65534 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1248);

    // rs1_val==65534 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1252);

    // rs1_val==65534 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1256);

    // rs1_val==65534 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1260);

    // rs1_val==65534 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1264);

    // rs1_val==65534 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1268);

    // rs1_val==65534 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1272);

    // rs1_val==65534 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1276);

    // rs1_val==65534 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1280);

    // rs1_val==65534 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1284);

    // rs1_val==65534 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1288);

    // rs1_val==65534 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1292);

    // rs1_val==65534 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1296);

    // rs1_val==65534 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1300);

    // rs1_val==65534 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1304);

    // rs1_val==65534 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1308);

    // rs1_val==65534 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1312);

    // rs1_val==65534 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfffe;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xfffe));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1316);

    // rs1_val==0 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1320);

    // rs1_val==0 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1324);

    // rs1_val==0 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1328);

    // rs1_val==0 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1332);

    // rs1_val==0 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1336);

    // rs1_val==0 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1340);

    // rs1_val==0 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1344);

    // rs1_val==0 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1348);

    // rs1_val==0 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1352);

    // rs1_val==0 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1356);

    // rs1_val==0 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1360);

    // rs1_val==0 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1364);

    // rs1_val==0 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1368);

    // rs1_val==0 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1372);

    // rs1_val==0 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1376);

    // rs1_val==0 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1380);

    // rs1_val==0 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1384);

    // rs1_val==0 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1388);

    // rs1_val==0 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1392);

    // rs1_val==0 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1396);

    // rs1_val==0 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1400);

    // rs1_val==0 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1404);

    // rs1_val==0 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1408);

    // rs1_val==0 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1412);

    // rs1_val==0 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1416);

    // rs1_val==46339 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1420);

    // rs1_val==46339 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1424);

    // rs1_val==46339 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1428);

    // rs1_val==46339 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1432);

    // rs1_val==46339 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1436);

    // rs1_val==46339 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1440);

    // rs1_val==46339 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1444);

    // rs1_val==46339 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1448);

    // rs1_val==46339 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1452);

    // rs1_val==46339 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1456);

    // rs1_val==46339 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1460);

    // rs1_val==46339 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1464);

    // rs1_val==46339 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1468);

    // rs1_val==46339 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1472);

    // rs1_val==46339 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1476);

    // rs1_val==46339 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1480);

    // rs1_val==46339 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1484);

    // rs1_val==46339 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1488);

    // rs1_val==46339 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1492);

    // rs1_val==46339 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1496);

    // rs1_val==46339 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1500);

    // rs1_val==46339 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1504);

    // rs1_val==46339 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1508);

    // rs1_val==46339 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1512);

    // rs1_val==46339 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1516);

    // rs1_val==1717986917 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1520);

    // rs1_val==1717986917 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1524);

    // rs1_val==1717986917 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1528);

    // rs1_val==1717986917 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1532);

    // rs1_val==1717986917 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1536);

    // rs1_val==1717986917 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1540);

    // rs1_val==1717986917 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1544);

    // rs1_val==1717986917 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1548);

    // rs1_val==1717986917 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1552);

    // rs1_val==1717986917 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1556);

    // rs1_val==1717986917 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1560);

    // rs1_val==1717986917 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1564);

    // rs1_val==1717986917 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1568);

    // rs1_val==1717986917 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1572);

    // rs1_val==1717986917 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1576);

    // rs1_val==1717986917 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1580);

    // rs1_val==1717986917 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1584);

    // rs1_val==1717986917 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1588);

    // rs1_val==1717986917 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1592);

    // rs1_val==1717986917 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1596);

    // rs1_val==1717986917 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1600);

    // rs1_val==1717986917 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1604);

    // rs1_val==1717986917 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1608);

    // rs1_val==1717986917 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1612);

    // rs1_val==1717986917 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1616);

    // rs1_val==858993458 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1620);

    // rs1_val==858993458 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1624);

    // rs1_val==858993458 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1628);

    // rs1_val==858993458 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1632);

    // rs1_val==858993458 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1636);

    // rs1_val==858993458 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1640);

    // rs1_val==858993458 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1644);

    // rs1_val==858993458 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1648);

    // rs1_val==858993458 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1652);

    // rs1_val==858993458 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1656);

    // rs1_val==858993458 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1660);

    // rs1_val==858993458 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1664);

    // rs1_val==858993458 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1668);

    // rs1_val==858993458 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1672);

    // rs1_val==858993458 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1676);

    // rs1_val==858993458 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1680);

    // rs1_val==858993458 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1684);

    // rs1_val==858993458 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1688);

    // rs1_val==858993458 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1692);

    // rs1_val==858993458 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1696);

    // rs1_val==858993458 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1700);

    // rs1_val==858993458 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1704);

    // rs1_val==858993458 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1708);

    // rs1_val==858993458 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1712);

    // rs1_val==858993458 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1716);

    // rs1_val==2863311529 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1720);

    // rs1_val==2863311529 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1724);

    // rs1_val==2863311529 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1728);

    // rs1_val==2863311529 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1732);

    // rs1_val==2863311529 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1736);

    // rs1_val==2863311529 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1740);

    // rs1_val==2863311529 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1744);

    // rs1_val==2863311529 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1748);

    // rs1_val==2863311529 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1752);

    // rs1_val==2863311529 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1756);

    // rs1_val==2863311529 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1760);

    // rs1_val==2863311529 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1764);

    // rs1_val==2863311529 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1768);

    // rs1_val==2863311529 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1772);

    // rs1_val==2863311529 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1776);

    // rs1_val==2863311529 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1780);

    // rs1_val==2863311529 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1784);

    // rs1_val==2863311529 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1788);

    // rs1_val==2863311529 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1792);

    // rs1_val==2863311529 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1796);

    // rs1_val==2863311529 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1800);

    // rs1_val==2863311529 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1804);

    // rs1_val==1431655764 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1808);

    // rs1_val==1431655764 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1812);

    // rs1_val==1431655764 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1816);

    // rs1_val==1431655764 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1820);

    // rs1_val==1431655764 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1824);

    // rs1_val==1431655764 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1828);

    // rs1_val==1431655764 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1832);

    // rs1_val==1431655764 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1836);

    // rs1_val==1431655764 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1840);

    // rs1_val==1431655764 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1844);

    // rs1_val==1431655764 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1848);

    // rs1_val==1431655764 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1852);

    // rs1_val==1431655764 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1856);

    // rs1_val==1431655764 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1860);

    // rs1_val==1431655764 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1864);

    // rs1_val==1431655764 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1868);

    // rs1_val==1431655764 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1872);

    // rs1_val==2 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1876);

    // rs1_val==2 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1880);

    // rs1_val==2 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1884);

    // rs1_val==2 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1888);

    // rs1_val==2 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1892);

    // rs1_val==2 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1896);

    // rs1_val==2 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1900);

    // rs1_val==2 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1904);

    // rs1_val==2 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1908);

    // rs1_val==2 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1912);

    // rs1_val==2 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1916);

    // rs1_val==2 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1920);

    // rs1_val==2 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1924);

    // rs1_val==2 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1928);

    // rs1_val==2 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1932);

    // rs1_val==2 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1936);

    // rs1_val==2 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1940);

    // rs1_val==2 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1944);

    // rs1_val==2 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1948);

    // rs1_val==2 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1952);

    // rs1_val==2 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1956);

    // rs1_val==2 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1960);

    // rs1_val==2 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1964);

    // rs1_val==2 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1968);

    // rs1_val==2 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1972);

    // rs1_val==65535 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1976);

    // rs1_val==65535 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1980);

    // rs1_val==65535 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1984);

    // rs1_val==65535 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1988);

    // rs1_val==65535 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1992);

    // rs1_val==65535 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 1996);

    // rs1_val==65535 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 2000);

    // rs1_val==65535 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 2004);

    // rs1_val==65535 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 2008);

    // rs1_val==65535 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 2012);

    // rs1_val==65535 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 2016);

    // rs1_val==65535 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 2020);

    // rs1_val==65535 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 2024);

    // rs1_val==65535 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 2028);

    // rs1_val==65535 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 2032);

    // rs1_val==65535 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 2036);

    // rs1_val==65535 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 2040);

    // rs1_val==65535 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 2044);

    c.li(Reg::X2, 2116);
    // rs1_val==65535 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 0);

    // rs1_val==65535 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 4);

    // rs1_val==65535 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 8);

    // rs1_val==65535 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 12);

    // rs1_val==65535 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 16);

    // rs1_val==65535 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 20);

    // rs1_val==65535 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffff;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xffff));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 24);

    // rs1_val==46340 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 28);

    // rs1_val==46340 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 32);

    // rs1_val==46340 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 36);

    // rs1_val==46340 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 40);

    // rs1_val==46340 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 44);

    // rs1_val==46340 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 48);

    // rs1_val==46340 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 52);

    // rs1_val==46340 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 56);

    // rs1_val==46340 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 60);

    // rs1_val==46340 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 64);

    // rs1_val==46340 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 68);

    // rs1_val==46340 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 72);

    // rs1_val==46340 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 76);

    // rs1_val==46340 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 80);

    // rs1_val==46340 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 84);

    // rs1_val==46340 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 88);

    // rs1_val==46340 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 92);

    // rs1_val==46340 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 96);

    // rs1_val==46340 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 100);

    // rs1_val==46340 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 104);

    // rs1_val==46340 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 108);

    // rs1_val==46340 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 112);

    // rs1_val==46340 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 116);

    // rs1_val==46340 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 120);

    // rs1_val==46340 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 124);

    // rs1_val==1717986918 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 128);

    // rs1_val==1717986918 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 132);

    // rs1_val==1717986918 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 136);

    // rs1_val==1717986918 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 140);

    // rs1_val==1717986918 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 144);

    // rs1_val==1717986918 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 148);

    // rs1_val==1717986918 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 152);

    // rs1_val==1717986918 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 156);

    // rs1_val==1717986918 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 160);

    // rs1_val==1717986918 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 164);

    // rs1_val==1717986918 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 168);

    // rs1_val==1717986918 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 172);

    // rs1_val==1717986918 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 176);

    // rs1_val==1717986918 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 180);

    // rs1_val==1717986918 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 184);

    // rs1_val==1717986918 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 188);

    // rs1_val==1717986918 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 192);

    // rs1_val==1717986918 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 196);

    // rs1_val==1717986918 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 200);

    // rs1_val==1717986918 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 204);

    // rs1_val==1717986918 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 208);

    // rs1_val==1717986918 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 212);

    // rs1_val==1717986918 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 216);

    // rs1_val==1717986918 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 220);

    // rs1_val==1717986918 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 224);

    // rs1_val==858993459 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 228);

    // rs1_val==858993459 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 232);

    // rs1_val==858993459 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 236);

    // rs1_val==858993459 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 240);

    // rs1_val==858993459 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 244);

    // rs1_val==858993459 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 248);

    // rs1_val==858993459 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 252);

    // rs1_val==858993459 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 256);

    // rs1_val==858993459 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 260);

    // rs1_val==858993459 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 264);

    // rs1_val==858993459 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 268);

    // rs1_val==2863311529 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 272);

    // rs2_val == (2**(xlen)-1), 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xefffffff;  op2val:0xffffffff
    c.li(Reg::X10, MASK_XLEN(0xefffffff));
    c.li(Reg::X11, MASK_XLEN(0xffffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 276);

    // rs1_val==858993459 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 280);

    // rs1_val==858993459 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 284);

    // rs1_val==858993459 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 288);

    // rs1_val==858993459 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 292);

    // rs1_val==858993459 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 296);

    // rs1_val==858993459 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 300);

    // rs1_val==858993459 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 304);

    // rs1_val==858993459 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 308);

    // rs1_val==858993459 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 312);

    // rs1_val==858993459 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 316);

    // rs1_val==858993459 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 320);

    // rs1_val==858993459 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 324);

    // rs1_val==858993459 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 328);

    // rs1_val==858993459 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 332);

    // rs1_val==5 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 336);

    // rs1_val==5 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 340);

    // rs1_val==5 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 344);

    // rs1_val==5 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 348);

    // rs1_val==5 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 352);

    // rs1_val==5 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 356);

    // rs1_val==5 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 360);

    // rs1_val==5 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 364);

    // rs1_val==5 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 368);

    // rs1_val==5 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 372);

    // rs1_val==5 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 376);

    // rs1_val==5 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 380);

    // rs1_val==5 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 384);

    // rs1_val==5 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 388);

    // rs1_val==5 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 392);

    // rs1_val==5 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 396);

    // rs1_val==5 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 400);

    // rs1_val==5 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 404);

    // rs1_val==5 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 408);

    // rs1_val==5 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 412);

    // rs1_val==5 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 416);

    // rs1_val==5 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 420);

    // rs1_val==5 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 424);

    // rs1_val==5 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 428);

    // rs1_val==5 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 432);

    // rs1_val==2863311530 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 436);

    // rs1_val==2863311530 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 440);

    // rs1_val==2863311530 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 444);

    // rs1_val==2863311530 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 448);

    // rs1_val==2863311530 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 452);

    // rs1_val==2863311530 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 456);

    // rs1_val==2863311530 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 460);

    // rs1_val==2863311530 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 464);

    // rs1_val==2863311530 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 468);

    // rs1_val==2863311530 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 472);

    // rs1_val==2863311530 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 476);

    // rs1_val==2863311530 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 480);

    // rs1_val==2863311530 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 484);

    // rs1_val==2863311530 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 488);

    // rs1_val==2863311530 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 492);

    // rs1_val==2863311530 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 496);

    // rs1_val==2863311530 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 500);

    // rs1_val==2863311530 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 504);

    // rs1_val==2863311530 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 508);

    // rs1_val==2863311530 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 512);

    // rs1_val==2863311530 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 516);

    // rs1_val==2863311530 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 520);

    // rs1_val==2863311530 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 524);

    // rs1_val==2863311530 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 528);

    // rs1_val==2863311530 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaaa;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 532);

    // rs1_val==1431655765 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 536);

    // rs1_val==1431655765 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 540);

    // rs1_val==1431655765 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 544);

    // rs1_val==1431655765 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 548);

    // rs1_val==1431655765 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 552);

    // rs1_val==1431655765 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 556);

    // rs1_val==1431655765 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 560);

    // rs1_val==1431655765 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 564);

    // rs1_val==1431655765 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 568);

    // rs1_val==1431655765 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 572);

    // rs1_val==1431655765 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 576);

    // rs1_val==1431655765 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 580);

    // rs1_val==1431655765 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 584);

    // rs1_val==1431655765 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 588);

    // rs1_val==1431655765 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 592);

    // rs1_val==1431655765 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 596);

    // rs1_val==1431655765 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 600);

    // rs1_val==1431655765 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 604);

    // rs1_val==1431655765 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 608);

    // rs1_val==1431655765 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 612);

    // rs1_val==1431655765 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 616);

    // rs1_val==1431655765 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 620);

    // rs1_val==1431655765 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 624);

    // rs1_val==1431655765 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 628);

    // rs1_val==1431655765 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 632);

    // rs1_val==3 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 636);

    // rs1_val==3 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 640);

    // rs1_val==3 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 644);

    // rs1_val==3 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 648);

    // rs1_val==3 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 652);

    // rs1_val==3 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 656);

    // rs1_val==3 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 660);

    // rs1_val==3 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 664);

    // rs1_val==3 and rs2_val==4, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x4
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x4));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 668);

    // rs1_val==3 and rs2_val==65534, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xfffe
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xfffe));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 672);

    // rs1_val==3 and rs2_val==0, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x0
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x0));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 676);

    // rs1_val==3 and rs2_val==46339, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb503
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb503));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 680);

    // rs1_val==3 and rs2_val==1717986917, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666665
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666665));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 684);

    // rs1_val==3 and rs2_val==858993458, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333332
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333332));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 688);

    // rs1_val==3 and rs2_val==2863311529, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xaaaaaaa9
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaa9));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 692);

    // rs1_val==3 and rs2_val==1431655764, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555554
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555554));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 696);

    // rs1_val==3 and rs2_val==2, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x2
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x2));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 700);

    // rs1_val==3 and rs2_val==65535, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xffff
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 704);

    // rs1_val==3 and rs2_val==46340, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb504
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xb504));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 708);

    // rs1_val==3 and rs2_val==1717986918, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666666
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x66666666));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 712);

    // rs1_val==3 and rs2_val==858993459, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333333
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x33333333));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 716);

    // rs1_val==3 and rs2_val==5, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x5
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x5));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 720);

    // rs1_val==3 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 724);

    // rs1_val==3 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 728);

    // rs1_val==3 and rs2_val==3, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x3
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.li(Reg::X11, MASK_XLEN(0x3));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 732);

    // rs1_val==2863311529 and rs2_val==2863311530, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0xaaaaaaaa
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaaa));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 736);

    // rs1_val == (2**(xlen)-1), 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xffffffff;  op2val:0xfffffbff
    c.li(Reg::X10, MASK_XLEN(0xffffffff));
    c.li(Reg::X11, MASK_XLEN(0xfffffbff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 740);

    // rs1_val==2863311529 and rs2_val==1431655765, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xaaaaaaa9;  op2val:0x55555555
    c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
    c.li(Reg::X11, MASK_XLEN(0x55555555));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 744);

    // rs1_val==1431655764 and rs2_val==65536, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x10000
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x10000));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 748);

    // rs1_val==1431655764 and rs2_val==1, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x1
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x1));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 752);

    // rs1_val==1431655764 and rs2_val==46341, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb505
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xb505));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 756);

    // rs1_val==1431655764 and rs2_val==1717986919, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666667
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x66666667));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 760);

    // rs1_val==1431655764 and rs2_val==858993460, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333334
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x33333334));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 764);

    // rs1_val==1431655764 and rs2_val==6, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x6));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 768);

    // rs1_val==1431655764 and rs2_val==2863311531, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xaaaaaaab
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0xaaaaaaab));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 772);

    // rs1_val==1431655764 and rs2_val==1431655766, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.li(Reg::X11, MASK_XLEN(0x55555556));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 776);

    // rs2_val == 3758096383, rs1_val == 4261412863
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xfdffffff;  op2val:0xdfffffff
    c.li(Reg::X10, MASK_XLEN(0xfdffffff));
    c.li(Reg::X11, MASK_XLEN(0xdfffffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 780);

    // rs2_val == 4286578687, rs1_val == 4026531839
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0xefffffff;  op2val:0xff7fffff
    c.li(Reg::X10, MASK_XLEN(0xefffffff));
    c.li(Reg::X11, MASK_XLEN(0xff7fffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 784);

    // rs2_val == 4294836223, 
    // opcode: sltu ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xfffdffff
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.li(Reg::X11, MASK_XLEN(0xfffdffff));
    c.sltu(Reg::X12, Reg::X10, Reg::X11);
    c.sw(Reg::X12, Reg::X2, 788);


        
    
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