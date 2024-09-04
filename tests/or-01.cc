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
class or_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        // Generate word-aligned vector to initialise memory
        const std::vector<uint32_t> scalarWordInitData{
        3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,};
        
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
        // rs2 == rd != rs1, rs1==x8, rs2==x26, rd==x26, rs1_val > 0 and rs2_val > 0, rs2_val == 16, rs1_val == 1048576, rs1_val != rs2_val
        // opcode: or ; op1:x8; op2:x26; dest:x26; op1val:0x100000;  op2val:0x10
        c.li(Reg::X8, MASK_XLEN(0x100000));
        c.li(Reg::X26, MASK_XLEN(0x10));
        c.or_(Reg::X26, Reg::X8, Reg::X26);
        c.sw(Reg::X26, Reg::X3, 0);

        // rs1 == rs2 != rd, rs1==x6, rs2==x6, rd==x17, rs1_val > 0 and rs2_val < 0, rs2_val == -2049, rs1_val == 2
        // opcode: or ; op1:x6; op2:x6; dest:x17; op1val:0x2;  op2val:0x2
        c.li(Reg::X6, MASK_XLEN(0x2));
        c.li(Reg::X6, MASK_XLEN(0x2));
        c.or_(Reg::X17, Reg::X6, Reg::X6);
        c.sw(Reg::X17, Reg::X3, 4);

        // rs1 == rs2 == rd, rs1==x31, rs2==x31, rd==x31, rs1_val < 0 and rs2_val < 0, rs1_val == -268435457, rs2_val == -1025
        // opcode: or ; op1:x31; op2:x31; dest:x31; op1val:-0x10000001;  op2val:-0x10000001
        c.li(Reg::X31, MASK_XLEN(-0x10000001));
        c.li(Reg::X31, MASK_XLEN(-0x10000001));
        c.or_(Reg::X31, Reg::X31, Reg::X31);
        c.sw(Reg::X31, Reg::X3, 8);

        // rs1 == rd != rs2, rs1==x27, rs2==x29, rd==x27, rs1_val < 0 and rs2_val > 0, rs1_val == -2049, rs2_val == 4194304
        // opcode: or ; op1:x27; op2:x29; dest:x27; op1val:-0x801;  op2val:0x400000
        c.li(Reg::X27, MASK_XLEN(-0x801));
        c.li(Reg::X29, MASK_XLEN(0x400000));
        c.or_(Reg::X27, Reg::X27, Reg::X29);
        c.sw(Reg::X27, Reg::X3, 12);

        // rs1 != rs2  and rs1 != rd and rs2 != rd, rs1==x30, rs2==x19, rd==x18, rs1_val == rs2_val, rs2_val == -1048577, rs1_val == -1048577
        // opcode: or ; op1:x30; op2:x19; dest:x18; op1val:-0x100001;  op2val:-0x100001
        c.li(Reg::X30, MASK_XLEN(-0x100001));
        c.li(Reg::X19, MASK_XLEN(-0x100001));
        c.or_(Reg::X18, Reg::X30, Reg::X19);
        c.sw(Reg::X18, Reg::X3, 16);

        // rs1==x21, rs2==x14, rd==x9, rs2_val == (-2**(xlen-1)), rs2_val == -2147483648, rs1_val == 131072
        // opcode: or ; op1:x21; op2:x14; dest:x9; op1val:0x20000;  op2val:-0x80000000
        c.li(Reg::X21, MASK_XLEN(0x20000));
        c.li(Reg::X14, MASK_XLEN(-0x80000000));
        c.or_(Reg::X9, Reg::X21, Reg::X14);
        c.sw(Reg::X9, Reg::X3, 20);

        // rs1==x26, rs2==x24, rd==x4, rs2_val == 0, rs1_val == -8193
        // opcode: or ; op1:x26; op2:x24; dest:x4; op1val:-0x2001;  op2val:0x0
        c.li(Reg::X26, MASK_XLEN(-0x2001));
        c.li(Reg::X24, MASK_XLEN(0x0));
        c.or_(Reg::X4, Reg::X26, Reg::X24);
        c.sw(Reg::X4, Reg::X3, 24);

        // rs1==x9, rs2==x8, rd==x30, rs2_val == (2**(xlen-1)-1), rs1_val == 0, rs2_val == 2147483647
        // opcode: or ; op1:x9; op2:x8; dest:x30; op1val:0x0;  op2val:0x7fffffff
        c.li(Reg::X9, MASK_XLEN(0x0));
        c.li(Reg::X8, MASK_XLEN(0x7fffffff));
        c.or_(Reg::X30, Reg::X9, Reg::X8);
        c.sw(Reg::X30, Reg::X3, 28);

        // rs1==x23, rs2==x7, rd==x8, rs2_val == 1, rs1_val == -8388609
        // opcode: or ; op1:x23; op2:x7; dest:x8; op1val:-0x800001;  op2val:0x1
        c.li(Reg::X23, MASK_XLEN(-0x800001));
        c.li(Reg::X7, MASK_XLEN(0x1));
        c.or_(Reg::X8, Reg::X23, Reg::X7);
        c.sw(Reg::X8, Reg::X3, 32);

        // rs1==x12, rs2==x0, rd==x22, rs1_val == (-2**(xlen-1)), rs1_val == -2147483648
        // opcode: or ; op1:x12; op2:x0; dest:x22; op1val:-0x80000000;  op2val:0x0
        c.li(Reg::X12, MASK_XLEN(-0x80000000));
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.or_(Reg::X22, Reg::X12, Reg::X0);
        c.sw(Reg::X22, Reg::X3, 36);

        // rs1==x10, rs2==x30, rd==x28, rs1_val == (2**(xlen-1)-1), rs2_val == 64, rs1_val == 2147483647
        // opcode: or ; op1:x10; op2:x30; dest:x28; op1val:0x7fffffff;  op2val:0x40
        c.li(Reg::X10, MASK_XLEN(0x7fffffff));
        c.li(Reg::X30, MASK_XLEN(0x40));
        c.or_(Reg::X28, Reg::X10, Reg::X30);
        c.sw(Reg::X28, Reg::X3, 40);

        // rs1==x18, rs2==x21, rd==x16, rs1_val == 1, 
        // opcode: or ; op1:x18; op2:x21; dest:x16; op1val:0x1;  op2val:0x55555554
        c.li(Reg::X18, MASK_XLEN(0x1));
        c.li(Reg::X21, MASK_XLEN(0x55555554));
        c.or_(Reg::X16, Reg::X18, Reg::X21);
        c.sw(Reg::X16, Reg::X3, 44);

        // rs1==x14, rs2==x17, rd==x12, rs2_val == 2, rs1_val == 4096
        // opcode: or ; op1:x14; op2:x17; dest:x12; op1val:0x1000;  op2val:0x2
        c.li(Reg::X14, MASK_XLEN(0x1000));
        c.li(Reg::X17, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X14, Reg::X17);
        c.sw(Reg::X12, Reg::X3, 48);

        // rs1==x19, rs2==x16, rd==x15, rs2_val == 4, 
        // opcode: or ; op1:x19; op2:x16; dest:x15; op1val:-0x800001;  op2val:0x4
        c.li(Reg::X19, MASK_XLEN(-0x800001));
        c.li(Reg::X16, MASK_XLEN(0x4));
        c.or_(Reg::X15, Reg::X19, Reg::X16);
        c.sw(Reg::X15, Reg::X3, 52);

        // rs1==x4, rs2==x2, rd==x7, rs2_val == 8, rs1_val == -1025
        // opcode: or ; op1:x4; op2:x2; dest:x7; op1val:-0x401;  op2val:0x8
        c.li(Reg::X4, MASK_XLEN(-0x401));
        c.li(Reg::X2, MASK_XLEN(0x8));
        c.or_(Reg::X7, Reg::X4, Reg::X2);
        c.sw(Reg::X7, Reg::X3, 56);

        // rs1==x2, rs2==x22, rd==x11, rs2_val == 32, 
        // opcode: or ; op1:x2; op2:x22; dest:x11; op1val:0x7fffffff;  op2val:0x20
        c.li(Reg::X2, MASK_XLEN(0x7fffffff));
        c.li(Reg::X22, MASK_XLEN(0x20));
        c.or_(Reg::X11, Reg::X2, Reg::X22);
        c.sw(Reg::X11, Reg::X3, 60);

        // rs1==x28, rs2==x15, rd==x25, rs2_val == 128, rs1_val == -513
        // opcode: or ; op1:x28; op2:x15; dest:x25; op1val:-0x201;  op2val:0x80
        c.li(Reg::X28, MASK_XLEN(-0x201));
        c.li(Reg::X15, MASK_XLEN(0x80));
        c.or_(Reg::X25, Reg::X28, Reg::X15);
        c.sw(Reg::X25, Reg::X3, 64);

        // rs1==x25, rs2==x1, rd==x6, rs2_val == 256, 
        // opcode: or ; op1:x25; op2:x1; dest:x6; op1val:0xb504;  op2val:0x100
        c.li(Reg::X25, MASK_XLEN(0xb504));
        c.li(Reg::X1, MASK_XLEN(0x100));
        c.or_(Reg::X6, Reg::X25, Reg::X1);
        c.sw(Reg::X6, Reg::X3, 68);

        // rs1==x17, rs2==x10, rd==x20, rs2_val == 512, rs1_val == 4
        // opcode: or ; op1:x17; op2:x10; dest:x20; op1val:0x4;  op2val:0x200
        c.li(Reg::X17, MASK_XLEN(0x4));
        c.li(Reg::X10, MASK_XLEN(0x200));
        c.or_(Reg::X20, Reg::X17, Reg::X10);
        c.sw(Reg::X20, Reg::X3, 72);

        c.li(Reg::X6, 76);
        // rs1==x20, rs2==x23, rd==x5, rs2_val == 1024, 
        // opcode: or ; op1:x20; op2:x23; dest:x5; op1val:-0x100001;  op2val:0x400
        c.li(Reg::X20, MASK_XLEN(-0x100001));
        c.li(Reg::X23, MASK_XLEN(0x400));
        c.or_(Reg::X5, Reg::X20, Reg::X23);
        c.sw(Reg::X5, Reg::X6, 0);

        // rs1==x11, rs2==x12, rd==x2, rs2_val == 2048, 
        // opcode: or ; op1:x11; op2:x12; dest:x2; op1val:0x0;  op2val:0x800
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.li(Reg::X12, MASK_XLEN(0x800));
        c.or_(Reg::X2, Reg::X11, Reg::X12);
        c.sw(Reg::X2, Reg::X6, 4);

        // rs1==x13, rs2==x20, rd==x1, rs2_val == 4096, rs1_val == -67108865
        // opcode: or ; op1:x13; op2:x20; dest:x1; op1val:-0x4000001;  op2val:0x1000
        c.li(Reg::X13, MASK_XLEN(-0x4000001));
        c.li(Reg::X20, MASK_XLEN(0x1000));
        c.or_(Reg::X1, Reg::X13, Reg::X20);
        c.sw(Reg::X1, Reg::X6, 8);

        // rs1==x15, rs2==x27, rd==x3, rs2_val == 8192, 
        // opcode: or ; op1:x15; op2:x27; dest:x3; op1val:0x4;  op2val:0x2000
        c.li(Reg::X15, MASK_XLEN(0x4));
        c.li(Reg::X27, MASK_XLEN(0x2000));
        c.or_(Reg::X3, Reg::X15, Reg::X27);
        c.sw(Reg::X3, Reg::X6, 12);

        // rs1==x22, rs2==x4, rd==x21, rs2_val == 16384, rs1_val == -65537
        // opcode: or ; op1:x22; op2:x4; dest:x21; op1val:-0x10001;  op2val:0x4000
        c.li(Reg::X22, MASK_XLEN(-0x10001));
        c.li(Reg::X4, MASK_XLEN(0x4000));
        c.or_(Reg::X21, Reg::X22, Reg::X4);
        c.sw(Reg::X21, Reg::X6, 16);

        // rs1==x7, rs2==x13, rd==x19, rs2_val == 32768, rs1_val == 524288
        // opcode: or ; op1:x7; op2:x13; dest:x19; op1val:0x80000;  op2val:0x8000
        c.li(Reg::X7, MASK_XLEN(0x80000));
        c.li(Reg::X13, MASK_XLEN(0x8000));
        c.or_(Reg::X19, Reg::X7, Reg::X13);
        c.sw(Reg::X19, Reg::X6, 20);

        // rs1==x24, rs2==x5, rd==x14, rs2_val == 65536, 
        // opcode: or ; op1:x24; op2:x5; dest:x14; op1val:-0x4;  op2val:0x10000
        c.li(Reg::X24, MASK_XLEN(-0x4));
        c.li(Reg::X5, MASK_XLEN(0x10000));
        c.or_(Reg::X14, Reg::X24, Reg::X5);
        c.sw(Reg::X14, Reg::X6, 24);

        // rs1==x16, rs2==x18, rd==x10, rs2_val == 131072, rs1_val == 1431655765
        // opcode: or ; op1:x16; op2:x18; dest:x10; op1val:0x55555555;  op2val:0x20000
        c.li(Reg::X16, MASK_XLEN(0x55555555));
        c.li(Reg::X18, MASK_XLEN(0x20000));
        c.or_(Reg::X10, Reg::X16, Reg::X18);
        c.sw(Reg::X10, Reg::X6, 28);

        // rs1==x1, rs2==x9, rd==x23, rs2_val == 262144, 
        // opcode: or ; op1:x1; op2:x9; dest:x23; op1val:-0xb504;  op2val:0x40000
        c.li(Reg::X1, MASK_XLEN(-0xb504));
        c.li(Reg::X9, MASK_XLEN(0x40000));
        c.or_(Reg::X23, Reg::X1, Reg::X9);
        c.sw(Reg::X23, Reg::X6, 32);

        // rs1==x3, rs2==x11, rd==x29, rs2_val == 524288, 
        // opcode: or ; op1:x3; op2:x11; dest:x29; op1val:0x4;  op2val:0x80000
        c.li(Reg::X3, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x80000));
        c.or_(Reg::X29, Reg::X3, Reg::X11);
        c.sw(Reg::X29, Reg::X6, 36);

        // rs1==x29, rs2==x28, rd==x13, rs2_val == 1048576, rs1_val == -4097
        // opcode: or ; op1:x29; op2:x28; dest:x13; op1val:-0x1001;  op2val:0x100000
        c.li(Reg::X29, MASK_XLEN(-0x1001));
        c.li(Reg::X28, MASK_XLEN(0x100000));
        c.or_(Reg::X13, Reg::X29, Reg::X28);
        c.sw(Reg::X13, Reg::X6, 40);

        // rs1==x5, rs2==x3, rd==x0, rs2_val == 2097152, 
        // opcode: or ; op1:x5; op2:x3; dest:x0; op1val:0x33333332;  op2val:0x200000
        c.li(Reg::X5, MASK_XLEN(0x33333332));
        c.li(Reg::X3, MASK_XLEN(0x200000));
        c.or_(Reg::X0, Reg::X5, Reg::X3);
        c.sw(Reg::X0, Reg::X6, 44);

        // rs1==x0, rs2==x25, rd==x24, rs2_val == 8388608, 
        // opcode: or ; op1:x0; op2:x25; dest:x24; op1val:0x0;  op2val:0x800000
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.li(Reg::X25, MASK_XLEN(0x800000));
        c.or_(Reg::X24, Reg::X0, Reg::X25);
        c.sw(Reg::X24, Reg::X6, 48);

        // rs2_val == 16777216, rs1_val == 64
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x40;  op2val:0x1000000
        c.li(Reg::X10, MASK_XLEN(0x40));
        c.li(Reg::X11, MASK_XLEN(0x1000000));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 52);

        // rs2_val == 33554432, rs1_val == -9
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x9;  op2val:0x2000000
        c.li(Reg::X10, MASK_XLEN(-0x9));
        c.li(Reg::X11, MASK_XLEN(0x2000000));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 56);

        // rs2_val == 67108864, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x4000000
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x4000000));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 60);

        // rs2_val == 134217728, rs1_val == 16777216
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x1000000;  op2val:0x8000000
        c.li(Reg::X10, MASK_XLEN(0x1000000));
        c.li(Reg::X11, MASK_XLEN(0x8000000));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 64);

        // rs2_val == 268435456, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x6;  op2val:0x10000000
        c.li(Reg::X10, MASK_XLEN(-0x6));
        c.li(Reg::X11, MASK_XLEN(0x10000000));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 68);

        // rs2_val == 536870912, rs1_val == -1431655766
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x20000000
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x20000000));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 72);

        // rs2_val == 1073741824, rs1_val == 8388608
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x800000;  op2val:0x40000000
        c.li(Reg::X10, MASK_XLEN(0x800000));
        c.li(Reg::X11, MASK_XLEN(0x40000000));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 76);

        // rs2_val == -2, rs1_val == 32
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x20;  op2val:-0x2
        c.li(Reg::X10, MASK_XLEN(0x20));
        c.li(Reg::X11, MASK_XLEN(-0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 80);

        // rs2_val == -3, rs1_val == 268435456
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x10000000;  op2val:-0x3
        c.li(Reg::X10, MASK_XLEN(0x10000000));
        c.li(Reg::X11, MASK_XLEN(-0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 84);

        // rs2_val == -5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x1;  op2val:-0x5
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.li(Reg::X11, MASK_XLEN(-0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 88);

        // rs2_val == -9, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x80000;  op2val:-0x9
        c.li(Reg::X10, MASK_XLEN(0x80000));
        c.li(Reg::X11, MASK_XLEN(-0x9));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 92);

        // rs2_val == -17, rs1_val == 8
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x8;  op2val:-0x11
        c.li(Reg::X10, MASK_XLEN(0x8));
        c.li(Reg::X11, MASK_XLEN(-0x11));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 96);

        // rs2_val == -33, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x21
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x21));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 100);

        // rs2_val == -65, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x20000;  op2val:-0x41
        c.li(Reg::X10, MASK_XLEN(0x20000));
        c.li(Reg::X11, MASK_XLEN(-0x41));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 104);

        // rs2_val == -129, rs1_val == -2097153
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x200001;  op2val:-0x81
        c.li(Reg::X10, MASK_XLEN(-0x200001));
        c.li(Reg::X11, MASK_XLEN(-0x81));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 108);

        // rs2_val == -257, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x101
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x101));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 112);

        // rs2_val == -513, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x7fffffff;  op2val:-0x201
        c.li(Reg::X10, MASK_XLEN(0x7fffffff));
        c.li(Reg::X11, MASK_XLEN(-0x201));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 116);

        // rs2_val == -4097, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x1001
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x1001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 120);

        // rs2_val == -8193, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x80000000;  op2val:-0x2001
        c.li(Reg::X10, MASK_XLEN(-0x80000000));
        c.li(Reg::X11, MASK_XLEN(-0x2001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 124);

        // rs2_val == -16385, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x4001
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0x4001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 128);

        // rs2_val == -32769, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x800001;  op2val:-0x8001
        c.li(Reg::X10, MASK_XLEN(-0x800001));
        c.li(Reg::X11, MASK_XLEN(-0x8001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 132);

        // rs2_val == -65537, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x9;  op2val:-0x10001
        c.li(Reg::X10, MASK_XLEN(-0x9));
        c.li(Reg::X11, MASK_XLEN(-0x10001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 136);

        // rs2_val == -131073, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x20001
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x20001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 140);

        // rs2_val == -262145, rs1_val == -33
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x21;  op2val:-0x40001
        c.li(Reg::X10, MASK_XLEN(-0x21));
        c.li(Reg::X11, MASK_XLEN(-0x40001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 144);

        // rs2_val == -524289, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x80001
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x80001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 148);

        // rs2_val == -2097153, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x200001
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x200001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 152);

        // rs2_val == -4194305, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x400001
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x400001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 156);

        // rs2_val == -8388609, rs1_val == -257
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x101;  op2val:-0x800001
        c.li(Reg::X10, MASK_XLEN(-0x101));
        c.li(Reg::X11, MASK_XLEN(-0x800001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 160);

        // rs2_val == -16777217, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x1000001
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0x1000001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 164);

        // rs2_val == -33554433, rs1_val == -32769
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x8001;  op2val:-0x2000001
        c.li(Reg::X10, MASK_XLEN(-0x8001));
        c.li(Reg::X11, MASK_XLEN(-0x2000001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 168);

        // rs2_val == -67108865, rs1_val == 65536
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x10000;  op2val:-0x4000001
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.li(Reg::X11, MASK_XLEN(-0x4000001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 172);

        // rs2_val == -134217729, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x8000001
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0x8000001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 176);

        // rs2_val == -268435457, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x40000000;  op2val:-0x10000001
        c.li(Reg::X10, MASK_XLEN(-0x40000000));
        c.li(Reg::X11, MASK_XLEN(-0x10000001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 180);

        // rs2_val == -536870913, rs1_val == 512
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x200;  op2val:-0x20000001
        c.li(Reg::X10, MASK_XLEN(0x200));
        c.li(Reg::X11, MASK_XLEN(-0x20000001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 184);

        // rs2_val == -1073741825, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x40000001
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x40000001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 188);

        // rs2_val == 1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x4;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 192);

        // rs2_val == -1431655766, rs1_val == -536870913
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x20000001;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x20000001));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 196);

        // rs1_val == 16, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x10;  op2val:-0x7
        c.li(Reg::X10, MASK_XLEN(0x10));
        c.li(Reg::X11, MASK_XLEN(-0x7));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 200);

        // rs1_val == 128, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x80;  op2val:-0x7
        c.li(Reg::X10, MASK_XLEN(0x80));
        c.li(Reg::X11, MASK_XLEN(-0x7));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 204);

        // rs1_val == 256, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x100;  op2val:-0x4000001
        c.li(Reg::X10, MASK_XLEN(0x100));
        c.li(Reg::X11, MASK_XLEN(-0x4000001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 208);

        // rs1_val == 1024, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x400;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x400));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 212);

        // rs1_val == 2048, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x800;  op2val:-0x200001
        c.li(Reg::X10, MASK_XLEN(0x800));
        c.li(Reg::X11, MASK_XLEN(-0x200001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 216);

        // rs1_val == 8192, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2000;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x2000));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 220);

        // rs1_val == 16384, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4000;  op2val:-0x6
        c.li(Reg::X10, MASK_XLEN(0x4000));
        c.li(Reg::X11, MASK_XLEN(-0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 224);

        // rs1_val == 32768, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x8000;  op2val:0x800
        c.li(Reg::X10, MASK_XLEN(0x8000));
        c.li(Reg::X11, MASK_XLEN(0x800));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 228);

        // rs1_val == 262144, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x40000;  op2val:-0x1000001
        c.li(Reg::X10, MASK_XLEN(0x40000));
        c.li(Reg::X11, MASK_XLEN(-0x1000001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 232);

        // rs1_val == 2097152, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x200000;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x200000));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 236);

        // rs1_val == 4194304, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x400000;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x400000));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 240);

        // rs1_val == 33554432, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2000000;  op2val:0x3fffffff
        c.li(Reg::X10, MASK_XLEN(0x2000000));
        c.li(Reg::X11, MASK_XLEN(0x3fffffff));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 244);

        // rs1_val == 67108864, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4000000;  op2val:-0x80001
        c.li(Reg::X10, MASK_XLEN(0x4000000));
        c.li(Reg::X11, MASK_XLEN(-0x80001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 248);

        // rs1_val == 134217728, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x8000000;  op2val:0x20
        c.li(Reg::X10, MASK_XLEN(0x8000000));
        c.li(Reg::X11, MASK_XLEN(0x20));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 252);

        // rs1_val == 536870912, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x20000000;  op2val:-0x2001
        c.li(Reg::X10, MASK_XLEN(0x20000000));
        c.li(Reg::X11, MASK_XLEN(-0x2001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 256);

        // rs1_val == 1073741824, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x40000000;  op2val:0x8
        c.li(Reg::X10, MASK_XLEN(0x40000000));
        c.li(Reg::X11, MASK_XLEN(0x8));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 260);

        // rs1_val == -2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x2;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x2));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 264);

        // rs1_val == -3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x3;  op2val:-0x8
        c.li(Reg::X10, MASK_XLEN(-0x3));
        c.li(Reg::X11, MASK_XLEN(-0x8));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 268);

        // rs1_val == -5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x5;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0x5));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 272);

        // rs1_val == -17, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x11;  op2val:-0x400001
        c.li(Reg::X10, MASK_XLEN(-0x11));
        c.li(Reg::X11, MASK_XLEN(-0x400001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 276);

        // rs1_val == -65, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x41;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x41));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 280);

        // rs1_val == -129, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x81;  op2val:0x800000
        c.li(Reg::X10, MASK_XLEN(-0x81));
        c.li(Reg::X11, MASK_XLEN(0x800000));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 284);

        // rs1_val == -16385, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x4001;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0x4001));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 288);

        // rs1_val == -131073, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x20001;  op2val:-0x4001
        c.li(Reg::X10, MASK_XLEN(-0x20001));
        c.li(Reg::X11, MASK_XLEN(-0x4001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 292);

        // rs1_val == -262145, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x40001;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x40001));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 296);

        // rs1_val == -524289, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x80001;  op2val:-0x41
        c.li(Reg::X10, MASK_XLEN(-0x80001));
        c.li(Reg::X11, MASK_XLEN(-0x41));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 300);

        // rs1_val == -4194305, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x400001;  op2val:-0x101
        c.li(Reg::X10, MASK_XLEN(-0x400001));
        c.li(Reg::X11, MASK_XLEN(-0x101));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 304);

        // rs1_val == -16777217, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x1000001;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x1000001));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 308);

        // rs1_val == -33554433, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x2000001;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x2000001));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 312);

        // rs1_val == -134217729, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x8000001;  op2val:-0x5
        c.li(Reg::X10, MASK_XLEN(-0x8000001));
        c.li(Reg::X11, MASK_XLEN(-0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 316);

        // rs1_val == -1073741825, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x40000001;  op2val:-0x40000001
        c.li(Reg::X10, MASK_XLEN(-0x40000001));
        c.li(Reg::X11, MASK_XLEN(-0x40000001));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 320);

        // rs1_val==3 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 324);

        // rs1_val==3 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 328);

        // rs1_val==3 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 332);

        // rs1_val==3 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 336);

        // rs1_val==3 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 340);

        // rs1_val==3 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 344);

        // rs1_val==3 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 348);

        // rs1_val==3 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 352);

        // rs1_val==3 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 356);

        // rs1_val==3 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 360);

        // rs1_val==3 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 364);

        // rs1_val==3 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 368);

        // rs1_val==3 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 372);

        // rs1_val==3 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 376);

        // rs1_val==3 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 380);

        // rs1_val==3 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 384);

        // rs1_val==3 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 388);

        // rs1_val==3 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 392);

        // rs1_val==3 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 396);

        // rs1_val==3 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 400);

        // rs1_val==3 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 404);

        // rs1_val==3 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 408);

        // rs1_val==1431655765 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 412);

        // rs1_val==1431655765 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 416);

        // rs1_val==1431655765 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 420);

        // rs1_val==1431655765 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 424);

        // rs1_val==1431655765 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 428);

        // rs1_val==1431655765 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 432);

        // rs1_val==1431655765 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 436);

        // rs1_val==1431655765 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 440);

        // rs1_val==1431655765 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 444);

        // rs1_val==1431655765 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 448);

        // rs1_val==1431655765 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 452);

        // rs1_val==1431655765 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 456);

        // rs1_val==1431655765 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 460);

        // rs1_val==1431655765 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 464);

        // rs1_val==1431655765 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 468);

        // rs1_val==1431655765 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 472);

        // rs1_val==1431655765 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 476);

        // rs1_val==1431655765 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 480);

        // rs1_val==1431655765 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 484);

        // rs1_val==1431655765 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 488);

        // rs1_val==1431655765 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 492);

        // rs1_val==1431655765 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 496);

        // rs1_val==-1431655766 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 500);

        // rs1_val==-1431655766 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 504);

        // rs1_val==-1431655766 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 508);

        // rs1_val==-1431655766 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 512);

        // rs1_val==-1431655766 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 516);

        // rs1_val==-1431655766 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 520);

        // rs1_val==-1431655766 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 524);

        // rs1_val==-1431655766 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 528);

        // rs1_val==-1431655766 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 532);

        // rs1_val==-1431655766 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 536);

        // rs1_val==-1431655766 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 540);

        // rs1_val==-1431655766 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 544);

        // rs1_val==-1431655766 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 548);

        // rs1_val==-1431655766 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 552);

        // rs1_val==-1431655766 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 556);

        // rs1_val==-1431655766 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 560);

        // rs1_val==-1431655766 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 564);

        // rs1_val==-1431655766 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 568);

        // rs1_val==-1431655766 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 572);

        // rs1_val==-1431655766 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 576);

        // rs1_val==-1431655766 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 580);

        // rs1_val==-1431655766 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 584);

        // rs1_val==5 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 588);

        // rs1_val==5 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 592);

        // rs1_val==5 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 596);

        // rs1_val==5 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 600);

        // rs1_val==5 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 604);

        // rs1_val==5 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 608);

        // rs1_val==5 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 612);

        // rs1_val==5 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 616);

        // rs1_val==5 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 620);

        // rs1_val==5 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 624);

        // rs1_val==5 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 628);

        // rs1_val==5 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 632);

        // rs1_val==5 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 636);

        // rs1_val==5 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 640);

        // rs1_val==5 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 644);

        // rs1_val==5 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 648);

        // rs1_val==5 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 652);

        // rs1_val==5 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 656);

        // rs1_val==5 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 660);

        // rs1_val==5 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 664);

        // rs1_val==5 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 668);

        // rs1_val==5 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 672);

        // rs1_val==858993459 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 676);

        // rs1_val==858993459 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 680);

        // rs1_val==858993459 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 684);

        // rs1_val==858993459 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 688);

        // rs1_val==858993459 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 692);

        // rs1_val==858993459 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 696);

        // rs1_val==858993459 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 700);

        // rs1_val==858993459 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 704);

        // rs1_val==858993459 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 708);

        // rs1_val==858993459 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 712);

        // rs1_val==858993459 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 716);

        // rs1_val==858993459 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 720);

        // rs1_val==858993459 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 724);

        // rs1_val==858993459 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 728);

        // rs1_val==858993459 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 732);

        // rs1_val==858993459 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 736);

        // rs1_val==858993459 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 740);

        // rs1_val==858993459 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 744);

        // rs1_val==858993459 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 748);

        // rs1_val==858993459 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 752);

        // rs1_val==858993459 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 756);

        // rs1_val==858993459 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 760);

        // rs1_val==1717986918 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 764);

        // rs1_val==1717986918 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 768);

        // rs1_val==1717986918 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 772);

        // rs1_val==1717986918 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 776);

        // rs1_val==1717986918 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 780);

        // rs1_val==1717986918 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 784);

        // rs1_val==1717986918 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 788);

        // rs1_val==1717986918 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 792);

        // rs1_val==1717986918 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 796);

        // rs1_val==1717986918 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 800);

        // rs1_val==1717986918 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 804);

        // rs1_val==1717986918 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 808);

        // rs1_val==1717986918 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 812);

        // rs1_val==1717986918 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 816);

        // rs1_val==1717986918 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 820);

        // rs1_val==1717986918 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 824);

        // rs1_val==1717986918 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 828);

        // rs1_val==1717986918 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 832);

        // rs1_val==1717986918 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 836);

        // rs1_val==1717986918 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 840);

        // rs1_val==1717986918 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 844);

        // rs1_val==1717986918 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 848);

        // rs1_val==-46340 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 852);

        // rs1_val==-46340 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 856);

        // rs1_val==-46340 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 860);

        // rs1_val==-46340 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 864);

        // rs1_val==-46340 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 868);

        // rs1_val==-46340 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 872);

        // rs1_val==-46340 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 876);

        // rs1_val==-46340 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 880);

        // rs1_val==-46340 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 884);

        // rs1_val==-46340 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 888);

        // rs1_val==-46340 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 892);

        // rs1_val==-46340 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 896);

        // rs1_val==-46340 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 900);

        // rs1_val==-46340 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 904);

        // rs1_val==-46340 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 908);

        // rs1_val==-46340 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 912);

        // rs1_val==-46340 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 916);

        // rs1_val==-46340 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 920);

        // rs1_val==-46340 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 924);

        // rs1_val==-46340 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 928);

        // rs1_val==-46340 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 932);

        // rs1_val==-46340 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 936);

        // rs1_val==46340 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 940);

        // rs1_val==46340 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 944);

        // rs1_val==46340 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 948);

        // rs1_val==46340 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 952);

        // rs1_val==46340 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 956);

        // rs1_val==46340 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 960);

        // rs1_val==46340 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 964);

        // rs1_val==46340 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 968);

        // rs1_val==46340 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 972);

        // rs1_val==46340 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 976);

        // rs1_val==46340 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 980);

        // rs1_val==46340 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 984);

        // rs1_val==46340 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 988);

        // rs1_val==46340 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 992);

        // rs1_val==46340 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 996);

        // rs1_val==46340 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1000);

        // rs1_val==46340 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1004);

        // rs1_val==46340 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1008);

        // rs1_val==46340 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1012);

        // rs1_val==46340 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1016);

        // rs1_val==46340 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1020);

        // rs1_val==46340 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1024);

        // rs1_val==2 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1028);

        // rs1_val==2 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1032);

        // rs1_val==2 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1036);

        // rs1_val==2 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1040);

        // rs1_val==2 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1044);

        // rs1_val==2 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1048);

        // rs1_val==2 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1052);

        // rs1_val==2 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1056);

        // rs1_val==2 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1060);

        // rs1_val==2 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1064);

        // rs1_val==2 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1068);

        // rs1_val==2 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1072);

        // rs1_val==2 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1076);

        // rs1_val==2 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1080);

        // rs1_val==2 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1084);

        // rs1_val==2 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1088);

        // rs1_val==2 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1092);

        // rs1_val==2 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1096);

        // rs1_val==2 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1100);

        // rs1_val==2 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1104);

        // rs1_val==2 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1108);

        // rs1_val==2 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1112);

        // rs1_val==1431655764 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1116);

        // rs1_val==1431655764 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1120);

        // rs1_val==1431655764 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1124);

        // rs1_val==1431655764 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1128);

        // rs1_val==1431655764 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1132);

        // rs1_val==1431655764 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1136);

        // rs1_val==1431655764 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1140);

        // rs1_val==1431655764 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1144);

        // rs1_val==1431655764 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1148);

        // rs1_val==1431655764 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1152);

        // rs1_val==1431655764 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1156);

        // rs1_val==1431655764 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1160);

        // rs1_val==1431655764 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1164);

        // rs1_val==1431655764 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1168);

        // rs1_val==1431655764 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1172);

        // rs1_val==1431655764 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1176);

        // rs1_val==1431655764 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1180);

        // rs1_val==1431655764 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1184);

        // rs1_val==1431655764 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1188);

        // rs1_val==1431655764 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1192);

        // rs1_val==1431655764 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1196);

        // rs1_val==1431655764 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1200);

        // rs1_val==0 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1204);

        // rs1_val==0 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1208);

        // rs1_val==0 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1212);

        // rs1_val==0 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1216);

        // rs1_val==0 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1220);

        // rs1_val==0 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1224);

        // rs1_val==0 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1228);

        // rs1_val==0 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1232);

        // rs1_val==0 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1236);

        // rs1_val==-1431655765 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1240);

        // rs1_val==-1431655765 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1244);

        // rs1_val==-1431655765 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1248);

        // rs1_val==-1431655765 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1252);

        // rs1_val==-1431655765 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1256);

        // rs1_val==-1431655765 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1260);

        // rs1_val==6 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1264);

        // rs1_val==6 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1268);

        // rs1_val==6 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1272);

        // rs1_val==6 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1276);

        // rs1_val==6 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1280);

        // rs1_val==6 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1284);

        // rs1_val==6 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1288);

        // rs1_val==6 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1292);

        // rs1_val==6 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1296);

        // rs1_val==6 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1300);

        // rs1_val==6 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1304);

        // rs1_val==6 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1308);

        // rs1_val==6 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1312);

        // rs1_val==6 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1316);

        // rs1_val==6 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1320);

        // rs1_val==6 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1324);

        // rs1_val==6 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1328);

        // rs1_val==6 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1332);

        // rs1_val==6 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1336);

        // rs1_val==6 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1340);

        // rs1_val==6 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1344);

        // rs1_val==6 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1348);

        // rs1_val==858993460 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1352);

        // rs1_val==858993460 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1356);

        // rs1_val==858993460 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1360);

        // rs1_val==858993460 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1364);

        // rs1_val==858993460 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1368);

        // rs1_val==858993460 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1372);

        // rs1_val==858993460 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1376);

        // rs1_val==858993460 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1380);

        // rs1_val==858993460 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1384);

        // rs1_val==858993460 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1388);

        // rs1_val==858993460 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1392);

        // rs1_val==858993460 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1396);

        // rs1_val==858993460 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1400);

        // rs1_val==858993460 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1404);

        // rs1_val==858993460 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1408);

        // rs1_val==858993460 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1412);

        // rs1_val==858993460 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1416);

        // rs1_val==858993460 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1420);

        // rs1_val==858993460 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1424);

        // rs1_val==858993460 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1428);

        // rs1_val==858993460 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1432);

        // rs1_val==858993460 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1436);

        // rs1_val==1717986919 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1440);

        // rs1_val==1717986919 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1444);

        // rs1_val==1717986919 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1448);

        // rs1_val==1717986919 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1452);

        // rs1_val==1717986919 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1456);

        // rs1_val==1717986919 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1460);

        // rs1_val==1717986919 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1464);

        // rs1_val==1717986919 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1468);

        // rs1_val==1717986919 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1472);

        // rs1_val==1717986919 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1476);

        // rs1_val==1717986919 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1480);

        // rs1_val==1717986919 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1484);

        // rs1_val==1717986919 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1488);

        // rs1_val==1717986919 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1492);

        // rs1_val==1717986919 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1496);

        // rs1_val==1717986919 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1500);

        // rs1_val==1717986919 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1504);

        // rs1_val==1717986919 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1508);

        // rs1_val==1717986919 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1512);

        // rs1_val==1717986919 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1516);

        // rs1_val==1717986919 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1520);

        // rs1_val==1717986919 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1524);

        // rs1_val==-46339 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1528);

        // rs1_val==-46339 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1532);

        // rs1_val==-46339 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1536);

        // rs1_val==-46339 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1540);

        // rs1_val==-46339 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1544);

        // rs1_val==-46339 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1548);

        // rs1_val==-46339 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1552);

        // rs1_val==-46339 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1556);

        // rs1_val==-46339 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1560);

        // rs1_val==-46339 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1564);

        // rs1_val==-46339 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1568);

        // rs1_val==-46339 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1572);

        // rs1_val==-46339 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1576);

        // rs1_val==-46339 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1580);

        // rs1_val==-46339 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1584);

        // rs1_val==-46339 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1588);

        // rs1_val==-46339 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1592);

        // rs1_val==-46339 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1596);

        // rs1_val==-46339 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1600);

        // rs1_val==-46339 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1604);

        // rs1_val==-46339 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1608);

        // rs1_val==-46339 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1612);

        // rs1_val==46341 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1616);

        // rs1_val==46341 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1620);

        // rs1_val==46341 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1624);

        // rs1_val==46341 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1628);

        // rs1_val==46341 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1632);

        // rs1_val==46341 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1636);

        // rs1_val==46341 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1640);

        // rs1_val==46341 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1644);

        // rs1_val==46341 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1648);

        // rs1_val==46341 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1652);

        // rs1_val==46341 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1656);

        // rs1_val==46341 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1660);

        // rs1_val==46341 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1664);

        // rs1_val==46341 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1668);

        // rs1_val==46341 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1672);

        // rs1_val==46341 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1676);

        // rs1_val==46341 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1680);

        // rs1_val==46341 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1684);

        // rs1_val==46341 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1688);

        // rs1_val==46341 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1692);

        // rs1_val==46341 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1696);

        // rs1_val==46341 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1700);

        // rs1_val==0 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1704);

        // rs1_val==0 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1708);

        // rs1_val==0 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1712);

        // rs1_val==0 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1716);

        // rs1_val==0 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1720);

        // rs1_val==0 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1724);

        // rs1_val==0 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1728);

        // rs1_val==0 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1732);

        // rs1_val==0 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1736);

        // rs1_val==0 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1740);

        // rs1_val==0 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1744);

        // rs1_val==0 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1748);

        // rs1_val==0 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1752);

        // rs1_val==4 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1756);

        // rs1_val==4 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1760);

        // rs1_val==4 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1764);

        // rs1_val==4 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1768);

        // rs1_val==4 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1772);

        // rs1_val==4 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1776);

        // rs1_val==4 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1780);

        // rs1_val==4 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1784);

        // rs1_val==4 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1788);

        // rs1_val==4 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1792);

        // rs1_val==4 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1796);

        // rs1_val==4 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1800);

        // rs1_val==4 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1804);

        // rs1_val==4 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1808);

        // rs1_val==4 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1812);

        // rs1_val==4 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1816);

        // rs1_val==4 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1820);

        // rs1_val==4 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1824);

        // rs1_val==4 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1828);

        // rs1_val==4 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1832);

        // rs1_val==4 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1836);

        // rs1_val==4 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1840);

        // rs1_val==858993458 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1844);

        // rs1_val==858993458 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1848);

        // rs1_val==858993458 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1852);

        // rs1_val==858993458 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1856);

        // rs1_val==858993458 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1860);

        // rs1_val==858993458 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1864);

        // rs1_val==858993458 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1868);

        // rs1_val==858993458 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1872);

        // rs1_val==858993458 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1876);

        // rs1_val==858993458 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1880);

        // rs1_val==858993458 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1884);

        // rs1_val==858993458 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1888);

        // rs1_val==858993458 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1892);

        // rs1_val==858993458 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1896);

        // rs1_val==858993458 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1900);

        // rs1_val==858993458 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1904);

        // rs1_val==858993458 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1908);

        // rs1_val==858993458 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1912);

        // rs1_val==858993458 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1916);

        // rs1_val==858993458 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1920);

        // rs1_val==858993458 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1924);

        // rs1_val==858993458 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1928);

        // rs1_val==1717986917 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1932);

        // rs1_val==1717986917 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1936);

        // rs1_val==1717986917 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1940);

        // rs1_val==1717986917 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1944);

        // rs1_val==1717986917 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1948);

        // rs1_val==1717986917 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1952);

        // rs1_val==1717986917 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1956);

        // rs1_val==1717986917 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1960);

        // rs1_val==1717986917 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1964);

        // rs1_val==1717986917 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1968);

        // rs1_val==1717986917 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1972);

        // rs1_val==1717986917 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1976);

        // rs1_val==1717986917 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1980);

        // rs1_val==1717986917 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1984);

        // rs1_val==1717986917 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1988);

        // rs1_val==1717986917 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1992);

        // rs1_val==1717986917 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 1996);

        // rs1_val==1717986917 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 2000);

        // rs1_val==1717986917 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 2004);

        // rs1_val==1717986917 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 2008);

        // rs1_val==1717986917 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 2012);

        // rs1_val==1717986917 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 2016);

        // rs1_val==46339 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 2020);

        // rs1_val==46339 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 2024);

        // rs1_val==46339 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 2028);

        // rs1_val==46339 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 2032);

        // rs1_val==46339 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 2036);

        // rs1_val==46339 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 2040);

        // rs1_val==46339 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 2044);

        c.li(Reg::X6, 2124);
        // rs1_val==46339 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 0);

        // rs1_val==46339 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 4);

        // rs1_val==46339 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 8);

        // rs1_val==46339 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 12);

        // rs1_val==46339 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 16);

        // rs1_val==46339 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 20);

        // rs1_val==46339 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 24);

        // rs1_val==46339 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 28);

        // rs1_val==46339 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 32);

        // rs1_val==46339 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 36);

        // rs1_val==46339 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 40);

        // rs1_val==46339 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 44);

        // rs1_val==46339 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 48);

        // rs1_val==46339 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 52);

        // rs1_val==46339 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 56);

        // rs1_val==1431655766 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 60);

        // rs1_val==1431655766 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 64);

        // rs1_val==1431655766 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 68);

        // rs1_val==1431655766 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 72);

        // rs1_val==1431655766 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 76);

        // rs1_val==1431655766 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 80);

        // rs1_val==1431655766 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 84);

        // rs1_val==1431655766 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 88);

        // rs1_val==1431655766 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 92);

        // rs1_val==1431655766 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 96);

        // rs1_val==1431655766 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 100);

        // rs1_val==1431655766 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 104);

        // rs1_val==1431655766 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 108);

        // rs1_val==1431655766 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 112);

        // rs1_val==1431655766 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 116);

        // rs1_val==1431655766 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 120);

        // rs1_val==1431655766 and rs2_val==-1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 124);

        // rs1_val==1431655766 and rs2_val==6, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 128);

        // rs1_val==1431655766 and rs2_val==858993460, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 132);

        // rs1_val==1431655766 and rs2_val==1717986919, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 136);

        // rs1_val==1431655766 and rs2_val==-46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 140);

        // rs1_val==1431655766 and rs2_val==46341, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 144);

        // rs1_val==-1431655765 and rs2_val==3, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 148);

        // rs1_val==-1431655765 and rs2_val==1431655765, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 152);

        // rs1_val==-1431655765 and rs2_val==-1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 156);

        // rs1_val==-1431655765 and rs2_val==5, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 160);

        // rs1_val==-1431655765 and rs2_val==858993459, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 164);

        // rs1_val==-1431655765 and rs2_val==1717986918, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 168);

        // rs1_val==-1431655765 and rs2_val==-46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 172);

        // rs1_val==-1431655765 and rs2_val==46340, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 176);

        // rs1_val==-1431655765 and rs2_val==2, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 180);

        // rs1_val==-1431655765 and rs2_val==1431655764, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 184);

        // rs1_val==-1431655765 and rs2_val==0, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 188);

        // rs1_val==-1431655765 and rs2_val==4, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 192);

        // rs1_val==-1431655765 and rs2_val==858993458, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 196);

        // rs1_val==-1431655765 and rs2_val==1717986917, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 200);

        // rs1_val==-1431655765 and rs2_val==46339, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 204);

        // rs1_val==-1431655765 and rs2_val==1431655766, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 208);

        // rs1_val > 0 and rs2_val < 0, rs2_val == -2049, rs1_val == 2
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x801
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x801));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 212);

        // rs1_val < 0 and rs2_val < 0, rs1_val == -268435457, rs2_val == -1025
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:-0x10000001;  op2val:-0x401
        c.li(Reg::X10, MASK_XLEN(-0x10000001));
        c.li(Reg::X11, MASK_XLEN(-0x401));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 216);

        // rs2_val == 2097152, 
        // opcode: or ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x200000
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x200000));
        c.or_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X6, 220);


                
        // End
        c.ecall();
        return c;
    }

    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> or_01::s_ScalarData;
}


// Generate google tests

TEST_F(or_01, opcode__or___op1_x8__op2_x26__dest_x26__op1val_0x100000___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[0], 1048592);
}


TEST_F(or_01, opcode__or___op1_x6__op2_x6__dest_x17__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[1], 2);
}


TEST_F(or_01, opcode__or___op1_x31__op2_x31__dest_x31__op1val__0x10000001___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[2], 4026531839);
}


TEST_F(or_01, opcode__or___op1_x27__op2_x29__dest_x27__op1val__0x801___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[3], 4294965247);
}


TEST_F(or_01, opcode__or___op1_x30__op2_x19__dest_x18__op1val__0x100001___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[4], 4293918719);
}


TEST_F(or_01, opcode__or___op1_x21__op2_x14__dest_x9__op1val_0x20000___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[5], 2147614720);
}


TEST_F(or_01, opcode__or___op1_x26__op2_x24__dest_x4__op1val__0x2001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[6], 4294959103);
}


TEST_F(or_01, opcode__or___op1_x9__op2_x8__dest_x30__op1val_0x0___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[7], 2147483647);
}


TEST_F(or_01, opcode__or___op1_x23__op2_x7__dest_x8__op1val__0x800001___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[8], 4286578687);
}


TEST_F(or_01, opcode__or___op1_x12__op2_x0__dest_x22__op1val__0x80000000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[9], 2147483648);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x30__dest_x28__op1val_0x7fffffff___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[10], 2147483647);
}


TEST_F(or_01, opcode__or___op1_x18__op2_x21__dest_x16__op1val_0x1___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[11], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x14__op2_x17__dest_x12__op1val_0x1000___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[12], 4098);
}


TEST_F(or_01, opcode__or___op1_x19__op2_x16__dest_x15__op1val__0x800001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[13], 4286578687);
}


TEST_F(or_01, opcode__or___op1_x4__op2_x2__dest_x7__op1val__0x401___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[14], 4294966271);
}


TEST_F(or_01, opcode__or___op1_x2__op2_x22__dest_x11__op1val_0x7fffffff___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[15], 2147483647);
}


TEST_F(or_01, opcode__or___op1_x28__op2_x15__dest_x25__op1val__0x201___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[16], 4294966783);
}


TEST_F(or_01, opcode__or___op1_x25__op2_x1__dest_x6__op1val_0xb504___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[17], 46340);
}


TEST_F(or_01, opcode__or___op1_x17__op2_x10__dest_x20__op1val_0x4___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[18], 516);
}


TEST_F(or_01, opcode__or___op1_x20__op2_x23__dest_x5__op1val__0x100001___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[19], 4293918719);
}


TEST_F(or_01, opcode__or___op1_x11__op2_x12__dest_x2__op1val_0x0___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[20], 2048);
}


TEST_F(or_01, opcode__or___op1_x13__op2_x20__dest_x1__op1val__0x4000001___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[21], 4227858431);
}


TEST_F(or_01, opcode__or___op1_x15__op2_x27__dest_x3__op1val_0x4___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[22], 8196);
}


TEST_F(or_01, opcode__or___op1_x22__op2_x4__dest_x21__op1val__0x10001___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[23], 4294901759);
}


TEST_F(or_01, opcode__or___op1_x7__op2_x13__dest_x19__op1val_0x80000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[24], 557056);
}


TEST_F(or_01, opcode__or___op1_x24__op2_x5__dest_x14__op1val__0x4___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[25], 4294967292);
}


TEST_F(or_01, opcode__or___op1_x16__op2_x18__dest_x10__op1val_0x55555555___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[26], 1431786837);
}


TEST_F(or_01, opcode__or___op1_x1__op2_x9__dest_x23__op1val__0xb504___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[27], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x3__op2_x11__dest_x29__op1val_0x4___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[28], 524292);
}


TEST_F(or_01, opcode__or___op1_x29__op2_x28__dest_x13__op1val__0x1001___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[29], 4294963199);
}


TEST_F(or_01, opcode__or___op1_x5__op2_x3__dest_x0__op1val_0x33333332___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[30], 0);
}


TEST_F(or_01, opcode__or___op1_x0__op2_x25__dest_x24__op1val_0x0___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[31], 8388608);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[32], 16777280);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[33], 4294967287);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[34], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[35], 150994944);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x6___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[36], 4294967290);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[37], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[38], 1082130432);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[39], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[40], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[41], 4294967291);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[42], 4294967287);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[43], 4294967279);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[44], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[45], 4294967231);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[46], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[47], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[48], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[49], 4294963199);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[50], 4294959103);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[51], 4294950911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[52], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[53], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[54], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[55], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[56], 4294443007);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[57], 4292870143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[58], 4290772991);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[59], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[60], 4278190079);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[61], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[62], 4227858431);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[63], 4160749567);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x40000000___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[64], 4026531839);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[65], 3758096383);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[66], 3221225471);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[67], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[68], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val__0x7) {
  ASSERT_EQ(getScalarWordData()[69], 4294967289);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val__0x7) {
  ASSERT_EQ(getScalarWordData()[70], 4294967289);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[71], 4227858431);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[72], 2863312554);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[73], 4292870143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[74], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val__0x6) {
  ASSERT_EQ(getScalarWordData()[75], 4294967290);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[76], 34816);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[77], 4278190079);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[78], 2097152);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[79], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val_0x3fffffff) {
  ASSERT_EQ(getScalarWordData()[80], 1073741823);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[81], 4294443007);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[82], 134217760);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[83], 4294959103);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[84], 1073741832);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[85], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val__0x8) {
  ASSERT_EQ(getScalarWordData()[86], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[87], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[88], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[89], 4294967231);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[90], 4294967167);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[91], 4294950911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[92], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[93], 4294705151);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[94], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[95], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[96], 4278190079);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[97], 4261412863);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[98], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[99], 3221225471);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[100], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[101], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[102], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[103], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[104], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[105], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[106], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[107], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[108], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[109], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[110], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[111], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[112], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[113], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[114], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[115], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[116], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[117], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[118], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[119], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[120], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[121], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[122], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[123], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[124], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[125], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[126], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[127], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[128], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[129], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[130], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[131], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[132], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[133], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[134], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[135], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[136], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[137], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[138], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[139], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[140], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[141], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[142], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[143], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[144], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[145], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[146], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[147], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[148], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[149], 4008636142);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[150], 4294961918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[151], 2863316910);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[152], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[153], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[154], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[155], 2863311534);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[156], 3149642682);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[157], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[158], 2863316907);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[159], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[160], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[161], 2863311534);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[162], 3149642686);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[163], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[164], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[165], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[166], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[167], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[168], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[169], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[170], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[171], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[172], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[173], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[174], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[175], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[176], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[177], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[178], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[179], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[180], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[181], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[182], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[183], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[184], 858993461);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[185], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[186], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[187], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[188], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[189], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[190], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[191], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[192], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[193], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[194], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[195], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[196], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[197], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[198], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[199], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[200], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[201], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[202], 859027251);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[203], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[204], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[205], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[206], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[207], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[208], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[209], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[210], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[211], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[212], 4008636142);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[213], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[214], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[215], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[216], 4294930174);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[217], 1718024038);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[218], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[219], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[220], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[221], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[222], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[223], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[224], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[225], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[226], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[227], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[228], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[229], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[230], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[231], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[232], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[233], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[234], 4294961918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[235], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[236], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[237], 4294930174);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[238], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[239], 4294967292);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[240], 4294920958);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[241], 4294926332);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[242], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[243], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[244], 4294933502);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[245], 4294930173);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[246], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[247], 4294926334);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[248], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[249], 4294920958);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[250], 4294933500);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[251], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[252], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[253], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[254], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[255], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[256], 2863316910);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[257], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[258], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[259], 1718024038);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[260], 4294967292);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[261], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[262], 46342);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[263], 1431696724);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[264], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[265], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[266], 859027254);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[267], 1718024037);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[268], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[269], 1431696726);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[270], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[271], 46342);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[272], 859027252);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[273], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[274], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[275], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[276], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[277], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[278], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[279], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[280], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[281], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[282], 4294920958);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[283], 46342);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[284], 2);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[285], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[286], 2);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[287], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[288], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[289], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[290], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[291], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[292], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[293], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[294], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[295], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[296], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[297], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[298], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[299], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[300], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[301], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[302], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[303], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[304], 4294926332);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[305], 1431696724);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[306], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[307], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[308], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[309], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[310], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[311], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[312], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[313], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[314], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[315], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[316], 2004318068);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[317], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[318], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[319], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[320], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[321], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[322], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[323], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[324], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[325], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[326], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[327], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[328], 2);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[329], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[330], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[331], 3149642687);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[332], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[333], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[334], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[335], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[336], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[337], 2863311534);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[338], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[339], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[340], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[341], 4294920958);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[342], 46342);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[343], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[344], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[345], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[346], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[347], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[348], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[349], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[350], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[351], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[352], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[353], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[354], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[355], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[356], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[357], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[358], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[359], 3149642686);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[360], 858993461);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[361], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[362], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[363], 4294933500);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[364], 859027252);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[365], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[366], 2004318068);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[367], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[368], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[369], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[370], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[371], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[372], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[373], 3149642687);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[374], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[375], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[376], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[377], 4294933501);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[378], 859027253);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[379], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[380], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[381], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[382], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[383], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[384], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[385], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[386], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[387], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[388], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[389], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[390], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[391], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[392], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[393], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[394], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[395], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[396], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[397], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[398], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[399], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[400], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[401], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[402], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[403], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[404], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[405], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[406], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[407], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[408], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[409], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[410], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[411], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[412], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[413], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[414], 4294930173);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[415], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[416], 4294926335);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[417], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[418], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[419], 4294933501);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[420], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[421], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[422], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[423], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[424], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[425], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[426], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[427], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[428], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[429], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[430], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[431], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[432], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[433], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[434], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[435], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[436], 1718024037);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[437], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[438], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[439], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[440], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[441], 859027253);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[442], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[443], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[444], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[445], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[446], 0);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[447], 4);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[448], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[449], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[450], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[451], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[452], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[453], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[454], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[455], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[456], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[457], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[458], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[459], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[460], 2863311534);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[461], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[462], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[463], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[464], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[465], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[466], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[467], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[468], 4);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[469], 4);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[470], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[471], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[472], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[473], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[474], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[475], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[476], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[477], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[478], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[479], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[480], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[481], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[482], 3149642682);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[483], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[484], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[485], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[486], 4294933502);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[487], 859027254);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[488], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[489], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[490], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[491], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[492], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[493], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[494], 859027251);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[495], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[496], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[497], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[498], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[499], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[500], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[501], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[502], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[503], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[504], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[505], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[506], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[507], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[508], 4294930173);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[509], 1718024037);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[510], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[511], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[512], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[513], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[514], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[515], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[516], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[517], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[518], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[519], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[520], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[521], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[522], 4294930173);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[523], 1718024037);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[524], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[525], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[526], 2863316907);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[527], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[528], 859027251);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[529], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[530], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[531], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[532], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[533], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[534], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[535], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[536], 859027251);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[537], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[538], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[539], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[540], 2863316907);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[541], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[542], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[543], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[544], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[545], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[546], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[547], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[548], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[549], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[550], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[551], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[552], 4294926334);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[553], 1431696726);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[554], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[555], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[556], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[557], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[558], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[559], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[560], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[561], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[562], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[563], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[564], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[565], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[566], 4294926335);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[567], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[568], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[569], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[570], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[571], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[572], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[573], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[574], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[575], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[576], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[577], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[578], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[579], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[580], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[581], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[582], 2863316907);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[583], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[584], 4294965247);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[585], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[586], 858993458);
}

