#include "lw-align-01.h"

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
3735928559, 3735928559, };
}

//----------------------------------------------------------------------------
// ArchTest::lw_align_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &ArchTest::lw_align_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
std::vector<uint32_t> ArchTest::lw_align_01::generateCode(bool simulate)
{
    CodeGenerator c;
    
    // Ready flag is always stored in first word, zero it
    c.sw(Reg::X0, Reg::X0);
    
    
    c.li(Reg::X2, 20);
    // rs1 != rd, rs1==x11, rd==x1, ea_align == 0 and (imm_val % 4) == 0, imm_val < 0
    // opcode:lw op1:x11; dest:x1; immval:-0x400; align:0
    c.li(Reg::X11, 4+(0*4)+(0)-(-0x400));
    c.lw(Reg::X1, Reg::X11, -0x400);
    c.sw(Reg::X1, Reg::X2, 0);

    // rs1 == rd, rs1==x14, rd==x14, ea_align == 0 and (imm_val % 4) == 1, 
    // opcode:lw op1:x14; dest:x14; immval:-0x3; align:0
    c.li(Reg::X14, 4+(0*4)+(0)-(-0x3));
    c.lw(Reg::X14, Reg::X14, -0x3);
    c.sw(Reg::X14, Reg::X2, 4);

    // rs1==x4, rd==x8, ea_align == 0 and (imm_val % 4) == 2, 
    // opcode:lw op1:x4; dest:x8; immval:-0x556; align:0
    c.li(Reg::X4, 4+(0*4)+(0)-(-0x556));
    c.lw(Reg::X8, Reg::X4, -0x556);
    c.sw(Reg::X8, Reg::X2, 8);

    // rs1==x1, rd==x10, ea_align == 0 and (imm_val % 4) == 3, 
    // opcode:lw op1:x1; dest:x10; immval:-0x5; align:0
    c.li(Reg::X1, 4+(0*4)+(0)-(-0x5));
    c.lw(Reg::X10, Reg::X1, -0x5);
    c.sw(Reg::X10, Reg::X2, 12);

    // rs1==x12, rd==x15, imm_val == 0, 
    // opcode:lw op1:x12; dest:x15; immval:0x0; align:0
    c.li(Reg::X12, 4+(0*4)+(0)-(0x0));
    c.lw(Reg::X15, Reg::X12, 0x0);
    c.sw(Reg::X15, Reg::X2, 16);

    // rs1==x7, rd==x13, imm_val > 0, 
    // opcode:lw op1:x7; dest:x13; immval:0x20; align:0
    c.li(Reg::X7, 4+(0*4)+(0)-(0x20));
    c.lw(Reg::X13, Reg::X7, 0x20);
    c.sw(Reg::X13, Reg::X2, 20);

    // rs1==x5, rd==x3, 
    // opcode:lw op1:x5; dest:x3; immval:-0x800; align:0
    c.li(Reg::X5, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X3, Reg::X5, -0x800);
    c.sw(Reg::X3, Reg::X2, 24);

    c.li(Reg::X1, 48);
    // rs1==x8, rd==x0, 
    // opcode:lw op1:x8; dest:x0; immval:-0x800; align:0
    c.li(Reg::X8, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X0, Reg::X8, -0x800);
    c.sw(Reg::X0, Reg::X1, 0);

    // rs1==x3, rd==x12, 
    // opcode:lw op1:x3; dest:x12; immval:-0x800; align:0
    c.li(Reg::X3, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X12, Reg::X3, -0x800);
    c.sw(Reg::X12, Reg::X1, 4);

    // rs1==x13, rd==x4, 
    // opcode:lw op1:x13; dest:x4; immval:-0x800; align:0
    c.li(Reg::X13, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X4, Reg::X13, -0x800);
    c.sw(Reg::X4, Reg::X1, 8);

    // rs1==x15, rd==x7, 
    // opcode:lw op1:x15; dest:x7; immval:-0x800; align:0
    c.li(Reg::X15, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X7, Reg::X15, -0x800);
    c.sw(Reg::X7, Reg::X1, 12);

    // rs1==x9, rd==x5, 
    // opcode:lw op1:x9; dest:x5; immval:-0x800; align:0
    c.li(Reg::X9, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X5, Reg::X9, -0x800);
    c.sw(Reg::X5, Reg::X1, 16);

    // rs1==x6, rd==x9, 
    // opcode:lw op1:x6; dest:x9; immval:-0x800; align:0
    c.li(Reg::X6, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X9, Reg::X6, -0x800);
    c.sw(Reg::X9, Reg::X1, 20);

    // rs1==x10, rd==x2, 
    // opcode:lw op1:x10; dest:x2; immval:-0x800; align:0
    c.li(Reg::X10, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X2, Reg::X10, -0x800);
    c.sw(Reg::X2, Reg::X1, 24);

    c.li(Reg::X1, 76);
    // rs1==x2, rd==x6, 
    // opcode:lw op1:x2; dest:x6; immval:-0x800; align:0
    c.li(Reg::X2, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X6, Reg::X2, -0x800);
    c.sw(Reg::X6, Reg::X1, 0);

    // rd==x11, 
    // opcode:lw op1:x5; dest:x11; immval:-0x800; align:0
    c.li(Reg::X5, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X11, Reg::X5, -0x800);
    c.sw(Reg::X11, Reg::X1, 4);


        
    
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