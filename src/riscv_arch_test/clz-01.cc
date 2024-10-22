#include "clz-01.h"

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

0, 3133065982, 2884415467, 3200974522, 3970952107, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559,
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
// ArchTest::clz_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &ArchTest::clz_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
std::vector<uint32_t> ArchTest::clz_01::generateCode(bool simulate)
{
    CodeGenerator c;
    
    // Ready flag is always stored in first word, zero it
    c.sw(Reg::X0, Reg::X0);
    
    
    c.li(Reg::X1, 20);
    // rs1 == rd, rs1==x31, rd==x31, 
    // opcode: clz ; op1:x31; dest:x31; op1val:0x55555555;
    c.li(Reg::X31, MASK_XLEN(0x55555555));
    c.clz(Reg::X31, Reg::X31);
    c.sw(Reg::X31, Reg::X1, 0);

    // rs1 != rd, rs1==x29, rd==x30, 
    // opcode: clz ; op1:x29; dest:x30; op1val:0x7fffffff;
    c.li(Reg::X29, MASK_XLEN(0x7fffffff));
    c.clz(Reg::X30, Reg::X29);
    c.sw(Reg::X30, Reg::X1, 4);

    // rs1==x30, rd==x29, 
    // opcode: clz ; op1:x30; dest:x29; op1val:0xbfffffff;
    c.li(Reg::X30, MASK_XLEN(0xbfffffff));
    c.clz(Reg::X29, Reg::X30);
    c.sw(Reg::X29, Reg::X1, 8);

    // rs1==x27, rd==x28, 
    // opcode: clz ; op1:x27; dest:x28; op1val:0xdfffffff;
    c.li(Reg::X27, MASK_XLEN(0xdfffffff));
    c.clz(Reg::X28, Reg::X27);
    c.sw(Reg::X28, Reg::X1, 12);

    // rs1==x28, rd==x27, 
    // opcode: clz ; op1:x28; dest:x27; op1val:0xefffffff;
    c.li(Reg::X28, MASK_XLEN(0xefffffff));
    c.clz(Reg::X27, Reg::X28);
    c.sw(Reg::X27, Reg::X1, 16);

    // rs1==x25, rd==x26, 
    // opcode: clz ; op1:x25; dest:x26; op1val:0xf7ffffff;
    c.li(Reg::X25, MASK_XLEN(0xf7ffffff));
    c.clz(Reg::X26, Reg::X25);
    c.sw(Reg::X26, Reg::X1, 20);

    // rs1==x26, rd==x25, 
    // opcode: clz ; op1:x26; dest:x25; op1val:0xfbffffff;
    c.li(Reg::X26, MASK_XLEN(0xfbffffff));
    c.clz(Reg::X25, Reg::X26);
    c.sw(Reg::X25, Reg::X1, 24);

    // rs1==x23, rd==x24, 
    // opcode: clz ; op1:x23; dest:x24; op1val:0xfdffffff;
    c.li(Reg::X23, MASK_XLEN(0xfdffffff));
    c.clz(Reg::X24, Reg::X23);
    c.sw(Reg::X24, Reg::X1, 28);

    // rs1==x24, rd==x23, 
    // opcode: clz ; op1:x24; dest:x23; op1val:0xfeffffff;
    c.li(Reg::X24, MASK_XLEN(0xfeffffff));
    c.clz(Reg::X23, Reg::X24);
    c.sw(Reg::X23, Reg::X1, 32);

    // rs1==x21, rd==x22, 
    // opcode: clz ; op1:x21; dest:x22; op1val:0xff7fffff;
    c.li(Reg::X21, MASK_XLEN(0xff7fffff));
    c.clz(Reg::X22, Reg::X21);
    c.sw(Reg::X22, Reg::X1, 36);

    // rs1==x22, rd==x21, 
    // opcode: clz ; op1:x22; dest:x21; op1val:0xffbfffff;
    c.li(Reg::X22, MASK_XLEN(0xffbfffff));
    c.clz(Reg::X21, Reg::X22);
    c.sw(Reg::X21, Reg::X1, 40);

    // rs1==x19, rd==x20, 
    // opcode: clz ; op1:x19; dest:x20; op1val:0xffdfffff;
    c.li(Reg::X19, MASK_XLEN(0xffdfffff));
    c.clz(Reg::X20, Reg::X19);
    c.sw(Reg::X20, Reg::X1, 44);

    // rs1==x20, rd==x19, 
    // opcode: clz ; op1:x20; dest:x19; op1val:0xffefffff;
    c.li(Reg::X20, MASK_XLEN(0xffefffff));
    c.clz(Reg::X19, Reg::X20);
    c.sw(Reg::X19, Reg::X1, 48);

    // rs1==x17, rd==x18, 
    // opcode: clz ; op1:x17; dest:x18; op1val:0xfff7ffff;
    c.li(Reg::X17, MASK_XLEN(0xfff7ffff));
    c.clz(Reg::X18, Reg::X17);
    c.sw(Reg::X18, Reg::X1, 52);

    // rs1==x18, rd==x17, 
    // opcode: clz ; op1:x18; dest:x17; op1val:0xfffbffff;
    c.li(Reg::X18, MASK_XLEN(0xfffbffff));
    c.clz(Reg::X17, Reg::X18);
    c.sw(Reg::X17, Reg::X1, 56);

    // rs1==x15, rd==x16, 
    // opcode: clz ; op1:x15; dest:x16; op1val:0xfffdffff;
    c.li(Reg::X15, MASK_XLEN(0xfffdffff));
    c.clz(Reg::X16, Reg::X15);
    c.sw(Reg::X16, Reg::X1, 60);

    // rs1==x16, rd==x15, 
    // opcode: clz ; op1:x16; dest:x15; op1val:0xfffeffff;
    c.li(Reg::X16, MASK_XLEN(0xfffeffff));
    c.clz(Reg::X15, Reg::X16);
    c.sw(Reg::X15, Reg::X1, 64);

    // rs1==x13, rd==x14, 
    // opcode: clz ; op1:x13; dest:x14; op1val:0xffff7fff;
    c.li(Reg::X13, MASK_XLEN(0xffff7fff));
    c.clz(Reg::X14, Reg::X13);
    c.sw(Reg::X14, Reg::X1, 68);

    // rs1==x14, rd==x13, 
    // opcode: clz ; op1:x14; dest:x13; op1val:0xffffbfff;
    c.li(Reg::X14, MASK_XLEN(0xffffbfff));
    c.clz(Reg::X13, Reg::X14);
    c.sw(Reg::X13, Reg::X1, 72);

    // rs1==x11, rd==x12, 
    // opcode: clz ; op1:x11; dest:x12; op1val:0xffffdfff;
    c.li(Reg::X11, MASK_XLEN(0xffffdfff));
    c.clz(Reg::X12, Reg::X11);
    c.sw(Reg::X12, Reg::X1, 76);

    // rs1==x12, rd==x11, 
    // opcode: clz ; op1:x12; dest:x11; op1val:0xffffefff;
    c.li(Reg::X12, MASK_XLEN(0xffffefff));
    c.clz(Reg::X11, Reg::X12);
    c.sw(Reg::X11, Reg::X1, 80);

    // rs1==x9, rd==x10, 
    // opcode: clz ; op1:x9; dest:x10; op1val:0xfffff7ff;
    c.li(Reg::X9, MASK_XLEN(0xfffff7ff));
    c.clz(Reg::X10, Reg::X9);
    c.sw(Reg::X10, Reg::X1, 84);

    // rs1==x10, rd==x9, 
    // opcode: clz ; op1:x10; dest:x9; op1val:0xfffffbff;
    c.li(Reg::X10, MASK_XLEN(0xfffffbff));
    c.clz(Reg::X9, Reg::X10);
    c.sw(Reg::X9, Reg::X1, 88);

    // rs1==x7, rd==x8, 
    // opcode: clz ; op1:x7; dest:x8; op1val:0xfffffdff;
    c.li(Reg::X7, MASK_XLEN(0xfffffdff));
    c.clz(Reg::X8, Reg::X7);
    c.sw(Reg::X8, Reg::X1, 92);

    // rs1==x8, rd==x7, 
    // opcode: clz ; op1:x8; dest:x7; op1val:0xfffffeff;
    c.li(Reg::X8, MASK_XLEN(0xfffffeff));
    c.clz(Reg::X7, Reg::X8);
    c.sw(Reg::X7, Reg::X1, 96);

    // rs1==x5, rd==x6, 
    // opcode: clz ; op1:x5; dest:x6; op1val:0xffffff7f;
    c.li(Reg::X5, MASK_XLEN(0xffffff7f));
    c.clz(Reg::X6, Reg::X5);
    c.sw(Reg::X6, Reg::X1, 100);

    // rs1==x6, rd==x5, 
    // opcode: clz ; op1:x6; dest:x5; op1val:0xffffffbf;
    c.li(Reg::X6, MASK_XLEN(0xffffffbf));
    c.clz(Reg::X5, Reg::X6);
    c.sw(Reg::X5, Reg::X1, 104);

    // rs1==x3, rd==x4, 
    // opcode: clz ; op1:x3; dest:x4; op1val:0xffffffdf;
    c.li(Reg::X3, MASK_XLEN(0xffffffdf));
    c.clz(Reg::X4, Reg::X3);
    c.sw(Reg::X4, Reg::X1, 108);

    c.li(Reg::X5, 132);
    // rs1==x4, rd==x3, 
    // opcode: clz ; op1:x4; dest:x3; op1val:0xffffffef;
    c.li(Reg::X4, MASK_XLEN(0xffffffef));
    c.clz(Reg::X3, Reg::X4);
    c.sw(Reg::X3, Reg::X5, 0);

    // rs1==x1, rd==x2, 
    // opcode: clz ; op1:x1; dest:x2; op1val:0xfffffff7;
    c.li(Reg::X1, MASK_XLEN(0xfffffff7));
    c.clz(Reg::X2, Reg::X1);
    c.sw(Reg::X2, Reg::X5, 4);

    // rs1==x2, rd==x1, 
    // opcode: clz ; op1:x2; dest:x1; op1val:0xfffffffb;
    c.li(Reg::X2, MASK_XLEN(0xfffffffb));
    c.clz(Reg::X1, Reg::X2);
    c.sw(Reg::X1, Reg::X5, 8);

    // rs1==x0, 
    // opcode: clz ; op1:x0; dest:x31; op1val:0x0;
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.clz(Reg::X31, Reg::X0);
    c.sw(Reg::X31, Reg::X5, 12);

    // rd==x0, 
    // opcode: clz ; op1:x31; dest:x0; op1val:0xfffffffe;
    c.li(Reg::X31, MASK_XLEN(0xfffffffe));
    c.clz(Reg::X0, Reg::X31);
    c.sw(Reg::X0, Reg::X5, 16);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x80000000;
    c.li(Reg::X30, MASK_XLEN(0x80000000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 20);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x40000000;
    c.li(Reg::X30, MASK_XLEN(0x40000000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 24);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x1;
    c.li(Reg::X30, MASK_XLEN(0x1));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 28);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0xaaaaaaaa;
    c.li(Reg::X30, MASK_XLEN(0xaaaaaaaa));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 32);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x20000000;
    c.li(Reg::X30, MASK_XLEN(0x20000000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 36);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x10000000;
    c.li(Reg::X30, MASK_XLEN(0x10000000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 40);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x8000000;
    c.li(Reg::X30, MASK_XLEN(0x8000000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 44);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x4000000;
    c.li(Reg::X30, MASK_XLEN(0x4000000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 48);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x2000000;
    c.li(Reg::X30, MASK_XLEN(0x2000000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 52);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x1000000;
    c.li(Reg::X30, MASK_XLEN(0x1000000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 56);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x800000;
    c.li(Reg::X30, MASK_XLEN(0x800000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 60);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x400000;
    c.li(Reg::X30, MASK_XLEN(0x400000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 64);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x200000;
    c.li(Reg::X30, MASK_XLEN(0x200000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 68);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x100000;
    c.li(Reg::X30, MASK_XLEN(0x100000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 72);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x80000;
    c.li(Reg::X30, MASK_XLEN(0x80000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 76);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x40000;
    c.li(Reg::X30, MASK_XLEN(0x40000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 80);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x20000;
    c.li(Reg::X30, MASK_XLEN(0x20000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 84);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x10000;
    c.li(Reg::X30, MASK_XLEN(0x10000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 88);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x8000;
    c.li(Reg::X30, MASK_XLEN(0x8000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 92);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x4000;
    c.li(Reg::X30, MASK_XLEN(0x4000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 96);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x2000;
    c.li(Reg::X30, MASK_XLEN(0x2000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 100);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x1000;
    c.li(Reg::X30, MASK_XLEN(0x1000));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 104);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x800;
    c.li(Reg::X30, MASK_XLEN(0x800));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 108);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x400;
    c.li(Reg::X30, MASK_XLEN(0x400));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 112);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x200;
    c.li(Reg::X30, MASK_XLEN(0x200));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 116);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x100;
    c.li(Reg::X30, MASK_XLEN(0x100));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 120);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x80;
    c.li(Reg::X30, MASK_XLEN(0x80));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 124);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x40;
    c.li(Reg::X30, MASK_XLEN(0x40));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 128);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x20;
    c.li(Reg::X30, MASK_XLEN(0x20));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 132);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x10;
    c.li(Reg::X30, MASK_XLEN(0x10));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 136);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x8;
    c.li(Reg::X30, MASK_XLEN(0x8));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 140);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x4;
    c.li(Reg::X30, MASK_XLEN(0x4));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 144);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0x2;
    c.li(Reg::X30, MASK_XLEN(0x2));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 148);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0xfffffffd;
    c.li(Reg::X30, MASK_XLEN(0xfffffffd));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 152);

    // 
    // opcode: clz ; op1:x30; dest:x31; op1val:0xfffffffe;
    c.li(Reg::X30, MASK_XLEN(0xfffffffe));
    c.clz(Reg::X31, Reg::X30);
    c.sw(Reg::X31, Reg::X5, 156);


        
    
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