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
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559, };
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
    
    
    c.li(Reg::X12, 20);
    // rs1 != rd, rs1==x6, rd==x24, ea_align == 0 and (imm_val % 4) == 0, imm_val < 0
    // opcode:lw op1:x6; dest:x24; immval:-0x4; align:0
    c.li(Reg::X6, 4+(0*4)+(0)-(-0x4));
    c.lw(Reg::X24, Reg::X6, -0x4);
    c.sw(Reg::X24, Reg::X12, 0);

    // rs1 == rd, rs1==x4, rd==x4, ea_align == 0 and (imm_val % 4) == 1, 
    // opcode:lw op1:x4; dest:x4; immval:-0x3; align:0
    c.li(Reg::X4, 4+(0*4)+(0)-(-0x3));
    c.lw(Reg::X4, Reg::X4, -0x3);
    c.sw(Reg::X4, Reg::X12, 4);

    // rs1==x21, rd==x15, ea_align == 0 and (imm_val % 4) == 2, 
    // opcode:lw op1:x21; dest:x15; immval:-0xa; align:0
    c.li(Reg::X21, 4+(0*4)+(0)-(-0xa));
    c.lw(Reg::X15, Reg::X21, -0xa);
    c.sw(Reg::X15, Reg::X12, 8);

    // rs1==x26, rd==x8, ea_align == 0 and (imm_val % 4) == 3, imm_val > 0
    // opcode:lw op1:x26; dest:x8; immval:0x7; align:0
    c.li(Reg::X26, 4+(0*4)+(0)-(0x7));
    c.lw(Reg::X8, Reg::X26, 0x7);
    c.sw(Reg::X8, Reg::X12, 12);

    // rs1==x27, rd==x26, imm_val == 0, 
    // opcode:lw op1:x27; dest:x26; immval:0x0; align:0
    c.li(Reg::X27, 4+(0*4)+(0)-(0x0));
    c.lw(Reg::X26, Reg::X27, 0x0);
    c.sw(Reg::X26, Reg::X12, 16);

    // rs1==x20, rd==x18, 
    // opcode:lw op1:x20; dest:x18; immval:-0x800; align:0
    c.li(Reg::X20, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X18, Reg::X20, -0x800);
    c.sw(Reg::X18, Reg::X12, 20);

    // rs1==x30, rd==x9, 
    // opcode:lw op1:x30; dest:x9; immval:-0x800; align:0
    c.li(Reg::X30, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X9, Reg::X30, -0x800);
    c.sw(Reg::X9, Reg::X12, 24);

    // rs1==x18, rd==x11, 
    // opcode:lw op1:x18; dest:x11; immval:-0x800; align:0
    c.li(Reg::X18, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X11, Reg::X18, -0x800);
    c.sw(Reg::X11, Reg::X12, 28);

    // rs1==x13, rd==x6, 
    // opcode:lw op1:x13; dest:x6; immval:-0x800; align:0
    c.li(Reg::X13, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X6, Reg::X13, -0x800);
    c.sw(Reg::X6, Reg::X12, 32);

    // rs1==x14, rd==x10, 
    // opcode:lw op1:x14; dest:x10; immval:-0x800; align:0
    c.li(Reg::X14, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X10, Reg::X14, -0x800);
    c.sw(Reg::X10, Reg::X12, 36);

    // rs1==x9, rd==x3, 
    // opcode:lw op1:x9; dest:x3; immval:-0x800; align:0
    c.li(Reg::X9, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X3, Reg::X9, -0x800);
    c.sw(Reg::X3, Reg::X12, 40);

    // rs1==x2, rd==x31, 
    // opcode:lw op1:x2; dest:x31; immval:-0x800; align:0
    c.li(Reg::X2, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X31, Reg::X2, -0x800);
    c.sw(Reg::X31, Reg::X12, 44);

    // rs1==x23, rd==x0, 
    // opcode:lw op1:x23; dest:x0; immval:-0x800; align:0
    c.li(Reg::X23, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X0, Reg::X23, -0x800);
    c.sw(Reg::X0, Reg::X12, 48);

    // rs1==x3, rd==x19, 
    // opcode:lw op1:x3; dest:x19; immval:-0x800; align:0
    c.li(Reg::X3, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X19, Reg::X3, -0x800);
    c.sw(Reg::X19, Reg::X12, 52);

    // rs1==x7, rd==x13, 
    // opcode:lw op1:x7; dest:x13; immval:-0x800; align:0
    c.li(Reg::X7, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X13, Reg::X7, -0x800);
    c.sw(Reg::X13, Reg::X12, 56);

    // rs1==x1, rd==x7, 
    // opcode:lw op1:x1; dest:x7; immval:-0x800; align:0
    c.li(Reg::X1, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X7, Reg::X1, -0x800);
    c.sw(Reg::X7, Reg::X12, 60);

    // rs1==x19, rd==x14, 
    // opcode:lw op1:x19; dest:x14; immval:-0x800; align:0
    c.li(Reg::X19, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X14, Reg::X19, -0x800);
    c.sw(Reg::X14, Reg::X12, 64);

    // rs1==x8, rd==x29, 
    // opcode:lw op1:x8; dest:x29; immval:-0x800; align:0
    c.li(Reg::X8, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X29, Reg::X8, -0x800);
    c.sw(Reg::X29, Reg::X12, 68);

    // rs1==x24, rd==x5, 
    // opcode:lw op1:x24; dest:x5; immval:-0x800; align:0
    c.li(Reg::X24, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X5, Reg::X24, -0x800);
    c.sw(Reg::X5, Reg::X12, 72);

    // rs1==x31, rd==x16, 
    // opcode:lw op1:x31; dest:x16; immval:-0x800; align:0
    c.li(Reg::X31, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X16, Reg::X31, -0x800);
    c.sw(Reg::X16, Reg::X12, 76);

    // rs1==x10, rd==x25, 
    // opcode:lw op1:x10; dest:x25; immval:-0x800; align:0
    c.li(Reg::X10, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X25, Reg::X10, -0x800);
    c.sw(Reg::X25, Reg::X12, 80);

    // rs1==x5, rd==x1, 
    // opcode:lw op1:x5; dest:x1; immval:-0x800; align:0
    c.li(Reg::X5, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X1, Reg::X5, -0x800);
    c.sw(Reg::X1, Reg::X12, 84);

    // rs1==x15, rd==x22, 
    // opcode:lw op1:x15; dest:x22; immval:-0x800; align:0
    c.li(Reg::X15, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X22, Reg::X15, -0x800);
    c.sw(Reg::X22, Reg::X12, 88);

    c.li(Reg::X1, 112);
    // rs1==x17, rd==x27, 
    // opcode:lw op1:x17; dest:x27; immval:-0x800; align:0
    c.li(Reg::X17, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X27, Reg::X17, -0x800);
    c.sw(Reg::X27, Reg::X1, 0);

    // rs1==x12, rd==x20, 
    // opcode:lw op1:x12; dest:x20; immval:-0x800; align:0
    c.li(Reg::X12, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X20, Reg::X12, -0x800);
    c.sw(Reg::X20, Reg::X1, 4);

    // rs1==x28, rd==x30, 
    // opcode:lw op1:x28; dest:x30; immval:-0x800; align:0
    c.li(Reg::X28, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X30, Reg::X28, -0x800);
    c.sw(Reg::X30, Reg::X1, 8);

    // rs1==x25, rd==x28, 
    // opcode:lw op1:x25; dest:x28; immval:-0x800; align:0
    c.li(Reg::X25, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X28, Reg::X25, -0x800);
    c.sw(Reg::X28, Reg::X1, 12);

    // rs1==x22, rd==x12, 
    // opcode:lw op1:x22; dest:x12; immval:-0x800; align:0
    c.li(Reg::X22, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X12, Reg::X22, -0x800);
    c.sw(Reg::X12, Reg::X1, 16);

    // rs1==x16, rd==x2, 
    // opcode:lw op1:x16; dest:x2; immval:-0x800; align:0
    c.li(Reg::X16, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X2, Reg::X16, -0x800);
    c.sw(Reg::X2, Reg::X1, 20);

    // rs1==x29, rd==x21, 
    // opcode:lw op1:x29; dest:x21; immval:-0x800; align:0
    c.li(Reg::X29, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X21, Reg::X29, -0x800);
    c.sw(Reg::X21, Reg::X1, 24);

    // rs1==x11, rd==x17, 
    // opcode:lw op1:x11; dest:x17; immval:-0x800; align:0
    c.li(Reg::X11, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X17, Reg::X11, -0x800);
    c.sw(Reg::X17, Reg::X1, 28);

    // rd==x23, 
    // opcode:lw op1:x12; dest:x23; immval:-0x800; align:0
    c.li(Reg::X12, 4+(0*4)+(0)-(-0x800));
    c.lw(Reg::X23, Reg::X12, -0x800);
    c.sw(Reg::X23, Reg::X1, 32);


        
    
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