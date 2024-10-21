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
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, };
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
    
    
    c.li(Reg::X7, 20);
    // rs1 == rd, rs1==x0, rd==x0, rs1_val == 1431655765, 
    // opcode: clz ; op1:x0; dest:x0; op1val:0x0;
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.clz(Reg::X0, Reg::X0);
    c.sw(Reg::X0, Reg::X7, 0);

    // rs1 != rd, rs1==x4, rd==x11, rs1_val == 2863311530, 
    // opcode: clz ; op1:x4; dest:x11; op1val:0xaaaaaaaa;
    c.li(Reg::X4, MASK_XLEN(0xaaaaaaaa));
    c.clz(Reg::X11, Reg::X4);
    c.sw(Reg::X11, Reg::X7, 4);

    // rs1==x8, rd==x5, rs1_val == 1, 
    // opcode: clz ; op1:x8; dest:x5; op1val:0x1;
    c.li(Reg::X8, MASK_XLEN(0x1));
    c.clz(Reg::X5, Reg::X8);
    c.sw(Reg::X5, Reg::X7, 8);

    // rs1==x5, rd==x3, rs1_val == 2, 
    // opcode: clz ; op1:x5; dest:x3; op1val:0x2;
    c.li(Reg::X5, MASK_XLEN(0x2));
    c.clz(Reg::X3, Reg::X5);
    c.sw(Reg::X3, Reg::X7, 12);

    // rs1==x6, rd==x12, rs1_val == 4, 
    // opcode: clz ; op1:x6; dest:x12; op1val:0x4;
    c.li(Reg::X6, MASK_XLEN(0x4));
    c.clz(Reg::X12, Reg::X6);
    c.sw(Reg::X12, Reg::X7, 16);

    // rs1==x1, rd==x8, rs1_val == 8, 
    // opcode: clz ; op1:x1; dest:x8; op1val:0x8;
    c.li(Reg::X1, MASK_XLEN(0x8));
    c.clz(Reg::X8, Reg::X1);
    c.sw(Reg::X8, Reg::X7, 20);

    // rs1==x2, rd==x4, rs1_val == 4294967294, 
    // opcode: clz ; op1:x2; dest:x4; op1val:0xfffffffe;
    c.li(Reg::X2, MASK_XLEN(0xfffffffe));
    c.clz(Reg::X4, Reg::X2);
    c.sw(Reg::X4, Reg::X7, 24);

    // rs1==x10, rd==x15, rs1_val == 4294967293, 
    // opcode: clz ; op1:x10; dest:x15; op1val:0xfffffffd;
    c.li(Reg::X10, MASK_XLEN(0xfffffffd));
    c.clz(Reg::X15, Reg::X10);
    c.sw(Reg::X15, Reg::X7, 28);

    c.li(Reg::X4, 52);
    // rs1==x3, rd==x14, rs1_val == 4294967291, 
    // opcode: clz ; op1:x3; dest:x14; op1val:0xfffffffb;
    c.li(Reg::X3, MASK_XLEN(0xfffffffb));
    c.clz(Reg::X14, Reg::X3);
    c.sw(Reg::X14, Reg::X4, 0);

    // rs1==x15, rd==x13, rs1_val == 4294967287, 
    // opcode: clz ; op1:x15; dest:x13; op1val:0xfffffff7;
    c.li(Reg::X15, MASK_XLEN(0xfffffff7));
    c.clz(Reg::X13, Reg::X15);
    c.sw(Reg::X13, Reg::X4, 4);

    // rs1==x7, rd==x1, rs1_val == 4294967279, 
    // opcode: clz ; op1:x7; dest:x1; op1val:0xffffffef;
    c.li(Reg::X7, MASK_XLEN(0xffffffef));
    c.clz(Reg::X1, Reg::X7);
    c.sw(Reg::X1, Reg::X4, 8);

    // rs1==x13, rd==x9, rs1_val == 4294967263, 
    // opcode: clz ; op1:x13; dest:x9; op1val:0xffffffdf;
    c.li(Reg::X13, MASK_XLEN(0xffffffdf));
    c.clz(Reg::X9, Reg::X13);
    c.sw(Reg::X9, Reg::X4, 12);

    // rs1==x14, rd==x6, rs1_val == 4294967231, 
    // opcode: clz ; op1:x14; dest:x6; op1val:0xffffffbf;
    c.li(Reg::X14, MASK_XLEN(0xffffffbf));
    c.clz(Reg::X6, Reg::X14);
    c.sw(Reg::X6, Reg::X4, 16);

    // rs1==x9, rd==x7, rs1_val == 4294967167, 
    // opcode: clz ; op1:x9; dest:x7; op1val:0xffffff7f;
    c.li(Reg::X9, MASK_XLEN(0xffffff7f));
    c.clz(Reg::X7, Reg::X9);
    c.sw(Reg::X7, Reg::X4, 20);

    // rs1==x12, rd==x10, rs1_val == 4294967039, 
    // opcode: clz ; op1:x12; dest:x10; op1val:0xfffffeff;
    c.li(Reg::X12, MASK_XLEN(0xfffffeff));
    c.clz(Reg::X10, Reg::X12);
    c.sw(Reg::X10, Reg::X4, 24);

    // rs1==x11, rd==x2, rs1_val == 4294966783, 
    // opcode: clz ; op1:x11; dest:x2; op1val:0xfffffdff;
    c.li(Reg::X11, MASK_XLEN(0xfffffdff));
    c.clz(Reg::X2, Reg::X11);
    c.sw(Reg::X2, Reg::X4, 28);

    c.li(Reg::X1, 84);
    // rs1_val == 4294966271, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xfffffbff;
    c.li(Reg::X14, MASK_XLEN(0xfffffbff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 0);

    // rs1_val == 4294965247, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xfffff7ff;
    c.li(Reg::X14, MASK_XLEN(0xfffff7ff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 4);

    // rs1_val == 4294963199, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xffffefff;
    c.li(Reg::X14, MASK_XLEN(0xffffefff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 8);

    // rs1_val == 4294959103, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xffffdfff;
    c.li(Reg::X14, MASK_XLEN(0xffffdfff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 12);

    // rs1_val == 4294950911, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xffffbfff;
    c.li(Reg::X14, MASK_XLEN(0xffffbfff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 16);

    // rs1_val == 4294934527, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xffff7fff;
    c.li(Reg::X14, MASK_XLEN(0xffff7fff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 20);

    // rs1_val == 4294901759, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xfffeffff;
    c.li(Reg::X14, MASK_XLEN(0xfffeffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 24);

    // rs1_val == 4294836223, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xfffdffff;
    c.li(Reg::X14, MASK_XLEN(0xfffdffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 28);

    // rs1_val == 4294705151, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xfffbffff;
    c.li(Reg::X14, MASK_XLEN(0xfffbffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 32);

    // rs1_val == 4294443007, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xfff7ffff;
    c.li(Reg::X14, MASK_XLEN(0xfff7ffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 36);

    // rs1_val == 4293918719, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xffefffff;
    c.li(Reg::X14, MASK_XLEN(0xffefffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 40);

    // rs1_val == 4292870143, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xffdfffff;
    c.li(Reg::X14, MASK_XLEN(0xffdfffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 44);

    // rs1_val == 4290772991, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xffbfffff;
    c.li(Reg::X14, MASK_XLEN(0xffbfffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 48);

    // rs1_val == 4286578687, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xff7fffff;
    c.li(Reg::X14, MASK_XLEN(0xff7fffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 52);

    // rs1_val == 4278190079, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xfeffffff;
    c.li(Reg::X14, MASK_XLEN(0xfeffffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 56);

    // rs1_val == 4261412863, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xfdffffff;
    c.li(Reg::X14, MASK_XLEN(0xfdffffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 60);

    // rs1_val == 4227858431, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xfbffffff;
    c.li(Reg::X14, MASK_XLEN(0xfbffffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 64);

    // rs1_val == 4160749567, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xf7ffffff;
    c.li(Reg::X14, MASK_XLEN(0xf7ffffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 68);

    // rs1_val == 4026531839, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xefffffff;
    c.li(Reg::X14, MASK_XLEN(0xefffffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 72);

    // rs1_val == 1073741824, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x40000000;
    c.li(Reg::X14, MASK_XLEN(0x40000000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 76);

    // rs1_val == 2147483648, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x80000000;
    c.li(Reg::X14, MASK_XLEN(0x80000000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 80);

    // rs1_val == 3758096383, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xdfffffff;
    c.li(Reg::X14, MASK_XLEN(0xdfffffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 84);

    // rs1_val == 3221225471, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0xbfffffff;
    c.li(Reg::X14, MASK_XLEN(0xbfffffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 88);

    // rs1_val == 2147483647, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x7fffffff;
    c.li(Reg::X14, MASK_XLEN(0x7fffffff));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 92);

    // rs1_val == 16, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x10;
    c.li(Reg::X14, MASK_XLEN(0x10));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 96);

    // rs1_val == 32, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x20;
    c.li(Reg::X14, MASK_XLEN(0x20));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 100);

    // rs1_val == 64, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x40;
    c.li(Reg::X14, MASK_XLEN(0x40));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 104);

    // rs1_val == 128, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x80;
    c.li(Reg::X14, MASK_XLEN(0x80));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 108);

    // rs1_val == 256, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x100;
    c.li(Reg::X14, MASK_XLEN(0x100));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 112);

    // rs1_val == 512, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x200;
    c.li(Reg::X14, MASK_XLEN(0x200));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 116);

    // rs1_val == 1024, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x400;
    c.li(Reg::X14, MASK_XLEN(0x400));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 120);

    // rs1_val == 2048, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x800;
    c.li(Reg::X14, MASK_XLEN(0x800));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 124);

    // rs1_val == 4096, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x1000;
    c.li(Reg::X14, MASK_XLEN(0x1000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 128);

    // rs1_val == 8192, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x2000;
    c.li(Reg::X14, MASK_XLEN(0x2000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 132);

    // rs1_val == 16384, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x4000;
    c.li(Reg::X14, MASK_XLEN(0x4000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 136);

    // rs1_val == 32768, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x8000;
    c.li(Reg::X14, MASK_XLEN(0x8000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 140);

    // rs1_val == 65536, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x10000;
    c.li(Reg::X14, MASK_XLEN(0x10000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 144);

    // rs1_val == 131072, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x20000;
    c.li(Reg::X14, MASK_XLEN(0x20000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 148);

    // rs1_val == 262144, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x40000;
    c.li(Reg::X14, MASK_XLEN(0x40000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 152);

    // rs1_val == 524288, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x80000;
    c.li(Reg::X14, MASK_XLEN(0x80000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 156);

    // rs1_val == 1048576, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x100000;
    c.li(Reg::X14, MASK_XLEN(0x100000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 160);

    // rs1_val == 2097152, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x200000;
    c.li(Reg::X14, MASK_XLEN(0x200000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 164);

    // rs1_val == 4194304, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x400000;
    c.li(Reg::X14, MASK_XLEN(0x400000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 168);

    // rs1_val == 8388608, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x800000;
    c.li(Reg::X14, MASK_XLEN(0x800000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 172);

    // rs1_val == 16777216, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x1000000;
    c.li(Reg::X14, MASK_XLEN(0x1000000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 176);

    // rs1_val == 33554432, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x2000000;
    c.li(Reg::X14, MASK_XLEN(0x2000000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 180);

    // rs1_val == 67108864, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x4000000;
    c.li(Reg::X14, MASK_XLEN(0x4000000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 184);

    // rs1_val == 134217728, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x8000000;
    c.li(Reg::X14, MASK_XLEN(0x8000000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 188);

    // rs1_val == 268435456, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x10000000;
    c.li(Reg::X14, MASK_XLEN(0x10000000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 192);

    // rs1_val == 536870912, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x20000000;
    c.li(Reg::X14, MASK_XLEN(0x20000000));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 196);

    // rs1_val == 1431655765, 
    // opcode: clz ; op1:x14; dest:x15; op1val:0x55555555;
    c.li(Reg::X14, MASK_XLEN(0x55555555));
    c.clz(Reg::X15, Reg::X14);
    c.sw(Reg::X15, Reg::X1, 200);


        
    
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