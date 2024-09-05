#include "lui-01.h"

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
3735928559, };
}

//----------------------------------------------------------------------------
// lui_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &lui_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
CodeGenerator lui_01::generateCode()
{
    CodeGenerator c;
    
    c.li(Reg::X9, 16);
    // rd==x8, imm_val == 0, imm_val==0
    // opcode: lui ; dest:x8; immval:0x0
    c.lui(Reg::X8, 0x0);
    c.sw(Reg::X8, Reg::X9, 0);

    // rd==x5, imm_val > 0, imm_val==5
    // opcode: lui ; dest:x5; immval:0x5
    c.lui(Reg::X5, 0x5);
    c.sw(Reg::X5, Reg::X9, 4);

    // rd==x19, imm_val == ((2**20)-1), 
    // opcode: lui ; dest:x19; immval:0xfffff
    c.lui(Reg::X19, 0xfffff);
    c.sw(Reg::X19, Reg::X9, 8);

    // rd==x13, imm_val==3, 
    // opcode: lui ; dest:x13; immval:0x3
    c.lui(Reg::X13, 0x3);
    c.sw(Reg::X13, Reg::X9, 12);

    // rd==x0, imm_val==349525, imm_val == 349525
    // opcode: lui ; dest:x0; immval:0x55555
    c.lui(Reg::X0, 0x55555);
    c.sw(Reg::X0, Reg::X9, 16);

    // rd==x4, imm_val==699050, imm_val == 699050
    // opcode: lui ; dest:x4; immval:0xaaaaa
    c.lui(Reg::X4, 0xaaaaa);
    c.sw(Reg::X4, Reg::X9, 20);

    // rd==x22, imm_val==209715, 
    // opcode: lui ; dest:x22; immval:0x33333
    c.lui(Reg::X22, 0x33333);
    c.sw(Reg::X22, Reg::X9, 24);

    // rd==x26, imm_val==419430, 
    // opcode: lui ; dest:x26; immval:0x66666
    c.lui(Reg::X26, 0x66666);
    c.sw(Reg::X26, Reg::X9, 28);

    // rd==x29, imm_val==724, 
    // opcode: lui ; dest:x29; immval:0x2d4
    c.lui(Reg::X29, 0x2d4);
    c.sw(Reg::X29, Reg::X9, 32);

    // rd==x3, imm_val==1023, 
    // opcode: lui ; dest:x3; immval:0x3ff
    c.lui(Reg::X3, 0x3ff);
    c.sw(Reg::X3, Reg::X9, 36);

    // rd==x30, imm_val==2, imm_val == 2
    // opcode: lui ; dest:x30; immval:0x2
    c.lui(Reg::X30, 0x2);
    c.sw(Reg::X30, Reg::X9, 40);

    // rd==x16, imm_val==349524, 
    // opcode: lui ; dest:x16; immval:0x55554
    c.lui(Reg::X16, 0x55554);
    c.sw(Reg::X16, Reg::X9, 44);

    // rd==x2, imm_val==699049, 
    // opcode: lui ; dest:x2; immval:0xaaaa9
    c.lui(Reg::X2, 0xaaaa9);
    c.sw(Reg::X2, Reg::X9, 48);

    // rd==x24, imm_val==4, imm_val == 4
    // opcode: lui ; dest:x24; immval:0x4
    c.lui(Reg::X24, 0x4);
    c.sw(Reg::X24, Reg::X9, 52);

    // rd==x27, imm_val==209714, 
    // opcode: lui ; dest:x27; immval:0x33332
    c.lui(Reg::X27, 0x33332);
    c.sw(Reg::X27, Reg::X9, 56);

    // rd==x20, imm_val==419429, 
    // opcode: lui ; dest:x20; immval:0x66665
    c.lui(Reg::X20, 0x66665);
    c.sw(Reg::X20, Reg::X9, 60);

    // rd==x28, imm_val==723, 
    // opcode: lui ; dest:x28; immval:0x2d3
    c.lui(Reg::X28, 0x2d3);
    c.sw(Reg::X28, Reg::X9, 64);

    // rd==x6, imm_val==1022, 
    // opcode: lui ; dest:x6; immval:0x3fe
    c.lui(Reg::X6, 0x3fe);
    c.sw(Reg::X6, Reg::X9, 68);

    // rd==x10, imm_val==349526, 
    // opcode: lui ; dest:x10; immval:0x55556
    c.lui(Reg::X10, 0x55556);
    c.sw(Reg::X10, Reg::X9, 72);

    // rd==x12, imm_val==699051, 
    // opcode: lui ; dest:x12; immval:0xaaaab
    c.lui(Reg::X12, 0xaaaab);
    c.sw(Reg::X12, Reg::X9, 76);

    // rd==x15, imm_val==6, 
    // opcode: lui ; dest:x15; immval:0x6
    c.lui(Reg::X15, 0x6);
    c.sw(Reg::X15, Reg::X9, 80);

    // rd==x23, imm_val==209716, 
    // opcode: lui ; dest:x23; immval:0x33334
    c.lui(Reg::X23, 0x33334);
    c.sw(Reg::X23, Reg::X9, 84);

    // rd==x1, imm_val==419431, 
    // opcode: lui ; dest:x1; immval:0x66667
    c.lui(Reg::X1, 0x66667);
    c.sw(Reg::X1, Reg::X9, 88);

    // rd==x18, imm_val==725, 
    // opcode: lui ; dest:x18; immval:0x2d5
    c.lui(Reg::X18, 0x2d5);
    c.sw(Reg::X18, Reg::X9, 92);

    // rd==x11, imm_val==1, imm_val == 1
    // opcode: lui ; dest:x11; immval:0x1
    c.lui(Reg::X11, 0x1);
    c.sw(Reg::X11, Reg::X9, 96);

    // rd==x21, imm_val==1024, imm_val == 1024
    // opcode: lui ; dest:x21; immval:0x400
    c.lui(Reg::X21, 0x400);
    c.sw(Reg::X21, Reg::X9, 100);

    // rd==x17, imm_val == 8, 
    // opcode: lui ; dest:x17; immval:0x8
    c.lui(Reg::X17, 0x8);
    c.sw(Reg::X17, Reg::X9, 104);

    // rd==x25, imm_val == 16, 
    // opcode: lui ; dest:x25; immval:0x10
    c.lui(Reg::X25, 0x10);
    c.sw(Reg::X25, Reg::X9, 108);

    // rd==x7, imm_val == 32, 
    // opcode: lui ; dest:x7; immval:0x20
    c.lui(Reg::X7, 0x20);
    c.sw(Reg::X7, Reg::X9, 112);

    c.li(Reg::X1, 132);
    // rd==x14, imm_val == 64, 
    // opcode: lui ; dest:x14; immval:0x40
    c.lui(Reg::X14, 0x40);
    c.sw(Reg::X14, Reg::X1, 0);

    // rd==x9, imm_val == 128, 
    // opcode: lui ; dest:x9; immval:0x80
    c.lui(Reg::X9, 0x80);
    c.sw(Reg::X9, Reg::X1, 4);

    // rd==x31, imm_val == 983039, 
    // opcode: lui ; dest:x31; immval:0xeffff
    c.lui(Reg::X31, 0xeffff);
    c.sw(Reg::X31, Reg::X1, 8);

    // imm_val == 917503, 
    // opcode: lui ; dest:x10; immval:0xdffff
    c.lui(Reg::X10, 0xdffff);
    c.sw(Reg::X10, Reg::X1, 12);

    // imm_val == 786431, 
    // opcode: lui ; dest:x10; immval:0xbffff
    c.lui(Reg::X10, 0xbffff);
    c.sw(Reg::X10, Reg::X1, 16);

    // imm_val == 524287, 
    // opcode: lui ; dest:x10; immval:0x7ffff
    c.lui(Reg::X10, 0x7ffff);
    c.sw(Reg::X10, Reg::X1, 20);

    // imm_val == 256, 
    // opcode: lui ; dest:x10; immval:0x100
    c.lui(Reg::X10, 0x100);
    c.sw(Reg::X10, Reg::X1, 24);

    // imm_val == 512, 
    // opcode: lui ; dest:x10; immval:0x200
    c.lui(Reg::X10, 0x200);
    c.sw(Reg::X10, Reg::X1, 28);

    // imm_val == 2048, 
    // opcode: lui ; dest:x10; immval:0x800
    c.lui(Reg::X10, 0x800);
    c.sw(Reg::X10, Reg::X1, 32);

    // imm_val == 4096, 
    // opcode: lui ; dest:x10; immval:0x1000
    c.lui(Reg::X10, 0x1000);
    c.sw(Reg::X10, Reg::X1, 36);

    // imm_val == 8192, 
    // opcode: lui ; dest:x10; immval:0x2000
    c.lui(Reg::X10, 0x2000);
    c.sw(Reg::X10, Reg::X1, 40);

    // imm_val == 16384, 
    // opcode: lui ; dest:x10; immval:0x4000
    c.lui(Reg::X10, 0x4000);
    c.sw(Reg::X10, Reg::X1, 44);

    // imm_val == 32768, 
    // opcode: lui ; dest:x10; immval:0x8000
    c.lui(Reg::X10, 0x8000);
    c.sw(Reg::X10, Reg::X1, 48);

    // imm_val == 65536, 
    // opcode: lui ; dest:x10; immval:0x10000
    c.lui(Reg::X10, 0x10000);
    c.sw(Reg::X10, Reg::X1, 52);

    // imm_val == 131072, 
    // opcode: lui ; dest:x10; immval:0x20000
    c.lui(Reg::X10, 0x20000);
    c.sw(Reg::X10, Reg::X1, 56);

    // imm_val == 262144, 
    // opcode: lui ; dest:x10; immval:0x40000
    c.lui(Reg::X10, 0x40000);
    c.sw(Reg::X10, Reg::X1, 60);

    // imm_val == 524288, 
    // opcode: lui ; dest:x10; immval:0x80000
    c.lui(Reg::X10, 0x80000);
    c.sw(Reg::X10, Reg::X1, 64);

    // imm_val == 1048574, 
    // opcode: lui ; dest:x10; immval:0xffffe
    c.lui(Reg::X10, 0xffffe);
    c.sw(Reg::X10, Reg::X1, 68);

    // imm_val == 1048573, 
    // opcode: lui ; dest:x10; immval:0xffffd
    c.lui(Reg::X10, 0xffffd);
    c.sw(Reg::X10, Reg::X1, 72);

    // imm_val == 1048571, 
    // opcode: lui ; dest:x10; immval:0xffffb
    c.lui(Reg::X10, 0xffffb);
    c.sw(Reg::X10, Reg::X1, 76);

    // imm_val == 1048567, 
    // opcode: lui ; dest:x10; immval:0xffff7
    c.lui(Reg::X10, 0xffff7);
    c.sw(Reg::X10, Reg::X1, 80);

    // imm_val == 1048559, 
    // opcode: lui ; dest:x10; immval:0xfffef
    c.lui(Reg::X10, 0xfffef);
    c.sw(Reg::X10, Reg::X1, 84);

    // imm_val == 1048543, 
    // opcode: lui ; dest:x10; immval:0xfffdf
    c.lui(Reg::X10, 0xfffdf);
    c.sw(Reg::X10, Reg::X1, 88);

    // imm_val == 1048511, 
    // opcode: lui ; dest:x10; immval:0xfffbf
    c.lui(Reg::X10, 0xfffbf);
    c.sw(Reg::X10, Reg::X1, 92);

    // imm_val == 1048447, 
    // opcode: lui ; dest:x10; immval:0xfff7f
    c.lui(Reg::X10, 0xfff7f);
    c.sw(Reg::X10, Reg::X1, 96);

    // imm_val == 1048319, 
    // opcode: lui ; dest:x10; immval:0xffeff
    c.lui(Reg::X10, 0xffeff);
    c.sw(Reg::X10, Reg::X1, 100);

    // imm_val == 1048063, 
    // opcode: lui ; dest:x10; immval:0xffdff
    c.lui(Reg::X10, 0xffdff);
    c.sw(Reg::X10, Reg::X1, 104);

    // imm_val == 1047551, 
    // opcode: lui ; dest:x10; immval:0xffbff
    c.lui(Reg::X10, 0xffbff);
    c.sw(Reg::X10, Reg::X1, 108);

    // imm_val == 1046527, 
    // opcode: lui ; dest:x10; immval:0xff7ff
    c.lui(Reg::X10, 0xff7ff);
    c.sw(Reg::X10, Reg::X1, 112);

    // imm_val == 1044479, 
    // opcode: lui ; dest:x10; immval:0xfefff
    c.lui(Reg::X10, 0xfefff);
    c.sw(Reg::X10, Reg::X1, 116);

    // imm_val == 1040383, 
    // opcode: lui ; dest:x10; immval:0xfdfff
    c.lui(Reg::X10, 0xfdfff);
    c.sw(Reg::X10, Reg::X1, 120);

    // imm_val == 1032191, 
    // opcode: lui ; dest:x10; immval:0xfbfff
    c.lui(Reg::X10, 0xfbfff);
    c.sw(Reg::X10, Reg::X1, 124);

    // imm_val == 1015807, 
    // opcode: lui ; dest:x10; immval:0xf7fff
    c.lui(Reg::X10, 0xf7fff);
    c.sw(Reg::X10, Reg::X1, 128);

    // imm_val==349525, imm_val == 349525
    // opcode: lui ; dest:x10; immval:0x55555
    c.lui(Reg::X10, 0x55555);
    c.sw(Reg::X10, Reg::X1, 132);


        
    // End
    c.ecall();
    return c;
}