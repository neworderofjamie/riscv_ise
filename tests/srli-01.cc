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
class srli_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        // Generate word-aligned vector to initialise memory
        const std::vector<uint32_t> scalarWordInitData{
        3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,};
        
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
        
        c.li(Reg::X5, 0);
        // rs1 != rd, rs1==x30, rd==x8, rs1_val < 0 and imm_val > 0 and imm_val < xlen, rs1_val==-46340, imm_val == 2
        // opcode: srli ; op1:x30; dest:x8; op1val:-0xb504;  immval:0x2
        c.li(Reg::X30, MASK_XLEN(-0xb504));
        c.srli(Reg::X8, Reg::X30, SEXT_IMM(0x2));
        c.sw(Reg::X8, Reg::X5, 0);

        // rs1 == rd, rs1==x17, rd==x17, rs1_val > 0 and imm_val > 0 and imm_val < xlen, 
        // opcode: srli ; op1:x17; dest:x17; op1val:0x7;  immval:0x13
        c.li(Reg::X17, MASK_XLEN(0x7));
        c.srli(Reg::X17, Reg::X17, SEXT_IMM(0x13));
        c.sw(Reg::X17, Reg::X5, 4);

        // rs1==x27, rd==x19, rs1_val < 0 and imm_val == 0, 
        // opcode: srli ; op1:x27; dest:x19; op1val:-0xb504;  immval:0x0
        c.li(Reg::X27, MASK_XLEN(-0xb504));
        c.srli(Reg::X19, Reg::X27, SEXT_IMM(0x0));
        c.sw(Reg::X19, Reg::X5, 8);

        // rs1==x29, rd==x9, rs1_val > 0 and imm_val == 0, 
        // opcode: srli ; op1:x29; dest:x9; op1val:0x3fffffff;  immval:0x0
        c.li(Reg::X29, MASK_XLEN(0x3fffffff));
        c.srli(Reg::X9, Reg::X29, SEXT_IMM(0x0));
        c.sw(Reg::X9, Reg::X5, 12);

        // rs1==x25, rd==x22, rs1_val < 0 and imm_val == (xlen-1), 
        // opcode: srli ; op1:x25; dest:x22; op1val:-0xa;  immval:0x1f
        c.li(Reg::X25, MASK_XLEN(-0xa));
        c.srli(Reg::X22, Reg::X25, SEXT_IMM(0x1f));
        c.sw(Reg::X22, Reg::X5, 16);

        // rs1==x1, rd==x13, rs1_val > 0 and imm_val == (xlen-1), rs1_val == 512
        // opcode: srli ; op1:x1; dest:x13; op1val:0x200;  immval:0x1f
        c.li(Reg::X1, MASK_XLEN(0x200));
        c.srli(Reg::X13, Reg::X1, SEXT_IMM(0x1f));
        c.sw(Reg::X13, Reg::X5, 20);

        // rs1==x21, rd==x0, rs1_val == imm_val and imm_val > 0 and imm_val < xlen, rs1_val==3
        // opcode: srli ; op1:x21; dest:x0; op1val:0x3;  immval:0x3
        c.li(Reg::X21, MASK_XLEN(0x3));
        c.srli(Reg::X0, Reg::X21, SEXT_IMM(0x3));
        c.sw(Reg::X0, Reg::X5, 24);

        // rs1==x0, rd==x29, rs1_val == (-2**(xlen-1)) and imm_val >= 0 and imm_val < xlen, rs1_val == -2147483648
        // opcode: srli ; op1:x0; dest:x29; op1val:0x0;  immval:0x9
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.srli(Reg::X29, Reg::X0, SEXT_IMM(0x9));
        c.sw(Reg::X29, Reg::X5, 28);

        // rs1==x16, rd==x18, rs1_val == 0 and imm_val >= 0 and imm_val < xlen, imm_val == 1, rs1_val==0
        // opcode: srli ; op1:x16; dest:x18; op1val:0x0;  immval:0x1
        c.li(Reg::X16, MASK_XLEN(0x0));
        c.srli(Reg::X18, Reg::X16, SEXT_IMM(0x1));
        c.sw(Reg::X18, Reg::X5, 32);

        // rs1==x20, rd==x27, rs1_val == (2**(xlen-1)-1) and imm_val >= 0 and imm_val < xlen, rs1_val == 2147483647
        // opcode: srli ; op1:x20; dest:x27; op1val:0x7fffffff;  immval:0x11
        c.li(Reg::X20, MASK_XLEN(0x7fffffff));
        c.srli(Reg::X27, Reg::X20, SEXT_IMM(0x11));
        c.sw(Reg::X27, Reg::X5, 36);

        // rs1==x31, rd==x2, rs1_val == 1 and imm_val >= 0 and imm_val < xlen, rs1_val == 1
        // opcode: srli ; op1:x31; dest:x2; op1val:0x1;  immval:0x12
        c.li(Reg::X31, MASK_XLEN(0x1));
        c.srli(Reg::X2, Reg::X31, SEXT_IMM(0x12));
        c.sw(Reg::X2, Reg::X5, 40);

        // rs1==x7, rd==x31, rs1_val == 2, imm_val == 29, rs1_val==2
        // opcode: srli ; op1:x7; dest:x31; op1val:0x2;  immval:0x1d
        c.li(Reg::X7, MASK_XLEN(0x2));
        c.srli(Reg::X31, Reg::X7, SEXT_IMM(0x1d));
        c.sw(Reg::X31, Reg::X5, 44);

        // rs1==x14, rd==x16, rs1_val == 4, rs1_val==4, imm_val == 15
        // opcode: srli ; op1:x14; dest:x16; op1val:0x4;  immval:0xf
        c.li(Reg::X14, MASK_XLEN(0x4));
        c.srli(Reg::X16, Reg::X14, SEXT_IMM(0xf));
        c.sw(Reg::X16, Reg::X5, 48);

        // rs1==x12, rd==x25, rs1_val == 8, imm_val == 27
        // opcode: srli ; op1:x12; dest:x25; op1val:0x8;  immval:0x1b
        c.li(Reg::X12, MASK_XLEN(0x8));
        c.srli(Reg::X25, Reg::X12, SEXT_IMM(0x1b));
        c.sw(Reg::X25, Reg::X5, 52);

        // rs1==x4, rd==x11, rs1_val == 16, 
        // opcode: srli ; op1:x4; dest:x11; op1val:0x10;  immval:0xf
        c.li(Reg::X4, MASK_XLEN(0x10));
        c.srli(Reg::X11, Reg::X4, SEXT_IMM(0xf));
        c.sw(Reg::X11, Reg::X5, 56);

        // rs1==x24, rd==x23, rs1_val == 32, imm_val == 23
        // opcode: srli ; op1:x24; dest:x23; op1val:0x20;  immval:0x17
        c.li(Reg::X24, MASK_XLEN(0x20));
        c.srli(Reg::X23, Reg::X24, SEXT_IMM(0x17));
        c.sw(Reg::X23, Reg::X5, 60);

        // rs1==x8, rd==x28, rs1_val == 64, 
        // opcode: srli ; op1:x8; dest:x28; op1val:0x40;  immval:0xd
        c.li(Reg::X8, MASK_XLEN(0x40));
        c.srli(Reg::X28, Reg::X8, SEXT_IMM(0xd));
        c.sw(Reg::X28, Reg::X5, 64);

        // rs1==x15, rd==x30, rs1_val == 128, imm_val == 30
        // opcode: srli ; op1:x15; dest:x30; op1val:0x80;  immval:0x1e
        c.li(Reg::X15, MASK_XLEN(0x80));
        c.srli(Reg::X30, Reg::X15, SEXT_IMM(0x1e));
        c.sw(Reg::X30, Reg::X5, 68);

        // rs1==x18, rd==x20, rs1_val == 256, 
        // opcode: srli ; op1:x18; dest:x20; op1val:0x100;  immval:0x1f
        c.li(Reg::X18, MASK_XLEN(0x100));
        c.srli(Reg::X20, Reg::X18, SEXT_IMM(0x1f));
        c.sw(Reg::X20, Reg::X5, 72);

        // rs1==x13, rd==x14, rs1_val == 1024, 
        // opcode: srli ; op1:x13; dest:x14; op1val:0x400;  immval:0x12
        c.li(Reg::X13, MASK_XLEN(0x400));
        c.srli(Reg::X14, Reg::X13, SEXT_IMM(0x12));
        c.sw(Reg::X14, Reg::X5, 76);

        // rs1==x11, rd==x3, rs1_val == 2048, 
        // opcode: srli ; op1:x11; dest:x3; op1val:0x800;  immval:0xf
        c.li(Reg::X11, MASK_XLEN(0x800));
        c.srli(Reg::X3, Reg::X11, SEXT_IMM(0xf));
        c.sw(Reg::X3, Reg::X5, 80);

        // rs1==x23, rd==x26, rs1_val == 4096, 
        // opcode: srli ; op1:x23; dest:x26; op1val:0x1000;  immval:0xf
        c.li(Reg::X23, MASK_XLEN(0x1000));
        c.srli(Reg::X26, Reg::X23, SEXT_IMM(0xf));
        c.sw(Reg::X26, Reg::X5, 84);

        c.li(Reg::X8, 88);
        // rs1==x19, rd==x10, rs1_val == 8192, 
        // opcode: srli ; op1:x19; dest:x10; op1val:0x2000;  immval:0x2
        c.li(Reg::X19, MASK_XLEN(0x2000));
        c.srli(Reg::X10, Reg::X19, SEXT_IMM(0x2));
        c.sw(Reg::X10, Reg::X8, 0);

        // rs1==x10, rd==x21, rs1_val == 16384, 
        // opcode: srli ; op1:x10; dest:x21; op1val:0x4000;  immval:0xe
        c.li(Reg::X10, MASK_XLEN(0x4000));
        c.srli(Reg::X21, Reg::X10, SEXT_IMM(0xe));
        c.sw(Reg::X21, Reg::X8, 4);

        // rs1==x5, rd==x7, rs1_val == 32768, 
        // opcode: srli ; op1:x5; dest:x7; op1val:0x8000;  immval:0x3
        c.li(Reg::X5, MASK_XLEN(0x8000));
        c.srli(Reg::X7, Reg::X5, SEXT_IMM(0x3));
        c.sw(Reg::X7, Reg::X8, 8);

        // rs1==x2, rd==x5, rs1_val == 65536, 
        // opcode: srli ; op1:x2; dest:x5; op1val:0x10000;  immval:0x17
        c.li(Reg::X2, MASK_XLEN(0x10000));
        c.srli(Reg::X5, Reg::X2, SEXT_IMM(0x17));
        c.sw(Reg::X5, Reg::X8, 12);

        // rs1==x28, rd==x6, rs1_val == 131072, 
        // opcode: srli ; op1:x28; dest:x6; op1val:0x20000;  immval:0x11
        c.li(Reg::X28, MASK_XLEN(0x20000));
        c.srli(Reg::X6, Reg::X28, SEXT_IMM(0x11));
        c.sw(Reg::X6, Reg::X8, 16);

        // rs1==x3, rd==x12, rs1_val == 262144, 
        // opcode: srli ; op1:x3; dest:x12; op1val:0x40000;  immval:0x7
        c.li(Reg::X3, MASK_XLEN(0x40000));
        c.srli(Reg::X12, Reg::X3, SEXT_IMM(0x7));
        c.sw(Reg::X12, Reg::X8, 20);

        // rs1==x26, rd==x4, rs1_val == 524288, 
        // opcode: srli ; op1:x26; dest:x4; op1val:0x80000;  immval:0x1d
        c.li(Reg::X26, MASK_XLEN(0x80000));
        c.srli(Reg::X4, Reg::X26, SEXT_IMM(0x1d));
        c.sw(Reg::X4, Reg::X8, 24);

        // rs1==x9, rd==x24, rs1_val == 1048576, 
        // opcode: srli ; op1:x9; dest:x24; op1val:0x100000;  immval:0x2
        c.li(Reg::X9, MASK_XLEN(0x100000));
        c.srli(Reg::X24, Reg::X9, SEXT_IMM(0x2));
        c.sw(Reg::X24, Reg::X8, 28);

        // rs1==x6, rd==x15, rs1_val == 2097152, 
        // opcode: srli ; op1:x6; dest:x15; op1val:0x200000;  immval:0x5
        c.li(Reg::X6, MASK_XLEN(0x200000));
        c.srli(Reg::X15, Reg::X6, SEXT_IMM(0x5));
        c.sw(Reg::X15, Reg::X8, 32);

        // rs1==x22, rd==x1, rs1_val == 4194304, 
        // opcode: srli ; op1:x22; dest:x1; op1val:0x400000;  immval:0xc
        c.li(Reg::X22, MASK_XLEN(0x400000));
        c.srli(Reg::X1, Reg::X22, SEXT_IMM(0xc));
        c.sw(Reg::X1, Reg::X8, 36);

        // rs1_val == 8388608, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x800000;  immval:0x9
        c.li(Reg::X10, MASK_XLEN(0x800000));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x9));
        c.sw(Reg::X11, Reg::X8, 40);

        // rs1_val == 16777216, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x1000000;  immval:0xe
        c.li(Reg::X10, MASK_XLEN(0x1000000));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0xe));
        c.sw(Reg::X11, Reg::X8, 44);

        // rs1_val == 33554432, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x2000000;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x2000000));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X8, 48);

        // rs1_val == 67108864, imm_val == 4
        // opcode: srli ; op1:x10; dest:x11; op1val:0x4000000;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x4000000));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X8, 52);

        // rs1_val == 134217728, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x8000000;  immval:0x7
        c.li(Reg::X10, MASK_XLEN(0x8000000));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x7));
        c.sw(Reg::X11, Reg::X8, 56);

        // rs1_val == 268435456, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x10000000;  immval:0x7
        c.li(Reg::X10, MASK_XLEN(0x10000000));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x7));
        c.sw(Reg::X11, Reg::X8, 60);

        // rs1_val == 536870912, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x20000000;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x20000000));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X8, 64);

        // rs1_val == 1073741824, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x40000000;  immval:0x1e
        c.li(Reg::X10, MASK_XLEN(0x40000000));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x1e));
        c.sw(Reg::X11, Reg::X8, 68);

        // rs1_val == -2, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x2;  immval:0x1d
        c.li(Reg::X10, MASK_XLEN(-0x2));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x1d));
        c.sw(Reg::X11, Reg::X8, 72);

        // rs1_val == -3, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x3;  immval:0xb
        c.li(Reg::X10, MASK_XLEN(-0x3));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0xb));
        c.sw(Reg::X11, Reg::X8, 76);

        // rs1_val == -5, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x5;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0x5));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X8, 80);

        // rs1_val == -9, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x9;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(-0x9));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X8, 84);

        // rs1_val == -17, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x11;  immval:0x1d
        c.li(Reg::X10, MASK_XLEN(-0x11));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x1d));
        c.sw(Reg::X11, Reg::X8, 88);

        // rs1_val == -33, imm_val == 16
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x21;  immval:0x10
        c.li(Reg::X10, MASK_XLEN(-0x21));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x10));
        c.sw(Reg::X11, Reg::X8, 92);

        // rs1_val == -65, imm_val == 8
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x41;  immval:0x8
        c.li(Reg::X10, MASK_XLEN(-0x41));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x8));
        c.sw(Reg::X11, Reg::X8, 96);

        // rs1_val == -129, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x81;  immval:0x12
        c.li(Reg::X10, MASK_XLEN(-0x81));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x12));
        c.sw(Reg::X11, Reg::X8, 100);

        // rs1_val == -257, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x101;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0x101));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X8, 104);

        // rs1_val == -513, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x201;  immval:0x13
        c.li(Reg::X10, MASK_XLEN(-0x201));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x13));
        c.sw(Reg::X11, Reg::X8, 108);

        // rs1_val == -1025, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x401;  immval:0x9
        c.li(Reg::X10, MASK_XLEN(-0x401));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x9));
        c.sw(Reg::X11, Reg::X8, 112);

        // rs1_val == -2049, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x801;  immval:0x7
        c.li(Reg::X10, MASK_XLEN(-0x801));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x7));
        c.sw(Reg::X11, Reg::X8, 116);

        // rs1_val == -4097, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x1001;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0x1001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X8, 120);

        // rs1_val == -8193, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x2001;  immval:0xe
        c.li(Reg::X10, MASK_XLEN(-0x2001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0xe));
        c.sw(Reg::X11, Reg::X8, 124);

        // rs1_val == -16385, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x4001;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(-0x4001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X8, 128);

        // rs1_val == -32769, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x8001;  immval:0x1d
        c.li(Reg::X10, MASK_XLEN(-0x8001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x1d));
        c.sw(Reg::X11, Reg::X8, 132);

        // rs1_val == -65537, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x10001;  immval:0x9
        c.li(Reg::X10, MASK_XLEN(-0x10001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x9));
        c.sw(Reg::X11, Reg::X8, 136);

        // rs1_val == -131073, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x20001;  immval:0x7
        c.li(Reg::X10, MASK_XLEN(-0x20001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x7));
        c.sw(Reg::X11, Reg::X8, 140);

        // rs1_val == -262145, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x40001;  immval:0x13
        c.li(Reg::X10, MASK_XLEN(-0x40001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x13));
        c.sw(Reg::X11, Reg::X8, 144);

        // rs1_val == -524289, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x80001;  immval:0xf
        c.li(Reg::X10, MASK_XLEN(-0x80001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0xf));
        c.sw(Reg::X11, Reg::X8, 148);

        // rs1_val == -1048577, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x100001;  immval:0x10
        c.li(Reg::X10, MASK_XLEN(-0x100001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x10));
        c.sw(Reg::X11, Reg::X8, 152);

        // rs1_val == -2097153, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x200001;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0x200001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X8, 156);

        // rs1_val == -4194305, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x400001;  immval:0x8
        c.li(Reg::X10, MASK_XLEN(-0x400001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x8));
        c.sw(Reg::X11, Reg::X8, 160);

        // rs1_val == -8388609, imm_val == 10
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x800001;  immval:0xa
        c.li(Reg::X10, MASK_XLEN(-0x800001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0xa));
        c.sw(Reg::X11, Reg::X8, 164);

        // rs1_val == -16777217, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x1000001;  immval:0xb
        c.li(Reg::X10, MASK_XLEN(-0x1000001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0xb));
        c.sw(Reg::X11, Reg::X8, 168);

        // rs1_val == -33554433, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x2000001;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(-0x2000001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X8, 172);

        // rs1_val == -67108865, imm_val == 21
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x4000001;  immval:0x15
        c.li(Reg::X10, MASK_XLEN(-0x4000001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x15));
        c.sw(Reg::X11, Reg::X8, 176);

        // rs1_val == -134217729, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x8000001;  immval:0x15
        c.li(Reg::X10, MASK_XLEN(-0x8000001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x15));
        c.sw(Reg::X11, Reg::X8, 180);

        // rs1_val == -268435457, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x10000001;  immval:0xe
        c.li(Reg::X10, MASK_XLEN(-0x10000001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0xe));
        c.sw(Reg::X11, Reg::X8, 184);

        // rs1_val == -536870913, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x20000001;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0x20000001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X8, 188);

        // rs1_val == -1073741825, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x40000001;  immval:0x12
        c.li(Reg::X10, MASK_XLEN(-0x40000001));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x12));
        c.sw(Reg::X11, Reg::X8, 192);

        // rs1_val == 1431655765, rs1_val==1431655765
        // opcode: srli ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x13
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x13));
        c.sw(Reg::X11, Reg::X8, 196);

        // rs1_val == -1431655766, rs1_val==-1431655766
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0xc
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0xc));
        c.sw(Reg::X11, Reg::X8, 200);

        // rs1_val==5, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x5;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X8, 204);

        // rs1_val==1717986919, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x17
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x17));
        c.sw(Reg::X11, Reg::X8, 208);

        // rs1_val==-46339, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0xb503;  immval:0xa
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0xa));
        c.sw(Reg::X11, Reg::X8, 212);

        // rs1_val==46341, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0xb505;  immval:0xd
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0xd));
        c.sw(Reg::X11, Reg::X8, 216);

        // rs1_val==858993459, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x33333333;  immval:0xc
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0xc));
        c.sw(Reg::X11, Reg::X8, 220);

        // rs1_val==1717986918, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x15
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x15));
        c.sw(Reg::X11, Reg::X8, 224);

        // rs1_val==46340, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0xb504;  immval:0xf
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0xf));
        c.sw(Reg::X11, Reg::X8, 228);

        // rs1_val==1431655764, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x1d
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x1d));
        c.sw(Reg::X11, Reg::X8, 232);

        // rs1_val==858993458, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x1b
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x1b));
        c.sw(Reg::X11, Reg::X8, 236);

        // rs1_val==1717986917, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x9
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x9));
        c.sw(Reg::X11, Reg::X8, 240);

        // rs1_val==46339, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0xb503;  immval:0x12
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x12));
        c.sw(Reg::X11, Reg::X8, 244);

        // rs1_val==1431655766, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x55555556;  immval:0xf
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0xf));
        c.sw(Reg::X11, Reg::X8, 248);

        // rs1_val==-1431655765, 
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x1d
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x1d));
        c.sw(Reg::X11, Reg::X8, 252);

        // rs1_val==6, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x6;  immval:0x1f
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x1f));
        c.sw(Reg::X11, Reg::X8, 256);

        // rs1_val==858993460, 
        // opcode: srli ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X8, 260);

        // rs1_val == imm_val and imm_val > 0 and imm_val < xlen, rs1_val==3
        // opcode: srli ; op1:x10; dest:x11; op1val:0x3;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X8, 264);

        // rs1_val == (-2**(xlen-1)) and imm_val >= 0 and imm_val < xlen, rs1_val == -2147483648
        // opcode: srli ; op1:x10; dest:x11; op1val:-0x80000000;  immval:0x9
        c.li(Reg::X10, MASK_XLEN(-0x80000000));
        c.srli(Reg::X11, Reg::X10, SEXT_IMM(0x9));
        c.sw(Reg::X11, Reg::X8, 268);


                
        // End
        c.ecall();
        return c;
    }

    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> srli_01::s_ScalarData;
}


// Generate google tests

TEST_F(srli_01, opcode__srli___op1_x30__dest_x8__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[0], 1073730239);
}


TEST_F(srli_01, opcode__srli___op1_x17__dest_x17__op1val_0x7___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[1], 0);
}


TEST_F(srli_01, opcode__srli___op1_x27__dest_x19__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[2], 4294920956);
}


TEST_F(srli_01, opcode__srli___op1_x29__dest_x9__op1val_0x3fffffff___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[3], 1073741823);
}


TEST_F(srli_01, opcode__srli___op1_x25__dest_x22__op1val__0xa___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[4], 1);
}


TEST_F(srli_01, opcode__srli___op1_x1__dest_x13__op1val_0x200___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(srli_01, opcode__srli___op1_x21__dest_x0__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[6], 0);
}


TEST_F(srli_01, opcode__srli___op1_x0__dest_x29__op1val_0x0___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[7], 0);
}


TEST_F(srli_01, opcode__srli___op1_x16__dest_x18__op1val_0x0___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[8], 0);
}


TEST_F(srli_01, opcode__srli___op1_x20__dest_x27__op1val_0x7fffffff___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[9], 16383);
}


TEST_F(srli_01, opcode__srli___op1_x31__dest_x2__op1val_0x1___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(srli_01, opcode__srli___op1_x7__dest_x31__op1val_0x2___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[11], 0);
}


TEST_F(srli_01, opcode__srli___op1_x14__dest_x16__op1val_0x4___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(srli_01, opcode__srli___op1_x12__dest_x25__op1val_0x8___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(srli_01, opcode__srli___op1_x4__dest_x11__op1val_0x10___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[14], 0);
}


TEST_F(srli_01, opcode__srli___op1_x24__dest_x23__op1val_0x20___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(srli_01, opcode__srli___op1_x8__dest_x28__op1val_0x40___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[16], 0);
}


TEST_F(srli_01, opcode__srli___op1_x15__dest_x30__op1val_0x80___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(srli_01, opcode__srli___op1_x18__dest_x20__op1val_0x100___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[18], 0);
}


TEST_F(srli_01, opcode__srli___op1_x13__dest_x14__op1val_0x400___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[19], 0);
}


TEST_F(srli_01, opcode__srli___op1_x11__dest_x3__op1val_0x800___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(srli_01, opcode__srli___op1_x23__dest_x26__op1val_0x1000___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[21], 0);
}


TEST_F(srli_01, opcode__srli___op1_x19__dest_x10__op1val_0x2000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[22], 2048);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x21__op1val_0x4000___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[23], 1);
}


TEST_F(srli_01, opcode__srli___op1_x5__dest_x7__op1val_0x8000___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[24], 4096);
}


TEST_F(srli_01, opcode__srli___op1_x2__dest_x5__op1val_0x10000___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[25], 0);
}


TEST_F(srli_01, opcode__srli___op1_x28__dest_x6__op1val_0x20000___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[26], 1);
}


TEST_F(srli_01, opcode__srli___op1_x3__dest_x12__op1val_0x40000___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[27], 2048);
}


TEST_F(srli_01, opcode__srli___op1_x26__dest_x4__op1val_0x80000___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[28], 0);
}


TEST_F(srli_01, opcode__srli___op1_x9__dest_x24__op1val_0x100000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[29], 262144);
}


TEST_F(srli_01, opcode__srli___op1_x6__dest_x15__op1val_0x200000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[30], 65536);
}


TEST_F(srli_01, opcode__srli___op1_x22__dest_x1__op1val_0x400000___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[31], 1024);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x800000___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[32], 16384);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x1000000___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[33], 1024);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x2000000___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[34], 4194304);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x4000000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[35], 4194304);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x8000000___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[36], 1048576);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x10000000___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[37], 2097152);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x20000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[38], 536870912);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x40000000___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[39], 1);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x2___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[40], 7);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x3___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[41], 2097151);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[42], 134217727);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x9___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[43], 2147483643);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x11___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[44], 7);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x21___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[45], 65535);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x41___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[46], 16777215);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x81___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[47], 16383);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x101___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[48], 536870879);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x201___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[49], 8191);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x401___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[50], 8388605);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x801___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[51], 33554415);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x1001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[52], 4294963199);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x2001___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[53], 262143);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x4001___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[54], 2147475455);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x8001___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[55], 7);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x10001___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[56], 8388479);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x20001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[57], 33553407);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x40001___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[58], 8191);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x80001___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[59], 131055);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x100001___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[60], 65519);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x200001___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[61], 67076095);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x400001___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[62], 16760831);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x800001___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[63], 4186111);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x1000001___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[64], 2088959);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x2000001___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[65], 2130706431);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x4000001___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[66], 2015);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x8000001___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[67], 1983);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x10000001___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[68], 245759);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x20000001___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[69], 939524095);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x40000001___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[70], 12287);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x55555555___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[71], 2730);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x55555556___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[72], 699050);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[73], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x66666667___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[74], 204);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0xb503___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[75], 4194258);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0xb505___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[76], 5);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x33333333___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[77], 209715);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x66666666___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[78], 819);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0xb504___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[79], 1);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x55555554___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[80], 2);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x33333332___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[81], 6);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x66666665___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[82], 3355443);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0xb503___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[83], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x55555556___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[84], 43690);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x55555555___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[85], 5);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x6___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[86], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[87], 13421772);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[88], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x80000000___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[89], 4194304);
}

