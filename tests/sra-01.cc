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

using namespace Xbyak_riscv;

namespace
{
class sra_01 : public testing::Test 
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
        
        c.li(Reg::X20, 0);
        // rs2 == rd != rs1, rs1==x16, rs2==x27, rd==x27, rs1_val < 0 and rs2_val > 0 and rs2_val < xlen, rs1_val == -2147483648, rs2_val == 8, rs1_val == (-2**(xlen-1)) and rs2_val >= 0 and rs2_val < xlen
        // opcode: sra ; op1:x16; op2:x27; dest:x27; op1val:-0x80000000;  op2val:0x8
        c.li(Reg::X16, MASK_XLEN(-0x80000000));
        c.li(Reg::X27, MASK_XLEN(0x8));
        c.sra(Reg::X27, Reg::X16, Reg::X27);
        c.sw(Reg::X27, Reg::X20, 0);

        // rs1 == rs2 != rd, rs1==x12, rs2==x12, rd==x16, rs1_val > 0 and rs2_val > 0 and rs2_val < xlen, rs1_val == 33554432
        // opcode: sra ; op1:x12; op2:x12; dest:x16; op1val:0x2000000;  op2val:0x2000000
        c.li(Reg::X12, MASK_XLEN(0x2000000));
        c.li(Reg::X12, MASK_XLEN(0x2000000));
        c.sra(Reg::X16, Reg::X12, Reg::X12);
        c.sw(Reg::X16, Reg::X20, 4);

        // rs1 == rs2 == rd, rs1==x1, rs2==x1, rd==x1, rs1_val < 0 and rs2_val == 0, rs1_val == -2049
        // opcode: sra ; op1:x1; op2:x1; dest:x1; op1val:-0x801;  op2val:-0x801
        c.li(Reg::X1, MASK_XLEN(-0x801));
        c.li(Reg::X1, MASK_XLEN(-0x801));
        c.sra(Reg::X1, Reg::X1, Reg::X1);
        c.sw(Reg::X1, Reg::X20, 8);

        // rs1 == rd != rs2, rs1==x13, rs2==x19, rd==x13, rs1_val > 0 and rs2_val == 0, rs1_val==858993459
        // opcode: sra ; op1:x13; op2:x19; dest:x13; op1val:0x33333333;  op2val:0x0
        c.li(Reg::X13, MASK_XLEN(0x33333333));
        c.li(Reg::X19, MASK_XLEN(0x0));
        c.sra(Reg::X13, Reg::X13, Reg::X19);
        c.sw(Reg::X13, Reg::X20, 12);

        // rs1 != rs2  and rs1 != rd and rs2 != rd, rs1==x28, rs2==x2, rd==x8, rs1_val == rs2_val and rs2_val > 0 and rs2_val < xlen, rs1_val==6
        // opcode: sra ; op1:x28; op2:x2; dest:x8; op1val:0x6;  op2val:0x6
        c.li(Reg::X28, MASK_XLEN(0x6));
        c.li(Reg::X2, MASK_XLEN(0x6));
        c.sra(Reg::X8, Reg::X28, Reg::X2);
        c.sw(Reg::X8, Reg::X20, 16);

        // rs1==x26, rs2==x31, rd==x19, rs1_val == 0 and rs2_val >= 0 and rs2_val < xlen, rs1_val==0
        // opcode: sra ; op1:x26; op2:x31; dest:x19; op1val:0x0;  op2val:0x3
        c.li(Reg::X26, MASK_XLEN(0x0));
        c.li(Reg::X31, MASK_XLEN(0x3));
        c.sra(Reg::X19, Reg::X26, Reg::X31);
        c.sw(Reg::X19, Reg::X20, 20);

        // rs1==x14, rs2==x28, rd==x29, rs1_val == (2**(xlen-1)-1) and rs2_val >= 0 and rs2_val < xlen, rs1_val == 2147483647
        // opcode: sra ; op1:x14; op2:x28; dest:x29; op1val:0x7fffffff;  op2val:0x12
        c.li(Reg::X14, MASK_XLEN(0x7fffffff));
        c.li(Reg::X28, MASK_XLEN(0x12));
        c.sra(Reg::X29, Reg::X14, Reg::X28);
        c.sw(Reg::X29, Reg::X20, 24);

        // rs1==x10, rs2==x26, rd==x12, rs1_val == 1 and rs2_val >= 0 and rs2_val < xlen, rs1_val == 1, rs2_val == 2
        // opcode: sra ; op1:x10; op2:x26; dest:x12; op1val:0x1;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.li(Reg::X26, MASK_XLEN(0x2));
        c.sra(Reg::X12, Reg::X10, Reg::X26);
        c.sw(Reg::X12, Reg::X20, 28);

        // rs1==x30, rs2==x16, rd==x15, rs1_val == 2, rs2_val == 4, rs1_val==2
        // opcode: sra ; op1:x30; op2:x16; dest:x15; op1val:0x2;  op2val:0x4
        c.li(Reg::X30, MASK_XLEN(0x2));
        c.li(Reg::X16, MASK_XLEN(0x4));
        c.sra(Reg::X15, Reg::X30, Reg::X16);
        c.sw(Reg::X15, Reg::X20, 32);

        // rs1==x24, rs2==x0, rd==x6, rs1_val == 4, rs1_val==4
        // opcode: sra ; op1:x24; op2:x0; dest:x6; op1val:0x4;  op2val:0x0
        c.li(Reg::X24, MASK_XLEN(0x4));
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.sra(Reg::X6, Reg::X24, Reg::X0);
        c.sw(Reg::X6, Reg::X20, 36);

        // rs1==x21, rs2==x15, rd==x3, rs1_val == 8, rs2_val == 10
        // opcode: sra ; op1:x21; op2:x15; dest:x3; op1val:0x8;  op2val:0xa
        c.li(Reg::X21, MASK_XLEN(0x8));
        c.li(Reg::X15, MASK_XLEN(0xa));
        c.sra(Reg::X3, Reg::X21, Reg::X15);
        c.sw(Reg::X3, Reg::X20, 40);

        // rs1==x15, rs2==x30, rd==x10, rs1_val == 16, 
        // opcode: sra ; op1:x15; op2:x30; dest:x10; op1val:0x10;  op2val:0x0
        c.li(Reg::X15, MASK_XLEN(0x10));
        c.li(Reg::X30, MASK_XLEN(0x0));
        c.sra(Reg::X10, Reg::X15, Reg::X30);
        c.sw(Reg::X10, Reg::X20, 44);

        // rs1==x18, rs2==x4, rd==x22, rs1_val == 32, 
        // opcode: sra ; op1:x18; op2:x4; dest:x22; op1val:0x20;  op2val:0x3
        c.li(Reg::X18, MASK_XLEN(0x20));
        c.li(Reg::X4, MASK_XLEN(0x3));
        c.sra(Reg::X22, Reg::X18, Reg::X4);
        c.sw(Reg::X22, Reg::X20, 48);

        // rs1==x19, rs2==x17, rd==x11, rs1_val == 64, rs2_val == 23
        // opcode: sra ; op1:x19; op2:x17; dest:x11; op1val:0x40;  op2val:0x17
        c.li(Reg::X19, MASK_XLEN(0x40));
        c.li(Reg::X17, MASK_XLEN(0x17));
        c.sra(Reg::X11, Reg::X19, Reg::X17);
        c.sw(Reg::X11, Reg::X20, 52);

        // rs1==x5, rs2==x14, rd==x0, rs1_val == 128, 
        // opcode: sra ; op1:x5; op2:x14; dest:x0; op1val:0x80;  op2val:0x8
        c.li(Reg::X5, MASK_XLEN(0x80));
        c.li(Reg::X14, MASK_XLEN(0x8));
        c.sra(Reg::X0, Reg::X5, Reg::X14);
        c.sw(Reg::X0, Reg::X20, 56);

        // rs1==x6, rs2==x24, rd==x5, rs1_val == 256, 
        // opcode: sra ; op1:x6; op2:x24; dest:x5; op1val:0x100;  op2val:0x4
        c.li(Reg::X6, MASK_XLEN(0x100));
        c.li(Reg::X24, MASK_XLEN(0x4));
        c.sra(Reg::X5, Reg::X6, Reg::X24);
        c.sw(Reg::X5, Reg::X20, 60);

        // rs1==x7, rs2==x18, rd==x31, rs1_val == 512, 
        // opcode: sra ; op1:x7; op2:x18; dest:x31; op1val:0x200;  op2val:0x1f
        c.li(Reg::X7, MASK_XLEN(0x200));
        c.li(Reg::X18, MASK_XLEN(0x1f));
        c.sra(Reg::X31, Reg::X7, Reg::X18);
        c.sw(Reg::X31, Reg::X20, 64);

        // rs1==x31, rs2==x21, rd==x30, rs1_val == 1024, rs2_val == 16
        // opcode: sra ; op1:x31; op2:x21; dest:x30; op1val:0x400;  op2val:0x10
        c.li(Reg::X31, MASK_XLEN(0x400));
        c.li(Reg::X21, MASK_XLEN(0x10));
        c.sra(Reg::X30, Reg::X31, Reg::X21);
        c.sw(Reg::X30, Reg::X20, 68);

        // rs1==x4, rs2==x11, rd==x2, rs1_val == 2048, 
        // opcode: sra ; op1:x4; op2:x11; dest:x2; op1val:0x800;  op2val:0x13
        c.li(Reg::X4, MASK_XLEN(0x800));
        c.li(Reg::X11, MASK_XLEN(0x13));
        c.sra(Reg::X2, Reg::X4, Reg::X11);
        c.sw(Reg::X2, Reg::X20, 72);

        // rs1==x22, rs2==x25, rd==x9, rs1_val == 4096, 
        // opcode: sra ; op1:x22; op2:x25; dest:x9; op1val:0x1000;  op2val:0x2
        c.li(Reg::X22, MASK_XLEN(0x1000));
        c.li(Reg::X25, MASK_XLEN(0x2));
        c.sra(Reg::X9, Reg::X22, Reg::X25);
        c.sw(Reg::X9, Reg::X20, 76);

        c.li(Reg::X1, 80);
        // rs1==x23, rs2==x13, rd==x14, rs1_val == 8192, 
        // opcode: sra ; op1:x23; op2:x13; dest:x14; op1val:0x2000;  op2val:0x1f
        c.li(Reg::X23, MASK_XLEN(0x2000));
        c.li(Reg::X13, MASK_XLEN(0x1f));
        c.sra(Reg::X14, Reg::X23, Reg::X13);
        c.sw(Reg::X14, Reg::X1, 0);

        // rs1==x8, rs2==x29, rd==x28, rs1_val == 16384, 
        // opcode: sra ; op1:x8; op2:x29; dest:x28; op1val:0x4000;  op2val:0xa
        c.li(Reg::X8, MASK_XLEN(0x4000));
        c.li(Reg::X29, MASK_XLEN(0xa));
        c.sra(Reg::X28, Reg::X8, Reg::X29);
        c.sw(Reg::X28, Reg::X1, 4);

        // rs1==x17, rs2==x8, rd==x26, rs1_val == 32768, 
        // opcode: sra ; op1:x17; op2:x8; dest:x26; op1val:0x8000;  op2val:0x13
        c.li(Reg::X17, MASK_XLEN(0x8000));
        c.li(Reg::X8, MASK_XLEN(0x13));
        c.sra(Reg::X26, Reg::X17, Reg::X8);
        c.sw(Reg::X26, Reg::X1, 8);

        // rs1==x29, rs2==x3, rd==x23, rs1_val == 65536, 
        // opcode: sra ; op1:x29; op2:x3; dest:x23; op1val:0x10000;  op2val:0x10
        c.li(Reg::X29, MASK_XLEN(0x10000));
        c.li(Reg::X3, MASK_XLEN(0x10));
        c.sra(Reg::X23, Reg::X29, Reg::X3);
        c.sw(Reg::X23, Reg::X1, 12);

        // rs1==x9, rs2==x5, rd==x7, rs1_val == 131072, 
        // opcode: sra ; op1:x9; op2:x5; dest:x7; op1val:0x20000;  op2val:0x0
        c.li(Reg::X9, MASK_XLEN(0x20000));
        c.li(Reg::X5, MASK_XLEN(0x0));
        c.sra(Reg::X7, Reg::X9, Reg::X5);
        c.sw(Reg::X7, Reg::X1, 16);

        // rs1==x20, rs2==x7, rd==x24, rs1_val == 262144, 
        // opcode: sra ; op1:x20; op2:x7; dest:x24; op1val:0x40000;  op2val:0x8
        c.li(Reg::X20, MASK_XLEN(0x40000));
        c.li(Reg::X7, MASK_XLEN(0x8));
        c.sra(Reg::X24, Reg::X20, Reg::X7);
        c.sw(Reg::X24, Reg::X1, 20);

        // rs1==x25, rs2==x10, rd==x17, rs1_val == 524288, 
        // opcode: sra ; op1:x25; op2:x10; dest:x17; op1val:0x80000;  op2val:0x8
        c.li(Reg::X25, MASK_XLEN(0x80000));
        c.li(Reg::X10, MASK_XLEN(0x8));
        c.sra(Reg::X17, Reg::X25, Reg::X10);
        c.sw(Reg::X17, Reg::X1, 24);

        // rs1==x27, rs2==x22, rd==x20, rs1_val == 1048576, rs2_val == 27
        // opcode: sra ; op1:x27; op2:x22; dest:x20; op1val:0x100000;  op2val:0x1b
        c.li(Reg::X27, MASK_XLEN(0x100000));
        c.li(Reg::X22, MASK_XLEN(0x1b));
        c.sra(Reg::X20, Reg::X27, Reg::X22);
        c.sw(Reg::X20, Reg::X1, 28);

        // rs1==x3, rs2==x6, rd==x25, rs1_val == 2097152, 
        // opcode: sra ; op1:x3; op2:x6; dest:x25; op1val:0x200000;  op2val:0xb
        c.li(Reg::X3, MASK_XLEN(0x200000));
        c.li(Reg::X6, MASK_XLEN(0xb));
        c.sra(Reg::X25, Reg::X3, Reg::X6);
        c.sw(Reg::X25, Reg::X1, 32);

        // rs1==x0, rs2==x23, rd==x4, rs1_val == 4194304, 
        // opcode: sra ; op1:x0; op2:x23; dest:x4; op1val:0x0;  op2val:0x8
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.li(Reg::X23, MASK_XLEN(0x8));
        c.sra(Reg::X4, Reg::X0, Reg::X23);
        c.sw(Reg::X4, Reg::X1, 36);

        // rs1==x2, rs2==x20, rd==x18, rs1_val == 8388608, 
        // opcode: sra ; op1:x2; op2:x20; dest:x18; op1val:0x800000;  op2val:0x1b
        c.li(Reg::X2, MASK_XLEN(0x800000));
        c.li(Reg::X20, MASK_XLEN(0x1b));
        c.sra(Reg::X18, Reg::X2, Reg::X20);
        c.sw(Reg::X18, Reg::X1, 40);

        // rs1==x11, rs2==x9, rd==x21, rs1_val == 16777216, 
        // opcode: sra ; op1:x11; op2:x9; dest:x21; op1val:0x1000000;  op2val:0xb
        c.li(Reg::X11, MASK_XLEN(0x1000000));
        c.li(Reg::X9, MASK_XLEN(0xb));
        c.sra(Reg::X21, Reg::X11, Reg::X9);
        c.sw(Reg::X21, Reg::X1, 44);

        // rs1_val == 67108864, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x4000000;  op2val:0xc
        c.li(Reg::X10, MASK_XLEN(0x4000000));
        c.li(Reg::X11, MASK_XLEN(0xc));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 48);

        // rs1_val == 134217728, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x8000000;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x8000000));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 52);

        // rs1_val == 268435456, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x10000000;  op2val:0xb
        c.li(Reg::X10, MASK_XLEN(0x10000000));
        c.li(Reg::X11, MASK_XLEN(0xb));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 56);

        // rs1_val == 536870912, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x20000000;  op2val:0xa
        c.li(Reg::X10, MASK_XLEN(0x20000000));
        c.li(Reg::X11, MASK_XLEN(0xa));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 60);

        // rs1_val == 1073741824, rs2_val == 15
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x40000000;  op2val:0xf
        c.li(Reg::X10, MASK_XLEN(0x40000000));
        c.li(Reg::X11, MASK_XLEN(0xf));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 64);

        // rs1_val == -2, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x2;  op2val:0x7
        c.li(Reg::X10, MASK_XLEN(-0x2));
        c.li(Reg::X11, MASK_XLEN(0x7));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 68);

        // rs1_val == -3, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x3;  op2val:0x12
        c.li(Reg::X10, MASK_XLEN(-0x3));
        c.li(Reg::X11, MASK_XLEN(0x12));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 72);

        // rs1_val == -5, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x5;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x5));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 76);

        // rs1_val == -9, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x9;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x9));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 80);

        // rs1_val == -17, rs2_val == 29
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x11;  op2val:0x1d
        c.li(Reg::X10, MASK_XLEN(-0x11));
        c.li(Reg::X11, MASK_XLEN(0x1d));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 84);

        // rs1_val == -33, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x21;  op2val:0xd
        c.li(Reg::X10, MASK_XLEN(-0x21));
        c.li(Reg::X11, MASK_XLEN(0xd));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 88);

        // rs1_val == -65, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x41;  op2val:0x17
        c.li(Reg::X10, MASK_XLEN(-0x41));
        c.li(Reg::X11, MASK_XLEN(0x17));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 92);

        // rs1_val == -129, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x81;  op2val:0xd
        c.li(Reg::X10, MASK_XLEN(-0x81));
        c.li(Reg::X11, MASK_XLEN(0xd));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 96);

        // rs1_val == -257, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x101;  op2val:0x13
        c.li(Reg::X10, MASK_XLEN(-0x101));
        c.li(Reg::X11, MASK_XLEN(0x13));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 100);

        // rs1_val == -513, rs2_val == 1
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x201;  op2val:0x1
        c.li(Reg::X10, MASK_XLEN(-0x201));
        c.li(Reg::X11, MASK_XLEN(0x1));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 104);

        // rs1_val == -1025, rs2_val == 21
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x401;  op2val:0x15
        c.li(Reg::X10, MASK_XLEN(-0x401));
        c.li(Reg::X11, MASK_XLEN(0x15));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 108);

        // rs1_val == -4097, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x1001;  op2val:0xe
        c.li(Reg::X10, MASK_XLEN(-0x1001));
        c.li(Reg::X11, MASK_XLEN(0xe));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 112);

        // rs1_val == -8193, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x2001;  op2val:0x8
        c.li(Reg::X10, MASK_XLEN(-0x2001));
        c.li(Reg::X11, MASK_XLEN(0x8));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 116);

        // rs1_val == -16385, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x4001;  op2val:0xe
        c.li(Reg::X10, MASK_XLEN(-0x4001));
        c.li(Reg::X11, MASK_XLEN(0xe));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 120);

        // rs1_val == -32769, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x8001;  op2val:0xc
        c.li(Reg::X10, MASK_XLEN(-0x8001));
        c.li(Reg::X11, MASK_XLEN(0xc));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 124);

        // rs1_val == -65537, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x10001;  op2val:0x17
        c.li(Reg::X10, MASK_XLEN(-0x10001));
        c.li(Reg::X11, MASK_XLEN(0x17));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 128);

        // rs1_val == -131073, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x20001;  op2val:0xa
        c.li(Reg::X10, MASK_XLEN(-0x20001));
        c.li(Reg::X11, MASK_XLEN(0xa));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 132);

        // rs1_val == -262145, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x40001;  op2val:0xb
        c.li(Reg::X10, MASK_XLEN(-0x40001));
        c.li(Reg::X11, MASK_XLEN(0xb));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 136);

        // rs1_val == -524289, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x80001;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x80001));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 140);

        // rs1_val == -8388609, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x800001;  op2val:0x13
        c.li(Reg::X10, MASK_XLEN(-0x800001));
        c.li(Reg::X11, MASK_XLEN(0x13));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 144);

        // rs1_val == -16777217, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x1000001;  op2val:0xc
        c.li(Reg::X10, MASK_XLEN(-0x1000001));
        c.li(Reg::X11, MASK_XLEN(0xc));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 148);

        // rs1_val == -33554433, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x2000001;  op2val:0x1f
        c.li(Reg::X10, MASK_XLEN(-0x2000001));
        c.li(Reg::X11, MASK_XLEN(0x1f));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 152);

        // rs1_val == -67108865, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x4000001;  op2val:0xb
        c.li(Reg::X10, MASK_XLEN(-0x4000001));
        c.li(Reg::X11, MASK_XLEN(0xb));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 156);

        // rs1_val == -134217729, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x8000001;  op2val:0xa
        c.li(Reg::X10, MASK_XLEN(-0x8000001));
        c.li(Reg::X11, MASK_XLEN(0xa));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 160);

        // rs1_val == -268435457, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x10000001;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x10000001));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 164);

        // rs1_val == -536870913, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x20000001;  op2val:0x7
        c.li(Reg::X10, MASK_XLEN(-0x20000001));
        c.li(Reg::X11, MASK_XLEN(0x7));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 168);

        // rs1_val == -1073741825, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x40000001;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x40000001));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 172);

        // rs1_val == 1431655765, rs1_val==1431655765
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 176);

        // rs1_val == -1431655766, rs1_val==-1431655766
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x13
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x13));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 180);

        // rs1_val==3, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x17
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x17));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 184);

        // rs1_val==5, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xe
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xe));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 188);

        // rs1_val==1717986918, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x13
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x13));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 192);

        // rs1_val==-46340, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x1
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x1));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 196);

        // rs1_val==46341, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x8
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x8));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 200);

        // rs2_val == 30, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x1e
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x1e));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 204);

        // rs1_val==46340, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xe
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xe));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 208);

        // rs1_val==1431655764, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xc
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xc));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 212);

        // rs1_val == -4194305, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x400001;  op2val:0x13
        c.li(Reg::X10, MASK_XLEN(-0x400001));
        c.li(Reg::X11, MASK_XLEN(0x13));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 216);

        // rs1_val==858993460, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x7
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x7));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 220);

        // rs1_val == -2097153, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x200001;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0x200001));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 224);

        // rs1_val==858993458, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 228);

        // rs1_val==1717986917, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x17
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x17));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 232);

        // rs1_val==46339, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x1f
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x1f));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 236);

        // rs1_val==1431655766, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 240);

        // rs1_val==-1431655765, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x1
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x1));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 244);

        // rs1_val == -1048577, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x100001;  op2val:0x10
        c.li(Reg::X10, MASK_XLEN(-0x100001));
        c.li(Reg::X11, MASK_XLEN(0x10));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 248);

        // rs1_val==1717986919, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 252);

        // rs1_val==-46339, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xd
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xd));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 256);

        // rs1_val > 0 and rs2_val > 0 and rs2_val < xlen, rs1_val == 33554432
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x2000000;  op2val:0x12
        c.li(Reg::X10, MASK_XLEN(0x2000000));
        c.li(Reg::X11, MASK_XLEN(0x12));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 260);

        // rs1_val < 0 and rs2_val == 0, rs1_val == -2049
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:-0x801;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x801));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 264);

        // rs1_val == 4, rs1_val==4
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 268);

        // rs1_val == 128, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x80;  op2val:0x8
        c.li(Reg::X10, MASK_XLEN(0x80));
        c.li(Reg::X11, MASK_XLEN(0x8));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 272);

        // rs1_val == 4194304, 
        // opcode: sra ; op1:x10; op2:x11; dest:x12; op1val:0x400000;  op2val:0x8
        c.li(Reg::X10, MASK_XLEN(0x400000));
        c.li(Reg::X11, MASK_XLEN(0x8));
        c.sra(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 276);


                
        // End
        c.ecall();
        return c;
    }

    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> sra_01::s_ScalarData;
}


// Generate google tests

TEST_F(sra_01, opcode__sra___op1_x16__op2_x27__dest_x27__op1val__0x80000000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[0], -8388608);
}


TEST_F(sra_01, opcode__sra___op1_x12__op2_x12__dest_x16__op1val_0x2000000___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[1], 33554432);
}


TEST_F(sra_01, opcode__sra___op1_x1__op2_x1__dest_x1__op1val__0x801___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[2], -1);
}


TEST_F(sra_01, opcode__sra___op1_x13__op2_x19__dest_x13__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[3], 858993459);
}


TEST_F(sra_01, opcode__sra___op1_x28__op2_x2__dest_x8__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[4], 0);
}


TEST_F(sra_01, opcode__sra___op1_x26__op2_x31__dest_x19__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(sra_01, opcode__sra___op1_x14__op2_x28__dest_x29__op1val_0x7fffffff___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[6], 8191);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x26__dest_x12__op1val_0x1___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[7], 0);
}


TEST_F(sra_01, opcode__sra___op1_x30__op2_x16__dest_x15__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[8], 0);
}


TEST_F(sra_01, opcode__sra___op1_x24__op2_x0__dest_x6__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[9], 4);
}


TEST_F(sra_01, opcode__sra___op1_x21__op2_x15__dest_x3__op1val_0x8___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(sra_01, opcode__sra___op1_x15__op2_x30__dest_x10__op1val_0x10___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[11], 16);
}


TEST_F(sra_01, opcode__sra___op1_x18__op2_x4__dest_x22__op1val_0x20___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[12], 4);
}


TEST_F(sra_01, opcode__sra___op1_x19__op2_x17__dest_x11__op1val_0x40___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(sra_01, opcode__sra___op1_x5__op2_x14__dest_x0__op1val_0x80___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[14], 0);
}


TEST_F(sra_01, opcode__sra___op1_x6__op2_x24__dest_x5__op1val_0x100___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[15], 16);
}


TEST_F(sra_01, opcode__sra___op1_x7__op2_x18__dest_x31__op1val_0x200___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[16], 0);
}


TEST_F(sra_01, opcode__sra___op1_x31__op2_x21__dest_x30__op1val_0x400___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(sra_01, opcode__sra___op1_x4__op2_x11__dest_x2__op1val_0x800___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[18], 0);
}


TEST_F(sra_01, opcode__sra___op1_x22__op2_x25__dest_x9__op1val_0x1000___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[19], 1024);
}


TEST_F(sra_01, opcode__sra___op1_x23__op2_x13__dest_x14__op1val_0x2000___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(sra_01, opcode__sra___op1_x8__op2_x29__dest_x28__op1val_0x4000___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[21], 16);
}


TEST_F(sra_01, opcode__sra___op1_x17__op2_x8__dest_x26__op1val_0x8000___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[22], 0);
}


TEST_F(sra_01, opcode__sra___op1_x29__op2_x3__dest_x23__op1val_0x10000___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[23], 1);
}


TEST_F(sra_01, opcode__sra___op1_x9__op2_x5__dest_x7__op1val_0x20000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[24], 131072);
}


TEST_F(sra_01, opcode__sra___op1_x20__op2_x7__dest_x24__op1val_0x40000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[25], 1024);
}


TEST_F(sra_01, opcode__sra___op1_x25__op2_x10__dest_x17__op1val_0x80000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[26], 2048);
}


TEST_F(sra_01, opcode__sra___op1_x27__op2_x22__dest_x20__op1val_0x100000___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[27], 0);
}


TEST_F(sra_01, opcode__sra___op1_x3__op2_x6__dest_x25__op1val_0x200000___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[28], 1024);
}


TEST_F(sra_01, opcode__sra___op1_x0__op2_x23__dest_x4__op1val_0x0___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[29], 0);
}


TEST_F(sra_01, opcode__sra___op1_x2__op2_x20__dest_x18__op1val_0x800000___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[30], 0);
}


TEST_F(sra_01, opcode__sra___op1_x11__op2_x9__dest_x21__op1val_0x1000000___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[31], 8192);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[32], 16384);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[33], 8388608);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[34], 131072);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[35], 524288);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[36], 32768);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[37], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[38], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[39], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[40], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[41], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[42], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[43], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[44], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[45], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[46], -257);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[47], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[48], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[49], -33);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[50], -2);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[51], -9);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[52], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[53], -129);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[54], -129);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[55], -524289);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[56], -17);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[57], -4097);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[58], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[59], -32769);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[60], -131073);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[61], -16777217);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[62], -4194305);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[63], -33554433);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[64], 699050);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[65], -2731);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[66], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[67], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[68], 3276);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[69], -23170);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[70], 181);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[71], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[72], 2);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[73], 349525);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[74], -9);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[75], 6710886);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[76], -524289);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[77], 858993458);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[78], 204);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[79], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[80], 44739242);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[81], -715827883);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[82], -17);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[83], 1717986919);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[84], -6);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[85], 128);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[86], -2049);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[87], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[88], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[89], 16384);
}

