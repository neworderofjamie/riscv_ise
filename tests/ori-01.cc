// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

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

namespace
{
class ori_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        // Generate word-aligned vector to initialise memory
        const std::vector<uint32_t> scalarWordInitData{
        3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,};
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(generateCode().getCode(), scalarInitData);
        
        if(!riscV.run()) {
            throw std::runtime_error("Error running test code");
        }
        
        // Copy output data
        s_ScalarData = riscV.getScalarDataMemory().getData();
    }
    

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite() 
    {
        s_ScalarData.clear();
    }
    
    static uint32_t *getScalarWordData(){ return reinterpret_cast<uint32_t*>(s_ScalarData.data()); }

private:
    static CodeGenerator generateCode()
    {
        CodeGenerator c;
        
        c.li(Reg::X2, 0);
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

        c.li(Reg::X4, 88);
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

        c.li(Reg::X4, 2136);
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

    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> ori_01::s_ScalarData;
}


// Generate google tests

TEST_F(ori_01, opcode__ori___op1_x5__dest_x22__op1val__0x201___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[0], 4294966783);
}


TEST_F(ori_01, opcode__ori___op1_x27__dest_x27__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[1], 0);
}


TEST_F(ori_01, opcode__ori___op1_x17__dest_x8__op1val_0x33333334___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[2], 858994687);
}


TEST_F(ori_01, opcode__ori___op1_x20__dest_x1__op1val__0xb504___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[3], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x12__dest_x19__op1val__0x80000000___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[4], 2147483693);
}


TEST_F(ori_01, opcode__ori___op1_x8__dest_x3__op1val_0x7fffffff___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[5], 2147483647);
}


TEST_F(ori_01, opcode__ori___op1_x28__dest_x26__op1val_0x1___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[6], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x16__dest_x23__op1val_0x7___immval__0x7) {
  ASSERT_EQ(getScalarWordData()[7], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x25__dest_x31__op1val_0x40000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[8], 262146);
}


TEST_F(ori_01, opcode__ori___op1_x23__dest_x11__op1val_0x20000000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[9], 536870916);
}


TEST_F(ori_01, opcode__ori___op1_x14__dest_x17__op1val__0x201___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[10], 4294966783);
}


TEST_F(ori_01, opcode__ori___op1_x31__dest_x7__op1val_0x2___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[11], 18);
}


TEST_F(ori_01, opcode__ori___op1_x21__dest_x4__op1val_0x8000___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[12], 32800);
}


TEST_F(ori_01, opcode__ori___op1_x15__dest_x5__op1val_0x800___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[13], 2112);
}


TEST_F(ori_01, opcode__ori___op1_x30__dest_x25__op1val__0x40001___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[14], 4294705151);
}


TEST_F(ori_01, opcode__ori___op1_x11__dest_x30__op1val__0x5___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[15], 4294967291);
}


TEST_F(ori_01, opcode__ori___op1_x4__dest_x10__op1val__0x80001___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[16], 4294443007);
}


TEST_F(ori_01, opcode__ori___op1_x13__dest_x0__op1val__0x40000001___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(ori_01, opcode__ori___op1_x26__dest_x6__op1val__0x21___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[18], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x19__dest_x18__op1val_0xb503___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[19], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x7__dest_x21__op1val_0x10000000___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[20], 4294967291);
}


TEST_F(ori_01, opcode__ori___op1_x24__dest_x16__op1val__0x80001___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[21], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x0__dest_x9__op1val_0x0___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[22], 4294967279);
}


TEST_F(ori_01, opcode__ori___op1_x3__dest_x28__op1val_0x2000___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[23], 4294967263);
}


TEST_F(ori_01, opcode__ori___op1_x1__dest_x15__op1val_0x55555556___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[24], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x18__dest_x2__op1val__0xb504___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[25], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x22__dest_x20__op1val__0x55555556___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[26], 4294967039);
}


TEST_F(ori_01, opcode__ori___op1_x6__dest_x13__op1val_0x1000000___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[27], 4294966783);
}


TEST_F(ori_01, opcode__ori___op1_x29__dest_x14__op1val_0x20000000___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[28], 4294966271);
}


TEST_F(ori_01, opcode__ori___op1_x2__dest_x12__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[29], 4294966014);
}


TEST_F(ori_01, opcode__ori___op1_x9__dest_x24__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[30], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x29__op1val_0x8___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[31], 14);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x10___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[32], 528);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x20___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[33], 44);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x40___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[34], 67);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x80___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[35], 160);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x100___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[36], 4294967231);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x200___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[37], 4294967290);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x400___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[38], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x1000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[39], 4101);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4000___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[40], 18021);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x10000___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[41], 65545);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x20000___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[42], 131584);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x80000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[43], 524288);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x100000___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[44], 1049088);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x200000___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[45], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x400000___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[46], 4294966783);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x800000___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[47], 8388736);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2000000___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[48], 4294967263);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4000000___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[49], 67108880);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x8000000___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[50], 134219092);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x40000000___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[51], 4294967290);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x2___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[52], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x3___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[53], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x9___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[54], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x11___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[55], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x41___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[56], 4294967231);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x81___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[57], 4294967167);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x101___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[58], 4294967039);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x401___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[59], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x801___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[60], 4294965247);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x1001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[61], 4294963199);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x2001___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[62], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x8001___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[63], 4294934527);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x10001___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[64], 4294901759);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x20001___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[65], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x100001___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[66], 4293918719);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x200001___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[67], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x400001___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[68], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x800001___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[69], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x1000001___immval_0x3ff) {
  ASSERT_EQ(getScalarWordData()[70], 4278190079);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x2000001___immval__0x8) {
  ASSERT_EQ(getScalarWordData()[71], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x4000001___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[72], 4227858431);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x8000001___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[73], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x10000001___immval__0x7) {
  ASSERT_EQ(getScalarWordData()[74], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x20000001___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[75], 3758096383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[76], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[77], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[78], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[79], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[80], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[81], 819);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[82], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[83], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[84], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[85], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[86], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[87], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[88], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[89], 819);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[90], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[91], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[92], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[93], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[94], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[95], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[96], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[97], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[98], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[99], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[100], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[101], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[102], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[103], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[104], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[105], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[106], 1431655805);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[107], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[108], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[109], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[110], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[111], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[112], 1431656309);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[113], 1431655805);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[114], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[115], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[116], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[117], 1431656309);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[118], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[119], 4294967253);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[120], 1431655807);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[121], 2863311531);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[122], 2863312895);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[123], 4294965930);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[124], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[125], 2863311803);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[126], 2863312622);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[127], 4294967291);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[128], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[129], 2863311530);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[130], 2863312894);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[131], 2863311530);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[132], 2863311534);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[133], 2863311802);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[134], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[135], 2863311534);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[136], 2863312894);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[137], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[138], 2863311534);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[139], 2863311806);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[140], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[141], 4294967294);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[142], 2863311534);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[143], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[144], 1365);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[145], 4294965935);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[146], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[147], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[148], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[149], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[150], 45);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[151], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[152], 1365);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[153], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[154], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[155], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[156], 1637);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[157], 45);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[158], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[159], 4294965935);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[160], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[161], 821);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[162], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[163], 4294967253);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[164], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[165], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[166], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[167], 4294966203);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[168], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[169], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[170], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[171], 4294967283);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[172], 858993471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[173], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[174], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[175], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[176], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[177], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[178], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[179], 858993471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[180], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[181], 4294966203);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[182], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[183], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[184], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[185], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[186], 858993471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[187], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[188], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[189], 4294967022);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[190], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[191], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[192], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[193], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[194], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[195], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[196], 1717987190);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[197], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[198], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[199], 1717987190);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[200], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[201], 1717986926);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[202], 1717987190);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[203], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[204], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[205], 1717987190);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[206], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[207], 4294967286);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[208], 1717986926);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[209], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[210], 4294922237);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[211], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[212], 4294921215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[213], 4294921982);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[214], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[215], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[216], 4294920958);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[217], 4294922236);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[218], 4294920956);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[219], 4294920956);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[220], 4294921214);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[221], 4294921981);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[222], 4294920956);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[223], 4294922238);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[224], 4294966015);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[225], 4294920958);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[226], 4294921212);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[227], 4294921983);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[228], 4294967292);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[229], 4294920958);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[230], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[231], 46421);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[232], 4294967214);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[233], 46341);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[234], 46903);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[235], 46950);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[236], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[237], 46381);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[238], 46342);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[239], 46420);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[240], 46340);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[241], 46340);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[242], 46902);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[243], 46949);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[244], 46380);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[245], 46422);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[246], 4294967215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[247], 46342);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[248], 46900);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[249], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[250], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[251], 46382);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[252], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[253], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[254], 4294965930);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[255], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[256], 819);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[257], 1638);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[258], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[259], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[260], 2);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[261], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[262], 2);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[263], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[264], 818);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[265], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[266], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[267], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[268], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[269], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[270], 822);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[271], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[272], 4294967254);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[273], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[274], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[275], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[276], 4294967294);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[277], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[278], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[279], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[280], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[281], 1431655805);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[282], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[283], 1431655764);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[284], 1431655764);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[285], 1431655764);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[286], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[287], 1431656309);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[288], 1431655804);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[289], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[290], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[291], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[292], 1431656308);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[293], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[294], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[295], 1431655806);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[296], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[297], 1365);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[298], 4294965930);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[299], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[300], 819);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[301], 1638);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[302], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[303], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[304], 858994549);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[305], 858993468);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[306], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[307], 4294966207);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[308], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[309], 858993460);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[310], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[311], 4294967284);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[312], 858993470);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[313], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[314], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[315], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[316], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[317], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[318], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[319], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[320], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[321], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[322], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[323], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[324], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[325], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[326], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[327], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[328], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[329], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[330], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[331], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[332], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[333], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[334], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[335], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[336], 4294922237);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[337], 4294966015);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[338], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[339], 4294921215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[340], 4294921983);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[341], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[342], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[343], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[344], 4294922237);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[345], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[346], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[347], 4294921215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[348], 4294921981);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[349], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[350], 4294922239);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[351], 4294966015);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[352], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[353], 4294921213);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[354], 4294921983);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[355], 4294967293);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[356], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[357], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[358], 46421);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[359], 4294967215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[360], 46341);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[361], 46903);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[362], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[363], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[364], 46381);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[365], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[366], 46421);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[367], 46341);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[368], 46341);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[369], 46903);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[370], 46949);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[371], 46381);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[372], 46423);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[373], 4294967215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[374], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[375], 46901);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[376], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[377], 4294967253);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[378], 46383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[379], 45);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[380], 2);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[381], 1364);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[382], 4);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[383], 818);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[384], 1637);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[385], 44);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[386], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[387], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[388], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[389], 820);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[390], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[391], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[392], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[393], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[394], 1365);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[395], 4294965934);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[396], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[397], 1638);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[398], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[399], 45);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[400], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[401], 1364);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[402], 4);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[403], 4);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[404], 822);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[405], 1637);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[406], 44);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[407], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[408], 4294965935);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[409], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[410], 820);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[411], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[412], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[413], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[414], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[415], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[416], 4294966202);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[417], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[418], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[419], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[420], 4294967283);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[421], 858993471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[422], 858993458);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[423], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[424], 858993458);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[425], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[426], 858993458);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[427], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[428], 858993470);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[429], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[430], 4294966203);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[431], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[432], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[433], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[434], 4294967286);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[435], 858993470);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[436], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[437], 1717987189);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[438], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[439], 1717986917);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[440], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[441], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[442], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[443], 1717986925);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[444], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[445], 1717987189);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[446], 1717986917);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[447], 1717986917);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[448], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[449], 1717986917);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[450], 1717986925);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[451], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[452], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[453], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[454], 1717987189);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[455], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[456], 4294967285);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[457], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[458], 46339);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[459], 46423);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[460], 4294967211);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[461], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[462], 46899);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[463], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[464], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[465], 46383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[466], 46339);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[467], 46423);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[468], 46339);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[469], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[470], 46899);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[471], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[472], 46383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[473], 46423);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[474], 4294967211);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[475], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[476], 46903);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[477], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[478], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[479], 46383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[480], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[481], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[482], 4294967294);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[483], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[484], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[485], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[486], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[487], 1431655807);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[488], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[489], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[490], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[491], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[492], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[493], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[494], 1431655806);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[495], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[496], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[497], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[498], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[499], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[500], 4294967254);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[501], 1431655806);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[502], 2863311531);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[503], 2863312895);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[504], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[505], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[506], 2863311803);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[507], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[508], 4294967291);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[509], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[510], 2863311531);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[511], 2863312895);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[512], 2863311531);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[513], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[514], 2863311803);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[515], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[516], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[517], 2863312895);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[518], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[519], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[520], 2863311807);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[521], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[522], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[523], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[524], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[525], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[526], 4294965934);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[527], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[528], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[529], 1638);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[530], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[531], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[532], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[533], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[534], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[535], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[536], 822);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[537], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[538], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[539], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[540], 4294965935);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[541], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[542], 822);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[543], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[544], 4294967254);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[545], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[546], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[547], 858994549);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[548], 4294966206);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[549], 858993461);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[550], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[551], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[552], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[553], 858993469);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[554], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[555], 858994548);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[556], 858993460);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[557], 858993460);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x40000001___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[558], 3221225471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x4001___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[559], 4294967295);
}

