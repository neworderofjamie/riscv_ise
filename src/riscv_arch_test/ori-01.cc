#include "ori-01.h"

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
3735928559, 3735928559, 3735928559, 3735928559, 3735928559, 3735928559, };
}

//----------------------------------------------------------------------------
// ori_01
//----------------------------------------------------------------------------
const std::vector<uint32_t> &ori_01::getScalarInitData()
{
    return g_ScalarData;
}
//----------------------------------------------------------------------------
CodeGenerator ori_01::generateCode()
{
    CodeGenerator c;
    
    c.li(Reg::X2, 16);
    // rs1 != rd, rs1==x5, rd==x22, imm_val == (-2**(12-1)), imm_val == -2048, rs1_val != imm_val, rs1_val < 0 and imm_val < 0, rs1_val == -513
    // opcode: ori ; op1:x5; dest:x22; op1val:-0x201;  immval:-0x800
    c.li(Reg::X5, MASK_XLEN(-0x201));
    c.ori(Reg::X22, Reg::X5, SEXT_IMM(-0x800));
    c.sw(Reg::X22, Reg::X2, 0);

    // rs1 == rd, rs1==x27, rd==x27, imm_val == 0, rs1_val == imm_val, rs1_val == 0, rs1_val==0 and imm_val==0
    // opcode: ori ; op1:x27; dest:x27; op1val:0x0;  immval:0x0
    c.li(Reg::X27, MASK_XLEN(0x0));
    c.ori(Reg::X27, Reg::X27, SEXT_IMM(0x0));
    c.sw(Reg::X27, Reg::X2, 4);

    // rs1==x17, rd==x8, imm_val == (2**(12-1)-1), rs1_val > 0 and imm_val > 0, imm_val == 2047
    // opcode: ori ; op1:x17; dest:x8; op1val:0x33333334;  immval:0x7ff
    c.li(Reg::X17, MASK_XLEN(0x33333334));
    c.ori(Reg::X8, Reg::X17, SEXT_IMM(0x7ff));
    c.sw(Reg::X8, Reg::X2, 8);

    // rs1==x20, rd==x1, imm_val == 1, rs1_val < 0 and imm_val > 0
    // opcode: ori ; op1:x20; dest:x1; op1val:-0xb504;  immval:0x1
    c.li(Reg::X20, MASK_XLEN(-0xb504));
    c.ori(Reg::X1, Reg::X20, SEXT_IMM(0x1));
    c.sw(Reg::X1, Reg::X2, 12);

    // rs1==x12, rd==x19, rs1_val == (-2**(xlen-1)), rs1_val == -2147483648
    // opcode: ori ; op1:x12; dest:x19; op1val:-0x80000000;  immval:0x2d
    c.li(Reg::X12, MASK_XLEN(-0x80000000));
    c.ori(Reg::X19, Reg::X12, SEXT_IMM(0x2d));
    c.sw(Reg::X19, Reg::X2, 16);

    // rs1==x8, rd==x3, rs1_val == (2**(xlen-1)-1), rs1_val == 2147483647, imm_val == 1365
    // opcode: ori ; op1:x8; dest:x3; op1val:0x7fffffff;  immval:0x555
    c.li(Reg::X8, MASK_XLEN(0x7fffffff));
    c.ori(Reg::X3, Reg::X8, SEXT_IMM(0x555));
    c.sw(Reg::X3, Reg::X2, 20);

    // rs1==x28, rd==x26, rs1_val == 1, 
    // opcode: ori ; op1:x28; dest:x26; op1val:0x1;  immval:0x667
    c.li(Reg::X28, MASK_XLEN(0x1));
    c.ori(Reg::X26, Reg::X28, SEXT_IMM(0x667));
    c.sw(Reg::X26, Reg::X2, 24);

    // rs1==x16, rd==x23, rs1_val > 0 and imm_val < 0, 
    // opcode: ori ; op1:x16; dest:x23; op1val:0x7;  immval:-0x7
    c.li(Reg::X16, MASK_XLEN(0x7));
    c.ori(Reg::X23, Reg::X16, SEXT_IMM(-0x7));
    c.sw(Reg::X23, Reg::X2, 28);

    // rs1==x25, rd==x31, imm_val == 2, rs1_val == 262144
    // opcode: ori ; op1:x25; dest:x31; op1val:0x40000;  immval:0x2
    c.li(Reg::X25, MASK_XLEN(0x40000));
    c.ori(Reg::X31, Reg::X25, SEXT_IMM(0x2));
    c.sw(Reg::X31, Reg::X2, 32);

    // rs1==x23, rd==x11, imm_val == 4, rs1_val == 536870912
    // opcode: ori ; op1:x23; dest:x11; op1val:0x20000000;  immval:0x4
    c.li(Reg::X23, MASK_XLEN(0x20000000));
    c.ori(Reg::X11, Reg::X23, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X2, 36);

    // rs1==x14, rd==x17, imm_val == 8, 
    // opcode: ori ; op1:x14; dest:x17; op1val:-0x201;  immval:0x8
    c.li(Reg::X14, MASK_XLEN(-0x201));
    c.ori(Reg::X17, Reg::X14, SEXT_IMM(0x8));
    c.sw(Reg::X17, Reg::X2, 40);

    // rs1==x31, rd==x7, imm_val == 16, rs1_val == 2
    // opcode: ori ; op1:x31; dest:x7; op1val:0x2;  immval:0x10
    c.li(Reg::X31, MASK_XLEN(0x2));
    c.ori(Reg::X7, Reg::X31, SEXT_IMM(0x10));
    c.sw(Reg::X7, Reg::X2, 44);

    // rs1==x21, rd==x4, imm_val == 32, rs1_val == 32768
    // opcode: ori ; op1:x21; dest:x4; op1val:0x8000;  immval:0x20
    c.li(Reg::X21, MASK_XLEN(0x8000));
    c.ori(Reg::X4, Reg::X21, SEXT_IMM(0x20));
    c.sw(Reg::X4, Reg::X2, 48);

    // rs1==x15, rd==x5, imm_val == 64, rs1_val == 2048
    // opcode: ori ; op1:x15; dest:x5; op1val:0x800;  immval:0x40
    c.li(Reg::X15, MASK_XLEN(0x800));
    c.ori(Reg::X5, Reg::X15, SEXT_IMM(0x40));
    c.sw(Reg::X5, Reg::X2, 52);

    // rs1==x30, rd==x25, imm_val == 128, rs1_val == -262145
    // opcode: ori ; op1:x30; dest:x25; op1val:-0x40001;  immval:0x80
    c.li(Reg::X30, MASK_XLEN(-0x40001));
    c.ori(Reg::X25, Reg::X30, SEXT_IMM(0x80));
    c.sw(Reg::X25, Reg::X2, 56);

    // rs1==x11, rd==x30, imm_val == 256, rs1_val == -5
    // opcode: ori ; op1:x11; dest:x30; op1val:-0x5;  immval:0x100
    c.li(Reg::X11, MASK_XLEN(-0x5));
    c.ori(Reg::X30, Reg::X11, SEXT_IMM(0x100));
    c.sw(Reg::X30, Reg::X2, 60);

    // rs1==x4, rd==x10, imm_val == 512, rs1_val == -524289
    // opcode: ori ; op1:x4; dest:x10; op1val:-0x80001;  immval:0x200
    c.li(Reg::X4, MASK_XLEN(-0x80001));
    c.ori(Reg::X10, Reg::X4, SEXT_IMM(0x200));
    c.sw(Reg::X10, Reg::X2, 64);

    // rs1==x13, rd==x0, imm_val == 1024, rs1_val == -1073741825
    // opcode: ori ; op1:x13; dest:x0; op1val:-0x40000001;  immval:0x400
    c.li(Reg::X13, MASK_XLEN(-0x40000001));
    c.ori(Reg::X0, Reg::X13, SEXT_IMM(0x400));
    c.sw(Reg::X0, Reg::X2, 68);

    // rs1==x26, rd==x6, imm_val == -2, rs1_val == -33
    // opcode: ori ; op1:x26; dest:x6; op1val:-0x21;  immval:-0x2
    c.li(Reg::X26, MASK_XLEN(-0x21));
    c.ori(Reg::X6, Reg::X26, SEXT_IMM(-0x2));
    c.sw(Reg::X6, Reg::X2, 72);

    // rs1==x19, rd==x18, imm_val == -3, 
    // opcode: ori ; op1:x19; dest:x18; op1val:0xb503;  immval:-0x3
    c.li(Reg::X19, MASK_XLEN(0xb503));
    c.ori(Reg::X18, Reg::X19, SEXT_IMM(-0x3));
    c.sw(Reg::X18, Reg::X2, 76);

    // rs1==x7, rd==x21, imm_val == -5, rs1_val == 268435456
    // opcode: ori ; op1:x7; dest:x21; op1val:0x10000000;  immval:-0x5
    c.li(Reg::X7, MASK_XLEN(0x10000000));
    c.ori(Reg::X21, Reg::X7, SEXT_IMM(-0x5));
    c.sw(Reg::X21, Reg::X2, 80);

    // rs1==x24, rd==x16, imm_val == -9, 
    // opcode: ori ; op1:x24; dest:x16; op1val:-0x80001;  immval:-0x9
    c.li(Reg::X24, MASK_XLEN(-0x80001));
    c.ori(Reg::X16, Reg::X24, SEXT_IMM(-0x9));
    c.sw(Reg::X16, Reg::X2, 84);

    c.li(Reg::X4, 104);
    // rs1==x0, rd==x9, imm_val == -17, rs1_val == -16385
    // opcode: ori ; op1:x0; dest:x9; op1val:0x0;  immval:-0x11
    c.li(Reg::X0, MASK_XLEN(0x0));
    c.ori(Reg::X9, Reg::X0, SEXT_IMM(-0x11));
    c.sw(Reg::X9, Reg::X4, 0);

    // rs1==x3, rd==x28, imm_val == -33, rs1_val == 8192
    // opcode: ori ; op1:x3; dest:x28; op1val:0x2000;  immval:-0x21
    c.li(Reg::X3, MASK_XLEN(0x2000));
    c.ori(Reg::X28, Reg::X3, SEXT_IMM(-0x21));
    c.sw(Reg::X28, Reg::X4, 4);

    // rs1==x1, rd==x15, imm_val == -65, 
    // opcode: ori ; op1:x1; dest:x15; op1val:0x55555556;  immval:-0x41
    c.li(Reg::X1, MASK_XLEN(0x55555556));
    c.ori(Reg::X15, Reg::X1, SEXT_IMM(-0x41));
    c.sw(Reg::X15, Reg::X4, 8);

    // rs1==x18, rd==x2, imm_val == -129, 
    // opcode: ori ; op1:x18; dest:x2; op1val:-0xb504;  immval:-0x81
    c.li(Reg::X18, MASK_XLEN(-0xb504));
    c.ori(Reg::X2, Reg::X18, SEXT_IMM(-0x81));
    c.sw(Reg::X2, Reg::X4, 12);

    // rs1==x22, rd==x20, imm_val == -257, rs1_val == -1431655766
    // opcode: ori ; op1:x22; dest:x20; op1val:-0x55555556;  immval:-0x101
    c.li(Reg::X22, MASK_XLEN(-0x55555556));
    c.ori(Reg::X20, Reg::X22, SEXT_IMM(-0x101));
    c.sw(Reg::X20, Reg::X4, 16);

    // rs1==x6, rd==x13, imm_val == -513, rs1_val == 16777216
    // opcode: ori ; op1:x6; dest:x13; op1val:0x1000000;  immval:-0x201
    c.li(Reg::X6, MASK_XLEN(0x1000000));
    c.ori(Reg::X13, Reg::X6, SEXT_IMM(-0x201));
    c.sw(Reg::X13, Reg::X4, 20);

    // rs1==x29, rd==x14, imm_val == -1025, 
    // opcode: ori ; op1:x29; dest:x14; op1val:0x20000000;  immval:-0x401
    c.li(Reg::X29, MASK_XLEN(0x20000000));
    c.ori(Reg::X14, Reg::X29, SEXT_IMM(-0x401));
    c.sw(Reg::X14, Reg::X4, 24);

    // rs1==x2, rd==x12, imm_val == -1366, rs1_val==-46340 and imm_val==-1366
    // opcode: ori ; op1:x2; dest:x12; op1val:-0xb504;  immval:-0x556
    c.li(Reg::X2, MASK_XLEN(-0xb504));
    c.ori(Reg::X12, Reg::X2, SEXT_IMM(-0x556));
    c.sw(Reg::X12, Reg::X4, 28);

    // rs1==x9, rd==x24, rs1_val == 4, rs1_val==4 and imm_val==5
    // opcode: ori ; op1:x9; dest:x24; op1val:0x4;  immval:0x5
    c.li(Reg::X9, MASK_XLEN(0x4));
    c.ori(Reg::X24, Reg::X9, SEXT_IMM(0x5));
    c.sw(Reg::X24, Reg::X4, 32);

    // rs1==x10, rd==x29, rs1_val == 8, 
    // opcode: ori ; op1:x10; dest:x29; op1val:0x8;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x8));
    c.ori(Reg::X29, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X29, Reg::X4, 36);

    // rs1_val == 16, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x10;  immval:0x200
    c.li(Reg::X10, MASK_XLEN(0x10));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x200));
    c.sw(Reg::X11, Reg::X4, 40);

    // rs1_val == 32, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x20;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x20));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 44);

    // rs1_val == 64, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x40;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x40));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 48);

    // rs1_val == 128, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x80;  immval:0x20
    c.li(Reg::X10, MASK_XLEN(0x80));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x20));
    c.sw(Reg::X11, Reg::X4, 52);

    // rs1_val == 256, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x100;  immval:-0x41
    c.li(Reg::X10, MASK_XLEN(0x100));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x41));
    c.sw(Reg::X11, Reg::X4, 56);

    // rs1_val == 512, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x200;  immval:-0x6
    c.li(Reg::X10, MASK_XLEN(0x200));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x6));
    c.sw(Reg::X11, Reg::X4, 60);

    // rs1_val == 1024, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x400;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x400));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 64);

    // rs1_val == 4096, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x1000;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x1000));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 68);

    // rs1_val == 16384, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4000;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x4000));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 72);

    // rs1_val == 65536, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x10000;  immval:0x9
    c.li(Reg::X10, MASK_XLEN(0x10000));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x9));
    c.sw(Reg::X11, Reg::X4, 76);

    // rs1_val == 131072, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x20000;  immval:0x200
    c.li(Reg::X10, MASK_XLEN(0x20000));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x200));
    c.sw(Reg::X11, Reg::X4, 80);

    // rs1_val == 524288, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x80000;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x80000));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 84);

    // rs1_val == 1048576, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x100000;  immval:0x200
    c.li(Reg::X10, MASK_XLEN(0x100000));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x200));
    c.sw(Reg::X11, Reg::X4, 88);

    // rs1_val == 2097152, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x200000;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x200000));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 92);

    // rs1_val == 4194304, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x400000;  immval:-0x201
    c.li(Reg::X10, MASK_XLEN(0x400000));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x201));
    c.sw(Reg::X11, Reg::X4, 96);

    // rs1_val == 8388608, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x800000;  immval:0x80
    c.li(Reg::X10, MASK_XLEN(0x800000));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x80));
    c.sw(Reg::X11, Reg::X4, 100);

    // rs1_val == 33554432, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2000000;  immval:-0x21
    c.li(Reg::X10, MASK_XLEN(0x2000000));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x21));
    c.sw(Reg::X11, Reg::X4, 104);

    // rs1_val == 67108864, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4000000;  immval:0x10
    c.li(Reg::X10, MASK_XLEN(0x4000000));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x10));
    c.sw(Reg::X11, Reg::X4, 108);

    // rs1_val == 134217728, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x8000000;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x8000000));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 112);

    // rs1_val == 1073741824, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x40000000;  immval:-0x6
    c.li(Reg::X10, MASK_XLEN(0x40000000));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x6));
    c.sw(Reg::X11, Reg::X4, 116);

    // rs1_val == -2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x2;  immval:-0x41
    c.li(Reg::X10, MASK_XLEN(-0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x41));
    c.sw(Reg::X11, Reg::X4, 120);

    // rs1_val == -3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x3;  immval:-0x9
    c.li(Reg::X10, MASK_XLEN(-0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x9));
    c.sw(Reg::X11, Reg::X4, 124);

    // rs1_val == -9, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x9;  immval:-0x800
    c.li(Reg::X10, MASK_XLEN(-0x9));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x800));
    c.sw(Reg::X11, Reg::X4, 128);

    // rs1_val == -17, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x11;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0x11));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 132);

    // rs1_val == -65, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x41;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(-0x41));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 136);

    // rs1_val == -129, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x81;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0x81));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 140);

    // rs1_val == -257, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x101;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0x101));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 144);

    // rs1_val == -1025, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x401;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0x401));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 148);

    // rs1_val == -2049, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x801;  immval:0x7ff
    c.li(Reg::X10, MASK_XLEN(-0x801));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x7ff));
    c.sw(Reg::X11, Reg::X4, 152);

    // rs1_val == -4097, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x1001;  immval:0x7
    c.li(Reg::X10, MASK_XLEN(-0x1001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x7));
    c.sw(Reg::X11, Reg::X4, 156);

    // rs1_val == -8193, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x2001;  immval:-0x21
    c.li(Reg::X10, MASK_XLEN(-0x2001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x21));
    c.sw(Reg::X11, Reg::X4, 160);

    // rs1_val == -32769, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x8001;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0x8001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 164);

    // rs1_val == -65537, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x10001;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0x10001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 168);

    // rs1_val == -131073, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x20001;  immval:-0xa
    c.li(Reg::X10, MASK_XLEN(-0x20001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0xa));
    c.sw(Reg::X11, Reg::X4, 172);

    // rs1_val == -1048577, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x100001;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0x100001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 176);

    // rs1_val == -2097153, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x200001;  immval:-0x400
    c.li(Reg::X10, MASK_XLEN(-0x200001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x400));
    c.sw(Reg::X11, Reg::X4, 180);

    // rs1_val == -4194305, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x400001;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0x400001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 184);

    // rs1_val == -8388609, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x800001;  immval:-0xa
    c.li(Reg::X10, MASK_XLEN(-0x800001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0xa));
    c.sw(Reg::X11, Reg::X4, 188);

    // rs1_val == -16777217, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x1000001;  immval:0x3ff
    c.li(Reg::X10, MASK_XLEN(-0x1000001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3ff));
    c.sw(Reg::X11, Reg::X4, 192);

    // rs1_val == -33554433, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x2000001;  immval:-0x8
    c.li(Reg::X10, MASK_XLEN(-0x2000001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x8));
    c.sw(Reg::X11, Reg::X4, 196);

    // rs1_val == -67108865, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x4000001;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0x4000001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 200);

    // rs1_val == -134217729, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x8000001;  immval:-0x6
    c.li(Reg::X10, MASK_XLEN(-0x8000001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x6));
    c.sw(Reg::X11, Reg::X4, 204);

    // rs1_val == -268435457, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x10000001;  immval:-0x7
    c.li(Reg::X10, MASK_XLEN(-0x10000001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x7));
    c.sw(Reg::X11, Reg::X4, 208);

    // rs1_val == -536870913, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x20000001;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(-0x20000001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 212);

    // rs1_val == 1431655765, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x101
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x101));
    c.sw(Reg::X11, Reg::X4, 216);

    // rs1_val==3 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 220);

    // rs1_val==3 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 224);

    // rs1_val==3 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 228);

    // rs1_val==3 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 232);

    // rs1_val==3 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 236);

    // rs1_val==3 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 240);

    // rs1_val==3 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 244);

    // rs1_val==3 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 248);

    // rs1_val==3 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 252);

    // rs1_val==3 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 256);

    // rs1_val==3 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 260);

    // rs1_val==3 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 264);

    // rs1_val==3 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 268);

    // rs1_val==3 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 272);

    // rs1_val==3 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 276);

    // rs1_val==3 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 280);

    // rs1_val==3 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 284);

    // rs1_val==3 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 288);

    // rs1_val==3 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 292);

    // rs1_val==3 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 296);

    // rs1_val==3 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 300);

    // rs1_val==3 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x3;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x3));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 304);

    // rs1_val==1431655765 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 308);

    // rs1_val==1431655765 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 312);

    // rs1_val==1431655765 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 316);

    // rs1_val==1431655765 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 320);

    // rs1_val==1431655765 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 324);

    // rs1_val==1431655765 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 328);

    // rs1_val==1431655765 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 332);

    // rs1_val==1431655765 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 336);

    // rs1_val==1431655765 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 340);

    // rs1_val==1431655765 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 344);

    // rs1_val==1431655765 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 348);

    // rs1_val==1431655765 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 352);

    // rs1_val==1431655765 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 356);

    // rs1_val==1431655765 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 360);

    // rs1_val==1431655765 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 364);

    // rs1_val==1431655765 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 368);

    // rs1_val==1431655765 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 372);

    // rs1_val==1431655765 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 376);

    // rs1_val==1431655765 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 380);

    // rs1_val==1431655765 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 384);

    // rs1_val==1431655765 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 388);

    // rs1_val==1431655765 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 392);

    // rs1_val==-1431655766 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 396);

    // rs1_val==-1431655766 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 400);

    // rs1_val==-1431655766 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 404);

    // rs1_val==-1431655766 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 408);

    // rs1_val==-1431655766 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 412);

    // rs1_val==-1431655766 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 416);

    // rs1_val==-1431655766 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 420);

    // rs1_val==-1431655766 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 424);

    // rs1_val==-1431655766 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 428);

    // rs1_val==-1431655766 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 432);

    // rs1_val==-1431655766 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 436);

    // rs1_val==-1431655766 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 440);

    // rs1_val==-1431655766 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 444);

    // rs1_val==-1431655766 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 448);

    // rs1_val==-1431655766 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 452);

    // rs1_val==-1431655766 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 456);

    // rs1_val==-1431655766 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 460);

    // rs1_val==-1431655766 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 464);

    // rs1_val==-1431655766 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 468);

    // rs1_val==-1431655766 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 472);

    // rs1_val==-1431655766 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 476);

    // rs1_val==-1431655766 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(-0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 480);

    // rs1_val==5 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 484);

    // rs1_val==5 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 488);

    // rs1_val==5 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 492);

    // rs1_val==5 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 496);

    // rs1_val==5 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 500);

    // rs1_val==5 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 504);

    // rs1_val==5 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 508);

    // rs1_val==5 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 512);

    // rs1_val==5 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 516);

    // rs1_val==5 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 520);

    // rs1_val==5 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 524);

    // rs1_val==5 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 528);

    // rs1_val==5 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 532);

    // rs1_val==5 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 536);

    // rs1_val==5 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 540);

    // rs1_val==5 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 544);

    // rs1_val==5 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 548);

    // rs1_val==5 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 552);

    // rs1_val==5 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 556);

    // rs1_val==5 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 560);

    // rs1_val==5 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 564);

    // rs1_val==5 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x5;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x5));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 568);

    // rs1_val==858993459 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 572);

    // rs1_val==858993459 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 576);

    // rs1_val==858993459 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 580);

    // rs1_val==858993459 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 584);

    // rs1_val==858993459 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 588);

    // rs1_val==858993459 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 592);

    // rs1_val==858993459 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 596);

    // rs1_val==858993459 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 600);

    // rs1_val==858993459 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 604);

    // rs1_val==858993459 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 608);

    // rs1_val==858993459 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 612);

    // rs1_val==858993459 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 616);

    // rs1_val==858993459 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 620);

    // rs1_val==858993459 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 624);

    // rs1_val==858993459 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 628);

    // rs1_val==858993459 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 632);

    // rs1_val==858993459 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 636);

    // rs1_val==858993459 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 640);

    // rs1_val==858993459 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 644);

    // rs1_val==858993459 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 648);

    // rs1_val==858993459 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 652);

    // rs1_val==858993459 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x33333333));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 656);

    // rs1_val==1717986918 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 660);

    // rs1_val==1717986918 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 664);

    // rs1_val==1717986918 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 668);

    // rs1_val==1717986918 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 672);

    // rs1_val==1717986918 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 676);

    // rs1_val==1717986918 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 680);

    // rs1_val==1717986918 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 684);

    // rs1_val==1717986918 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 688);

    // rs1_val==1717986918 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 692);

    // rs1_val==1717986918 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 696);

    // rs1_val==1717986918 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 700);

    // rs1_val==1717986918 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 704);

    // rs1_val==1717986918 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 708);

    // rs1_val==1717986918 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 712);

    // rs1_val==1717986918 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 716);

    // rs1_val==1717986918 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 720);

    // rs1_val==1717986918 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 724);

    // rs1_val==1717986918 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 728);

    // rs1_val==1717986918 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 732);

    // rs1_val==1717986918 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 736);

    // rs1_val==1717986918 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 740);

    // rs1_val==1717986918 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x66666666));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 744);

    // rs1_val==-46340 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 748);

    // rs1_val==-46340 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 752);

    // rs1_val==-46340 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 756);

    // rs1_val==-46340 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 760);

    // rs1_val==-46340 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 764);

    // rs1_val==-46340 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 768);

    // rs1_val==-46340 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 772);

    // rs1_val==-46340 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 776);

    // rs1_val==-46340 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 780);

    // rs1_val==-46340 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 784);

    // rs1_val==-46340 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 788);

    // rs1_val==-46340 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 792);

    // rs1_val==-46340 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 796);

    // rs1_val==-46340 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 800);

    // rs1_val==-46340 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 804);

    // rs1_val==-46340 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 808);

    // rs1_val==-46340 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 812);

    // rs1_val==-46340 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 816);

    // rs1_val==-46340 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 820);

    // rs1_val==-46340 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 824);

    // rs1_val==-46340 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(-0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 828);

    // rs1_val==46340 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 832);

    // rs1_val==46340 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 836);

    // rs1_val==46340 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 840);

    // rs1_val==46340 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 844);

    // rs1_val==46340 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 848);

    // rs1_val==46340 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 852);

    // rs1_val==46340 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 856);

    // rs1_val==46340 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 860);

    // rs1_val==46340 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 864);

    // rs1_val==46340 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 868);

    // rs1_val==46340 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 872);

    // rs1_val==46340 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 876);

    // rs1_val==46340 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 880);

    // rs1_val==46340 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 884);

    // rs1_val==46340 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 888);

    // rs1_val==46340 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 892);

    // rs1_val==46340 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 896);

    // rs1_val==46340 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 900);

    // rs1_val==46340 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 904);

    // rs1_val==46340 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 908);

    // rs1_val==46340 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 912);

    // rs1_val==46340 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0xb504));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 916);

    // rs1_val==2 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 920);

    // rs1_val==2 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 924);

    // rs1_val==2 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 928);

    // rs1_val==2 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 932);

    // rs1_val==2 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 936);

    // rs1_val==2 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 940);

    // rs1_val==2 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 944);

    // rs1_val==2 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 948);

    // rs1_val==2 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 952);

    // rs1_val==2 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 956);

    // rs1_val==2 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 960);

    // rs1_val==2 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 964);

    // rs1_val==2 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 968);

    // rs1_val==2 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 972);

    // rs1_val==2 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 976);

    // rs1_val==2 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 980);

    // rs1_val==2 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 984);

    // rs1_val==2 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 988);

    // rs1_val==2 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 992);

    // rs1_val==2 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 996);

    // rs1_val==2 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 1000);

    // rs1_val==2 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x2;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x2));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 1004);

    // rs1_val==1431655764 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 1008);

    // rs1_val==1431655764 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 1012);

    // rs1_val==1431655764 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 1016);

    // rs1_val==1431655764 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 1020);

    // rs1_val==1431655764 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 1024);

    // rs1_val==1431655764 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 1028);

    // rs1_val==1431655764 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 1032);

    // rs1_val==1431655764 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 1036);

    // rs1_val==1431655764 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 1040);

    // rs1_val==1431655764 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 1044);

    // rs1_val==1431655764 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 1048);

    // rs1_val==1431655764 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 1052);

    // rs1_val==1431655764 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 1056);

    // rs1_val==1431655764 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 1060);

    // rs1_val==1431655764 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 1064);

    // rs1_val==1431655764 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 1068);

    // rs1_val==1431655764 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 1072);

    // rs1_val==1431655764 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 1076);

    // rs1_val==1431655764 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 1080);

    // rs1_val==1431655764 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 1084);

    // rs1_val==1431655764 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 1088);

    // rs1_val==1431655764 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x55555554));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 1092);

    // rs1_val==0 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 1096);

    // rs1_val==0 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 1100);

    // rs1_val==0 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 1104);

    // rs1_val==0 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 1108);

    // rs1_val==0 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 1112);

    // rs1_val==0 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 1116);

    // rs1_val==0 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 1120);

    // rs1_val==858993460 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 1124);

    // rs1_val==858993460 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 1128);

    // rs1_val==858993460 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 1132);

    // rs1_val==858993460 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 1136);

    // rs1_val==858993460 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 1140);

    // rs1_val==858993460 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 1144);

    // rs1_val==858993460 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 1148);

    // rs1_val==858993460 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 1152);

    // rs1_val==858993460 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 1156);

    // rs1_val==858993460 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 1160);

    // rs1_val==1717986919 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 1164);

    // rs1_val==1717986919 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 1168);

    // rs1_val==1717986919 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 1172);

    // rs1_val==1717986919 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 1176);

    // rs1_val==1717986919 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 1180);

    // rs1_val==1717986919 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 1184);

    // rs1_val==1717986919 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 1188);

    // rs1_val==1717986919 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 1192);

    // rs1_val==1717986919 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 1196);

    // rs1_val==1717986919 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 1200);

    // rs1_val==1717986919 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 1204);

    // rs1_val==1717986919 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 1208);

    // rs1_val==1717986919 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 1212);

    // rs1_val==1717986919 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 1216);

    // rs1_val==1717986919 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 1220);

    // rs1_val==1717986919 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 1224);

    // rs1_val==1717986919 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 1228);

    // rs1_val==1717986919 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 1232);

    // rs1_val==1717986919 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 1236);

    // rs1_val==1717986919 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 1240);

    // rs1_val==1717986919 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 1244);

    // rs1_val==1717986919 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x66666667));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 1248);

    // rs1_val==-46339 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 1252);

    // rs1_val==-46339 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 1256);

    // rs1_val==-46339 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 1260);

    // rs1_val==-46339 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 1264);

    // rs1_val==-46339 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 1268);

    // rs1_val==-46339 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 1272);

    // rs1_val==-46339 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 1276);

    // rs1_val==-46339 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 1280);

    // rs1_val==-46339 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 1284);

    // rs1_val==-46339 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 1288);

    // rs1_val==-46339 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 1292);

    // rs1_val==-46339 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 1296);

    // rs1_val==-46339 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 1300);

    // rs1_val==-46339 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 1304);

    // rs1_val==-46339 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 1308);

    // rs1_val==-46339 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 1312);

    // rs1_val==-46339 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 1316);

    // rs1_val==-46339 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 1320);

    // rs1_val==-46339 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 1324);

    // rs1_val==-46339 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 1328);

    // rs1_val==-46339 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 1332);

    // rs1_val==-46339 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(-0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 1336);

    // rs1_val==46341 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 1340);

    // rs1_val==46341 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 1344);

    // rs1_val==46341 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 1348);

    // rs1_val==46341 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 1352);

    // rs1_val==46341 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 1356);

    // rs1_val==46341 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 1360);

    // rs1_val==46341 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 1364);

    // rs1_val==46341 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 1368);

    // rs1_val==46341 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 1372);

    // rs1_val==46341 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 1376);

    // rs1_val==46341 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 1380);

    // rs1_val==46341 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 1384);

    // rs1_val==46341 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 1388);

    // rs1_val==46341 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 1392);

    // rs1_val==46341 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 1396);

    // rs1_val==46341 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 1400);

    // rs1_val==46341 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 1404);

    // rs1_val==46341 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 1408);

    // rs1_val==46341 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 1412);

    // rs1_val==46341 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 1416);

    // rs1_val==46341 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 1420);

    // rs1_val==46341 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0xb505));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 1424);

    // rs1_val==0 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 1428);

    // rs1_val==0 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 1432);

    // rs1_val==0 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 1436);

    // rs1_val==0 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 1440);

    // rs1_val==0 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 1444);

    // rs1_val==0 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 1448);

    // rs1_val==0 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 1452);

    // rs1_val==0 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 1456);

    // rs1_val==0 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 1460);

    // rs1_val==0 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 1464);

    // rs1_val==0 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 1468);

    // rs1_val==0 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 1472);

    // rs1_val==0 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 1476);

    // rs1_val==0 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x0;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x0));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 1480);

    // rs1_val==4 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 1484);

    // rs1_val==4 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 1488);

    // rs1_val==4 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 1492);

    // rs1_val==4 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 1496);

    // rs1_val==4 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 1500);

    // rs1_val==4 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 1504);

    // rs1_val==4 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 1508);

    // rs1_val==4 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 1512);

    // rs1_val==4 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 1516);

    // rs1_val==4 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 1520);

    // rs1_val==4 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 1524);

    // rs1_val==4 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 1528);

    // rs1_val==4 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 1532);

    // rs1_val==4 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 1536);

    // rs1_val==4 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 1540);

    // rs1_val==4 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 1544);

    // rs1_val==4 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 1548);

    // rs1_val==4 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 1552);

    // rs1_val==4 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 1556);

    // rs1_val==4 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 1560);

    // rs1_val==4 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x4;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x4));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 1564);

    // rs1_val==858993458 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 1568);

    // rs1_val==858993458 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 1572);

    // rs1_val==858993458 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 1576);

    // rs1_val==858993458 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 1580);

    // rs1_val==858993458 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 1584);

    // rs1_val==858993458 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 1588);

    // rs1_val==858993458 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 1592);

    // rs1_val==858993458 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 1596);

    // rs1_val==858993458 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 1600);

    // rs1_val==858993458 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 1604);

    // rs1_val==858993458 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 1608);

    // rs1_val==858993458 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 1612);

    // rs1_val==858993458 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 1616);

    // rs1_val==858993458 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 1620);

    // rs1_val==858993458 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 1624);

    // rs1_val==858993458 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 1628);

    // rs1_val==858993458 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 1632);

    // rs1_val==858993458 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 1636);

    // rs1_val==858993458 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 1640);

    // rs1_val==858993458 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 1644);

    // rs1_val==858993458 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 1648);

    // rs1_val==858993458 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x33333332));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 1652);

    // rs1_val==1717986917 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 1656);

    // rs1_val==1717986917 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 1660);

    // rs1_val==1717986917 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 1664);

    // rs1_val==1717986917 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 1668);

    // rs1_val==1717986917 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 1672);

    // rs1_val==1717986917 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 1676);

    // rs1_val==1717986917 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 1680);

    // rs1_val==1717986917 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 1684);

    // rs1_val==1717986917 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 1688);

    // rs1_val==1717986917 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 1692);

    // rs1_val==1717986917 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 1696);

    // rs1_val==1717986917 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 1700);

    // rs1_val==1717986917 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 1704);

    // rs1_val==1717986917 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 1708);

    // rs1_val==1717986917 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 1712);

    // rs1_val==1717986917 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 1716);

    // rs1_val==1717986917 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 1720);

    // rs1_val==1717986917 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 1724);

    // rs1_val==1717986917 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 1728);

    // rs1_val==1717986917 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 1732);

    // rs1_val==1717986917 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 1736);

    // rs1_val==1717986917 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x66666665));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 1740);

    // rs1_val==46339 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 1744);

    // rs1_val==46339 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 1748);

    // rs1_val==46339 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 1752);

    // rs1_val==46339 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 1756);

    // rs1_val==46339 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 1760);

    // rs1_val==46339 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 1764);

    // rs1_val==46339 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 1768);

    // rs1_val==46339 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 1772);

    // rs1_val==46339 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 1776);

    // rs1_val==46339 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 1780);

    // rs1_val==46339 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 1784);

    // rs1_val==46339 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 1788);

    // rs1_val==46339 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 1792);

    // rs1_val==46339 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 1796);

    // rs1_val==46339 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 1800);

    // rs1_val==46339 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 1804);

    // rs1_val==46339 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 1808);

    // rs1_val==46339 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 1812);

    // rs1_val==46339 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 1816);

    // rs1_val==46339 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 1820);

    // rs1_val==46339 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 1824);

    // rs1_val==46339 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0xb503));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 1828);

    // rs1_val==1431655766 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 1832);

    // rs1_val==1431655766 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 1836);

    // rs1_val==1431655766 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 1840);

    // rs1_val==1431655766 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 1844);

    // rs1_val==1431655766 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 1848);

    // rs1_val==1431655766 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 1852);

    // rs1_val==1431655766 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 1856);

    // rs1_val==1431655766 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 1860);

    // rs1_val==1431655766 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 1864);

    // rs1_val==1431655766 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 1868);

    // rs1_val==1431655766 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 1872);

    // rs1_val==1431655766 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 1876);

    // rs1_val==1431655766 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 1880);

    // rs1_val==1431655766 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 1884);

    // rs1_val==1431655766 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 1888);

    // rs1_val==1431655766 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 1892);

    // rs1_val==1431655766 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 1896);

    // rs1_val==1431655766 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 1900);

    // rs1_val==1431655766 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 1904);

    // rs1_val==1431655766 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 1908);

    // rs1_val==1431655766 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 1912);

    // rs1_val==1431655766 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x55555556));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 1916);

    // rs1_val==-1431655765 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 1920);

    // rs1_val==-1431655765 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 1924);

    // rs1_val==-1431655765 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 1928);

    // rs1_val==-1431655765 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 1932);

    // rs1_val==-1431655765 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 1936);

    // rs1_val==-1431655765 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 1940);

    // rs1_val==-1431655765 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 1944);

    // rs1_val==-1431655765 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 1948);

    // rs1_val==-1431655765 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 1952);

    // rs1_val==-1431655765 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 1956);

    // rs1_val==-1431655765 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 1960);

    // rs1_val==-1431655765 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 1964);

    // rs1_val==-1431655765 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 1968);

    // rs1_val==-1431655765 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 1972);

    // rs1_val==-1431655765 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 1976);

    // rs1_val==-1431655765 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 1980);

    // rs1_val==-1431655765 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 1984);

    // rs1_val==-1431655765 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 1988);

    // rs1_val==-1431655765 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 1992);

    // rs1_val==-1431655765 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 1996);

    // rs1_val==-1431655765 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 2000);

    // rs1_val==-1431655765 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(-0x55555555));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 2004);

    // rs1_val==6 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 2008);

    // rs1_val==6 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 2012);

    // rs1_val==6 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 2016);

    // rs1_val==6 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 2020);

    // rs1_val==6 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 2024);

    // rs1_val==6 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 2028);

    // rs1_val==6 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 2032);

    // rs1_val==6 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 2036);

    // rs1_val==6 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 2040);

    // rs1_val==6 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 2044);

    c.li(Reg::X4, 2152);
    // rs1_val==6 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 0);

    // rs1_val==6 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 4);

    // rs1_val==6 and imm_val==818, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x332
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
    c.sw(Reg::X11, Reg::X4, 8);

    // rs1_val==6 and imm_val==1637, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x665
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
    c.sw(Reg::X11, Reg::X4, 12);

    // rs1_val==6 and imm_val==44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x2c
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
    c.sw(Reg::X11, Reg::X4, 16);

    // rs1_val==6 and imm_val==1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x556
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
    c.sw(Reg::X11, Reg::X4, 20);

    // rs1_val==6 and imm_val==-1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:-0x555
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
    c.sw(Reg::X11, Reg::X4, 24);

    // rs1_val==6 and imm_val==6, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x6
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
    c.sw(Reg::X11, Reg::X4, 28);

    // rs1_val==6 and imm_val==820, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x334
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
    c.sw(Reg::X11, Reg::X4, 32);

    // rs1_val==6 and imm_val==1639, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x667
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
    c.sw(Reg::X11, Reg::X4, 36);

    // rs1_val==6 and imm_val==-44, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:-0x2c
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
    c.sw(Reg::X11, Reg::X4, 40);

    // rs1_val==6 and imm_val==46, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x6;  immval:0x2e
    c.li(Reg::X10, MASK_XLEN(0x6));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
    c.sw(Reg::X11, Reg::X4, 44);

    // rs1_val==858993460 and imm_val==3, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x3
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
    c.sw(Reg::X11, Reg::X4, 48);

    // rs1_val==858993460 and imm_val==1365, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x555
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
    c.sw(Reg::X11, Reg::X4, 52);

    // rs1_val==858993460 and imm_val==-1366, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x556
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
    c.sw(Reg::X11, Reg::X4, 56);

    // rs1_val==858993460 and imm_val==5, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x5
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
    c.sw(Reg::X11, Reg::X4, 60);

    // rs1_val==858993460 and imm_val==819, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x333
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
    c.sw(Reg::X11, Reg::X4, 64);

    // rs1_val==858993460 and imm_val==1638, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x666
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
    c.sw(Reg::X11, Reg::X4, 68);

    // rs1_val==858993460 and imm_val==-45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
    c.sw(Reg::X11, Reg::X4, 72);

    // rs1_val==858993460 and imm_val==45, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2d
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
    c.sw(Reg::X11, Reg::X4, 76);

    // rs1_val==858993460 and imm_val==2, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
    c.sw(Reg::X11, Reg::X4, 80);

    // rs1_val==858993460 and imm_val==1364, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x554
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
    c.sw(Reg::X11, Reg::X4, 84);

    // rs1_val==858993460 and imm_val==0, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x0
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
    c.sw(Reg::X11, Reg::X4, 88);

    // rs1_val==858993460 and imm_val==4, 
    // opcode: ori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x4
    c.li(Reg::X10, MASK_XLEN(0x33333334));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
    c.sw(Reg::X11, Reg::X4, 92);

    // imm_val == 1024, rs1_val == -1073741825
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x40000001;  immval:0x400
    c.li(Reg::X10, MASK_XLEN(-0x40000001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(0x400));
    c.sw(Reg::X11, Reg::X4, 96);

    // imm_val == -17, rs1_val == -16385
    // opcode: ori ; op1:x10; dest:x11; op1val:-0x4001;  immval:-0x11
    c.li(Reg::X10, MASK_XLEN(-0x4001));
    c.ori(Reg::X11, Reg::X10, SEXT_IMM(-0x11));
    c.sw(Reg::X11, Reg::X4, 100);


        
    // End
    c.ecall();
    return c;
}