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
class slt_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        // Generate word-aligned vector to initialise memory
        const std::vector<uint32_t> scalarWordInitData{
        3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,};
        
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
        
        c.li(Reg::X12, 0);
        // rs2 == rd != rs1, rs1==x18, rs2==x26, rd==x26, rs1_val > 0 and rs2_val > 0, rs1_val == rs2_val, rs1_val==1717986919 and rs2_val==1717986919
        // opcode: slt ; op1:x18; op2:x26; dest:x26; op1val:0x66666667;  op2val:0x66666667
        c.li(Reg::X18, MASK_XLEN(0x66666667));
        c.li(Reg::X26, MASK_XLEN(0x66666667));
        c.slt(Reg::X26, Reg::X18, Reg::X26);
        c.sw(Reg::X26, Reg::X12, 0);

        // rs1 == rs2 != rd, rs1==x20, rs2==x20, rd==x1, rs1_val > 0 and rs2_val < 0, rs1_val != rs2_val
        // opcode: slt ; op1:x20; op2:x20; dest:x1; op1val:0x33333334;  op2val:0x33333334
        c.li(Reg::X20, MASK_XLEN(0x33333334));
        c.li(Reg::X20, MASK_XLEN(0x33333334));
        c.slt(Reg::X1, Reg::X20, Reg::X20);
        c.sw(Reg::X1, Reg::X12, 4);

        // rs1 == rs2 == rd, rs1==x21, rs2==x21, rd==x21, rs1_val < 0 and rs2_val < 0, rs2_val == -9, rs1_val == -16385
        // opcode: slt ; op1:x21; op2:x21; dest:x21; op1val:-0x4001;  op2val:-0x4001
        c.li(Reg::X21, MASK_XLEN(-0x4001));
        c.li(Reg::X21, MASK_XLEN(-0x4001));
        c.slt(Reg::X21, Reg::X21, Reg::X21);
        c.sw(Reg::X21, Reg::X12, 8);

        // rs1 == rd != rs2, rs1==x15, rs2==x27, rd==x15, rs1_val < 0 and rs2_val > 0, rs1_val == -513
        // opcode: slt ; op1:x15; op2:x27; dest:x15; op1val:-0x201;  op2val:0x5
        c.li(Reg::X15, MASK_XLEN(-0x201));
        c.li(Reg::X27, MASK_XLEN(0x5));
        c.slt(Reg::X15, Reg::X15, Reg::X27);
        c.sw(Reg::X15, Reg::X12, 12);

        // rs1 != rs2  and rs1 != rd and rs2 != rd, rs1==x5, rs2==x18, rd==x7, rs2_val == (-2**(xlen-1)), rs2_val == -2147483648
        // opcode: slt ; op1:x5; op2:x18; dest:x7; op1val:0x33333334;  op2val:-0x80000000
        c.li(Reg::X5, MASK_XLEN(0x33333334));
        c.li(Reg::X18, MASK_XLEN(-0x80000000));
        c.slt(Reg::X7, Reg::X5, Reg::X18);
        c.sw(Reg::X7, Reg::X12, 16);

        // rs1==x25, rs2==x19, rd==x17, rs2_val == 0, rs1_val == 134217728
        // opcode: slt ; op1:x25; op2:x19; dest:x17; op1val:0x8000000;  op2val:0x0
        c.li(Reg::X25, MASK_XLEN(0x8000000));
        c.li(Reg::X19, MASK_XLEN(0x0));
        c.slt(Reg::X17, Reg::X25, Reg::X19);
        c.sw(Reg::X17, Reg::X12, 20);

        // rs1==x9, rs2==x31, rd==x23, rs2_val == (2**(xlen-1)-1), rs2_val == 2147483647, rs1_val == 131072
        // opcode: slt ; op1:x9; op2:x31; dest:x23; op1val:0x20000;  op2val:0x7fffffff
        c.li(Reg::X9, MASK_XLEN(0x20000));
        c.li(Reg::X31, MASK_XLEN(0x7fffffff));
        c.slt(Reg::X23, Reg::X9, Reg::X31);
        c.sw(Reg::X23, Reg::X12, 24);

        // rs1==x2, rs2==x15, rd==x11, rs2_val == 1, rs1_val == -131073
        // opcode: slt ; op1:x2; op2:x15; dest:x11; op1val:-0x20001;  op2val:0x1
        c.li(Reg::X2, MASK_XLEN(-0x20001));
        c.li(Reg::X15, MASK_XLEN(0x1));
        c.slt(Reg::X11, Reg::X2, Reg::X15);
        c.sw(Reg::X11, Reg::X12, 28);

        // rs1==x28, rs2==x13, rd==x22, rs1_val == (-2**(xlen-1)), rs1_val == -2147483648, rs2_val == 1024
        // opcode: slt ; op1:x28; op2:x13; dest:x22; op1val:-0x80000000;  op2val:0x400
        c.li(Reg::X28, MASK_XLEN(-0x80000000));
        c.li(Reg::X13, MASK_XLEN(0x400));
        c.slt(Reg::X22, Reg::X28, Reg::X13);
        c.sw(Reg::X22, Reg::X12, 32);

        // rs1==x10, rs2==x7, rd==x30, rs1_val == 0, rs2_val == 8
        // opcode: slt ; op1:x10; op2:x7; dest:x30; op1val:0x0;  op2val:0x8
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X7, MASK_XLEN(0x8));
        c.slt(Reg::X30, Reg::X10, Reg::X7);
        c.sw(Reg::X30, Reg::X12, 36);

        // rs1==x24, rs2==x22, rd==x5, rs1_val == (2**(xlen-1)-1), rs2_val == -257, rs1_val == 2147483647
        // opcode: slt ; op1:x24; op2:x22; dest:x5; op1val:0x7fffffff;  op2val:-0x101
        c.li(Reg::X24, MASK_XLEN(0x7fffffff));
        c.li(Reg::X22, MASK_XLEN(-0x101));
        c.slt(Reg::X5, Reg::X24, Reg::X22);
        c.sw(Reg::X5, Reg::X12, 40);

        // rs1==x8, rs2==x3, rd==x24, rs1_val == 1, rs2_val == -513
        // opcode: slt ; op1:x8; op2:x3; dest:x24; op1val:0x1;  op2val:-0x201
        c.li(Reg::X8, MASK_XLEN(0x1));
        c.li(Reg::X3, MASK_XLEN(-0x201));
        c.slt(Reg::X24, Reg::X8, Reg::X3);
        c.sw(Reg::X24, Reg::X12, 44);

        // rs1==x26, rs2==x9, rd==x2, rs2_val == 2, rs1_val == 268435456
        // opcode: slt ; op1:x26; op2:x9; dest:x2; op1val:0x10000000;  op2val:0x2
        c.li(Reg::X26, MASK_XLEN(0x10000000));
        c.li(Reg::X9, MASK_XLEN(0x2));
        c.slt(Reg::X2, Reg::X26, Reg::X9);
        c.sw(Reg::X2, Reg::X12, 48);

        // rs1==x4, rs2==x14, rd==x8, rs2_val == 4, rs1_val == 32768
        // opcode: slt ; op1:x4; op2:x14; dest:x8; op1val:0x8000;  op2val:0x4
        c.li(Reg::X4, MASK_XLEN(0x8000));
        c.li(Reg::X14, MASK_XLEN(0x4));
        c.slt(Reg::X8, Reg::X4, Reg::X14);
        c.sw(Reg::X8, Reg::X12, 52);

        // rs1==x6, rs2==x4, rd==x25, rs2_val == 16, rs1_val == -4097
        // opcode: slt ; op1:x6; op2:x4; dest:x25; op1val:-0x1001;  op2val:0x10
        c.li(Reg::X6, MASK_XLEN(-0x1001));
        c.li(Reg::X4, MASK_XLEN(0x10));
        c.slt(Reg::X25, Reg::X6, Reg::X4);
        c.sw(Reg::X25, Reg::X12, 56);

        c.li(Reg::X15, 60);
        // rs1==x31, rs2==x25, rd==x14, rs2_val == 32, rs1_val == 16777216
        // opcode: slt ; op1:x31; op2:x25; dest:x14; op1val:0x1000000;  op2val:0x20
        c.li(Reg::X31, MASK_XLEN(0x1000000));
        c.li(Reg::X25, MASK_XLEN(0x20));
        c.slt(Reg::X14, Reg::X31, Reg::X25);
        c.sw(Reg::X14, Reg::X15, 0);

        // rs1==x14, rs2==x8, rd==x20, rs2_val == 64, 
        // opcode: slt ; op1:x14; op2:x8; dest:x20; op1val:0x5;  op2val:0x40
        c.li(Reg::X14, MASK_XLEN(0x5));
        c.li(Reg::X8, MASK_XLEN(0x40));
        c.slt(Reg::X20, Reg::X14, Reg::X8);
        c.sw(Reg::X20, Reg::X15, 4);

        // rs1==x7, rs2==x11, rd==x19, rs2_val == 128, 
        // opcode: slt ; op1:x7; op2:x11; dest:x19; op1val:0x0;  op2val:0x80
        c.li(Reg::X7, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x80));
        c.slt(Reg::X19, Reg::X7, Reg::X11);
        c.sw(Reg::X19, Reg::X15, 8);

        // rs1==x19, rs2==x29, rd==x9, rs2_val == 256, 
        // opcode: slt ; op1:x19; op2:x29; dest:x9; op1val:0x5;  op2val:0x100
        c.li(Reg::X19, MASK_XLEN(0x5));
        c.li(Reg::X29, MASK_XLEN(0x100));
        c.slt(Reg::X9, Reg::X19, Reg::X29);
        c.sw(Reg::X9, Reg::X15, 12);

        // rs1==x3, rs2==x23, rd==x6, rs2_val == 512, rs1_val == 4194304
        // opcode: slt ; op1:x3; op2:x23; dest:x6; op1val:0x400000;  op2val:0x200
        c.li(Reg::X3, MASK_XLEN(0x400000));
        c.li(Reg::X23, MASK_XLEN(0x200));
        c.slt(Reg::X6, Reg::X3, Reg::X23);
        c.sw(Reg::X6, Reg::X15, 16);

        // rs1==x11, rs2==x1, rd==x31, rs2_val == 2048, rs1_val == -17
        // opcode: slt ; op1:x11; op2:x1; dest:x31; op1val:-0x11;  op2val:0x800
        c.li(Reg::X11, MASK_XLEN(-0x11));
        c.li(Reg::X1, MASK_XLEN(0x800));
        c.slt(Reg::X31, Reg::X11, Reg::X1);
        c.sw(Reg::X31, Reg::X15, 20);

        // rs1==x30, rs2==x16, rd==x12, rs2_val == 4096, rs1_val == -16777217
        // opcode: slt ; op1:x30; op2:x16; dest:x12; op1val:-0x1000001;  op2val:0x1000
        c.li(Reg::X30, MASK_XLEN(-0x1000001));
        c.li(Reg::X16, MASK_XLEN(0x1000));
        c.slt(Reg::X12, Reg::X30, Reg::X16);
        c.sw(Reg::X12, Reg::X15, 24);

        // rs1==x13, rs2==x2, rd==x16, rs2_val == 8192, 
        // opcode: slt ; op1:x13; op2:x2; dest:x16; op1val:0x0;  op2val:0x2000
        c.li(Reg::X13, MASK_XLEN(0x0));
        c.li(Reg::X2, MASK_XLEN(0x2000));
        c.slt(Reg::X16, Reg::X13, Reg::X2);
        c.sw(Reg::X16, Reg::X15, 28);

        // rs1==x22, rs2==x12, rd==x10, rs2_val == 16384, rs1_val == 32
        // opcode: slt ; op1:x22; op2:x12; dest:x10; op1val:0x20;  op2val:0x4000
        c.li(Reg::X22, MASK_XLEN(0x20));
        c.li(Reg::X12, MASK_XLEN(0x4000));
        c.slt(Reg::X10, Reg::X22, Reg::X12);
        c.sw(Reg::X10, Reg::X15, 32);

        // rs1==x29, rs2==x24, rd==x27, rs2_val == 32768, rs1_val == -2
        // opcode: slt ; op1:x29; op2:x24; dest:x27; op1val:-0x2;  op2val:0x8000
        c.li(Reg::X29, MASK_XLEN(-0x2));
        c.li(Reg::X24, MASK_XLEN(0x8000));
        c.slt(Reg::X27, Reg::X29, Reg::X24);
        c.sw(Reg::X27, Reg::X15, 36);

        // rs1==x16, rs2==x30, rd==x4, rs2_val == 65536, rs1_val == -67108865
        // opcode: slt ; op1:x16; op2:x30; dest:x4; op1val:-0x4000001;  op2val:0x10000
        c.li(Reg::X16, MASK_XLEN(-0x4000001));
        c.li(Reg::X30, MASK_XLEN(0x10000));
        c.slt(Reg::X4, Reg::X16, Reg::X30);
        c.sw(Reg::X4, Reg::X15, 40);

        // rs1==x17, rs2==x28, rd==x3, rs2_val == 131072, rs1_val == 4096
        // opcode: slt ; op1:x17; op2:x28; dest:x3; op1val:0x1000;  op2val:0x20000
        c.li(Reg::X17, MASK_XLEN(0x1000));
        c.li(Reg::X28, MASK_XLEN(0x20000));
        c.slt(Reg::X3, Reg::X17, Reg::X28);
        c.sw(Reg::X3, Reg::X15, 44);

        // rs1==x1, rs2==x5, rd==x13, rs2_val == 262144, rs1_val == 33554432
        // opcode: slt ; op1:x1; op2:x5; dest:x13; op1val:0x2000000;  op2val:0x40000
        c.li(Reg::X1, MASK_XLEN(0x2000000));
        c.li(Reg::X5, MASK_XLEN(0x40000));
        c.slt(Reg::X13, Reg::X1, Reg::X5);
        c.sw(Reg::X13, Reg::X15, 48);

        // rs1==x27, rs2==x10, rd==x18, rs2_val == 524288, rs1_val == -2097153
        // opcode: slt ; op1:x27; op2:x10; dest:x18; op1val:-0x200001;  op2val:0x80000
        c.li(Reg::X27, MASK_XLEN(-0x200001));
        c.li(Reg::X10, MASK_XLEN(0x80000));
        c.slt(Reg::X18, Reg::X27, Reg::X10);
        c.sw(Reg::X18, Reg::X15, 52);

        c.li(Reg::X1, 116);
        // rs1==x0, rs2==x17, rd==x28, rs2_val == 1048576, 
        // opcode: slt ; op1:x0; op2:x17; dest:x28; op1val:0x0;  op2val:0x100000
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.li(Reg::X17, MASK_XLEN(0x100000));
        c.slt(Reg::X28, Reg::X0, Reg::X17);
        c.sw(Reg::X28, Reg::X1, 0);

        // rs1==x12, rs2==x0, rd==x29, rs2_val == 2097152, rs1_val == 4
        // opcode: slt ; op1:x12; op2:x0; dest:x29; op1val:0x4;  op2val:0x0
        c.li(Reg::X12, MASK_XLEN(0x4));
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.slt(Reg::X29, Reg::X12, Reg::X0);
        c.sw(Reg::X29, Reg::X1, 4);

        // rs1==x23, rs2==x6, rd==x0, rs2_val == 4194304, 
        // opcode: slt ; op1:x23; op2:x6; dest:x0; op1val:0x9;  op2val:0x400000
        c.li(Reg::X23, MASK_XLEN(0x9));
        c.li(Reg::X6, MASK_XLEN(0x400000));
        c.slt(Reg::X0, Reg::X23, Reg::X6);
        c.sw(Reg::X0, Reg::X1, 8);

        // rs2_val == 8388608, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x80000000;  op2val:0x800000
        c.li(Reg::X10, MASK_XLEN(-0x80000000));
        c.li(Reg::X11, MASK_XLEN(0x800000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 12);

        // rs2_val == 16777216, rs1_val == -129
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x81;  op2val:0x1000000
        c.li(Reg::X10, MASK_XLEN(-0x81));
        c.li(Reg::X11, MASK_XLEN(0x1000000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 16);

        // rs2_val == 33554432, rs1_val == 262144
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x40000;  op2val:0x2000000
        c.li(Reg::X10, MASK_XLEN(0x40000));
        c.li(Reg::X11, MASK_XLEN(0x2000000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 20);

        // rs2_val == 67108864, rs1_val == -1431655766
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x4000000
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x4000000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 24);

        // rs2_val == 134217728, rs1_val == -257
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x101;  op2val:0x8000000
        c.li(Reg::X10, MASK_XLEN(-0x101));
        c.li(Reg::X11, MASK_XLEN(0x8000000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 28);

        // rs2_val == 268435456, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2000000;  op2val:0x10000000
        c.li(Reg::X10, MASK_XLEN(0x2000000));
        c.li(Reg::X11, MASK_XLEN(0x10000000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 32);

        // rs2_val == 536870912, rs1_val == -32769
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x8001;  op2val:0x20000000
        c.li(Reg::X10, MASK_XLEN(-0x8001));
        c.li(Reg::X11, MASK_XLEN(0x20000000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 36);

        // rs2_val == 1073741824, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2000000;  op2val:0x40000000
        c.li(Reg::X10, MASK_XLEN(0x2000000));
        c.li(Reg::X11, MASK_XLEN(0x40000000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 40);

        // rs2_val == -2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x2
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 44);

        // rs2_val == -3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x7;  op2val:-0x3
        c.li(Reg::X10, MASK_XLEN(0x7));
        c.li(Reg::X11, MASK_XLEN(-0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 48);

        // rs2_val == -5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x11;  op2val:-0x5
        c.li(Reg::X10, MASK_XLEN(-0x11));
        c.li(Reg::X11, MASK_XLEN(-0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 52);

        // rs2_val == -17, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x8000;  op2val:-0x11
        c.li(Reg::X10, MASK_XLEN(0x8000));
        c.li(Reg::X11, MASK_XLEN(-0x11));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 56);

        // rs2_val == -33, rs1_val == -1025
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x401;  op2val:-0x21
        c.li(Reg::X10, MASK_XLEN(-0x401));
        c.li(Reg::X11, MASK_XLEN(-0x21));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 60);

        // rs2_val == -65, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x11;  op2val:-0x41
        c.li(Reg::X10, MASK_XLEN(-0x11));
        c.li(Reg::X11, MASK_XLEN(-0x41));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 64);

        // rs2_val == -129, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x4001;  op2val:-0x81
        c.li(Reg::X10, MASK_XLEN(-0x4001));
        c.li(Reg::X11, MASK_XLEN(-0x81));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 68);

        // rs2_val == -1025, rs1_val == 524288
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x80000;  op2val:-0x401
        c.li(Reg::X10, MASK_XLEN(0x80000));
        c.li(Reg::X11, MASK_XLEN(-0x401));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 72);

        // rs2_val == -2049, rs1_val == 1073741824
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x40000000;  op2val:-0x801
        c.li(Reg::X10, MASK_XLEN(0x40000000));
        c.li(Reg::X11, MASK_XLEN(-0x801));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 76);

        // rs2_val == -4097, rs1_val == -8388609
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x800001;  op2val:-0x1001
        c.li(Reg::X10, MASK_XLEN(-0x800001));
        c.li(Reg::X11, MASK_XLEN(-0x1001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 80);

        // rs2_val == -8193, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x20000;  op2val:-0x2001
        c.li(Reg::X10, MASK_XLEN(0x20000));
        c.li(Reg::X11, MASK_XLEN(-0x2001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 84);

        // rs2_val == -16385, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x9;  op2val:-0x4001
        c.li(Reg::X10, MASK_XLEN(0x9));
        c.li(Reg::X11, MASK_XLEN(-0x4001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 88);

        // rs2_val == -32769, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x8001
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x8001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 92);

        // rs2_val == -65537, rs1_val == -268435457
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x10000001;  op2val:-0x10001
        c.li(Reg::X10, MASK_XLEN(-0x10000001));
        c.li(Reg::X11, MASK_XLEN(-0x10001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 96);

        // rs2_val == -131073, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x20001
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0x20001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 100);

        // rs2_val == -262145, rs1_val == 1024
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x400;  op2val:-0x40001
        c.li(Reg::X10, MASK_XLEN(0x400));
        c.li(Reg::X11, MASK_XLEN(-0x40001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 104);

        // rs2_val == -524289, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x80001
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x80001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 108);

        // rs2_val == -1048577, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x100001
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0x100001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 112);

        // rs2_val == -2097153, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x1000;  op2val:-0x200001
        c.li(Reg::X10, MASK_XLEN(0x1000));
        c.li(Reg::X11, MASK_XLEN(-0x200001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 116);

        // rs2_val == -4194305, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x20001;  op2val:-0x400001
        c.li(Reg::X10, MASK_XLEN(-0x20001));
        c.li(Reg::X11, MASK_XLEN(-0x400001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 120);

        // rs2_val == -8388609, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x401;  op2val:-0x800001
        c.li(Reg::X10, MASK_XLEN(-0x401));
        c.li(Reg::X11, MASK_XLEN(-0x800001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 124);

        // rs2_val == -16777217, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x20;  op2val:-0x1000001
        c.li(Reg::X10, MASK_XLEN(0x20));
        c.li(Reg::X11, MASK_XLEN(-0x1000001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 128);

        // rs2_val == -33554433, rs1_val == 2097152
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x200000;  op2val:-0x2000001
        c.li(Reg::X10, MASK_XLEN(0x200000));
        c.li(Reg::X11, MASK_XLEN(-0x2000001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 132);

        // rs2_val == -67108865, rs1_val == 1431655765
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x4000001
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x4000001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 136);

        // rs2_val == -134217729, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x6;  op2val:-0x8000001
        c.li(Reg::X10, MASK_XLEN(-0x6));
        c.li(Reg::X11, MASK_XLEN(-0x8000001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 140);

        // rs2_val == -268435457, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x10000001
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x10000001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 144);

        // rs2_val == -536870913, rs1_val == -8193
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x2001;  op2val:-0x20000001
        c.li(Reg::X10, MASK_XLEN(-0x2001));
        c.li(Reg::X11, MASK_XLEN(-0x20000001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 148);

        // rs2_val == -1073741825, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x40000001
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x40000001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 152);

        // rs2_val == 1431655765, rs1_val==2 and rs2_val==1431655765, rs1_val == 2
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 156);

        // rs2_val == -1431655766, rs1_val==3 and rs2_val==-1431655766
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 160);

        // rs1_val == 8, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x8;  op2val:0x100
        c.li(Reg::X10, MASK_XLEN(0x8));
        c.li(Reg::X11, MASK_XLEN(0x100));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 164);

        // rs1_val == 16, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x10;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x10));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 168);

        // rs1_val == 64, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x40;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x40));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 172);

        // rs1_val == 128, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x80;  op2val:-0x2001
        c.li(Reg::X10, MASK_XLEN(0x80));
        c.li(Reg::X11, MASK_XLEN(-0x2001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 176);

        // rs1_val == 256, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x100;  op2val:-0x40000001
        c.li(Reg::X10, MASK_XLEN(0x100));
        c.li(Reg::X11, MASK_XLEN(-0x40000001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 180);

        // rs1_val == 512, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x200;  op2val:-0x41
        c.li(Reg::X10, MASK_XLEN(0x200));
        c.li(Reg::X11, MASK_XLEN(-0x41));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 184);

        // rs1_val == 2048, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x800;  op2val:0x40000
        c.li(Reg::X10, MASK_XLEN(0x800));
        c.li(Reg::X11, MASK_XLEN(0x40000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 188);

        // rs1_val == 8192, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2000;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x2000));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 192);

        // rs1_val == 16384, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4000;  op2val:-0x11
        c.li(Reg::X10, MASK_XLEN(0x4000));
        c.li(Reg::X11, MASK_XLEN(-0x11));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 196);

        // rs1_val == 65536, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 200);

        // rs1_val == 1048576, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x100000;  op2val:-0x1000001
        c.li(Reg::X10, MASK_XLEN(0x100000));
        c.li(Reg::X11, MASK_XLEN(-0x1000001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 204);

        // rs1_val == 8388608, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x800000;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x800000));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 208);

        // rs1_val == 67108864, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4000000;  op2val:0x4000
        c.li(Reg::X10, MASK_XLEN(0x4000000));
        c.li(Reg::X11, MASK_XLEN(0x4000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 212);

        // rs1_val == 536870912, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x20000000;  op2val:-0x2000001
        c.li(Reg::X10, MASK_XLEN(0x20000000));
        c.li(Reg::X11, MASK_XLEN(-0x2000001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 216);

        // rs1_val == -3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x3;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x3));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 220);

        // rs1_val == -5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x5;  op2val:0x400
        c.li(Reg::X10, MASK_XLEN(-0x5));
        c.li(Reg::X11, MASK_XLEN(0x400));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 224);

        // rs1_val == -9, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x9;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0x9));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 228);

        // rs1_val == -33, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x21;  op2val:-0x40001
        c.li(Reg::X10, MASK_XLEN(-0x21));
        c.li(Reg::X11, MASK_XLEN(-0x40001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 232);

        // rs1_val == -65, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x41;  op2val:0x40000000
        c.li(Reg::X10, MASK_XLEN(-0x41));
        c.li(Reg::X11, MASK_XLEN(0x40000000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 236);

        // rs1_val == -2049, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x801;  op2val:0x8
        c.li(Reg::X10, MASK_XLEN(-0x801));
        c.li(Reg::X11, MASK_XLEN(0x8));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 240);

        // rs1_val == -65537, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x10001;  op2val:-0x40001
        c.li(Reg::X10, MASK_XLEN(-0x10001));
        c.li(Reg::X11, MASK_XLEN(-0x40001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 244);

        // rs1_val == -262145, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x40001;  op2val:-0x20000001
        c.li(Reg::X10, MASK_XLEN(-0x40001));
        c.li(Reg::X11, MASK_XLEN(-0x20000001));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 248);

        // rs1_val == -524289, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x80001;  op2val:-0x11
        c.li(Reg::X10, MASK_XLEN(-0x80001));
        c.li(Reg::X11, MASK_XLEN(-0x11));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 252);

        // rs1_val == -1048577, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x100001;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0x100001));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 256);

        // rs1_val == -4194305, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x400001;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0x400001));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 260);

        // rs1_val == -33554433, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x2000001;  op2val:0x1000
        c.li(Reg::X10, MASK_XLEN(-0x2000001));
        c.li(Reg::X11, MASK_XLEN(0x1000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 264);

        // rs1_val == -134217729, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x8000001;  op2val:0x800
        c.li(Reg::X10, MASK_XLEN(-0x8000001));
        c.li(Reg::X11, MASK_XLEN(0x800));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 268);

        // rs1_val == -536870913, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x20000001;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0x20000001));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 272);

        // rs1_val == -1073741825, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x40000001;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x40000001));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 276);

        // rs1_val==3 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 280);

        // rs1_val==3 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 284);

        // rs1_val==3 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 288);

        // rs1_val==3 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 292);

        // rs1_val==3 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 296);

        // rs1_val==3 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 300);

        // rs1_val==3 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 304);

        // rs1_val==3 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 308);

        // rs1_val==3 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 312);

        // rs1_val==3 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 316);

        // rs1_val==3 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 320);

        // rs1_val==3 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 324);

        // rs1_val==3 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 328);

        // rs1_val==3 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 332);

        // rs1_val==3 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 336);

        // rs1_val==3 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 340);

        // rs1_val==3 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 344);

        // rs1_val==3 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 348);

        // rs1_val==3 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 352);

        // rs1_val==3 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 356);

        // rs1_val==3 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 360);

        // rs1_val==1431655765 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 364);

        // rs1_val==1431655765 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 368);

        // rs1_val==1431655765 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 372);

        // rs1_val==1431655765 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 376);

        // rs1_val==1431655765 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 380);

        // rs1_val==1431655765 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 384);

        // rs1_val==1431655765 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 388);

        // rs1_val==1431655765 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 392);

        // rs1_val==1431655765 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 396);

        // rs1_val==1431655765 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 400);

        // rs1_val==1431655765 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 404);

        // rs1_val==1431655765 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 408);

        // rs1_val==1431655765 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 412);

        // rs1_val==1431655765 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 416);

        // rs1_val==1431655765 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 420);

        // rs1_val==1431655765 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 424);

        // rs1_val==1431655765 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 428);

        // rs1_val==1431655765 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 432);

        // rs1_val==1431655765 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 436);

        // rs1_val==1431655765 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 440);

        // rs1_val==1431655765 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 444);

        // rs1_val==1431655765 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 448);

        // rs1_val==-1431655766 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 452);

        // rs1_val==-1431655766 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 456);

        // rs1_val==-1431655766 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 460);

        // rs1_val==-1431655766 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 464);

        // rs1_val==-1431655766 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 468);

        // rs1_val==-1431655766 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 472);

        // rs1_val==-1431655766 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 476);

        // rs1_val==-1431655766 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 480);

        // rs1_val==-1431655766 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 484);

        // rs1_val==-1431655766 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 488);

        // rs1_val==-1431655766 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 492);

        // rs1_val==-1431655766 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 496);

        // rs1_val==-1431655766 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 500);

        // rs1_val==-1431655766 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 504);

        // rs1_val==-1431655766 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 508);

        // rs1_val==-1431655766 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 512);

        // rs1_val==-1431655766 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 516);

        // rs1_val==-1431655766 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 520);

        // rs1_val==-1431655766 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 524);

        // rs1_val==-1431655766 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 528);

        // rs1_val==-1431655766 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 532);

        // rs1_val==-1431655766 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 536);

        // rs1_val==5 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 540);

        // rs1_val==5 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 544);

        // rs1_val==5 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 548);

        // rs1_val==5 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 552);

        // rs1_val==5 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 556);

        // rs1_val==5 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 560);

        // rs1_val==5 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 564);

        // rs1_val==5 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 568);

        // rs1_val==5 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 572);

        // rs1_val==5 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 576);

        // rs1_val==5 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 580);

        // rs1_val==5 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 584);

        // rs1_val==5 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 588);

        // rs1_val==5 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 592);

        // rs1_val==5 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 596);

        // rs1_val==5 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 600);

        // rs1_val==5 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 604);

        // rs1_val==5 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 608);

        // rs1_val==5 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 612);

        // rs1_val==5 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 616);

        // rs1_val==5 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 620);

        // rs1_val==5 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 624);

        // rs1_val==858993459 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 628);

        // rs1_val==858993459 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 632);

        // rs1_val==858993459 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 636);

        // rs1_val==858993459 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 640);

        // rs1_val==858993459 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 644);

        // rs1_val==858993459 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 648);

        // rs1_val==858993459 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 652);

        // rs1_val==858993459 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 656);

        // rs1_val==858993459 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 660);

        // rs1_val==858993459 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 664);

        // rs1_val==858993459 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 668);

        // rs1_val==858993459 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 672);

        // rs1_val==858993459 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 676);

        // rs1_val==858993459 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 680);

        // rs1_val==858993459 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 684);

        // rs1_val==858993459 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 688);

        // rs1_val==858993459 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 692);

        // rs1_val==858993459 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 696);

        // rs1_val==858993459 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 700);

        // rs1_val==858993459 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 704);

        // rs1_val==858993459 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 708);

        // rs1_val==858993459 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 712);

        // rs1_val==1717986918 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 716);

        // rs1_val==1717986918 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 720);

        // rs1_val==1717986918 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 724);

        // rs1_val==1717986918 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 728);

        // rs1_val==1717986918 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 732);

        // rs1_val==1717986918 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 736);

        // rs1_val==1717986918 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 740);

        // rs1_val==1717986918 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 744);

        // rs1_val==1717986918 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 748);

        // rs1_val==1717986918 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 752);

        // rs1_val==1717986918 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 756);

        // rs1_val==1717986918 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 760);

        // rs1_val==1717986918 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 764);

        // rs1_val==1717986918 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 768);

        // rs1_val==1717986918 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 772);

        // rs1_val==1717986918 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 776);

        // rs1_val==1717986918 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 780);

        // rs1_val==1717986918 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 784);

        // rs1_val==1717986918 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 788);

        // rs1_val==1717986918 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 792);

        // rs1_val==1717986918 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 796);

        // rs1_val==1717986918 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 800);

        // rs1_val==-46340 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 804);

        // rs1_val==-46340 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 808);

        // rs1_val==-46340 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 812);

        // rs1_val==-46340 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 816);

        // rs1_val==-46340 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 820);

        // rs1_val==-46340 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 824);

        // rs1_val==-46340 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 828);

        // rs1_val==-46340 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 832);

        // rs1_val==-46340 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 836);

        // rs1_val==-46340 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 840);

        // rs1_val==-46340 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 844);

        // rs1_val==-46340 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 848);

        // rs1_val==-46340 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 852);

        // rs1_val==-46340 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 856);

        // rs1_val==-46340 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 860);

        // rs1_val==-46340 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 864);

        // rs1_val==-46340 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 868);

        // rs1_val==-46340 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 872);

        // rs1_val==-46340 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 876);

        // rs1_val==-46340 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 880);

        // rs1_val==-46340 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 884);

        // rs1_val==-46340 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 888);

        // rs1_val==46340 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 892);

        // rs1_val==46340 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 896);

        // rs1_val==46340 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 900);

        // rs1_val==46340 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 904);

        // rs1_val==46340 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 908);

        // rs1_val==46340 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 912);

        // rs1_val==46340 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 916);

        // rs1_val==46340 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 920);

        // rs1_val==46340 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 924);

        // rs1_val==46340 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 928);

        // rs1_val==46340 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 932);

        // rs1_val==46340 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 936);

        // rs1_val==46340 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 940);

        // rs1_val==46340 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 944);

        // rs1_val==46340 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 948);

        // rs1_val==46340 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 952);

        // rs1_val==46340 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 956);

        // rs1_val==46340 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 960);

        // rs1_val==46340 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 964);

        // rs1_val==46340 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 968);

        // rs1_val==46340 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 972);

        // rs1_val==46340 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 976);

        // rs1_val==2 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 980);

        // rs1_val==2 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 984);

        // rs1_val==2 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 988);

        // rs1_val==2 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 992);

        // rs1_val==2 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 996);

        // rs1_val==2 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1000);

        // rs1_val==2 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1004);

        // rs1_val==2 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1008);

        // rs1_val==2 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1012);

        // rs1_val==2 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1016);

        // rs1_val==2 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1020);

        // rs1_val==2 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1024);

        // rs1_val==2 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1028);

        // rs1_val==2 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1032);

        // rs1_val==2 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1036);

        // rs1_val==2 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1040);

        // rs1_val==2 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1044);

        // rs1_val==2 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1048);

        // rs1_val==2 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1052);

        // rs1_val==2 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1056);

        // rs1_val==2 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1060);

        // rs1_val==1431655764 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1064);

        // rs1_val==1431655764 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1068);

        // rs1_val==1431655764 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1072);

        // rs1_val==1431655764 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1076);

        // rs1_val==1431655764 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1080);

        // rs1_val==1431655764 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1084);

        // rs1_val==1431655764 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1088);

        // rs1_val==1431655764 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1092);

        // rs1_val==1431655764 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1096);

        // rs1_val==1431655764 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1100);

        // rs1_val==1431655764 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1104);

        // rs1_val==1431655764 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1108);

        // rs1_val==1431655764 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1112);

        // rs1_val==1431655764 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1116);

        // rs1_val==1431655764 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1120);

        // rs1_val==1431655764 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1124);

        // rs1_val==1431655764 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1128);

        // rs1_val==1431655764 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1132);

        // rs1_val==1431655764 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1136);

        // rs1_val==1431655764 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1140);

        // rs1_val==1431655764 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1144);

        // rs1_val==1431655764 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1148);

        // rs1_val==0 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1152);

        // rs1_val==0 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1156);

        // rs1_val==0 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1160);

        // rs1_val==0 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1164);

        // rs1_val==0 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1168);

        // rs1_val==0 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1172);

        // rs1_val==0 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1176);

        // rs1_val==0 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1180);

        // rs1_val==-1431655765 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1184);

        // rs1_val==-1431655765 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1188);

        // rs1_val==-1431655765 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1192);

        // rs1_val==-1431655765 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1196);

        // rs1_val==-1431655765 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1200);

        // rs1_val==-1431655765 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1204);

        // rs1_val==6 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1208);

        // rs1_val==6 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1212);

        // rs1_val==6 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1216);

        // rs1_val==6 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1220);

        // rs1_val==6 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1224);

        // rs1_val==6 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1228);

        // rs1_val==6 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1232);

        // rs1_val==6 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1236);

        // rs1_val==6 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1240);

        // rs1_val==6 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1244);

        // rs1_val==6 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1248);

        // rs1_val==6 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1252);

        // rs1_val==6 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1256);

        // rs1_val==6 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1260);

        // rs1_val==6 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1264);

        // rs1_val==6 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1268);

        // rs1_val==6 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1272);

        // rs1_val==6 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1276);

        // rs1_val==6 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1280);

        // rs1_val==6 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1284);

        // rs1_val==6 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1288);

        // rs1_val==6 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1292);

        // rs1_val==858993460 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1296);

        // rs1_val==858993460 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1300);

        // rs1_val==858993460 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1304);

        // rs1_val==858993460 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1308);

        // rs1_val==858993460 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1312);

        // rs1_val==858993460 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1316);

        // rs1_val==858993460 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1320);

        // rs1_val==858993460 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1324);

        // rs1_val==858993460 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1328);

        // rs1_val==858993460 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1332);

        // rs1_val==858993460 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1336);

        // rs1_val==858993460 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1340);

        // rs1_val==858993460 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1344);

        // rs1_val==858993460 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1348);

        // rs1_val==858993460 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1352);

        // rs1_val==858993460 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1356);

        // rs1_val==858993460 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1360);

        // rs1_val==858993460 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1364);

        // rs1_val==858993460 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1368);

        // rs1_val==858993460 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1372);

        // rs1_val==858993460 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1376);

        // rs1_val==858993460 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1380);

        // rs1_val==1717986919 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1384);

        // rs1_val==1717986919 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1388);

        // rs1_val==1717986919 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1392);

        // rs1_val==1717986919 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1396);

        // rs1_val==1717986919 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1400);

        // rs1_val==1717986919 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1404);

        // rs1_val==1717986919 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1408);

        // rs1_val==1717986919 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1412);

        // rs1_val==1717986919 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1416);

        // rs1_val==1717986919 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1420);

        // rs1_val==1717986919 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1424);

        // rs1_val==1717986919 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1428);

        // rs1_val==1717986919 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1432);

        // rs1_val==1717986919 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1436);

        // rs1_val==1717986919 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1440);

        // rs1_val==1717986919 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1444);

        // rs1_val==1717986919 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1448);

        // rs1_val==1717986919 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1452);

        // rs1_val==1717986919 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1456);

        // rs1_val==1717986919 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1460);

        // rs1_val==1717986919 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1464);

        // rs1_val==-46339 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1468);

        // rs1_val==-46339 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1472);

        // rs1_val==-46339 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1476);

        // rs1_val==-46339 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1480);

        // rs1_val==-46339 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1484);

        // rs1_val==-46339 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1488);

        // rs1_val==-46339 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1492);

        // rs1_val==-46339 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1496);

        // rs1_val==-46339 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1500);

        // rs1_val==-46339 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1504);

        // rs1_val==-46339 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1508);

        // rs1_val==-46339 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1512);

        // rs1_val==-46339 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1516);

        // rs1_val==-46339 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1520);

        // rs1_val==-46339 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1524);

        // rs1_val==-46339 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1528);

        // rs1_val==-46339 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1532);

        // rs1_val==-46339 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1536);

        // rs1_val==-46339 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1540);

        // rs1_val==-46339 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1544);

        // rs1_val==-46339 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1548);

        // rs1_val==-46339 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1552);

        // rs1_val==46341 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1556);

        // rs1_val==46341 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1560);

        // rs1_val==46341 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1564);

        // rs1_val==46341 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1568);

        // rs1_val==46341 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1572);

        // rs1_val==46341 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1576);

        // rs1_val==46341 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1580);

        // rs1_val==46341 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1584);

        // rs1_val==46341 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1588);

        // rs1_val==46341 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1592);

        // rs1_val==46341 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1596);

        // rs1_val==46341 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1600);

        // rs1_val==46341 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1604);

        // rs1_val==46341 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1608);

        // rs1_val==46341 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1612);

        // rs1_val==46341 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1616);

        // rs1_val==46341 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1620);

        // rs1_val==46341 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1624);

        // rs1_val==46341 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1628);

        // rs1_val==46341 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1632);

        // rs1_val==46341 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1636);

        // rs1_val==46341 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1640);

        // rs1_val==0 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1644);

        // rs1_val==0 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1648);

        // rs1_val==0 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1652);

        // rs1_val==0 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1656);

        // rs1_val==0 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1660);

        // rs1_val==0 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1664);

        // rs1_val==0 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1668);

        // rs1_val==0 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1672);

        // rs1_val==0 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1676);

        // rs1_val==0 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1680);

        // rs1_val==0 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1684);

        // rs1_val==0 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1688);

        // rs1_val==0 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1692);

        // rs1_val==0 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1696);

        // rs1_val==4 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1700);

        // rs1_val==4 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1704);

        // rs1_val==4 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1708);

        // rs1_val==4 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1712);

        // rs1_val==4 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1716);

        // rs1_val==4 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1720);

        // rs1_val==4 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1724);

        // rs1_val==4 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1728);

        // rs1_val==4 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1732);

        // rs1_val==4 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1736);

        // rs1_val==4 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1740);

        // rs1_val==4 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1744);

        // rs1_val==4 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1748);

        // rs1_val==4 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1752);

        // rs1_val==4 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1756);

        // rs1_val==4 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1760);

        // rs1_val==4 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1764);

        // rs1_val==4 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1768);

        // rs1_val==4 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1772);

        // rs1_val==4 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1776);

        // rs1_val==4 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1780);

        // rs1_val==4 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1784);

        // rs1_val==858993458 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1788);

        // rs1_val==858993458 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1792);

        // rs1_val==858993458 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1796);

        // rs1_val==858993458 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1800);

        // rs1_val==858993458 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1804);

        // rs1_val==858993458 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1808);

        // rs1_val==858993458 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1812);

        // rs1_val==858993458 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1816);

        // rs1_val==858993458 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1820);

        // rs1_val==858993458 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1824);

        // rs1_val==858993458 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1828);

        // rs1_val==858993458 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1832);

        // rs1_val==858993458 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1836);

        // rs1_val==858993458 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1840);

        // rs1_val==858993458 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1844);

        // rs1_val==858993458 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1848);

        // rs1_val==858993458 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1852);

        // rs1_val==858993458 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1856);

        // rs1_val==858993458 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1860);

        // rs1_val==858993458 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1864);

        // rs1_val==858993458 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1868);

        // rs1_val==858993458 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1872);

        // rs1_val==1717986917 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1876);

        // rs1_val==1717986917 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1880);

        // rs1_val==1717986917 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1884);

        // rs1_val==1717986917 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1888);

        // rs1_val==1717986917 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1892);

        // rs1_val==1717986917 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1896);

        // rs1_val==1717986917 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1900);

        // rs1_val==1717986917 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1904);

        // rs1_val==1717986917 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1908);

        // rs1_val==1717986917 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1912);

        // rs1_val==1717986917 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1916);

        // rs1_val==1717986917 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1920);

        // rs1_val==1717986917 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1924);

        // rs1_val==1717986917 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1928);

        // rs1_val==1717986917 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1932);

        // rs1_val==1717986917 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1936);

        // rs1_val==1717986917 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1940);

        // rs1_val==1717986917 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1944);

        // rs1_val==1717986917 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1948);

        // rs1_val==1717986917 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1952);

        // rs1_val==1717986917 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1956);

        // rs1_val==1717986917 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1960);

        // rs1_val==46339 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1964);

        // rs1_val==46339 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1968);

        // rs1_val==46339 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1972);

        // rs1_val==46339 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1976);

        // rs1_val==46339 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1980);

        // rs1_val==46339 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1984);

        // rs1_val==46339 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1988);

        // rs1_val==46339 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1992);

        // rs1_val==46339 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 1996);

        // rs1_val==46339 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2000);

        // rs1_val==46339 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2004);

        // rs1_val==46339 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2008);

        // rs1_val==46339 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2012);

        // rs1_val==46339 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2016);

        // rs1_val==46339 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2020);

        // rs1_val==46339 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2024);

        // rs1_val==46339 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2028);

        // rs1_val==46339 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2032);

        // rs1_val==46339 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2036);

        // rs1_val==46339 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2040);

        // rs1_val==46339 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 2044);

        c.li(Reg::X1, 2164);
        // rs1_val==46339 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 0);

        // rs1_val==1431655766 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 4);

        // rs1_val==1431655766 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 8);

        // rs1_val==1431655766 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 12);

        // rs1_val==1431655766 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 16);

        // rs1_val==1431655766 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 20);

        // rs1_val==1431655766 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 24);

        // rs1_val==1431655766 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 28);

        // rs1_val==1431655766 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 32);

        // rs1_val==1431655766 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 36);

        // rs1_val==1431655766 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 40);

        // rs1_val==1431655766 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 44);

        // rs1_val==1431655766 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 48);

        // rs1_val==1431655766 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 52);

        // rs1_val==1431655766 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 56);

        // rs1_val==1431655766 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 60);

        // rs1_val==1431655766 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 64);

        // rs1_val==1431655766 and rs2_val==-1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 68);

        // rs1_val==1431655766 and rs2_val==6, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 72);

        // rs1_val==1431655766 and rs2_val==858993460, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 76);

        // rs1_val==1431655766 and rs2_val==1717986919, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 80);

        // rs1_val==1431655766 and rs2_val==-46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 84);

        // rs1_val==1431655766 and rs2_val==46341, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 88);

        // rs1_val==-1431655765 and rs2_val==3, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 92);

        // rs1_val==-1431655765 and rs2_val==1431655765, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 96);

        // rs1_val==-1431655765 and rs2_val==-1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 100);

        // rs1_val==-1431655765 and rs2_val==5, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 104);

        // rs1_val==-1431655765 and rs2_val==858993459, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 108);

        // rs1_val==-1431655765 and rs2_val==1717986918, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 112);

        // rs1_val==-1431655765 and rs2_val==-46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 116);

        // rs1_val==-1431655765 and rs2_val==46340, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 120);

        // rs1_val==-1431655765 and rs2_val==2, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 124);

        // rs1_val==-1431655765 and rs2_val==1431655764, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 128);

        // rs1_val==-1431655765 and rs2_val==0, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 132);

        // rs1_val==-1431655765 and rs2_val==4, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 136);

        // rs1_val==-1431655765 and rs2_val==858993458, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 140);

        // rs1_val==-1431655765 and rs2_val==1717986917, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 144);

        // rs1_val==-1431655765 and rs2_val==46339, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 148);

        // rs1_val==-1431655765 and rs2_val==1431655766, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 152);

        // rs1_val < 0 and rs2_val < 0, rs2_val == -9, rs1_val == -16385
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x4001;  op2val:-0x9
        c.li(Reg::X10, MASK_XLEN(-0x4001));
        c.li(Reg::X11, MASK_XLEN(-0x9));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 156);

        // rs2_val == 1048576, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:-0x4001;  op2val:0x100000
        c.li(Reg::X10, MASK_XLEN(-0x4001));
        c.li(Reg::X11, MASK_XLEN(0x100000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 160);

        // rs2_val == 2097152, rs1_val == 4
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x200000
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x200000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 164);

        // rs2_val == 4194304, 
        // opcode: slt ; op1:x10; op2:x11; dest:x12; op1val:0x9;  op2val:0x400000
        c.li(Reg::X10, MASK_XLEN(0x9));
        c.li(Reg::X11, MASK_XLEN(0x400000));
        c.slt(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X1, 168);


                
        // End
        c.ecall();
        return c;
    }

    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> slt_01::s_ScalarData;
}


// Generate google tests

TEST_F(slt_01, opcode__slt___op1_x18__op2_x26__dest_x26__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[0], 0);
}


TEST_F(slt_01, opcode__slt___op1_x20__op2_x20__dest_x1__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[1], 0);
}


TEST_F(slt_01, opcode__slt___op1_x21__op2_x21__dest_x21__op1val__0x4001___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[2], 0);
}


TEST_F(slt_01, opcode__slt___op1_x15__op2_x27__dest_x15__op1val__0x201___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[3], 1);
}


TEST_F(slt_01, opcode__slt___op1_x5__op2_x18__dest_x7__op1val_0x33333334___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[4], 0);
}


TEST_F(slt_01, opcode__slt___op1_x25__op2_x19__dest_x17__op1val_0x8000000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(slt_01, opcode__slt___op1_x9__op2_x31__dest_x23__op1val_0x20000___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[6], 1);
}


TEST_F(slt_01, opcode__slt___op1_x2__op2_x15__dest_x11__op1val__0x20001___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[7], 1);
}


TEST_F(slt_01, opcode__slt___op1_x28__op2_x13__dest_x22__op1val__0x80000000___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[8], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x7__dest_x30__op1val_0x0___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[9], 1);
}


TEST_F(slt_01, opcode__slt___op1_x24__op2_x22__dest_x5__op1val_0x7fffffff___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(slt_01, opcode__slt___op1_x8__op2_x3__dest_x24__op1val_0x1___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[11], 0);
}


TEST_F(slt_01, opcode__slt___op1_x26__op2_x9__dest_x2__op1val_0x10000000___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(slt_01, opcode__slt___op1_x4__op2_x14__dest_x8__op1val_0x8000___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(slt_01, opcode__slt___op1_x6__op2_x4__dest_x25__op1val__0x1001___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[14], 1);
}


TEST_F(slt_01, opcode__slt___op1_x31__op2_x25__dest_x14__op1val_0x1000000___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(slt_01, opcode__slt___op1_x14__op2_x8__dest_x20__op1val_0x5___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[16], 1);
}


TEST_F(slt_01, opcode__slt___op1_x7__op2_x11__dest_x19__op1val_0x0___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[17], 1);
}


TEST_F(slt_01, opcode__slt___op1_x19__op2_x29__dest_x9__op1val_0x5___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[18], 1);
}


TEST_F(slt_01, opcode__slt___op1_x3__op2_x23__dest_x6__op1val_0x400000___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[19], 0);
}


TEST_F(slt_01, opcode__slt___op1_x11__op2_x1__dest_x31__op1val__0x11___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[20], 1);
}


TEST_F(slt_01, opcode__slt___op1_x30__op2_x16__dest_x12__op1val__0x1000001___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[21], 1);
}


TEST_F(slt_01, opcode__slt___op1_x13__op2_x2__dest_x16__op1val_0x0___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[22], 1);
}


TEST_F(slt_01, opcode__slt___op1_x22__op2_x12__dest_x10__op1val_0x20___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[23], 1);
}


TEST_F(slt_01, opcode__slt___op1_x29__op2_x24__dest_x27__op1val__0x2___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[24], 1);
}


TEST_F(slt_01, opcode__slt___op1_x16__op2_x30__dest_x4__op1val__0x4000001___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[25], 1);
}


TEST_F(slt_01, opcode__slt___op1_x17__op2_x28__dest_x3__op1val_0x1000___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[26], 1);
}


TEST_F(slt_01, opcode__slt___op1_x1__op2_x5__dest_x13__op1val_0x2000000___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[27], 0);
}


TEST_F(slt_01, opcode__slt___op1_x27__op2_x10__dest_x18__op1val__0x200001___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[28], 1);
}


TEST_F(slt_01, opcode__slt___op1_x0__op2_x17__dest_x28__op1val_0x0___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[29], 1);
}


TEST_F(slt_01, opcode__slt___op1_x12__op2_x0__dest_x29__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[30], 0);
}


TEST_F(slt_01, opcode__slt___op1_x23__op2_x6__dest_x0__op1val_0x9___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[31], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[32], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[33], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[34], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[35], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[36], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[37], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[38], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[39], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[40], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x7___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[41], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[42], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[43], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[44], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[45], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[46], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[47], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[48], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[49], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[50], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x9___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[51], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[52], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[53], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[54], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[55], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[56], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[57], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[58], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[59], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[60], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[61], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[62], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[63], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x6___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[64], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[65], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[66], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[67], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[68], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[69], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[70], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[71], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[72], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[73], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[74], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[75], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[76], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[77], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[78], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[79], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[80], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[81], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[82], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[83], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[84], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[85], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[86], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[87], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[88], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[89], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[90], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[91], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[92], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[93], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[94], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[95], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[96], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[97], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[98], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[99], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[100], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[101], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[102], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[103], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[104], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[105], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[106], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[107], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[108], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[109], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[110], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[111], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[112], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[113], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[114], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[115], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[116], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[117], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[118], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[119], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[120], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[121], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[122], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[123], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[124], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[125], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[126], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[127], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[128], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[129], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[130], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[131], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[132], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[133], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[134], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[135], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[136], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[137], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[138], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[139], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[140], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[141], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[142], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[143], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[144], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[145], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[146], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[147], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[148], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[149], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[150], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[151], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[152], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[153], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[154], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[155], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[156], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[157], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[158], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[159], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[160], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[161], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[162], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[163], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[164], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[165], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[166], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[167], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[168], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[169], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[170], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[171], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[172], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[173], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[174], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[175], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[176], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[177], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[178], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[179], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[180], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[181], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[182], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[183], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[184], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[185], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[186], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[187], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[188], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[189], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[190], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[191], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[192], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[193], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[194], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[195], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[196], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[197], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[198], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[199], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[200], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[201], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[202], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[203], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[204], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[205], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[206], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[207], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[208], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[209], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[210], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[211], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[212], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[213], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[214], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[215], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[216], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[217], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[218], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[219], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[220], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[221], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[222], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[223], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[224], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[225], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[226], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[227], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[228], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[229], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[230], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[231], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[232], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[233], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[234], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[235], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[236], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[237], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[238], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[239], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[240], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[241], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[242], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[243], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[244], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[245], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[246], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[247], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[248], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[249], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[250], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[251], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[252], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[253], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[254], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[255], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[256], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[257], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[258], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[259], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[260], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[261], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[262], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[263], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[264], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[265], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[266], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[267], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[268], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[269], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[270], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[271], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[272], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[273], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[274], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[275], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[276], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[277], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[278], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[279], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[280], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[281], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[282], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[283], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[284], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[285], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[286], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[287], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[288], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[289], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[290], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[291], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[292], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[293], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[294], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[295], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[296], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[297], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[298], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[299], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[300], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[301], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[302], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[303], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[304], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[305], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[306], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[307], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[308], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[309], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[310], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[311], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[312], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[313], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[314], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[315], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[316], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[317], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[318], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[319], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[320], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[321], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[322], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[323], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[324], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[325], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[326], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[327], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[328], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[329], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[330], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[331], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[332], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[333], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[334], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[335], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[336], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[337], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[338], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[339], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[340], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[341], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[342], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[343], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[344], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[345], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[346], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[347], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[348], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[349], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[350], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[351], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[352], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[353], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[354], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[355], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[356], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[357], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[358], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[359], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[360], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[361], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[362], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[363], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[364], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[365], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[366], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[367], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[368], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[369], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[370], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[371], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[372], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[373], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[374], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[375], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[376], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[377], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[378], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[379], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[380], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[381], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[382], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[383], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[384], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[385], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[386], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[387], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[388], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[389], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[390], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[391], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[392], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[393], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[394], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[395], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[396], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[397], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[398], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[399], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[400], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[401], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[402], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[403], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[404], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[405], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[406], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[407], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[408], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[409], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[410], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[411], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[412], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[413], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[414], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[415], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[416], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[417], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[418], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[419], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[420], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[421], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[422], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[423], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[424], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[425], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[426], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[427], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[428], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[429], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[430], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[431], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[432], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[433], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[434], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[435], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[436], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[437], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[438], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[439], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[440], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[441], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[442], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[443], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[444], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[445], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[446], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[447], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[448], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[449], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[450], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[451], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[452], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[453], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[454], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[455], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[456], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[457], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[458], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[459], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[460], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[461], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[462], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[463], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[464], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[465], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[466], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[467], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[468], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[469], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[470], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[471], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[472], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[473], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[474], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[475], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[476], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[477], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[478], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[479], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[480], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[481], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[482], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[483], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[484], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[485], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[486], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[487], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[488], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[489], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[490], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[491], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[492], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[493], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[494], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[495], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[496], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[497], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[498], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[499], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[500], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[501], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[502], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[503], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[504], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[505], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[506], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[507], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[508], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[509], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[510], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[511], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[512], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[513], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[514], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[515], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[516], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[517], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[518], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[519], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[520], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[521], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[522], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[523], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[524], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[525], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[526], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[527], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[528], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[529], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[530], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[531], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[532], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[533], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[534], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[535], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[536], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[537], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[538], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[539], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[540], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[541], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[542], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[543], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[544], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[545], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[546], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[547], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[548], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[549], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[550], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[551], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[552], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[553], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[554], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[555], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[556], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[557], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[558], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[559], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[560], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[561], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[562], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[563], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[564], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[565], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[566], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[567], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[568], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[569], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[570], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[571], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[572], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[573], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[574], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[575], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[576], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[577], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[578], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[579], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[580], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[581], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[582], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x9___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[583], 1);
}

