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
class sub_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        // Generate word-aligned vector to initialise memory
        const std::vector<uint32_t> scalarWordInitData{
        3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,};
        
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
        
        c.li(Reg::X1, 0);
        // rs2 == rd != rs1, rs1==x24, rs2==x26, rd==x26, rs1_val > 0 and rs2_val > 0, rs1_val != rs2_val, rs1_val==1431655764 and rs2_val==6
        // opcode: sub ; op1:x24; op2:x26; dest:x26; op1val:0x55555554;  op2val:0x6
        c.li(Reg::X24, MASK_XLEN(0x55555554));
        c.li(Reg::X26, MASK_XLEN(0x6));
        c.sub(Reg::X26, Reg::X24, Reg::X26);
        c.sw(Reg::X26, Reg::X1, 0);

        // rs1 == rs2 != rd, rs1==x17, rs2==x17, rd==x23, rs1_val > 0 and rs2_val < 0, rs1_val == 33554432
        // opcode: sub ; op1:x17; op2:x17; dest:x23; op1val:0x2000000;  op2val:0x2000000
        c.li(Reg::X17, MASK_XLEN(0x2000000));
        c.li(Reg::X17, MASK_XLEN(0x2000000));
        c.sub(Reg::X23, Reg::X17, Reg::X17);
        c.sw(Reg::X23, Reg::X1, 4);

        // rs1 == rs2 == rd, rs1==x16, rs2==x16, rd==x16, rs1_val < 0 and rs2_val < 0, rs2_val == -129
        // opcode: sub ; op1:x16; op2:x16; dest:x16; op1val:-0x7;  op2val:-0x7
        c.li(Reg::X16, MASK_XLEN(-0x7));
        c.li(Reg::X16, MASK_XLEN(-0x7));
        c.sub(Reg::X16, Reg::X16, Reg::X16);
        c.sw(Reg::X16, Reg::X1, 8);

        // rs1 == rd != rs2, rs1==x31, rs2==x19, rd==x31, rs1_val < 0 and rs2_val > 0, rs1_val == -3
        // opcode: sub ; op1:x31; op2:x19; dest:x31; op1val:-0x3;  op2val:0x66666665
        c.li(Reg::X31, MASK_XLEN(-0x3));
        c.li(Reg::X19, MASK_XLEN(0x66666665));
        c.sub(Reg::X31, Reg::X31, Reg::X19);
        c.sw(Reg::X31, Reg::X1, 12);

        // rs1 != rs2  and rs1 != rd and rs2 != rd, rs1==x23, rs2==x14, rd==x8, rs1_val == rs2_val, rs2_val == 524288, rs1_val == 524288
        // opcode: sub ; op1:x23; op2:x14; dest:x8; op1val:0x80000;  op2val:0x80000
        c.li(Reg::X23, MASK_XLEN(0x80000));
        c.li(Reg::X14, MASK_XLEN(0x80000));
        c.sub(Reg::X8, Reg::X23, Reg::X14);
        c.sw(Reg::X8, Reg::X1, 16);

        // rs1==x13, rs2==x24, rd==x18, rs2_val == (-2**(xlen-1)), rs1_val == -67108865, rs2_val == -2147483648
        // opcode: sub ; op1:x13; op2:x24; dest:x18; op1val:-0x4000001;  op2val:-0x80000000
        c.li(Reg::X13, MASK_XLEN(-0x4000001));
        c.li(Reg::X24, MASK_XLEN(-0x80000000));
        c.sub(Reg::X18, Reg::X13, Reg::X24);
        c.sw(Reg::X18, Reg::X1, 20);

        // rs1==x12, rs2==x4, rd==x0, rs2_val == 0, rs1_val == 32
        // opcode: sub ; op1:x12; op2:x4; dest:x0; op1val:0x20;  op2val:0x0
        c.li(Reg::X12, MASK_XLEN(0x20));
        c.li(Reg::X4, MASK_XLEN(0x0));
        c.sub(Reg::X0, Reg::X12, Reg::X4);
        c.sw(Reg::X0, Reg::X1, 24);

        // rs1==x22, rs2==x9, rd==x10, rs2_val == (2**(xlen-1)-1), rs1_val == -536870913, rs2_val == 2147483647
        // opcode: sub ; op1:x22; op2:x9; dest:x10; op1val:-0x20000001;  op2val:0x7fffffff
        c.li(Reg::X22, MASK_XLEN(-0x20000001));
        c.li(Reg::X9, MASK_XLEN(0x7fffffff));
        c.sub(Reg::X10, Reg::X22, Reg::X9);
        c.sw(Reg::X10, Reg::X1, 28);

        // rs1==x10, rs2==x27, rd==x25, rs2_val == 1, rs1_val == 65536
        // opcode: sub ; op1:x10; op2:x27; dest:x25; op1val:0x10000;  op2val:0x1
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.li(Reg::X27, MASK_XLEN(0x1));
        c.sub(Reg::X25, Reg::X10, Reg::X27);
        c.sw(Reg::X25, Reg::X1, 32);

        // rs1==x8, rs2==x3, rd==x14, rs1_val == (-2**(xlen-1)), rs1_val == -2147483648
        // opcode: sub ; op1:x8; op2:x3; dest:x14; op1val:-0x80000000;  op2val:-0x40000000
        c.li(Reg::X8, MASK_XLEN(-0x80000000));
        c.li(Reg::X3, MASK_XLEN(-0x40000000));
        c.sub(Reg::X14, Reg::X8, Reg::X3);
        c.sw(Reg::X14, Reg::X1, 36);

        // rs1==x25, rs2==x30, rd==x29, rs1_val == 0, rs2_val == 2097152
        // opcode: sub ; op1:x25; op2:x30; dest:x29; op1val:0x0;  op2val:0x200000
        c.li(Reg::X25, MASK_XLEN(0x0));
        c.li(Reg::X30, MASK_XLEN(0x200000));
        c.sub(Reg::X29, Reg::X25, Reg::X30);
        c.sw(Reg::X29, Reg::X1, 40);

        // rs1==x18, rs2==x8, rd==x15, rs1_val == (2**(xlen-1)-1), rs2_val == 8192, rs1_val == 2147483647
        // opcode: sub ; op1:x18; op2:x8; dest:x15; op1val:0x7fffffff;  op2val:0x2000
        c.li(Reg::X18, MASK_XLEN(0x7fffffff));
        c.li(Reg::X8, MASK_XLEN(0x2000));
        c.sub(Reg::X15, Reg::X18, Reg::X8);
        c.sw(Reg::X15, Reg::X1, 44);

        // rs1==x14, rs2==x15, rd==x3, rs1_val == 1, rs2_val == 16
        // opcode: sub ; op1:x14; op2:x15; dest:x3; op1val:0x1;  op2val:0x10
        c.li(Reg::X14, MASK_XLEN(0x1));
        c.li(Reg::X15, MASK_XLEN(0x10));
        c.sub(Reg::X3, Reg::X14, Reg::X15);
        c.sw(Reg::X3, Reg::X1, 48);

        // rs1==x26, rs2==x29, rd==x13, rs2_val == 2, 
        // opcode: sub ; op1:x26; op2:x29; dest:x13; op1val:-0x7;  op2val:0x2
        c.li(Reg::X26, MASK_XLEN(-0x7));
        c.li(Reg::X29, MASK_XLEN(0x2));
        c.sub(Reg::X13, Reg::X26, Reg::X29);
        c.sw(Reg::X13, Reg::X1, 52);

        // rs1==x21, rs2==x31, rd==x19, rs2_val == 4, rs1_val==2 and rs2_val==4, rs1_val == 2
        // opcode: sub ; op1:x21; op2:x31; dest:x19; op1val:0x2;  op2val:0x4
        c.li(Reg::X21, MASK_XLEN(0x2));
        c.li(Reg::X31, MASK_XLEN(0x4));
        c.sub(Reg::X19, Reg::X21, Reg::X31);
        c.sw(Reg::X19, Reg::X1, 56);

        // rs1==x30, rs2==x5, rd==x11, rs2_val == 8, rs1_val == -16777217
        // opcode: sub ; op1:x30; op2:x5; dest:x11; op1val:-0x1000001;  op2val:0x8
        c.li(Reg::X30, MASK_XLEN(-0x1000001));
        c.li(Reg::X5, MASK_XLEN(0x8));
        c.sub(Reg::X11, Reg::X30, Reg::X5);
        c.sw(Reg::X11, Reg::X1, 60);

        // rs1==x28, rs2==x7, rd==x30, rs2_val == 32, rs1_val == -1431655766
        // opcode: sub ; op1:x28; op2:x7; dest:x30; op1val:-0x55555556;  op2val:0x20
        c.li(Reg::X28, MASK_XLEN(-0x55555556));
        c.li(Reg::X7, MASK_XLEN(0x20));
        c.sub(Reg::X30, Reg::X28, Reg::X7);
        c.sw(Reg::X30, Reg::X1, 64);

        // rs1==x9, rs2==x10, rd==x7, rs2_val == 64, rs1_val == -65
        // opcode: sub ; op1:x9; op2:x10; dest:x7; op1val:-0x41;  op2val:0x40
        c.li(Reg::X9, MASK_XLEN(-0x41));
        c.li(Reg::X10, MASK_XLEN(0x40));
        c.sub(Reg::X7, Reg::X9, Reg::X10);
        c.sw(Reg::X7, Reg::X1, 68);

        // rs1==x0, rs2==x18, rd==x17, rs2_val == 128, 
        // opcode: sub ; op1:x0; op2:x18; dest:x17; op1val:0x0;  op2val:0x80
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.li(Reg::X18, MASK_XLEN(0x80));
        c.sub(Reg::X17, Reg::X0, Reg::X18);
        c.sw(Reg::X17, Reg::X1, 72);

        // rs1==x2, rs2==x12, rd==x27, rs2_val == 256, 
        // opcode: sub ; op1:x2; op2:x12; dest:x27; op1val:-0x40000000;  op2val:0x100
        c.li(Reg::X2, MASK_XLEN(-0x40000000));
        c.li(Reg::X12, MASK_XLEN(0x100));
        c.sub(Reg::X27, Reg::X2, Reg::X12);
        c.sw(Reg::X27, Reg::X1, 76);

        c.li(Reg::X8, 80);
        // rs1==x4, rs2==x28, rd==x20, rs2_val == 512, 
        // opcode: sub ; op1:x4; op2:x28; dest:x20; op1val:-0xb503;  op2val:0x200
        c.li(Reg::X4, MASK_XLEN(-0xb503));
        c.li(Reg::X28, MASK_XLEN(0x200));
        c.sub(Reg::X20, Reg::X4, Reg::X28);
        c.sw(Reg::X20, Reg::X8, 0);

        // rs1==x15, rs2==x1, rd==x6, rs2_val == 1024, 
        // opcode: sub ; op1:x15; op2:x1; dest:x6; op1val:0x0;  op2val:0x400
        c.li(Reg::X15, MASK_XLEN(0x0));
        c.li(Reg::X1, MASK_XLEN(0x400));
        c.sub(Reg::X6, Reg::X15, Reg::X1);
        c.sw(Reg::X6, Reg::X8, 4);

        // rs1==x5, rs2==x0, rd==x9, rs2_val == 2048, 
        // opcode: sub ; op1:x5; op2:x0; dest:x9; op1val:-0x80000000;  op2val:0x0
        c.li(Reg::X5, MASK_XLEN(-0x80000000));
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.sub(Reg::X9, Reg::X5, Reg::X0);
        c.sw(Reg::X9, Reg::X8, 8);

        // rs1==x7, rs2==x11, rd==x4, rs2_val == 4096, rs1_val == 131072
        // opcode: sub ; op1:x7; op2:x11; dest:x4; op1val:0x20000;  op2val:0x1000
        c.li(Reg::X7, MASK_XLEN(0x20000));
        c.li(Reg::X11, MASK_XLEN(0x1000));
        c.sub(Reg::X4, Reg::X7, Reg::X11);
        c.sw(Reg::X4, Reg::X8, 12);

        // rs1==x19, rs2==x6, rd==x1, rs2_val == 16384, rs1_val == -268435457
        // opcode: sub ; op1:x19; op2:x6; dest:x1; op1val:-0x10000001;  op2val:0x4000
        c.li(Reg::X19, MASK_XLEN(-0x10000001));
        c.li(Reg::X6, MASK_XLEN(0x4000));
        c.sub(Reg::X1, Reg::X19, Reg::X6);
        c.sw(Reg::X1, Reg::X8, 16);

        // rs1==x6, rs2==x20, rd==x5, rs2_val == 32768, 
        // opcode: sub ; op1:x6; op2:x20; dest:x5; op1val:0x7fffffff;  op2val:0x8000
        c.li(Reg::X6, MASK_XLEN(0x7fffffff));
        c.li(Reg::X20, MASK_XLEN(0x8000));
        c.sub(Reg::X5, Reg::X6, Reg::X20);
        c.sw(Reg::X5, Reg::X8, 20);

        // rs1==x29, rs2==x22, rd==x2, rs2_val == 65536, 
        // opcode: sub ; op1:x29; op2:x22; dest:x2; op1val:-0xb503;  op2val:0x10000
        c.li(Reg::X29, MASK_XLEN(-0xb503));
        c.li(Reg::X22, MASK_XLEN(0x10000));
        c.sub(Reg::X2, Reg::X29, Reg::X22);
        c.sw(Reg::X2, Reg::X8, 24);

        // rs1==x3, rs2==x13, rd==x21, rs2_val == 131072, rs1_val == -8388609
        // opcode: sub ; op1:x3; op2:x13; dest:x21; op1val:-0x800001;  op2val:0x20000
        c.li(Reg::X3, MASK_XLEN(-0x800001));
        c.li(Reg::X13, MASK_XLEN(0x20000));
        c.sub(Reg::X21, Reg::X3, Reg::X13);
        c.sw(Reg::X21, Reg::X8, 28);

        // rs1==x1, rs2==x21, rd==x28, rs2_val == 262144, rs1_val == 256
        // opcode: sub ; op1:x1; op2:x21; dest:x28; op1val:0x100;  op2val:0x40000
        c.li(Reg::X1, MASK_XLEN(0x100));
        c.li(Reg::X21, MASK_XLEN(0x40000));
        c.sub(Reg::X28, Reg::X1, Reg::X21);
        c.sw(Reg::X28, Reg::X8, 32);

        // rs1==x20, rs2==x23, rd==x12, rs2_val == 1048576, 
        // opcode: sub ; op1:x20; op2:x23; dest:x12; op1val:0x100;  op2val:0x100000
        c.li(Reg::X20, MASK_XLEN(0x100));
        c.li(Reg::X23, MASK_XLEN(0x100000));
        c.sub(Reg::X12, Reg::X20, Reg::X23);
        c.sw(Reg::X12, Reg::X8, 36);

        // rs1==x27, rs2==x25, rd==x22, rs2_val == 4194304, 
        // opcode: sub ; op1:x27; op2:x25; dest:x22; op1val:-0x4;  op2val:0x400000
        c.li(Reg::X27, MASK_XLEN(-0x4));
        c.li(Reg::X25, MASK_XLEN(0x400000));
        c.sub(Reg::X22, Reg::X27, Reg::X25);
        c.sw(Reg::X22, Reg::X8, 40);

        // rs1==x11, rs2==x2, rd==x24, rs2_val == 8388608, 
        // opcode: sub ; op1:x11; op2:x2; dest:x24; op1val:0x66666665;  op2val:0x800000
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.li(Reg::X2, MASK_XLEN(0x800000));
        c.sub(Reg::X24, Reg::X11, Reg::X2);
        c.sw(Reg::X24, Reg::X8, 44);

        // rs2_val == 16777216, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3fffffff;  op2val:0x1000000
        c.li(Reg::X10, MASK_XLEN(0x3fffffff));
        c.li(Reg::X11, MASK_XLEN(0x1000000));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 48);

        // rs2_val == 33554432, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x2000000
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x2000000));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 52);

        // rs2_val == 67108864, rs1_val == -32769
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x8001;  op2val:0x4000000
        c.li(Reg::X10, MASK_XLEN(-0x8001));
        c.li(Reg::X11, MASK_XLEN(0x4000000));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 56);

        // rs2_val == 134217728, rs1_val == 4
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x8000000
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x8000000));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 60);

        // rs2_val == 268435456, rs1_val == -4097
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x1001;  op2val:0x10000000
        c.li(Reg::X10, MASK_XLEN(-0x1001));
        c.li(Reg::X11, MASK_XLEN(0x10000000));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 64);

        // rs2_val == 536870912, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x1001;  op2val:0x20000000
        c.li(Reg::X10, MASK_XLEN(-0x1001));
        c.li(Reg::X11, MASK_XLEN(0x20000000));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 68);

        // rs2_val == 1073741824, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x80000;  op2val:0x40000000
        c.li(Reg::X10, MASK_XLEN(0x80000));
        c.li(Reg::X11, MASK_XLEN(0x40000000));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 72);

        // rs2_val == -2, rs1_val == 1431655765
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x2
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 76);

        // rs2_val == -3, rs1_val == -65537
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x10001;  op2val:-0x3
        c.li(Reg::X10, MASK_XLEN(-0x10001));
        c.li(Reg::X11, MASK_XLEN(-0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 80);

        // rs2_val == -5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x100;  op2val:-0x5
        c.li(Reg::X10, MASK_XLEN(0x100));
        c.li(Reg::X11, MASK_XLEN(-0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 84);

        // rs2_val == -9, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2000000;  op2val:-0x9
        c.li(Reg::X10, MASK_XLEN(0x2000000));
        c.li(Reg::X11, MASK_XLEN(-0x9));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 88);

        // rs2_val == -17, rs1_val == 268435456
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x10000000;  op2val:-0x11
        c.li(Reg::X10, MASK_XLEN(0x10000000));
        c.li(Reg::X11, MASK_XLEN(-0x11));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 92);

        // rs2_val == -33, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x21
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x21));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 96);

        // rs2_val == -65, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x41
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x41));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 100);

        // rs2_val == -257, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x101
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0x101));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 104);

        // rs2_val == -513, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x201
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0x201));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 108);

        // rs2_val == -1025, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3fffffff;  op2val:-0x401
        c.li(Reg::X10, MASK_XLEN(0x3fffffff));
        c.li(Reg::X11, MASK_XLEN(-0x401));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 112);

        // rs2_val == -2049, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x801
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x801));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 116);

        // rs2_val == -4097, rs1_val == -33
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x21;  op2val:-0x1001
        c.li(Reg::X10, MASK_XLEN(-0x21));
        c.li(Reg::X11, MASK_XLEN(-0x1001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 120);

        // rs2_val == -8193, rs1_val == -1025
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x401;  op2val:-0x2001
        c.li(Reg::X10, MASK_XLEN(-0x401));
        c.li(Reg::X11, MASK_XLEN(-0x2001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 124);

        // rs2_val == -16385, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x10001;  op2val:-0x4001
        c.li(Reg::X10, MASK_XLEN(-0x10001));
        c.li(Reg::X11, MASK_XLEN(-0x4001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 128);

        // rs2_val == -32769, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x8001
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x8001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 132);

        // rs2_val == -65537, rs1_val == -2049
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x801;  op2val:-0x10001
        c.li(Reg::X10, MASK_XLEN(-0x801));
        c.li(Reg::X11, MASK_XLEN(-0x10001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 136);

        // rs2_val == -131073, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x800001;  op2val:-0x20001
        c.li(Reg::X10, MASK_XLEN(-0x800001));
        c.li(Reg::X11, MASK_XLEN(-0x20001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 140);

        // rs2_val == -262145, rs1_val == -257
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x101;  op2val:-0x40001
        c.li(Reg::X10, MASK_XLEN(-0x101));
        c.li(Reg::X11, MASK_XLEN(-0x40001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 144);

        // rs2_val == -524289, rs1_val == -134217729
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x8000001;  op2val:-0x80001
        c.li(Reg::X10, MASK_XLEN(-0x8000001));
        c.li(Reg::X11, MASK_XLEN(-0x80001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 148);

        // rs2_val == -1048577, rs1_val == -4194305
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x400001;  op2val:-0x100001
        c.li(Reg::X10, MASK_XLEN(-0x400001));
        c.li(Reg::X11, MASK_XLEN(-0x100001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 152);

        // rs2_val == -2097153, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x3;  op2val:-0x200001
        c.li(Reg::X10, MASK_XLEN(-0x3));
        c.li(Reg::X11, MASK_XLEN(-0x200001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 156);

        // rs2_val == -4194305, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x8000001;  op2val:-0x400001
        c.li(Reg::X10, MASK_XLEN(-0x8000001));
        c.li(Reg::X11, MASK_XLEN(-0x400001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 160);

        // rs2_val == -8388609, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x8000001;  op2val:-0x800001
        c.li(Reg::X10, MASK_XLEN(-0x8000001));
        c.li(Reg::X11, MASK_XLEN(-0x800001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 164);

        // rs2_val == -16777217, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x1000001
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x1000001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 168);

        // rs2_val == -33554433, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x2000001
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x2000001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 172);

        // rs2_val == -67108865, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x4000001
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x4000001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 176);

        // rs2_val == -134217729, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x8000001
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x8000001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 180);

        // rs2_val == -268435457, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x10000001
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x10000001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 184);

        // rs2_val == -536870913, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x20000001
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0x20000001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 188);

        // rs2_val == -1073741825, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x40000001
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0x40000001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 192);

        // rs2_val == 1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x100;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x100));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 196);

        // rs2_val == -1431655766, rs1_val == 262144
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x40000;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x40000));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 200);

        // rs1_val == 8, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x8;  op2val:-0x1001
        c.li(Reg::X10, MASK_XLEN(0x8));
        c.li(Reg::X11, MASK_XLEN(-0x1001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 204);

        // rs1_val == 16, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x10;  op2val:-0x40001
        c.li(Reg::X10, MASK_XLEN(0x10));
        c.li(Reg::X11, MASK_XLEN(-0x40001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 208);

        // rs1_val == 64, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x40;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x40));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 212);

        // rs1_val == 128, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x80;  op2val:-0x2001
        c.li(Reg::X10, MASK_XLEN(0x80));
        c.li(Reg::X11, MASK_XLEN(-0x2001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 216);

        // rs1_val == 512, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x200;  op2val:-0x8001
        c.li(Reg::X10, MASK_XLEN(0x200));
        c.li(Reg::X11, MASK_XLEN(-0x8001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 220);

        // rs1_val == 1024, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x400;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x400));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 224);

        // rs1_val == 2048, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x800;  op2val:-0x200001
        c.li(Reg::X10, MASK_XLEN(0x800));
        c.li(Reg::X11, MASK_XLEN(-0x200001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 228);

        // rs1_val == 4096, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x1000;  op2val:0x10
        c.li(Reg::X10, MASK_XLEN(0x1000));
        c.li(Reg::X11, MASK_XLEN(0x10));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 232);

        // rs1_val == 8192, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2000;  op2val:0x800000
        c.li(Reg::X10, MASK_XLEN(0x2000));
        c.li(Reg::X11, MASK_XLEN(0x800000));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 236);

        // rs1_val == 16384, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4000;  op2val:0x20000
        c.li(Reg::X10, MASK_XLEN(0x4000));
        c.li(Reg::X11, MASK_XLEN(0x20000));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 240);

        // rs1_val == 32768, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x8000;  op2val:-0x1
        c.li(Reg::X10, MASK_XLEN(0x8000));
        c.li(Reg::X11, MASK_XLEN(-0x1));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 244);

        // rs1_val == 1048576, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x100000;  op2val:0x8
        c.li(Reg::X10, MASK_XLEN(0x100000));
        c.li(Reg::X11, MASK_XLEN(0x8));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 248);

        // rs1_val == 2097152, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x200000;  op2val:-0x11
        c.li(Reg::X10, MASK_XLEN(0x200000));
        c.li(Reg::X11, MASK_XLEN(-0x11));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 252);

        // rs1_val == 4194304, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x400000;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x400000));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 256);

        // rs1_val == 8388608, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x800000;  op2val:-0x21
        c.li(Reg::X10, MASK_XLEN(0x800000));
        c.li(Reg::X11, MASK_XLEN(-0x21));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 260);

        // rs1_val == 16777216, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x1000000;  op2val:-0x20001
        c.li(Reg::X10, MASK_XLEN(0x1000000));
        c.li(Reg::X11, MASK_XLEN(-0x20001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 264);

        // rs1_val == 67108864, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4000000;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x4000000));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 268);

        // rs1_val == 134217728, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x8000000;  op2val:0x8
        c.li(Reg::X10, MASK_XLEN(0x8000000));
        c.li(Reg::X11, MASK_XLEN(0x8));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 272);

        // rs1_val == 536870912, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x20000000;  op2val:-0x1000001
        c.li(Reg::X10, MASK_XLEN(0x20000000));
        c.li(Reg::X11, MASK_XLEN(-0x1000001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 276);

        // rs1_val == 1073741824, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x40000000;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x40000000));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 280);

        // rs1_val == -2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x2;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 284);

        // rs1_val == -5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x5;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x5));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 288);

        // rs1_val == -9, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x9;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x9));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 292);

        // rs1_val == -17, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x11;  op2val:-0x20001
        c.li(Reg::X10, MASK_XLEN(-0x11));
        c.li(Reg::X11, MASK_XLEN(-0x20001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 296);

        // rs1_val == -129, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x81;  op2val:0x400
        c.li(Reg::X10, MASK_XLEN(-0x81));
        c.li(Reg::X11, MASK_XLEN(0x400));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 300);

        // rs1_val == -513, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x201;  op2val:0x800000
        c.li(Reg::X10, MASK_XLEN(-0x201));
        c.li(Reg::X11, MASK_XLEN(0x800000));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 304);

        // rs1_val == -8193, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x2001;  op2val:-0x2
        c.li(Reg::X10, MASK_XLEN(-0x2001));
        c.li(Reg::X11, MASK_XLEN(-0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 308);

        // rs1_val == -16385, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x4001;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x4001));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 312);

        // rs1_val == -131073, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x20001;  op2val:-0x80001
        c.li(Reg::X10, MASK_XLEN(-0x20001));
        c.li(Reg::X11, MASK_XLEN(-0x80001));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 316);

        // rs1_val == -262145, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x40001;  op2val:0x10
        c.li(Reg::X10, MASK_XLEN(-0x40001));
        c.li(Reg::X11, MASK_XLEN(0x10));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 320);

        // rs1_val == -524289, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x80001;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x80001));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 324);

        // rs1_val == -1048577, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x100001;  op2val:0x10
        c.li(Reg::X10, MASK_XLEN(-0x100001));
        c.li(Reg::X11, MASK_XLEN(0x10));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 328);

        // rs1_val == -2097153, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x200001;  op2val:-0x7
        c.li(Reg::X10, MASK_XLEN(-0x200001));
        c.li(Reg::X11, MASK_XLEN(-0x7));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 332);

        // rs1_val == -33554433, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x2000001;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0x2000001));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 336);

        // rs1_val == -1073741825, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x40000001;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0x40000001));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 340);

        // rs1_val==3 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 344);

        // rs1_val==3 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 348);

        // rs1_val==3 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 352);

        // rs1_val==3 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 356);

        // rs1_val==3 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 360);

        // rs1_val==3 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 364);

        // rs1_val==3 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 368);

        // rs1_val==3 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 372);

        // rs1_val==3 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 376);

        // rs1_val==3 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 380);

        // rs1_val==3 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 384);

        // rs1_val==3 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 388);

        // rs1_val==3 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 392);

        // rs1_val==3 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 396);

        // rs1_val==3 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 400);

        // rs1_val==3 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 404);

        // rs1_val==3 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 408);

        // rs1_val==3 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 412);

        // rs1_val==3 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 416);

        // rs1_val==3 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 420);

        // rs1_val==3 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 424);

        // rs1_val==3 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 428);

        // rs1_val==1431655765 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 432);

        // rs1_val==1431655765 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 436);

        // rs1_val==1431655765 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 440);

        // rs1_val==1431655765 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 444);

        // rs1_val==1431655765 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 448);

        // rs1_val==1431655765 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 452);

        // rs1_val==1431655765 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 456);

        // rs1_val==1431655765 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 460);

        // rs1_val==1431655765 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 464);

        // rs1_val==1431655765 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 468);

        // rs1_val==1431655765 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 472);

        // rs1_val==1431655765 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 476);

        // rs1_val==1431655765 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 480);

        // rs1_val==1431655765 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 484);

        // rs1_val==1431655765 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 488);

        // rs1_val==1431655765 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 492);

        // rs1_val==1431655765 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 496);

        // rs1_val==1431655765 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 500);

        // rs1_val==1431655765 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 504);

        // rs1_val==1431655765 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 508);

        // rs1_val==1431655765 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 512);

        // rs1_val==1431655765 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 516);

        // rs1_val==-1431655766 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 520);

        // rs1_val==-1431655766 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 524);

        // rs1_val==-1431655766 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 528);

        // rs1_val==-1431655766 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 532);

        // rs1_val==-1431655766 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 536);

        // rs1_val==-1431655766 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 540);

        // rs1_val==-1431655766 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 544);

        // rs1_val==-1431655766 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 548);

        // rs1_val==-1431655766 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 552);

        // rs1_val==-1431655766 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 556);

        // rs1_val==-1431655766 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 560);

        // rs1_val==-1431655766 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 564);

        // rs1_val==-1431655766 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 568);

        // rs1_val==-1431655766 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 572);

        // rs1_val==-1431655766 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 576);

        // rs1_val==-1431655766 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 580);

        // rs1_val==-1431655766 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 584);

        // rs1_val==-1431655766 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 588);

        // rs1_val==-1431655766 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 592);

        // rs1_val==-1431655766 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 596);

        // rs1_val==-1431655766 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 600);

        // rs1_val==-1431655766 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 604);

        // rs1_val==5 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 608);

        // rs1_val==5 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 612);

        // rs1_val==5 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 616);

        // rs1_val==5 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 620);

        // rs1_val==5 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 624);

        // rs1_val==5 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 628);

        // rs1_val==5 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 632);

        // rs1_val==5 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 636);

        // rs1_val==5 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 640);

        // rs1_val==5 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 644);

        // rs1_val==5 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 648);

        // rs1_val==5 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 652);

        // rs1_val==5 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 656);

        // rs1_val==5 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 660);

        // rs1_val==5 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 664);

        // rs1_val==5 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 668);

        // rs1_val==5 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 672);

        // rs1_val==5 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 676);

        // rs1_val==5 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 680);

        // rs1_val==5 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 684);

        // rs1_val==5 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 688);

        // rs1_val==5 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 692);

        // rs1_val==858993459 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 696);

        // rs1_val==858993459 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 700);

        // rs1_val==858993459 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 704);

        // rs1_val==858993459 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 708);

        // rs1_val==858993459 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 712);

        // rs1_val==858993459 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 716);

        // rs1_val==858993459 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 720);

        // rs1_val==858993459 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 724);

        // rs1_val==858993459 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 728);

        // rs1_val==858993459 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 732);

        // rs1_val==858993459 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 736);

        // rs1_val==858993459 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 740);

        // rs1_val==858993459 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 744);

        // rs1_val==858993459 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 748);

        // rs1_val==858993459 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 752);

        // rs1_val==858993459 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 756);

        // rs1_val==858993459 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 760);

        // rs1_val==858993459 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 764);

        // rs1_val==858993459 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 768);

        // rs1_val==858993459 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 772);

        // rs1_val==858993459 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 776);

        // rs1_val==858993459 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 780);

        // rs1_val==1717986918 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 784);

        // rs1_val==1717986918 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 788);

        // rs1_val==1717986918 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 792);

        // rs1_val==1717986918 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 796);

        // rs1_val==1717986918 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 800);

        // rs1_val==1717986918 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 804);

        // rs1_val==1717986918 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 808);

        // rs1_val==1717986918 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 812);

        // rs1_val==1717986918 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 816);

        // rs1_val==1717986918 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 820);

        // rs1_val==1717986918 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 824);

        // rs1_val==1717986918 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 828);

        // rs1_val==1717986918 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 832);

        // rs1_val==1717986918 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 836);

        // rs1_val==1717986918 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 840);

        // rs1_val==1717986918 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 844);

        // rs1_val==1717986918 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 848);

        // rs1_val==1717986918 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 852);

        // rs1_val==1717986918 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 856);

        // rs1_val==1717986918 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 860);

        // rs1_val==1717986918 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 864);

        // rs1_val==1717986918 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 868);

        // rs1_val==-46340 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 872);

        // rs1_val==-46340 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 876);

        // rs1_val==-46340 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 880);

        // rs1_val==-46340 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 884);

        // rs1_val==-46340 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 888);

        // rs1_val==-46340 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 892);

        // rs1_val==-46340 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 896);

        // rs1_val==-46340 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 900);

        // rs1_val==-46340 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 904);

        // rs1_val==-46340 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 908);

        // rs1_val==-46340 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 912);

        // rs1_val==-46340 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 916);

        // rs1_val==-46340 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 920);

        // rs1_val==-46340 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 924);

        // rs1_val==-46340 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 928);

        // rs1_val==-46340 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 932);

        // rs1_val==-46340 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 936);

        // rs1_val==-46340 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 940);

        // rs1_val==-46340 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 944);

        // rs1_val==-46340 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 948);

        // rs1_val==-46340 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 952);

        // rs1_val==-46340 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 956);

        // rs1_val==46340 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 960);

        // rs1_val==46340 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 964);

        // rs1_val==46340 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 968);

        // rs1_val==46340 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 972);

        // rs1_val==46340 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 976);

        // rs1_val==46340 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 980);

        // rs1_val==46340 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 984);

        // rs1_val==46340 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 988);

        // rs1_val==46340 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 992);

        // rs1_val==46340 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 996);

        // rs1_val==46340 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1000);

        // rs1_val==46340 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1004);

        // rs1_val==46340 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1008);

        // rs1_val==46340 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1012);

        // rs1_val==46340 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1016);

        // rs1_val==46340 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1020);

        // rs1_val==46340 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1024);

        // rs1_val==46340 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1028);

        // rs1_val==46340 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1032);

        // rs1_val==46340 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1036);

        // rs1_val==46340 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1040);

        // rs1_val==46340 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1044);

        // rs1_val==2 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1048);

        // rs1_val==2 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1052);

        // rs1_val==2 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1056);

        // rs1_val==2 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1060);

        // rs1_val==2 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1064);

        // rs1_val==2 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1068);

        // rs1_val==2 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1072);

        // rs1_val==2 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1076);

        // rs1_val==2 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1080);

        // rs1_val==2 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1084);

        // rs1_val==2 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1088);

        // rs1_val==2 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1092);

        // rs1_val==2 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1096);

        // rs1_val==2 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1100);

        // rs1_val==2 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1104);

        // rs1_val==2 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1108);

        // rs1_val==2 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1112);

        // rs1_val==2 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1116);

        // rs1_val==2 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1120);

        // rs1_val==2 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1124);

        // rs1_val==2 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1128);

        // rs1_val==1431655764 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1132);

        // rs1_val==1431655764 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1136);

        // rs1_val==1431655764 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1140);

        // rs1_val==1431655764 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1144);

        // rs1_val==1431655764 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1148);

        // rs1_val==1431655764 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1152);

        // rs1_val==1431655764 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1156);

        // rs1_val==1431655764 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1160);

        // rs1_val==1431655764 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1164);

        // rs1_val==1431655764 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1168);

        // rs1_val==1431655764 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1172);

        // rs1_val==1431655764 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1176);

        // rs1_val==1431655764 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1180);

        // rs1_val==1431655764 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1184);

        // rs1_val==1431655764 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1188);

        // rs1_val==1431655764 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1192);

        // rs1_val==1431655764 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1196);

        // rs1_val==1431655764 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1200);

        // rs1_val==1431655764 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1204);

        // rs1_val==1431655764 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1208);

        // rs1_val==1431655764 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1212);

        // rs1_val==0 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1216);

        // rs1_val==0 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1220);

        // rs1_val==0 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1224);

        // rs1_val==0 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1228);

        // rs1_val==0 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1232);

        // rs1_val==0 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1236);

        // rs1_val==0 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1240);

        // rs1_val==0 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1244);

        // rs1_val==0 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1248);

        // rs1_val==-1431655765 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1252);

        // rs1_val==-1431655765 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1256);

        // rs1_val==-1431655765 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1260);

        // rs1_val==-1431655765 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1264);

        // rs1_val==-1431655765 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1268);

        // rs1_val==-1431655765 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1272);

        // rs1_val==6 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1276);

        // rs1_val==6 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1280);

        // rs1_val==6 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1284);

        // rs1_val==6 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1288);

        // rs1_val==6 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1292);

        // rs1_val==6 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1296);

        // rs1_val==6 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1300);

        // rs1_val==6 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1304);

        // rs1_val==6 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1308);

        // rs1_val==6 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1312);

        // rs1_val==6 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1316);

        // rs1_val==6 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1320);

        // rs1_val==6 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1324);

        // rs1_val==6 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1328);

        // rs1_val==6 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1332);

        // rs1_val==6 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1336);

        // rs1_val==6 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1340);

        // rs1_val==6 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1344);

        // rs1_val==6 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1348);

        // rs1_val==6 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1352);

        // rs1_val==6 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1356);

        // rs1_val==6 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1360);

        // rs1_val==858993460 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1364);

        // rs1_val==858993460 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1368);

        // rs1_val==858993460 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1372);

        // rs1_val==858993460 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1376);

        // rs1_val==858993460 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1380);

        // rs1_val==858993460 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1384);

        // rs1_val==858993460 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1388);

        // rs1_val==858993460 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1392);

        // rs1_val==858993460 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1396);

        // rs1_val==858993460 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1400);

        // rs1_val==858993460 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1404);

        // rs1_val==858993460 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1408);

        // rs1_val==858993460 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1412);

        // rs1_val==858993460 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1416);

        // rs1_val==858993460 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1420);

        // rs1_val==858993460 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1424);

        // rs1_val==858993460 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1428);

        // rs1_val==858993460 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1432);

        // rs1_val==858993460 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1436);

        // rs1_val==858993460 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1440);

        // rs1_val==858993460 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1444);

        // rs1_val==858993460 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1448);

        // rs1_val==1717986919 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1452);

        // rs1_val==1717986919 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1456);

        // rs1_val==1717986919 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1460);

        // rs1_val==1717986919 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1464);

        // rs1_val==1717986919 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1468);

        // rs1_val==1717986919 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1472);

        // rs1_val==1717986919 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1476);

        // rs1_val==1717986919 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1480);

        // rs1_val==1717986919 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1484);

        // rs1_val==1717986919 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1488);

        // rs1_val==1717986919 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1492);

        // rs1_val==1717986919 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1496);

        // rs1_val==1717986919 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1500);

        // rs1_val==1717986919 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1504);

        // rs1_val==1717986919 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1508);

        // rs1_val==1717986919 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1512);

        // rs1_val==1717986919 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1516);

        // rs1_val==1717986919 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1520);

        // rs1_val==1717986919 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1524);

        // rs1_val==1717986919 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1528);

        // rs1_val==1717986919 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1532);

        // rs1_val==1717986919 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1536);

        // rs1_val==-46339 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1540);

        // rs1_val==-46339 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1544);

        // rs1_val==-46339 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1548);

        // rs1_val==-46339 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1552);

        // rs1_val==-46339 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1556);

        // rs1_val==-46339 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1560);

        // rs1_val==-46339 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1564);

        // rs1_val==-46339 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1568);

        // rs1_val==-46339 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1572);

        // rs1_val==-46339 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1576);

        // rs1_val==-46339 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1580);

        // rs1_val==-46339 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1584);

        // rs1_val==-46339 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1588);

        // rs1_val==-46339 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1592);

        // rs1_val==-46339 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1596);

        // rs1_val==-46339 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1600);

        // rs1_val==-46339 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1604);

        // rs1_val==-46339 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1608);

        // rs1_val==-46339 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1612);

        // rs1_val==-46339 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1616);

        // rs1_val==-46339 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1620);

        // rs1_val==-46339 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1624);

        // rs1_val==46341 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1628);

        // rs1_val==46341 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1632);

        // rs1_val==46341 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1636);

        // rs1_val==46341 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1640);

        // rs1_val==46341 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1644);

        // rs1_val==46341 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1648);

        // rs1_val==46341 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1652);

        // rs1_val==46341 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1656);

        // rs1_val==46341 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1660);

        // rs1_val==46341 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1664);

        // rs1_val==46341 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1668);

        // rs1_val==46341 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1672);

        // rs1_val==46341 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1676);

        // rs1_val==46341 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1680);

        // rs1_val==46341 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1684);

        // rs1_val==46341 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1688);

        // rs1_val==46341 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1692);

        // rs1_val==46341 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1696);

        // rs1_val==46341 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1700);

        // rs1_val==46341 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1704);

        // rs1_val==46341 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1708);

        // rs1_val==46341 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1712);

        // rs1_val==0 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1716);

        // rs1_val==0 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1720);

        // rs1_val==0 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1724);

        // rs1_val==0 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1728);

        // rs1_val==0 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1732);

        // rs1_val==0 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1736);

        // rs1_val==0 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1740);

        // rs1_val==0 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1744);

        // rs1_val==0 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1748);

        // rs1_val==0 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1752);

        // rs1_val==0 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1756);

        // rs1_val==0 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1760);

        // rs1_val==0 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1764);

        // rs1_val==4 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1768);

        // rs1_val==4 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1772);

        // rs1_val==4 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1776);

        // rs1_val==4 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1780);

        // rs1_val==4 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1784);

        // rs1_val==4 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1788);

        // rs1_val==4 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1792);

        // rs1_val==4 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1796);

        // rs1_val==4 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1800);

        // rs1_val==4 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1804);

        // rs1_val==4 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1808);

        // rs1_val==4 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1812);

        // rs1_val==4 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1816);

        // rs1_val==4 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1820);

        // rs1_val==4 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1824);

        // rs1_val==4 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1828);

        // rs1_val==4 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1832);

        // rs1_val==4 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1836);

        // rs1_val==4 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1840);

        // rs1_val==4 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1844);

        // rs1_val==4 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1848);

        // rs1_val==4 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1852);

        // rs1_val==858993458 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1856);

        // rs1_val==858993458 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1860);

        // rs1_val==858993458 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1864);

        // rs1_val==858993458 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1868);

        // rs1_val==858993458 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1872);

        // rs1_val==858993458 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1876);

        // rs1_val==858993458 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1880);

        // rs1_val==858993458 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1884);

        // rs1_val==858993458 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1888);

        // rs1_val==858993458 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1892);

        // rs1_val==858993458 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1896);

        // rs1_val==858993458 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1900);

        // rs1_val==858993458 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1904);

        // rs1_val==858993458 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1908);

        // rs1_val==858993458 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1912);

        // rs1_val==858993458 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1916);

        // rs1_val==858993458 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1920);

        // rs1_val==858993458 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1924);

        // rs1_val==858993458 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1928);

        // rs1_val==858993458 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1932);

        // rs1_val==858993458 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1936);

        // rs1_val==858993458 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1940);

        // rs1_val==1717986917 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1944);

        // rs1_val==1717986917 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1948);

        // rs1_val==1717986917 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1952);

        // rs1_val==1717986917 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1956);

        // rs1_val==1717986917 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1960);

        // rs1_val==1717986917 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1964);

        // rs1_val==1717986917 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1968);

        // rs1_val==1717986917 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1972);

        // rs1_val==1717986917 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1976);

        // rs1_val==1717986917 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1980);

        // rs1_val==1717986917 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1984);

        // rs1_val==1717986917 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1988);

        // rs1_val==1717986917 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1992);

        // rs1_val==1717986917 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 1996);

        // rs1_val==1717986917 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 2000);

        // rs1_val==1717986917 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 2004);

        // rs1_val==1717986917 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 2008);

        // rs1_val==1717986917 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 2012);

        // rs1_val==1717986917 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 2016);

        // rs1_val==1717986917 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 2020);

        // rs1_val==1717986917 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 2024);

        // rs1_val==1717986917 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 2028);

        // rs1_val==46339 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 2032);

        // rs1_val==46339 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 2036);

        // rs1_val==46339 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 2040);

        // rs1_val==46339 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 2044);

        c.li(Reg::X8, 2128);
        // rs1_val==46339 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 0);

        // rs1_val==46339 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 4);

        // rs1_val==46339 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 8);

        // rs1_val==46339 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 12);

        // rs1_val==46339 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 16);

        // rs1_val==46339 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 20);

        // rs1_val==46339 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 24);

        // rs1_val==46339 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 28);

        // rs1_val==46339 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 32);

        // rs1_val==46339 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 36);

        // rs1_val==46339 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 40);

        // rs1_val==46339 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 44);

        // rs1_val==46339 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 48);

        // rs1_val==46339 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 52);

        // rs1_val==46339 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 56);

        // rs1_val==46339 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 60);

        // rs1_val==46339 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 64);

        // rs1_val==46339 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 68);

        // rs1_val==1431655766 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 72);

        // rs1_val==1431655766 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 76);

        // rs1_val==1431655766 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 80);

        // rs1_val==1431655766 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 84);

        // rs1_val==1431655766 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 88);

        // rs1_val==1431655766 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 92);

        // rs1_val==1431655766 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 96);

        // rs1_val==1431655766 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 100);

        // rs1_val==1431655766 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 104);

        // rs1_val==1431655766 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 108);

        // rs1_val==1431655766 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 112);

        // rs1_val==1431655766 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 116);

        // rs1_val==1431655766 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 120);

        // rs1_val==1431655766 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 124);

        // rs1_val==1431655766 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 128);

        // rs1_val==1431655766 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 132);

        // rs1_val==1431655766 and rs2_val==-1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 136);

        // rs1_val==1431655766 and rs2_val==6, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 140);

        // rs1_val==1431655766 and rs2_val==858993460, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 144);

        // rs1_val==1431655766 and rs2_val==1717986919, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 148);

        // rs1_val==1431655766 and rs2_val==-46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 152);

        // rs1_val==1431655766 and rs2_val==46341, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 156);

        // rs1_val==-1431655765 and rs2_val==3, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 160);

        // rs1_val==-1431655765 and rs2_val==1431655765, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 164);

        // rs1_val==-1431655765 and rs2_val==-1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 168);

        // rs1_val==-1431655765 and rs2_val==5, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 172);

        // rs1_val==-1431655765 and rs2_val==858993459, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 176);

        // rs1_val==-1431655765 and rs2_val==1717986918, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 180);

        // rs1_val==-1431655765 and rs2_val==-46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 184);

        // rs1_val==-1431655765 and rs2_val==46340, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 188);

        // rs1_val==-1431655765 and rs2_val==2, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 192);

        // rs1_val==-1431655765 and rs2_val==1431655764, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 196);

        // rs1_val==-1431655765 and rs2_val==0, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 200);

        // rs1_val==-1431655765 and rs2_val==4, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 204);

        // rs1_val==-1431655765 and rs2_val==858993458, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 208);

        // rs1_val==-1431655765 and rs2_val==1717986917, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 212);

        // rs1_val==-1431655765 and rs2_val==46339, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 216);

        // rs1_val==-1431655765 and rs2_val==1431655766, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 220);

        // rs1_val < 0 and rs2_val < 0, rs2_val == -129
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x7;  op2val:-0x81
        c.li(Reg::X10, MASK_XLEN(-0x7));
        c.li(Reg::X11, MASK_XLEN(-0x81));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 224);

        // rs2_val == 0, rs1_val == 32
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x20;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x20));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 228);

        // rs2_val == 128, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:0x9;  op2val:0x80
        c.li(Reg::X10, MASK_XLEN(0x9));
        c.li(Reg::X11, MASK_XLEN(0x80));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 232);

        // rs2_val == 2048, 
        // opcode: sub ; op1:x10; op2:x11; dest:x12; op1val:-0x80000000;  op2val:0x800
        c.li(Reg::X10, MASK_XLEN(-0x80000000));
        c.li(Reg::X11, MASK_XLEN(0x800));
        c.sub(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 236);


                
        // End
        c.ecall();
        return c;
    }

    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> sub_01::s_ScalarData;
}


// Generate google tests

TEST_F(sub_01, opcode__sub___op1_x24__op2_x26__dest_x26__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[0], 1431655758);
}


TEST_F(sub_01, opcode__sub___op1_x17__op2_x17__dest_x23__op1val_0x2000000___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[1], 0);
}


TEST_F(sub_01, opcode__sub___op1_x16__op2_x16__dest_x16__op1val__0x7___op2val__0x7) {
  ASSERT_EQ(getScalarWordData()[2], 0);
}


TEST_F(sub_01, opcode__sub___op1_x31__op2_x19__dest_x31__op1val__0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[3], 2576980376);
}


TEST_F(sub_01, opcode__sub___op1_x23__op2_x14__dest_x8__op1val_0x80000___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[4], 0);
}


TEST_F(sub_01, opcode__sub___op1_x13__op2_x24__dest_x18__op1val__0x4000001___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[5], 2080374783);
}


TEST_F(sub_01, opcode__sub___op1_x12__op2_x4__dest_x0__op1val_0x20___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[6], 0);
}


TEST_F(sub_01, opcode__sub___op1_x22__op2_x9__dest_x10__op1val__0x20000001___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[7], 1610612736);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x27__dest_x25__op1val_0x10000___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[8], 65535);
}


TEST_F(sub_01, opcode__sub___op1_x8__op2_x3__dest_x14__op1val__0x80000000___op2val__0x40000000) {
  ASSERT_EQ(getScalarWordData()[9], 3221225472);
}


TEST_F(sub_01, opcode__sub___op1_x25__op2_x30__dest_x29__op1val_0x0___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[10], 4292870144);
}


TEST_F(sub_01, opcode__sub___op1_x18__op2_x8__dest_x15__op1val_0x7fffffff___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[11], 2147475455);
}


TEST_F(sub_01, opcode__sub___op1_x14__op2_x15__dest_x3__op1val_0x1___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[12], 4294967281);
}


TEST_F(sub_01, opcode__sub___op1_x26__op2_x29__dest_x13__op1val__0x7___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[13], 4294967287);
}


TEST_F(sub_01, opcode__sub___op1_x21__op2_x31__dest_x19__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[14], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x30__op2_x5__dest_x11__op1val__0x1000001___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[15], 4278190071);
}


TEST_F(sub_01, opcode__sub___op1_x28__op2_x7__dest_x30__op1val__0x55555556___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[16], 2863311498);
}


TEST_F(sub_01, opcode__sub___op1_x9__op2_x10__dest_x7__op1val__0x41___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[17], 4294967167);
}


TEST_F(sub_01, opcode__sub___op1_x0__op2_x18__dest_x17__op1val_0x0___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[18], 4294967168);
}


TEST_F(sub_01, opcode__sub___op1_x2__op2_x12__dest_x27__op1val__0x40000000___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[19], 3221225216);
}


TEST_F(sub_01, opcode__sub___op1_x4__op2_x28__dest_x20__op1val__0xb503___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[20], 4294920445);
}


TEST_F(sub_01, opcode__sub___op1_x15__op2_x1__dest_x6__op1val_0x0___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[21], 4294966272);
}


TEST_F(sub_01, opcode__sub___op1_x5__op2_x0__dest_x9__op1val__0x80000000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[22], 2147483648);
}


TEST_F(sub_01, opcode__sub___op1_x7__op2_x11__dest_x4__op1val_0x20000___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[23], 126976);
}


TEST_F(sub_01, opcode__sub___op1_x19__op2_x6__dest_x1__op1val__0x10000001___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[24], 4026515455);
}


TEST_F(sub_01, opcode__sub___op1_x6__op2_x20__dest_x5__op1val_0x7fffffff___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[25], 2147450879);
}


TEST_F(sub_01, opcode__sub___op1_x29__op2_x22__dest_x2__op1val__0xb503___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[26], 4294855421);
}


TEST_F(sub_01, opcode__sub___op1_x3__op2_x13__dest_x21__op1val__0x800001___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[27], 4286447615);
}


TEST_F(sub_01, opcode__sub___op1_x1__op2_x21__dest_x28__op1val_0x100___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[28], 4294705408);
}


TEST_F(sub_01, opcode__sub___op1_x20__op2_x23__dest_x12__op1val_0x100___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[29], 4293918976);
}


TEST_F(sub_01, opcode__sub___op1_x27__op2_x25__dest_x22__op1val__0x4___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[30], 4290772988);
}


TEST_F(sub_01, opcode__sub___op1_x11__op2_x2__dest_x24__op1val_0x66666665___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[31], 1709598309);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3fffffff___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[32], 1056964607);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[33], 4261366524);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[34], 4227825663);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[35], 4160749572);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[36], 4026527743);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[37], 3758092287);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[38], 3221749760);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[39], 1431655767);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[40], 4294901762);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[41], 261);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[42], 33554441);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[43], 268435473);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[44], 35);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[45], 2863311595);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[46], 46598);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[47], 46854);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3fffffff___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[48], 1073742848);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[49], 4294923005);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[50], 4064);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[51], 7168);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[52], 4294918144);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[53], 32771);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[54], 63488);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[55], 4286709760);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[56], 261888);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[57], 4161273856);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[58], 4291821568);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[59], 2097150);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[60], 4164943872);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[61], 4169138176);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[62], 875770677);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[63], 1465210198);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[64], 67108868);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[65], 993211189);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[66], 268481796);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[67], 536870917);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[68], 1073741825);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[69], 2863311787);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[70], 1431917910);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[71], 4105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[72], 262161);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[73], 1431655830);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[74], 8321);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[75], 33281);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[76], 1021);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[77], 2099201);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[78], 4080);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[79], 4286586880);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[80], 4294852608);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val__0x1) {
  ASSERT_EQ(getScalarWordData()[81], 32769);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[82], 1048568);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[83], 2097169);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[84], 2867505835);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[85], 8388641);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[86], 16908289);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[87], 67108858);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[88], 134217720);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[89], 553648129);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[90], 1073741821);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[91], 3435973835);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[92], 4294967286);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[93], 4294967287);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[94], 131056);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[95], 4294966143);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[96], 4286578175);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[97], 4294959105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[98], 29955);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[99], 393216);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[100], 4294705135);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[101], 4294489347);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[102], 4293918703);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val__0x7) {
  ASSERT_EQ(getScalarWordData()[103], 4292870150);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[104], 3402419405);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[105], 2362232012);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[106], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[107], 2863311534);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[108], 1431655769);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[109], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[110], 3435973840);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[111], 2576980381);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[112], 46343);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[113], 4294920959);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[114], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[115], 2863311535);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[116], 3);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[117], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[118], 3435973841);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[119], 2576980382);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[120], 4294920960);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[121], 2863311533);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[122], 1431655768);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[123], 4294967293);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[124], 3435973839);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[125], 2576980380);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[126], 46342);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[127], 4294920958);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[128], 1431655762);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[129], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[130], 2863311531);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[131], 1431655760);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[132], 572662306);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[133], 4008636143);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[134], 1431702105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[135], 1431609425);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[136], 1431655763);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[137], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[138], 1431655765);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[139], 1431655761);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[140], 572662307);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[141], 4008636144);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[142], 1431609426);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[143], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[144], 2863311530);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[145], 1431655759);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[146], 572662305);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[147], 4008636142);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[148], 1431702104);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[149], 1431609424);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[150], 2863311527);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[151], 1431655765);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[152], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[153], 2863311525);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[154], 2004318071);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[155], 1145324612);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[156], 2863357870);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[157], 2863265190);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[158], 2863311528);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[159], 1431655766);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[160], 2863311530);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[161], 2863311526);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[162], 2004318072);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[163], 1145324613);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[164], 2863265191);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[165], 1431655764);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[166], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[167], 2863311524);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[168], 2004318070);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[169], 1145324611);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[170], 2863357869);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[171], 2863265189);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[172], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[173], 2863311536);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[174], 1431655771);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[175], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[176], 3435973842);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[177], 2576980383);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[178], 46345);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[179], 4294920961);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[180], 3);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[181], 2863311537);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[182], 5);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[183], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[184], 3435973843);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[185], 2576980384);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[186], 4294920962);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[187], 2863311535);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[188], 1431655770);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[189], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[190], 3435973841);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[191], 2576980382);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[192], 46344);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[193], 4294920960);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[194], 858993456);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[195], 3722304990);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[196], 2290649225);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[197], 858993454);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[198], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[199], 3435973837);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[200], 859039799);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[201], 858947119);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[202], 858993457);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[203], 3722304991);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[204], 858993459);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[205], 858993455);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[206], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[207], 3435973838);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[208], 858947120);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[209], 3722304989);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[210], 2290649224);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[211], 858993453);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[212], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[213], 3435973836);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[214], 859039798);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[215], 858947118);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[216], 1717986915);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[217], 286331153);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[218], 3149642684);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[219], 1717986913);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[220], 858993459);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[221], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[222], 1718033258);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[223], 1717940578);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[224], 1717986916);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[225], 286331154);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[226], 1717986918);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[227], 1717986914);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[228], 858993460);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[229], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[230], 1717940579);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[231], 286331152);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[232], 3149642683);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[233], 1717986912);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[234], 858993458);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[235], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[236], 1718033257);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[237], 1717940577);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[238], 4294920953);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[239], 2863265191);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[240], 1431609426);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[241], 4294920951);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[242], 3435927497);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[243], 2576934038);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[244], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[245], 4294874616);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[246], 4294920954);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[247], 2863265192);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[248], 4294920956);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[249], 4294920952);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[250], 3435927498);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[251], 2576934039);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[252], 4294874617);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[253], 2863265190);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[254], 1431609425);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[255], 4294920950);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[256], 3435927496);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[257], 2576934037);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[258], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[259], 4294874615);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[260], 46337);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[261], 2863357871);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[262], 1431702106);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[263], 46335);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[264], 3436020177);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[265], 2577026718);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[266], 92680);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[267], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[268], 46338);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[269], 2863357872);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[270], 46340);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[271], 46336);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[272], 3436020178);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[273], 2577026719);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[274], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[275], 2863357870);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[276], 1431702105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[277], 46334);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[278], 3436020176);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[279], 2577026717);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[280], 92679);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[281], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[282], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[283], 2863311533);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[284], 1431655768);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[285], 4294967293);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[286], 3435973839);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[287], 2576980380);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[288], 46342);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[289], 4294920958);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[290], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[291], 2863311534);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[292], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[293], 3435973840);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[294], 2576980381);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[295], 4294920959);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[296], 2863311532);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[297], 1431655767);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[298], 4294967292);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[299], 3435973838);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[300], 2576980379);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[301], 46341);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[302], 4294920957);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[303], 1431655761);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[304], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[305], 2863311530);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[306], 1431655759);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[307], 572662305);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[308], 4008636142);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[309], 1431702104);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[310], 1431609424);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[311], 1431655762);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[312], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[313], 1431655764);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[314], 1431655760);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[315], 572662306);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[316], 4008636143);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[317], 1431609425);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[318], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[319], 2863311529);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[320], 572662304);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[321], 4008636141);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[322], 1431702103);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[323], 1431609423);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[324], 4294967293);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[325], 2863311531);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[326], 1431655766);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[327], 4294967291);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[328], 3435973837);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[329], 2576980378);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[330], 46340);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[331], 4294920956);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[332], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[333], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[334], 2863311525);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[335], 2004318071);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[336], 1145324612);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[337], 2863357870);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[338], 2863265190);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[339], 3);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[340], 2863311537);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[341], 1431655772);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[342], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[343], 3435973843);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[344], 2576980384);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[345], 46346);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[346], 4294920962);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[347], 4);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[348], 2863311538);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[349], 6);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[350], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[351], 3435973844);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[352], 2576980385);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[353], 4294920963);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[354], 2863311536);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[355], 1431655771);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[356], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[357], 3435973842);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[358], 2576980383);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[359], 46345);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[360], 4294920961);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[361], 858993457);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[362], 3722304991);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[363], 2290649226);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[364], 858993455);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[365], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[366], 3435973838);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[367], 859039800);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[368], 858947120);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[369], 858993458);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[370], 3722304992);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[371], 858993460);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[372], 858993456);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[373], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[374], 3435973839);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[375], 858947121);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[376], 3722304990);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[377], 2290649225);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[378], 858993454);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[379], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[380], 3435973837);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[381], 859039799);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[382], 858947119);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[383], 1717986916);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[384], 286331154);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[385], 3149642685);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[386], 1717986914);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[387], 858993460);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[388], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[389], 1718033259);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[390], 1717940579);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[391], 1717986917);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[392], 286331155);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[393], 1717986919);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[394], 1717986915);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[395], 858993461);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[396], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[397], 1717940580);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[398], 286331153);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[399], 3149642684);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[400], 1717986913);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[401], 858993459);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[402], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[403], 1718033258);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[404], 1717940578);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[405], 4294920954);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[406], 2863265192);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[407], 1431609427);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[408], 4294920952);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[409], 3435927498);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[410], 2576934039);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[411], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[412], 4294874617);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[413], 4294920955);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[414], 2863265193);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[415], 4294920957);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[416], 4294920953);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[417], 3435927499);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[418], 2576934040);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[419], 4294874618);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[420], 2863265191);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[421], 1431609426);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[422], 4294920951);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[423], 3435927497);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[424], 2576934038);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[425], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[426], 4294874616);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[427], 46338);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[428], 2863357872);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[429], 1431702107);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[430], 46336);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[431], 3436020178);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[432], 2577026719);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[433], 92681);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[434], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[435], 46339);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[436], 2863357873);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[437], 46341);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[438], 46337);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[439], 3436020179);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[440], 2577026720);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[441], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[442], 2863357871);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[443], 1431702106);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[444], 46335);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[445], 3436020177);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[446], 2577026718);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[447], 92680);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[448], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[449], 2863311532);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[450], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[451], 4294967292);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[452], 3435973838);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[453], 2576980379);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[454], 4294920957);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[455], 2863311530);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[456], 1431655765);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[457], 4294967290);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[458], 3435973836);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[459], 2576980377);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[460], 46339);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[461], 4294920955);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[462], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[463], 2863311535);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[464], 1431655770);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[465], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[466], 3435973841);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[467], 2576980382);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[468], 46344);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[469], 4294920960);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[470], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[471], 2863311536);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[472], 4);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[473], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[474], 3435973842);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[475], 2576980383);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[476], 4294920961);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[477], 2863311534);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[478], 1431655769);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[479], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[480], 3435973840);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[481], 2576980381);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[482], 46343);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[483], 4294920959);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[484], 858993455);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[485], 3722304989);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[486], 2290649224);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[487], 858993453);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[488], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[489], 3435973836);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[490], 859039798);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[491], 858947118);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[492], 858993456);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[493], 3722304990);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[494], 858993458);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[495], 858993454);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[496], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[497], 3435973837);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[498], 858947119);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[499], 3722304988);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[500], 2290649223);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[501], 858993452);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[502], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[503], 3435973835);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[504], 859039797);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[505], 858947117);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[506], 1717986914);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[507], 286331152);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[508], 3149642683);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[509], 1717986912);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[510], 858993458);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[511], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[512], 1718033257);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[513], 1717940577);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[514], 1717986915);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[515], 286331153);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[516], 1717986917);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[517], 1717986913);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[518], 858993459);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[519], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[520], 1717940578);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[521], 286331151);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[522], 3149642682);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[523], 1717986911);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[524], 858993457);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[525], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[526], 1718033256);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[527], 1717940576);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[528], 46336);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[529], 2863357870);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[530], 1431702105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[531], 46334);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[532], 3436020176);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[533], 2577026717);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[534], 92679);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[535], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[536], 46337);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[537], 2863357871);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[538], 46339);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[539], 46335);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[540], 3436020177);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[541], 2577026718);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[542], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[543], 2863357869);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[544], 1431702104);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[545], 46333);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[546], 3436020175);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[547], 2577026716);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[548], 92678);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[549], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[550], 1431655763);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[551], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[552], 2863311532);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[553], 1431655761);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[554], 572662307);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[555], 4008636144);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[556], 1431702106);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[557], 1431609426);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[558], 1431655764);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[559], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[560], 1431655766);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[561], 1431655762);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[562], 572662308);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[563], 4008636145);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[564], 1431609427);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[565], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[566], 2863311531);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[567], 1431655760);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[568], 572662306);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[569], 4008636143);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[570], 1431702105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[571], 1431609425);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[572], 2863311528);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[573], 1431655766);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[574], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[575], 2863311526);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[576], 2004318072);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[577], 1145324613);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[578], 2863357871);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[579], 2863265191);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[580], 2863311529);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[581], 1431655767);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[582], 2863311531);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[583], 2863311527);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[584], 2004318073);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[585], 1145324614);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[586], 2863265192);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[587], 1431655765);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x7___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[588], 122);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[589], 32);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x9___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[590], 4294967177);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[591], 2147481600);
}

