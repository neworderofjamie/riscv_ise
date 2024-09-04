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
class add_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        // Generate word-aligned vector to initialise memory
        const std::vector<uint32_t> scalarWordInitData{
        3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,};
        
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
        
        c.li(Reg::X3, 0);
        // rs2 == rd != rs1, rs1==x4, rs2==x24, rd==x24, rs1_val > 0 and rs2_val > 0, rs2_val == 1, rs1_val == (2**(xlen-1)-1), rs1_val != rs2_val, rs1_val == 2147483647
        // opcode: add ; op1:x4; op2:x24; dest:x24; op1val:0x7fffffff;  op2val:0x1
        c.li(Reg::X4, MASK_XLEN(0x7fffffff));
        c.li(Reg::X24, MASK_XLEN(0x1));
        c.add(Reg::X24, Reg::X4, Reg::X24);
        c.sw(Reg::X24, Reg::X3, 0);

        // rs1 == rs2 != rd, rs1==x10, rs2==x10, rd==x28, rs1_val > 0 and rs2_val < 0, rs2_val == -257, rs1_val == 131072
        // opcode: add ; op1:x10; op2:x10; dest:x28; op1val:0x20000;  op2val:0x20000
        c.li(Reg::X10, MASK_XLEN(0x20000));
        c.li(Reg::X10, MASK_XLEN(0x20000));
        c.add(Reg::X28, Reg::X10, Reg::X10);
        c.sw(Reg::X28, Reg::X3, 4);

        // rs1 == rs2 == rd, rs1==x21, rs2==x21, rd==x21, rs1_val < 0 and rs2_val < 0, rs1_val == -16777217
        // opcode: add ; op1:x21; op2:x21; dest:x21; op1val:-0x1000001;  op2val:-0x1000001
        c.li(Reg::X21, MASK_XLEN(-0x1000001));
        c.li(Reg::X21, MASK_XLEN(-0x1000001));
        c.add(Reg::X21, Reg::X21, Reg::X21);
        c.sw(Reg::X21, Reg::X3, 8);

        // rs1 == rd != rs2, rs1==x22, rs2==x31, rd==x22, rs1_val < 0 and rs2_val > 0, rs1_val == -2, rs2_val == 262144
        // opcode: add ; op1:x22; op2:x31; dest:x22; op1val:-0x2;  op2val:0x40000
        c.li(Reg::X22, MASK_XLEN(-0x2));
        c.li(Reg::X31, MASK_XLEN(0x40000));
        c.add(Reg::X22, Reg::X22, Reg::X31);
        c.sw(Reg::X22, Reg::X3, 12);

        // rs1 != rs2  and rs1 != rd and rs2 != rd, rs1==x12, rs2==x6, rd==x11, rs1_val == rs2_val, rs1_val==1431655766 and rs2_val==1431655766
        // opcode: add ; op1:x12; op2:x6; dest:x11; op1val:0x55555556;  op2val:0x55555556
        c.li(Reg::X12, MASK_XLEN(0x55555556));
        c.li(Reg::X6, MASK_XLEN(0x55555556));
        c.add(Reg::X11, Reg::X12, Reg::X6);
        c.sw(Reg::X11, Reg::X3, 16);

        // rs1==x29, rs2==x13, rd==x10, rs2_val == (-2**(xlen-1)), rs2_val == -2147483648, rs1_val == 2
        // opcode: add ; op1:x29; op2:x13; dest:x10; op1val:0x2;  op2val:-0x80000000
        c.li(Reg::X29, MASK_XLEN(0x2));
        c.li(Reg::X13, MASK_XLEN(-0x80000000));
        c.add(Reg::X10, Reg::X29, Reg::X13);
        c.sw(Reg::X10, Reg::X3, 20);

        // rs1==x31, rs2==x5, rd==x26, rs2_val == 0, rs1_val == -17
        // opcode: add ; op1:x31; op2:x5; dest:x26; op1val:-0x11;  op2val:0x0
        c.li(Reg::X31, MASK_XLEN(-0x11));
        c.li(Reg::X5, MASK_XLEN(0x0));
        c.add(Reg::X26, Reg::X31, Reg::X5);
        c.sw(Reg::X26, Reg::X3, 24);

        // rs1==x2, rs2==x1, rd==x7, rs2_val == (2**(xlen-1)-1), rs2_val == 2147483647
        // opcode: add ; op1:x2; op2:x1; dest:x7; op1val:0x66666666;  op2val:0x7fffffff
        c.li(Reg::X2, MASK_XLEN(0x66666666));
        c.li(Reg::X1, MASK_XLEN(0x7fffffff));
        c.add(Reg::X7, Reg::X2, Reg::X1);
        c.sw(Reg::X7, Reg::X3, 28);

        // rs1==x8, rs2==x25, rd==x14, rs1_val == (-2**(xlen-1)), rs1_val == -2147483648, rs2_val == -1431655766
        // opcode: add ; op1:x8; op2:x25; dest:x14; op1val:-0x80000000;  op2val:-0x55555556
        c.li(Reg::X8, MASK_XLEN(-0x80000000));
        c.li(Reg::X25, MASK_XLEN(-0x55555556));
        c.add(Reg::X14, Reg::X8, Reg::X25);
        c.sw(Reg::X14, Reg::X3, 32);

        // rs1==x13, rs2==x8, rd==x1, rs1_val == 0, rs2_val == -33554433
        // opcode: add ; op1:x13; op2:x8; dest:x1; op1val:0x0;  op2val:-0x2000001
        c.li(Reg::X13, MASK_XLEN(0x0));
        c.li(Reg::X8, MASK_XLEN(-0x2000001));
        c.add(Reg::X1, Reg::X13, Reg::X8);
        c.sw(Reg::X1, Reg::X3, 36);

        // rs1==x28, rs2==x9, rd==x0, rs1_val == 1, rs2_val == 8388608
        // opcode: add ; op1:x28; op2:x9; dest:x0; op1val:0x1;  op2val:0x800000
        c.li(Reg::X28, MASK_XLEN(0x1));
        c.li(Reg::X9, MASK_XLEN(0x800000));
        c.add(Reg::X0, Reg::X28, Reg::X9);
        c.sw(Reg::X0, Reg::X3, 40);

        // rs1==x14, rs2==x4, rd==x20, rs2_val == 2, 
        // opcode: add ; op1:x14; op2:x4; dest:x20; op1val:0x7;  op2val:0x2
        c.li(Reg::X14, MASK_XLEN(0x7));
        c.li(Reg::X4, MASK_XLEN(0x2));
        c.add(Reg::X20, Reg::X14, Reg::X4);
        c.sw(Reg::X20, Reg::X3, 44);

        // rs1==x7, rs2==x19, rd==x16, rs2_val == 4, rs1_val == 8
        // opcode: add ; op1:x7; op2:x19; dest:x16; op1val:0x8;  op2val:0x4
        c.li(Reg::X7, MASK_XLEN(0x8));
        c.li(Reg::X19, MASK_XLEN(0x4));
        c.add(Reg::X16, Reg::X7, Reg::X19);
        c.sw(Reg::X16, Reg::X3, 48);

        // rs1==x23, rs2==x29, rd==x8, rs2_val == 8, rs1_val == 2048
        // opcode: add ; op1:x23; op2:x29; dest:x8; op1val:0x800;  op2val:0x8
        c.li(Reg::X23, MASK_XLEN(0x800));
        c.li(Reg::X29, MASK_XLEN(0x8));
        c.add(Reg::X8, Reg::X23, Reg::X29);
        c.sw(Reg::X8, Reg::X3, 52);

        // rs1==x5, rs2==x27, rd==x13, rs2_val == 16, 
        // opcode: add ; op1:x5; op2:x27; dest:x13; op1val:0x0;  op2val:0x10
        c.li(Reg::X5, MASK_XLEN(0x0));
        c.li(Reg::X27, MASK_XLEN(0x10));
        c.add(Reg::X13, Reg::X5, Reg::X27);
        c.sw(Reg::X13, Reg::X3, 56);

        // rs1==x25, rs2==x20, rd==x27, rs2_val == 32, 
        // opcode: add ; op1:x25; op2:x20; dest:x27; op1val:0x55555556;  op2val:0x20
        c.li(Reg::X25, MASK_XLEN(0x55555556));
        c.li(Reg::X20, MASK_XLEN(0x20));
        c.add(Reg::X27, Reg::X25, Reg::X20);
        c.sw(Reg::X27, Reg::X3, 60);

        // rs1==x15, rs2==x26, rd==x17, rs2_val == 64, 
        // opcode: add ; op1:x15; op2:x26; dest:x17; op1val:-0x11;  op2val:0x40
        c.li(Reg::X15, MASK_XLEN(-0x11));
        c.li(Reg::X26, MASK_XLEN(0x40));
        c.add(Reg::X17, Reg::X15, Reg::X26);
        c.sw(Reg::X17, Reg::X3, 64);

        c.li(Reg::X8, 68);
        // rs1==x17, rs2==x2, rd==x29, rs2_val == 128, rs1_val == -5
        // opcode: add ; op1:x17; op2:x2; dest:x29; op1val:-0x5;  op2val:0x80
        c.li(Reg::X17, MASK_XLEN(-0x5));
        c.li(Reg::X2, MASK_XLEN(0x80));
        c.add(Reg::X29, Reg::X17, Reg::X2);
        c.sw(Reg::X29, Reg::X8, 0);

        // rs1==x24, rs2==x17, rd==x4, rs2_val == 256, rs1_val == 32
        // opcode: add ; op1:x24; op2:x17; dest:x4; op1val:0x20;  op2val:0x100
        c.li(Reg::X24, MASK_XLEN(0x20));
        c.li(Reg::X17, MASK_XLEN(0x100));
        c.add(Reg::X4, Reg::X24, Reg::X17);
        c.sw(Reg::X4, Reg::X8, 4);

        // rs1==x16, rs2==x11, rd==x2, rs2_val == 512, rs1_val == 1073741824
        // opcode: add ; op1:x16; op2:x11; dest:x2; op1val:0x40000000;  op2val:0x200
        c.li(Reg::X16, MASK_XLEN(0x40000000));
        c.li(Reg::X11, MASK_XLEN(0x200));
        c.add(Reg::X2, Reg::X16, Reg::X11);
        c.sw(Reg::X2, Reg::X8, 8);

        // rs1==x0, rs2==x16, rd==x6, rs2_val == 1024, 
        // opcode: add ; op1:x0; op2:x16; dest:x6; op1val:0x0;  op2val:0x400
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.li(Reg::X16, MASK_XLEN(0x400));
        c.add(Reg::X6, Reg::X0, Reg::X16);
        c.sw(Reg::X6, Reg::X8, 12);

        // rs1==x11, rs2==x3, rd==x31, rs2_val == 2048, 
        // opcode: add ; op1:x11; op2:x3; dest:x31; op1val:-0x55555555;  op2val:0x800
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.li(Reg::X3, MASK_XLEN(0x800));
        c.add(Reg::X31, Reg::X11, Reg::X3);
        c.sw(Reg::X31, Reg::X8, 16);

        // rs1==x18, rs2==x0, rd==x12, rs2_val == 4096, rs1_val == 4
        // opcode: add ; op1:x18; op2:x0; dest:x12; op1val:0x4;  op2val:0x0
        c.li(Reg::X18, MASK_XLEN(0x4));
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X18, Reg::X0);
        c.sw(Reg::X12, Reg::X8, 20);

        // rs1==x30, rs2==x7, rd==x9, rs2_val == 8192, rs1_val == -33554433
        // opcode: add ; op1:x30; op2:x7; dest:x9; op1val:-0x2000001;  op2val:0x2000
        c.li(Reg::X30, MASK_XLEN(-0x2000001));
        c.li(Reg::X7, MASK_XLEN(0x2000));
        c.add(Reg::X9, Reg::X30, Reg::X7);
        c.sw(Reg::X9, Reg::X8, 24);

        // rs1==x3, rs2==x30, rd==x5, rs2_val == 16384, 
        // opcode: add ; op1:x3; op2:x30; dest:x5; op1val:0xb503;  op2val:0x4000
        c.li(Reg::X3, MASK_XLEN(0xb503));
        c.li(Reg::X30, MASK_XLEN(0x4000));
        c.add(Reg::X5, Reg::X3, Reg::X30);
        c.sw(Reg::X5, Reg::X8, 28);

        // rs1==x27, rs2==x28, rd==x18, rs2_val == 32768, 
        // opcode: add ; op1:x27; op2:x28; dest:x18; op1val:-0xb503;  op2val:0x8000
        c.li(Reg::X27, MASK_XLEN(-0xb503));
        c.li(Reg::X28, MASK_XLEN(0x8000));
        c.add(Reg::X18, Reg::X27, Reg::X28);
        c.sw(Reg::X18, Reg::X8, 32);

        // rs1==x9, rs2==x14, rd==x15, rs2_val == 65536, rs1_val == -513
        // opcode: add ; op1:x9; op2:x14; dest:x15; op1val:-0x201;  op2val:0x10000
        c.li(Reg::X9, MASK_XLEN(-0x201));
        c.li(Reg::X14, MASK_XLEN(0x10000));
        c.add(Reg::X15, Reg::X9, Reg::X14);
        c.sw(Reg::X15, Reg::X8, 36);

        // rs1==x26, rs2==x15, rd==x3, rs2_val == 131072, rs1_val == 64
        // opcode: add ; op1:x26; op2:x15; dest:x3; op1val:0x40;  op2val:0x20000
        c.li(Reg::X26, MASK_XLEN(0x40));
        c.li(Reg::X15, MASK_XLEN(0x20000));
        c.add(Reg::X3, Reg::X26, Reg::X15);
        c.sw(Reg::X3, Reg::X8, 40);

        // rs1==x1, rs2==x18, rd==x23, rs2_val == 524288, 
        // opcode: add ; op1:x1; op2:x18; dest:x23; op1val:0x3;  op2val:0x80000
        c.li(Reg::X1, MASK_XLEN(0x3));
        c.li(Reg::X18, MASK_XLEN(0x80000));
        c.add(Reg::X23, Reg::X1, Reg::X18);
        c.sw(Reg::X23, Reg::X8, 44);

        // rs1==x6, rs2==x12, rd==x30, rs2_val == 1048576, rs1_val == -65537
        // opcode: add ; op1:x6; op2:x12; dest:x30; op1val:-0x10001;  op2val:0x100000
        c.li(Reg::X6, MASK_XLEN(-0x10001));
        c.li(Reg::X12, MASK_XLEN(0x100000));
        c.add(Reg::X30, Reg::X6, Reg::X12);
        c.sw(Reg::X30, Reg::X8, 48);

        // rs1==x20, rs2==x22, rd==x19, rs2_val == 2097152, rs1_val == -4194305
        // opcode: add ; op1:x20; op2:x22; dest:x19; op1val:-0x400001;  op2val:0x200000
        c.li(Reg::X20, MASK_XLEN(-0x400001));
        c.li(Reg::X22, MASK_XLEN(0x200000));
        c.add(Reg::X19, Reg::X20, Reg::X22);
        c.sw(Reg::X19, Reg::X8, 52);

        // rs1==x19, rs2==x23, rd==x25, rs2_val == 4194304, rs1_val == 4194304
        // opcode: add ; op1:x19; op2:x23; dest:x25; op1val:0x400000;  op2val:0x400000
        c.li(Reg::X19, MASK_XLEN(0x400000));
        c.li(Reg::X23, MASK_XLEN(0x400000));
        c.add(Reg::X25, Reg::X19, Reg::X23);
        c.sw(Reg::X25, Reg::X8, 56);

        // rs2_val == 16777216, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x1000000
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x1000000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X8, 60);

        c.li(Reg::X1, 132);
        // rs2_val == 33554432, rs1_val == -131073
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x20001;  op2val:0x2000000
        c.li(Reg::X10, MASK_XLEN(-0x20001));
        c.li(Reg::X11, MASK_XLEN(0x2000000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 0);

        // rs2_val == 67108864, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x4000000
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x4000000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 4);

        // rs2_val == 134217728, rs1_val == -32769
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x8001;  op2val:0x8000000
        c.li(Reg::X10, MASK_XLEN(-0x8001));
        c.li(Reg::X11, MASK_XLEN(0x8000000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 8);

        // rs2_val == 268435456, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x40000000;  op2val:0x10000000
        c.li(Reg::X10, MASK_XLEN(0x40000000));
        c.li(Reg::X11, MASK_XLEN(0x10000000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 12);

        // rs2_val == 536870912, rs1_val == -16385
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x4001;  op2val:0x20000000
        c.li(Reg::X10, MASK_XLEN(-0x4001));
        c.li(Reg::X11, MASK_XLEN(0x20000000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 16);

        // rs2_val == 1073741824, rs1_val == 65536
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x40000000
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.li(Reg::X11, MASK_XLEN(0x40000000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 20);

        // rs2_val == -2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x2
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 24);

        // rs2_val == -3, rs1_val == -33
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x21;  op2val:-0x3
        c.li(Reg::X10, MASK_XLEN(-0x21));
        c.li(Reg::X11, MASK_XLEN(-0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 28);

        // rs2_val == -5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x21;  op2val:-0x5
        c.li(Reg::X10, MASK_XLEN(-0x21));
        c.li(Reg::X11, MASK_XLEN(-0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 32);

        // rs2_val == -9, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x20001;  op2val:-0x9
        c.li(Reg::X10, MASK_XLEN(-0x20001));
        c.li(Reg::X11, MASK_XLEN(-0x9));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 36);

        // rs2_val == -17, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x800;  op2val:-0x11
        c.li(Reg::X10, MASK_XLEN(0x800));
        c.li(Reg::X11, MASK_XLEN(-0x11));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 40);

        // rs2_val == -33, rs1_val == 536870912
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x20000000;  op2val:-0x21
        c.li(Reg::X10, MASK_XLEN(0x20000000));
        c.li(Reg::X11, MASK_XLEN(-0x21));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 44);

        // rs2_val == -65, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x7fffffff;  op2val:-0x41
        c.li(Reg::X10, MASK_XLEN(0x7fffffff));
        c.li(Reg::X11, MASK_XLEN(-0x41));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 48);

        // rs2_val == -129, rs1_val == -8388609
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x800001;  op2val:-0x81
        c.li(Reg::X10, MASK_XLEN(-0x800001));
        c.li(Reg::X11, MASK_XLEN(-0x81));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 52);

        // rs2_val == -513, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x201
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x201));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 56);

        // rs2_val == -1025, rs1_val == -4097
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x1001;  op2val:-0x401
        c.li(Reg::X10, MASK_XLEN(-0x1001));
        c.li(Reg::X11, MASK_XLEN(-0x401));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 60);

        // rs2_val == -2049, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x801
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0x801));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 64);

        // rs2_val == -4097, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x1001
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x1001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 68);

        // rs2_val == -8193, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x2001
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x2001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 72);

        // rs2_val == -16385, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x7fffffff;  op2val:-0x4001
        c.li(Reg::X10, MASK_XLEN(0x7fffffff));
        c.li(Reg::X11, MASK_XLEN(-0x4001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 76);

        // rs2_val == -32769, rs1_val == -3
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x3;  op2val:-0x8001
        c.li(Reg::X10, MASK_XLEN(-0x3));
        c.li(Reg::X11, MASK_XLEN(-0x8001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 80);

        // rs2_val == -65537, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x10001
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x10001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 84);

        // rs2_val == -131073, rs1_val == -1431655766
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x20001
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x20001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 88);

        // rs2_val == -262145, rs1_val == 16777216
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x1000000;  op2val:-0x40001
        c.li(Reg::X10, MASK_XLEN(0x1000000));
        c.li(Reg::X11, MASK_XLEN(-0x40001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 92);

        // rs2_val == -524289, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x7fffffff;  op2val:-0x80001
        c.li(Reg::X10, MASK_XLEN(0x7fffffff));
        c.li(Reg::X11, MASK_XLEN(-0x80001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 96);

        // rs2_val == -1048577, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x800;  op2val:-0x100001
        c.li(Reg::X10, MASK_XLEN(0x800));
        c.li(Reg::X11, MASK_XLEN(-0x100001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 100);

        // rs2_val == -2097153, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x8;  op2val:-0x200001
        c.li(Reg::X10, MASK_XLEN(0x8));
        c.li(Reg::X11, MASK_XLEN(-0x200001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 104);

        // rs2_val == -4194305, rs1_val == 8192
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2000;  op2val:-0x400001
        c.li(Reg::X10, MASK_XLEN(0x2000));
        c.li(Reg::X11, MASK_XLEN(-0x400001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 108);

        // rs2_val == -8388609, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x21;  op2val:-0x800001
        c.li(Reg::X10, MASK_XLEN(-0x21));
        c.li(Reg::X11, MASK_XLEN(-0x800001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 112);

        // rs2_val == -16777217, rs1_val == -2097153
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x200001;  op2val:-0x1000001
        c.li(Reg::X10, MASK_XLEN(-0x200001));
        c.li(Reg::X11, MASK_XLEN(-0x1000001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 116);

        // rs2_val == -67108865, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x4000001
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x4000001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 120);

        // rs2_val == -134217729, rs1_val == 1431655765
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x8000001
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x8000001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 124);

        // rs2_val == -268435457, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x20000;  op2val:-0x10000001
        c.li(Reg::X10, MASK_XLEN(0x20000));
        c.li(Reg::X11, MASK_XLEN(-0x10000001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 128);

        // rs2_val == -536870913, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x20000;  op2val:-0x20000001
        c.li(Reg::X10, MASK_XLEN(0x20000));
        c.li(Reg::X11, MASK_XLEN(-0x20000001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 132);

        // rs2_val == -1073741825, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x7;  op2val:-0x40000001
        c.li(Reg::X10, MASK_XLEN(0x7));
        c.li(Reg::X11, MASK_XLEN(-0x40000001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 136);

        // rs2_val == 1431655765, rs1_val == 1048576
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x100000;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x100000));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 140);

        // rs1_val == 16, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x10;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x10));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 144);

        // rs1_val == 128, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x80;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x80));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 148);

        // rs1_val == 256, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x100;  op2val:-0xa
        c.li(Reg::X10, MASK_XLEN(0x100));
        c.li(Reg::X11, MASK_XLEN(-0xa));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 152);

        // rs1_val == 512, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x200;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x200));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 156);

        // rs1_val == 1024, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x400;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x400));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 160);

        // rs1_val == 4096, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x1000;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x1000));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 164);

        // rs1_val == 16384, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4000;  op2val:-0x801
        c.li(Reg::X10, MASK_XLEN(0x4000));
        c.li(Reg::X11, MASK_XLEN(-0x801));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 168);

        // rs1_val == 32768, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x8000;  op2val:-0x1000001
        c.li(Reg::X10, MASK_XLEN(0x8000));
        c.li(Reg::X11, MASK_XLEN(-0x1000001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 172);

        // rs1_val == 262144, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x40000;  op2val:0x1000000
        c.li(Reg::X10, MASK_XLEN(0x40000));
        c.li(Reg::X11, MASK_XLEN(0x1000000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 176);

        // rs1_val == 524288, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x80000;  op2val:0x8000
        c.li(Reg::X10, MASK_XLEN(0x80000));
        c.li(Reg::X11, MASK_XLEN(0x8000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 180);

        // rs1_val == 2097152, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x200000;  op2val:0x10000000
        c.li(Reg::X10, MASK_XLEN(0x200000));
        c.li(Reg::X11, MASK_XLEN(0x10000000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 184);

        // rs1_val == 8388608, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x800000;  op2val:-0x401
        c.li(Reg::X10, MASK_XLEN(0x800000));
        c.li(Reg::X11, MASK_XLEN(-0x401));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 188);

        // rs1_val == 33554432, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2000000;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x2000000));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 192);

        // rs1_val == 67108864, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4000000;  op2val:0x8000
        c.li(Reg::X10, MASK_XLEN(0x4000000));
        c.li(Reg::X11, MASK_XLEN(0x8000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 196);

        // rs1_val == 134217728, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x8000000;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x8000000));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 200);

        // rs1_val == 268435456, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x10000000;  op2val:-0x40000000
        c.li(Reg::X10, MASK_XLEN(0x10000000));
        c.li(Reg::X11, MASK_XLEN(-0x40000000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 204);

        // rs1_val == -9, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x9;  op2val:-0x80001
        c.li(Reg::X10, MASK_XLEN(-0x9));
        c.li(Reg::X11, MASK_XLEN(-0x80001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 208);

        // rs1_val == -65, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x41;  op2val:-0x80000000
        c.li(Reg::X10, MASK_XLEN(-0x41));
        c.li(Reg::X11, MASK_XLEN(-0x80000000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 212);

        // rs1_val == -129, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x81;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x81));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 216);

        // rs1_val == -257, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x101;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0x101));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 220);

        // rs1_val == -1025, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x401;  op2val:-0x8
        c.li(Reg::X10, MASK_XLEN(-0x401));
        c.li(Reg::X11, MASK_XLEN(-0x8));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 224);

        // rs1_val == -2049, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x801;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0x801));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 228);

        // rs1_val == -8193, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x2001;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0x2001));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 232);

        // rs1_val == -262145, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x40001;  op2val:-0x800001
        c.li(Reg::X10, MASK_XLEN(-0x40001));
        c.li(Reg::X11, MASK_XLEN(-0x800001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 236);

        // rs1_val == -524289, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x80001;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0x80001));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 240);

        // rs1_val == -1048577, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x100001;  op2val:-0x40000000
        c.li(Reg::X10, MASK_XLEN(-0x100001));
        c.li(Reg::X11, MASK_XLEN(-0x40000000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 244);

        // rs1_val == -67108865, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x4000001;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0x4000001));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 248);

        // rs1_val == -134217729, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x8000001;  op2val:-0x10000001
        c.li(Reg::X10, MASK_XLEN(-0x8000001));
        c.li(Reg::X11, MASK_XLEN(-0x10000001));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 252);

        // rs1_val == -268435457, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x10000001;  op2val:0x800
        c.li(Reg::X10, MASK_XLEN(-0x10000001));
        c.li(Reg::X11, MASK_XLEN(0x800));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 256);

        // rs1_val == -536870913, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x20000001;  op2val:0x20000
        c.li(Reg::X10, MASK_XLEN(-0x20000001));
        c.li(Reg::X11, MASK_XLEN(0x20000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 260);

        // rs1_val == -1073741825, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x40000001;  op2val:-0x81
        c.li(Reg::X10, MASK_XLEN(-0x40000001));
        c.li(Reg::X11, MASK_XLEN(-0x81));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 264);

        // rs1_val==3 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 268);

        // rs1_val==3 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 272);

        // rs1_val==3 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 276);

        // rs1_val==3 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 280);

        // rs1_val==3 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 284);

        // rs1_val==3 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 288);

        // rs1_val==3 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 292);

        // rs1_val==3 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 296);

        // rs1_val==3 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 300);

        // rs1_val==3 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 304);

        // rs1_val==3 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 308);

        // rs1_val==3 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 312);

        // rs1_val==3 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 316);

        // rs1_val==3 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 320);

        // rs1_val==3 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 324);

        // rs1_val==3 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 328);

        // rs1_val==3 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 332);

        // rs1_val==3 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 336);

        // rs1_val==3 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 340);

        // rs1_val==3 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 344);

        // rs1_val==3 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 348);

        // rs1_val==3 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 352);

        // rs1_val==1431655765 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 356);

        // rs1_val==1431655765 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 360);

        // rs1_val==1431655765 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 364);

        // rs1_val==1431655765 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 368);

        // rs1_val==1431655765 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 372);

        // rs1_val==1431655765 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 376);

        // rs1_val==1431655765 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 380);

        // rs1_val==1431655765 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 384);

        // rs1_val==1431655765 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 388);

        // rs1_val==1431655765 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 392);

        // rs1_val==1431655765 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 396);

        // rs1_val==1431655765 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 400);

        // rs1_val==1431655765 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 404);

        // rs1_val==1431655765 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 408);

        // rs1_val==1431655765 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 412);

        // rs1_val==1431655765 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 416);

        // rs1_val==1431655765 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 420);

        // rs1_val==1431655765 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 424);

        // rs1_val==1431655765 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 428);

        // rs1_val==1431655765 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 432);

        // rs1_val==1431655765 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 436);

        // rs1_val==1431655765 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 440);

        // rs1_val==-1431655766 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 444);

        // rs1_val==-1431655766 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 448);

        // rs1_val==-1431655766 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 452);

        // rs1_val==-1431655766 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 456);

        // rs1_val==-1431655766 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 460);

        // rs1_val==-1431655766 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 464);

        // rs1_val==-1431655766 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 468);

        // rs1_val==-1431655766 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 472);

        // rs1_val==-1431655766 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 476);

        // rs1_val==-1431655766 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 480);

        // rs1_val==-1431655766 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 484);

        // rs1_val==-1431655766 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 488);

        // rs1_val==-1431655766 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 492);

        // rs1_val==-1431655766 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 496);

        // rs1_val==-1431655766 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 500);

        // rs1_val==-1431655766 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 504);

        // rs1_val==-1431655766 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 508);

        // rs1_val==-1431655766 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 512);

        // rs1_val==-1431655766 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 516);

        // rs1_val==-1431655766 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 520);

        // rs1_val==-1431655766 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 524);

        // rs1_val==-1431655766 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 528);

        // rs1_val==5 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 532);

        // rs1_val==5 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 536);

        // rs1_val==5 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 540);

        // rs1_val==5 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 544);

        // rs1_val==5 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 548);

        // rs1_val==5 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 552);

        // rs1_val==5 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 556);

        // rs1_val==5 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 560);

        // rs1_val==5 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 564);

        // rs1_val==5 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 568);

        // rs1_val==5 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 572);

        // rs1_val==5 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 576);

        // rs1_val==5 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 580);

        // rs1_val==5 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 584);

        // rs1_val==5 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 588);

        // rs1_val==5 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 592);

        // rs1_val==5 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 596);

        // rs1_val==5 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 600);

        // rs1_val==5 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 604);

        // rs1_val==5 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 608);

        // rs1_val==5 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 612);

        // rs1_val==5 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 616);

        // rs1_val==858993459 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 620);

        // rs1_val==858993459 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 624);

        // rs1_val==858993459 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 628);

        // rs1_val==858993459 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 632);

        // rs1_val==858993459 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 636);

        // rs1_val==858993459 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 640);

        // rs1_val==858993459 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 644);

        // rs1_val==858993459 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 648);

        // rs1_val==858993459 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 652);

        // rs1_val==858993459 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 656);

        // rs1_val==858993459 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 660);

        // rs1_val==858993459 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 664);

        // rs1_val==858993459 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 668);

        // rs1_val==858993459 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 672);

        // rs1_val==858993459 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 676);

        // rs1_val==858993459 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 680);

        // rs1_val==858993459 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 684);

        // rs1_val==858993459 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 688);

        // rs1_val==858993459 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 692);

        // rs1_val==858993459 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 696);

        // rs1_val==858993459 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 700);

        // rs1_val==858993459 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 704);

        // rs1_val==1717986918 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 708);

        // rs1_val==1717986918 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 712);

        // rs1_val==1717986918 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 716);

        // rs1_val==1717986918 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 720);

        // rs1_val==1717986918 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 724);

        // rs1_val==1717986918 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 728);

        // rs1_val==1717986918 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 732);

        // rs1_val==1717986918 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 736);

        // rs1_val==1717986918 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 740);

        // rs1_val==1717986918 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 744);

        // rs1_val==1717986918 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 748);

        // rs1_val==1717986918 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 752);

        // rs1_val==1717986918 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 756);

        // rs1_val==1717986918 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 760);

        // rs1_val==1717986918 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 764);

        // rs1_val==1717986918 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 768);

        // rs1_val==1717986918 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 772);

        // rs1_val==1717986918 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 776);

        // rs1_val==1717986918 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 780);

        // rs1_val==1717986918 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 784);

        // rs1_val==1717986918 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 788);

        // rs1_val==1717986918 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 792);

        // rs1_val==-46340 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 796);

        // rs1_val==-46340 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 800);

        // rs1_val==-46340 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 804);

        // rs1_val==-46340 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 808);

        // rs1_val==-46340 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 812);

        // rs1_val==-46340 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 816);

        // rs1_val==-46340 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 820);

        // rs1_val==-46340 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 824);

        // rs1_val==-46340 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 828);

        // rs1_val==-46340 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 832);

        // rs1_val==-46340 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 836);

        // rs1_val==-46340 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 840);

        // rs1_val==-46340 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 844);

        // rs1_val==-46340 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 848);

        // rs1_val==-46340 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 852);

        // rs1_val==-46340 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 856);

        // rs1_val==-46340 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 860);

        // rs1_val==-46340 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 864);

        // rs1_val==-46340 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 868);

        // rs1_val==-46340 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 872);

        // rs1_val==-46340 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 876);

        // rs1_val==-46340 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 880);

        // rs1_val==46340 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 884);

        // rs1_val==46340 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 888);

        // rs1_val==46340 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 892);

        // rs1_val==46340 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 896);

        // rs1_val==46340 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 900);

        // rs1_val==46340 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 904);

        // rs1_val==46340 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 908);

        // rs1_val==46340 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 912);

        // rs1_val==46340 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 916);

        // rs1_val==46340 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 920);

        // rs1_val==46340 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 924);

        // rs1_val==46340 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 928);

        // rs1_val==46340 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 932);

        // rs1_val==46340 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 936);

        // rs1_val==46340 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 940);

        // rs1_val==46340 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 944);

        // rs1_val==46340 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 948);

        // rs1_val==46340 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 952);

        // rs1_val==46340 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 956);

        // rs1_val==46340 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 960);

        // rs1_val==46340 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 964);

        // rs1_val==46340 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 968);

        // rs1_val==2 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 972);

        // rs1_val==2 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 976);

        // rs1_val==2 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 980);

        // rs1_val==2 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 984);

        // rs1_val==2 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 988);

        // rs1_val==2 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 992);

        // rs1_val==2 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 996);

        // rs1_val==2 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1000);

        // rs1_val==2 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1004);

        // rs1_val==2 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1008);

        // rs1_val==2 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1012);

        // rs1_val==2 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1016);

        // rs1_val==2 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1020);

        // rs1_val==2 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1024);

        // rs1_val==2 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1028);

        // rs1_val==2 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1032);

        // rs1_val==2 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1036);

        // rs1_val==2 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1040);

        // rs1_val==2 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1044);

        // rs1_val==2 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1048);

        // rs1_val==2 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1052);

        // rs1_val==2 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1056);

        // rs1_val==1431655764 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1060);

        // rs1_val==1431655764 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1064);

        // rs1_val==1431655764 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1068);

        // rs1_val==1431655764 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1072);

        // rs1_val==1431655764 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1076);

        // rs1_val==1431655764 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1080);

        // rs1_val==1431655764 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1084);

        // rs1_val==1431655764 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1088);

        // rs1_val==1431655764 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1092);

        // rs1_val==1431655764 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1096);

        // rs1_val==1431655764 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1100);

        // rs1_val==1431655764 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1104);

        // rs1_val==1431655764 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1108);

        // rs1_val==1431655764 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1112);

        // rs1_val==1431655764 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1116);

        // rs1_val==1431655764 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1120);

        // rs1_val==1431655764 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1124);

        // rs1_val==1431655764 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1128);

        // rs1_val==1431655764 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1132);

        // rs1_val==1431655764 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1136);

        // rs1_val==1431655764 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1140);

        // rs1_val==1431655764 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1144);

        // rs1_val==0 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1148);

        // rs1_val==0 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1152);

        // rs1_val==0 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1156);

        // rs1_val==0 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1160);

        // rs1_val==0 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1164);

        // rs1_val==0 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1168);

        // rs1_val==0 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1172);

        // rs1_val==0 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1176);

        // rs1_val==-1431655765 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1180);

        // rs1_val==-1431655765 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1184);

        // rs1_val==-1431655765 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1188);

        // rs1_val==-1431655765 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1192);

        // rs1_val==-1431655765 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1196);

        // rs1_val==-1431655765 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1200);

        // rs1_val==6 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1204);

        // rs1_val==6 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1208);

        // rs1_val==6 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1212);

        // rs1_val==6 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1216);

        // rs1_val==6 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1220);

        // rs1_val==6 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1224);

        // rs1_val==6 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1228);

        // rs1_val==6 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1232);

        // rs1_val==6 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1236);

        // rs1_val==6 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1240);

        // rs1_val==6 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1244);

        // rs1_val==6 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1248);

        // rs1_val==6 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1252);

        // rs1_val==6 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1256);

        // rs1_val==6 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1260);

        // rs1_val==6 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1264);

        // rs1_val==6 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1268);

        // rs1_val==6 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1272);

        // rs1_val==6 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1276);

        // rs1_val==6 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1280);

        // rs1_val==6 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1284);

        // rs1_val==6 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1288);

        // rs1_val==858993460 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1292);

        // rs1_val==858993460 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1296);

        // rs1_val==858993460 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1300);

        // rs1_val==858993460 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1304);

        // rs1_val==858993460 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1308);

        // rs1_val==858993460 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1312);

        // rs1_val==858993460 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1316);

        // rs1_val==858993460 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1320);

        // rs1_val==858993460 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1324);

        // rs1_val==858993460 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1328);

        // rs1_val==858993460 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1332);

        // rs1_val==858993460 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1336);

        // rs1_val==858993460 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1340);

        // rs1_val==858993460 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1344);

        // rs1_val==858993460 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1348);

        // rs1_val==858993460 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1352);

        // rs1_val==858993460 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1356);

        // rs1_val==858993460 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1360);

        // rs1_val==858993460 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1364);

        // rs1_val==858993460 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1368);

        // rs1_val==858993460 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1372);

        // rs1_val==858993460 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1376);

        // rs1_val==1717986919 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1380);

        // rs1_val==1717986919 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1384);

        // rs1_val==1717986919 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1388);

        // rs1_val==1717986919 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1392);

        // rs1_val==1717986919 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1396);

        // rs1_val==1717986919 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1400);

        // rs1_val==1717986919 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1404);

        // rs1_val==1717986919 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1408);

        // rs1_val==1717986919 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1412);

        // rs1_val==1717986919 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1416);

        // rs1_val==1717986919 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1420);

        // rs1_val==1717986919 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1424);

        // rs1_val==1717986919 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1428);

        // rs1_val==1717986919 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1432);

        // rs1_val==1717986919 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1436);

        // rs1_val==1717986919 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1440);

        // rs1_val==1717986919 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1444);

        // rs1_val==1717986919 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1448);

        // rs1_val==1717986919 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1452);

        // rs1_val==1717986919 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1456);

        // rs1_val==1717986919 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1460);

        // rs1_val==1717986919 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1464);

        // rs1_val==-46339 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1468);

        // rs1_val==-46339 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1472);

        // rs1_val==-46339 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1476);

        // rs1_val==-46339 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1480);

        // rs1_val==-46339 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1484);

        // rs1_val==-46339 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1488);

        // rs1_val==-46339 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1492);

        // rs1_val==-46339 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1496);

        // rs1_val==-46339 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1500);

        // rs1_val==-46339 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1504);

        // rs1_val==-46339 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1508);

        // rs1_val==-46339 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1512);

        // rs1_val==-46339 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1516);

        // rs1_val==-46339 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1520);

        // rs1_val==-46339 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1524);

        // rs1_val==-46339 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1528);

        // rs1_val==-46339 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1532);

        // rs1_val==-46339 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1536);

        // rs1_val==-46339 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1540);

        // rs1_val==-46339 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1544);

        // rs1_val==-46339 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1548);

        // rs1_val==-46339 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1552);

        // rs1_val==46341 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1556);

        // rs1_val==46341 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1560);

        // rs1_val==46341 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1564);

        // rs1_val==46341 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1568);

        // rs1_val==46341 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1572);

        // rs1_val==46341 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1576);

        // rs1_val==46341 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1580);

        // rs1_val==46341 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1584);

        // rs1_val==46341 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1588);

        // rs1_val==46341 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1592);

        // rs1_val==46341 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1596);

        // rs1_val==46341 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1600);

        // rs1_val==46341 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1604);

        // rs1_val==46341 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1608);

        // rs1_val==46341 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1612);

        // rs1_val==46341 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1616);

        // rs1_val==46341 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1620);

        // rs1_val==46341 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1624);

        // rs1_val==46341 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1628);

        // rs1_val==46341 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1632);

        // rs1_val==46341 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1636);

        // rs1_val==46341 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1640);

        // rs1_val==0 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1644);

        // rs1_val==0 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1648);

        // rs1_val==0 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1652);

        // rs1_val==0 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1656);

        // rs1_val==0 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1660);

        // rs1_val==0 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1664);

        // rs1_val==0 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1668);

        // rs1_val==0 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1672);

        // rs1_val==0 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1676);

        // rs1_val==0 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1680);

        // rs1_val==0 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1684);

        // rs1_val==0 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1688);

        // rs1_val==0 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1692);

        // rs1_val==0 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1696);

        // rs1_val==4 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1700);

        // rs1_val==4 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1704);

        // rs1_val==4 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1708);

        // rs1_val==4 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1712);

        // rs1_val==4 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1716);

        // rs1_val==4 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1720);

        // rs1_val==4 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1724);

        // rs1_val==4 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1728);

        // rs1_val==4 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1732);

        // rs1_val==4 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1736);

        // rs1_val==4 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1740);

        // rs1_val==4 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1744);

        // rs1_val==4 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1748);

        // rs1_val==4 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1752);

        // rs1_val==4 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1756);

        // rs1_val==4 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1760);

        // rs1_val==4 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1764);

        // rs1_val==4 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1768);

        // rs1_val==4 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1772);

        // rs1_val==4 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1776);

        // rs1_val==4 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1780);

        // rs1_val==4 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1784);

        // rs1_val==858993458 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1788);

        // rs1_val==858993458 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1792);

        // rs1_val==858993458 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1796);

        // rs1_val==858993458 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1800);

        // rs1_val==858993458 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1804);

        // rs1_val==858993458 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1808);

        // rs1_val==858993458 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1812);

        // rs1_val==858993458 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1816);

        // rs1_val==858993458 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1820);

        // rs1_val==858993458 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1824);

        // rs1_val==858993458 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1828);

        // rs1_val==858993458 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1832);

        // rs1_val==858993458 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1836);

        // rs1_val==858993458 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1840);

        // rs1_val==858993458 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1844);

        // rs1_val==858993458 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1848);

        // rs1_val==858993458 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1852);

        // rs1_val==858993458 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1856);

        // rs1_val==858993458 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1860);

        // rs1_val==858993458 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1864);

        // rs1_val==858993458 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1868);

        // rs1_val==858993458 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1872);

        // rs1_val==1717986917 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1876);

        // rs1_val==1717986917 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1880);

        // rs1_val==1717986917 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1884);

        // rs1_val==1717986917 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1888);

        // rs1_val==1717986917 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1892);

        // rs1_val==1717986917 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1896);

        // rs1_val==1717986917 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1900);

        // rs1_val==1717986917 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1904);

        // rs1_val==1717986917 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1908);

        // rs1_val==1717986917 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1912);

        // rs1_val==1717986917 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1916);

        // rs1_val==1717986917 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1920);

        // rs1_val==1717986917 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1924);

        // rs1_val==1717986917 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1928);

        // rs1_val==1717986917 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1932);

        // rs1_val==1717986917 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1936);

        // rs1_val==1717986917 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1940);

        // rs1_val==1717986917 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1944);

        // rs1_val==1717986917 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1948);

        // rs1_val==1717986917 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1952);

        // rs1_val==1717986917 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1956);

        // rs1_val==1717986917 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1960);

        // rs1_val==46339 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1964);

        // rs1_val==46339 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1968);

        // rs1_val==46339 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1972);

        // rs1_val==46339 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1976);

        // rs1_val==46339 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1980);

        // rs1_val==46339 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1984);

        // rs1_val==46339 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1988);

        // rs1_val==46339 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1992);

        // rs1_val==46339 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1996);

        // rs1_val==46339 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2000);

        // rs1_val==46339 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2004);

        // rs1_val==46339 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2008);

        // rs1_val==46339 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2012);

        // rs1_val==46339 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2016);

        // rs1_val==46339 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2020);

        // rs1_val==46339 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2024);

        // rs1_val==46339 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2028);

        // rs1_val==46339 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2032);

        // rs1_val==46339 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2036);

        // rs1_val==46339 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2040);

        // rs1_val==46339 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2044);

        c.li(Reg::X1, 2180);
        // rs1_val==46339 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 0);

        // rs1_val==1431655766 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 4);

        // rs1_val==1431655766 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 8);

        // rs1_val==1431655766 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 12);

        // rs1_val==1431655766 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 16);

        // rs1_val==1431655766 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 20);

        // rs1_val==1431655766 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 24);

        // rs1_val==1431655766 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 28);

        // rs1_val==1431655766 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 32);

        // rs1_val==1431655766 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 36);

        // rs1_val==1431655766 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 40);

        // rs1_val==1431655766 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 44);

        // rs1_val==1431655766 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 48);

        // rs1_val==1431655766 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 52);

        // rs1_val==1431655766 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 56);

        // rs1_val==1431655766 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 60);

        // rs1_val==1431655766 and rs2_val==-1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 64);

        // rs1_val==1431655766 and rs2_val==6, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 68);

        // rs1_val==1431655766 and rs2_val==858993460, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 72);

        // rs1_val==1431655766 and rs2_val==1717986919, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 76);

        // rs1_val==1431655766 and rs2_val==-46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 80);

        // rs1_val==1431655766 and rs2_val==46341, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 84);

        // rs1_val==-1431655765 and rs2_val==3, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 88);

        // rs1_val==-1431655765 and rs2_val==1431655765, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 92);

        // rs1_val==-1431655765 and rs2_val==-1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 96);

        // rs1_val==-1431655765 and rs2_val==5, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 100);

        // rs1_val==-1431655765 and rs2_val==858993459, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 104);

        // rs1_val==-1431655765 and rs2_val==1717986918, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 108);

        // rs1_val==-1431655765 and rs2_val==-46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 112);

        // rs1_val==-1431655765 and rs2_val==46340, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 116);

        // rs1_val==-1431655765 and rs2_val==2, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 120);

        // rs1_val==-1431655765 and rs2_val==1431655764, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 124);

        // rs1_val==-1431655765 and rs2_val==0, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 128);

        // rs1_val==-1431655765 and rs2_val==4, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 132);

        // rs1_val==-1431655765 and rs2_val==858993458, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 136);

        // rs1_val==-1431655765 and rs2_val==1717986917, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 140);

        // rs1_val==-1431655765 and rs2_val==46339, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 144);

        // rs1_val==-1431655765 and rs2_val==1431655766, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 148);

        // rs1_val > 0 and rs2_val < 0, rs2_val == -257, rs1_val == 131072
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x20000;  op2val:-0x101
        c.li(Reg::X10, MASK_XLEN(0x20000));
        c.li(Reg::X11, MASK_XLEN(-0x101));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 152);

        // rs1_val < 0 and rs2_val < 0, rs1_val == -16777217
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0x1000001;  op2val:-0x7
        c.li(Reg::X10, MASK_XLEN(-0x1000001));
        c.li(Reg::X11, MASK_XLEN(-0x7));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 156);

        // rs1_val == 1, rs2_val == 8388608
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:0x800000
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.li(Reg::X11, MASK_XLEN(0x800000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 160);

        // rs2_val == 1024, 
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x400
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x400));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 164);

        // rs2_val == 4096, rs1_val == 4
        // opcode: add ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x1000
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x1000));
        c.add(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 168);


                
        // End
        c.ecall();
        return c;
    }

    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> add_01::s_ScalarData;
}


// Generate google tests

TEST_F(add_01, opcode__add___op1_x4__op2_x24__dest_x24__op1val_0x7fffffff___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[0], 2147483648);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x10__dest_x28__op1val_0x20000___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[1], 262144);
}


TEST_F(add_01, opcode__add___op1_x21__op2_x21__dest_x21__op1val__0x1000001___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[2], 4261412862);
}


TEST_F(add_01, opcode__add___op1_x22__op2_x31__dest_x22__op1val__0x2___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[3], 262142);
}


TEST_F(add_01, opcode__add___op1_x12__op2_x6__dest_x11__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[4], 2863311532);
}


TEST_F(add_01, opcode__add___op1_x29__op2_x13__dest_x10__op1val_0x2___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[5], 2147483650);
}


TEST_F(add_01, opcode__add___op1_x31__op2_x5__dest_x26__op1val__0x11___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[6], 4294967279);
}


TEST_F(add_01, opcode__add___op1_x2__op2_x1__dest_x7__op1val_0x66666666___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[7], 3865470565);
}


TEST_F(add_01, opcode__add___op1_x8__op2_x25__dest_x14__op1val__0x80000000___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[8], 715827882);
}


TEST_F(add_01, opcode__add___op1_x13__op2_x8__dest_x1__op1val_0x0___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[9], 4261412863);
}


TEST_F(add_01, opcode__add___op1_x28__op2_x9__dest_x0__op1val_0x1___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(add_01, opcode__add___op1_x14__op2_x4__dest_x20__op1val_0x7___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[11], 9);
}


TEST_F(add_01, opcode__add___op1_x7__op2_x19__dest_x16__op1val_0x8___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[12], 12);
}


TEST_F(add_01, opcode__add___op1_x23__op2_x29__dest_x8__op1val_0x800___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[13], 2056);
}


TEST_F(add_01, opcode__add___op1_x5__op2_x27__dest_x13__op1val_0x0___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[14], 16);
}


TEST_F(add_01, opcode__add___op1_x25__op2_x20__dest_x27__op1val_0x55555556___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[15], 1431655798);
}


TEST_F(add_01, opcode__add___op1_x15__op2_x26__dest_x17__op1val__0x11___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[16], 47);
}


TEST_F(add_01, opcode__add___op1_x17__op2_x2__dest_x29__op1val__0x5___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[17], 123);
}


TEST_F(add_01, opcode__add___op1_x24__op2_x17__dest_x4__op1val_0x20___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[18], 288);
}


TEST_F(add_01, opcode__add___op1_x16__op2_x11__dest_x2__op1val_0x40000000___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[19], 1073742336);
}


TEST_F(add_01, opcode__add___op1_x0__op2_x16__dest_x6__op1val_0x0___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[20], 1024);
}


TEST_F(add_01, opcode__add___op1_x11__op2_x3__dest_x31__op1val__0x55555555___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[21], 2863313579);
}


TEST_F(add_01, opcode__add___op1_x18__op2_x0__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[22], 4);
}


TEST_F(add_01, opcode__add___op1_x30__op2_x7__dest_x9__op1val__0x2000001___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[23], 4261421055);
}


TEST_F(add_01, opcode__add___op1_x3__op2_x30__dest_x5__op1val_0xb503___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[24], 62723);
}


TEST_F(add_01, opcode__add___op1_x27__op2_x28__dest_x18__op1val__0xb503___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[25], 4294953725);
}


TEST_F(add_01, opcode__add___op1_x9__op2_x14__dest_x15__op1val__0x201___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[26], 65023);
}


TEST_F(add_01, opcode__add___op1_x26__op2_x15__dest_x3__op1val_0x40___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[27], 131136);
}


TEST_F(add_01, opcode__add___op1_x1__op2_x18__dest_x23__op1val_0x3___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[28], 524291);
}


TEST_F(add_01, opcode__add___op1_x6__op2_x12__dest_x30__op1val__0x10001___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[29], 983039);
}


TEST_F(add_01, opcode__add___op1_x20__op2_x22__dest_x19__op1val__0x400001___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[30], 4292870143);
}


TEST_F(add_01, opcode__add___op1_x19__op2_x23__dest_x25__op1val_0x400000___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[31], 8388608);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[32], 16730877);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[33], 33423359);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[34], 67108864);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[35], 134184959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[36], 1342177280);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[37], 536854527);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[38], 1073807360);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[39], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[40], 4294967260);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[41], 4294967258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[42], 4294836214);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[43], 2031);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[44], 536870879);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[45], 2147483582);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[46], 4286578558);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[47], 4294920443);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[48], 4294962174);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[49], 4294965251);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[50], 4294916860);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[51], 38147);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[52], 2147467262);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[53], 4294934524);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[54], 4294948098);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[55], 2863180457);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[56], 16515071);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[57], 2146959358);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[58], 4293920767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[59], 4292870151);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[60], 4290781183);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[61], 4286578654);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[62], 4276092926);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[63], 4227858433);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[64], 1297438036);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[65], 4026662911);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[66], 3758227455);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x7___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[67], 3221225478);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[68], 1432704341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[69], 858993475);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[70], 858993586);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0xa) {
  ASSERT_EQ(getScalarWordData()[71], 246);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[72], 4294921469);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[73], 1431656789);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[74], 4101);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[75], 14335);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[76], 4278222847);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[77], 17039360);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[78], 557056);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[79], 270532608);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[80], 8387583);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[81], 1465210197);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[82], 67141632);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[83], 993211186);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val__0x40000000) {
  ASSERT_EQ(getScalarWordData()[84], 3489660928);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[85], 4294442998);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[86], 2147483583);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[87], 2863311401);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[88], 4294967042);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val__0x8) {
  ASSERT_EQ(getScalarWordData()[89], 4294966263);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[90], 4294965250);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[91], 4294959105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[92], 4286316542);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[93], 4294489347);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val__0x40000000) {
  ASSERT_EQ(getScalarWordData()[94], 3220176895);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[95], 1650878052);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[96], 3892314110);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[97], 4026533887);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[98], 3758227455);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[99], 3221225342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[100], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[101], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[102], 2863311533);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[103], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[104], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[105], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[106], 4294920959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[107], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[108], 5);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[109], 1431655767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[110], 3);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[111], 7);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[112], 858993461);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[113], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[114], 46342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[115], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[116], 2863311534);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[117], 9);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[118], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[119], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[120], 4294920960);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[121], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[122], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[123], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[124], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[125], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[126], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[127], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[128], 1431609425);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[129], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[130], 1431655767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[131], 2863311529);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[132], 1431655765);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[133], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[134], 2290649223);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[135], 3149642682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[136], 1431702104);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[137], 2863311531);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[138], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[139], 1431655771);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[140], 2290649225);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[141], 3149642684);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[142], 1431609426);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[143], 1431702106);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[144], 2863311533);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[145], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[146], 1431655764);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[147], 2863311535);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[148], 3722304989);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[149], 286331152);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[150], 2863265190);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[151], 2863357870);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[152], 2863311532);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[153], 4294967294);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[154], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[155], 2863311534);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[156], 3722304988);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[157], 286331151);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[158], 2863357869);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[159], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[160], 1431655765);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[161], 2863311536);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[162], 3722304990);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[163], 286331153);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[164], 2863265191);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[165], 2863357871);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[166], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[167], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[168], 2863311535);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[169], 10);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[170], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[171], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[172], 4294920961);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[173], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[174], 7);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[175], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[176], 5);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[177], 9);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[178], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[179], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[180], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[181], 1431655771);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[182], 2863311536);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[183], 11);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[184], 858993465);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[185], 1717986924);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[186], 4294920962);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[187], 46346);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[188], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[189], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[190], 3722304989);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[191], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[192], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[193], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[194], 858947119);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[195], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[196], 858993461);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[197], 2290649223);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[198], 858993459);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[199], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[200], 1717986917);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[201], 2576980376);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[202], 859039798);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[203], 2290649225);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[204], 3722304990);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[205], 858993465);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[206], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[207], 2576980378);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[208], 858947120);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[209], 859039800);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[210], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[211], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[212], 286331152);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[213], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[214], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[215], 3435973836);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[216], 1717940578);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[217], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[218], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[219], 3149642682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[220], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[221], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[222], 2576980376);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[223], 3435973835);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[224], 1718033257);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[225], 3149642684);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[226], 286331153);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[227], 1717986924);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[228], 2576980378);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[229], 3435973837);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[230], 1717940579);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[231], 1718033259);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[232], 4294920959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[233], 1431609425);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[234], 2863265190);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[235], 4294920961);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[236], 858947119);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[237], 1717940578);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[238], 4294874616);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[239], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[240], 4294920958);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[241], 1431609424);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[242], 4294920956);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[243], 4294920960);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[244], 858947118);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[245], 1717940577);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[246], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[247], 1431609426);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[248], 2863265191);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[249], 4294920962);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[250], 858947120);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[251], 1717940579);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[252], 4294874617);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[253], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[254], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[255], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[256], 2863357870);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[257], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[258], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[259], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[260], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[261], 92680);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[262], 46342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[263], 1431702104);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[264], 46340);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[265], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[266], 859039798);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[267], 1718033257);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[268], 92679);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[269], 1431702106);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[270], 2863357871);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[271], 46346);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[272], 859039800);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[273], 1718033259);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[274], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[275], 92681);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[276], 5);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[277], 1431655767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[278], 2863311532);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[279], 7);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[280], 858993461);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[281], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[282], 4294920958);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[283], 46342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[284], 4);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[285], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[286], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[287], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[288], 858993460);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[289], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[290], 46341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[291], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[292], 2863311533);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[293], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[294], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[295], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[296], 4294920959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[297], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[298], 1431655767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[299], 2863311529);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[300], 4294967294);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[301], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[302], 2290649223);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[303], 3149642682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[304], 1431609424);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[305], 1431702104);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[306], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[307], 2863311528);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[308], 1431655764);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[309], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[310], 2290649222);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[311], 3149642681);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[312], 1431702103);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[313], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[314], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[315], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[316], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[317], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[318], 1431609425);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[319], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[320], 3);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[321], 1431655765);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[322], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[323], 5);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[324], 858993459);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[325], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[326], 4294920956);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[327], 46340);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[328], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[329], 2863311537);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[330], 3722304991);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[331], 286331154);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[332], 2863265192);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[333], 2863357872);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[334], 9);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[335], 1431655771);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[336], 2863311536);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[337], 11);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[338], 858993465);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[339], 1717986924);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[340], 4294920962);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[341], 46346);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[342], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[343], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[344], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[345], 10);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[346], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[347], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[348], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[349], 1431655772);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[350], 2863311537);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[351], 12);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[352], 858993466);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[353], 1717986925);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[354], 4294920963);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[355], 46347);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[356], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[357], 2290649225);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[358], 3722304990);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[359], 858993465);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[360], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[361], 2576980378);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[362], 858947120);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[363], 859039800);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[364], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[365], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[366], 858993460);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[367], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[368], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[369], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[370], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[371], 2290649226);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[372], 3722304991);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[373], 858993466);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[374], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[375], 2576980379);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[376], 858947121);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[377], 859039801);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[378], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[379], 3149642684);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[380], 286331153);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[381], 1717986924);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[382], 2576980378);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[383], 3435973837);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[384], 1717940579);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[385], 1718033259);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[386], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[387], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[388], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[389], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[390], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[391], 3435973836);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[392], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[393], 3149642685);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[394], 286331154);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[395], 1717986925);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[396], 2576980379);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[397], 3435973838);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[398], 1717940580);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[399], 1718033260);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[400], 4294920960);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[401], 1431609426);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[402], 2863265191);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[403], 4294920962);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[404], 858947120);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[405], 1717940579);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[406], 4294874617);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[407], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[408], 4294920959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[409], 1431609425);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[410], 4294920957);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[411], 4294920961);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[412], 858947119);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[413], 1717940578);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[414], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[415], 1431609427);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[416], 2863265192);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[417], 4294920963);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[418], 858947121);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[419], 1717940580);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[420], 4294874618);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[421], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[422], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[423], 1431702106);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[424], 2863357871);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[425], 46346);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[426], 859039800);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[427], 1718033259);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[428], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[429], 92681);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[430], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[431], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[432], 46341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[433], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[434], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[435], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[436], 92680);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[437], 1431702107);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[438], 2863357872);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[439], 46347);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[440], 859039801);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[441], 1718033260);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[442], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[443], 92682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[444], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[445], 1431655764);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[446], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[447], 4);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[448], 858993458);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[449], 1717986917);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[450], 46339);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[451], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[452], 2863311531);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[453], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[454], 858993460);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[455], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[456], 4294920957);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[457], 46341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[458], 7);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[459], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[460], 2863311534);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[461], 9);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[462], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[463], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[464], 4294920960);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[465], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[466], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[467], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[468], 4);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[469], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[470], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[471], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[472], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[473], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[474], 2863311535);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[475], 10);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[476], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[477], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[478], 4294920961);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[479], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[480], 858993461);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[481], 2290649223);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[482], 3722304988);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[483], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[484], 1717986917);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[485], 2576980376);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[486], 858947118);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[487], 859039798);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[488], 858993460);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[489], 2290649222);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[490], 858993458);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[491], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[492], 1717986916);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[493], 2576980375);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[494], 859039797);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[495], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[496], 3722304989);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[497], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[498], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[499], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[500], 858947119);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[501], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[502], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[503], 3149642682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[504], 286331151);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[505], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[506], 2576980376);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[507], 3435973835);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[508], 1717940577);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[509], 1718033257);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[510], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[511], 3149642681);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[512], 1717986917);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[513], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[514], 2576980375);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[515], 3435973834);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[516], 1718033256);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[517], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[518], 286331152);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[519], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[520], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[521], 3435973836);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[522], 1717940578);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[523], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[524], 46342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[525], 1431702104);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[526], 2863357869);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[527], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[528], 859039798);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[529], 1718033257);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[530], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[531], 92679);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[532], 46341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[533], 1431702103);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[534], 46339);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[535], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[536], 859039797);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[537], 1718033256);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[538], 92678);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[539], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[540], 2863357870);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[541], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[542], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[543], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[544], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[545], 92680);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[546], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[547], 2863311531);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[548], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[549], 1431655771);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[550], 2290649225);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[551], 3149642684);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[552], 1431609426);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[553], 1431702106);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[554], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[555], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[556], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[557], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[558], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[559], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[560], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[561], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[562], 1431655772);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[563], 2290649226);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[564], 3149642685);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[565], 1431609427);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[566], 1431702107);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[567], 2863311534);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[568], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[569], 1431655765);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[570], 2863311536);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[571], 3722304990);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[572], 286331153);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[573], 2863265191);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[574], 2863357871);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[575], 2863311533);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[576], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[577], 2863311531);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[578], 2863311535);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[579], 3722304989);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[580], 286331152);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[581], 2863357870);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[582], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[583], 130815);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val__0x7) {
  ASSERT_EQ(getScalarWordData()[584], 4278190072);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[585], 8388609);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[586], 4294921980);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[587], 4100);
}

