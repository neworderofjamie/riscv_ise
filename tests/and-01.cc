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
class and_01 : public testing::Test 
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
        
        c.li(Reg::X6, 0);
        // rs2 == rd != rs1, rs1==x24, rs2==x25, rd==x25, rs1_val > 0 and rs2_val > 0, rs1_val == 16384, rs1_val != rs2_val
        // opcode: and ; op1:x24; op2:x25; dest:x25; op1val:0x4000;  op2val:0x7
        c.li(Reg::X24, MASK_XLEN(0x4000));
        c.li(Reg::X25, MASK_XLEN(0x7));
        c.and_(Reg::X25, Reg::X24, Reg::X25);
        c.sw(Reg::X25, Reg::X6, 0);

        // rs1 == rs2 != rd, rs1==x3, rs2==x3, rd==x18, rs1_val > 0 and rs2_val < 0, rs1_val == 2048, rs2_val == -1048577
        // opcode: and ; op1:x3; op2:x3; dest:x18; op1val:0x800;  op2val:0x800
        c.li(Reg::X3, MASK_XLEN(0x800));
        c.li(Reg::X3, MASK_XLEN(0x800));
        c.and_(Reg::X18, Reg::X3, Reg::X3);
        c.sw(Reg::X18, Reg::X6, 4);

        // rs1 == rs2 == rd, rs1==x19, rs2==x19, rd==x19, rs1_val < 0 and rs2_val < 0, rs1_val == -3
        // opcode: and ; op1:x19; op2:x19; dest:x19; op1val:-0x3;  op2val:-0x3
        c.li(Reg::X19, MASK_XLEN(-0x3));
        c.li(Reg::X19, MASK_XLEN(-0x3));
        c.and_(Reg::X19, Reg::X19, Reg::X19);
        c.sw(Reg::X19, Reg::X6, 8);

        // rs1 == rd != rs2, rs1==x5, rs2==x14, rd==x5, rs1_val < 0 and rs2_val > 0, rs2_val == (2**(xlen-1)-1), rs2_val == 2147483647
        // opcode: and ; op1:x5; op2:x14; dest:x5; op1val:-0x1;  op2val:0x7fffffff
        c.li(Reg::X5, MASK_XLEN(-0x1));
        c.li(Reg::X14, MASK_XLEN(0x7fffffff));
        c.and_(Reg::X5, Reg::X5, Reg::X14);
        c.sw(Reg::X5, Reg::X6, 12);

        // rs1 != rs2  and rs1 != rd and rs2 != rd, rs1==x23, rs2==x16, rd==x20, rs1_val == rs2_val, rs1_val==5 and rs2_val==5
        // opcode: and ; op1:x23; op2:x16; dest:x20; op1val:0x5;  op2val:0x5
        c.li(Reg::X23, MASK_XLEN(0x5));
        c.li(Reg::X16, MASK_XLEN(0x5));
        c.and_(Reg::X20, Reg::X23, Reg::X16);
        c.sw(Reg::X20, Reg::X6, 16);

        // rs1==x20, rs2==x2, rd==x30, rs2_val == (-2**(xlen-1)), rs2_val == -2147483648, rs1_val == 2
        // opcode: and ; op1:x20; op2:x2; dest:x30; op1val:0x2;  op2val:-0x80000000
        c.li(Reg::X20, MASK_XLEN(0x2));
        c.li(Reg::X2, MASK_XLEN(-0x80000000));
        c.and_(Reg::X30, Reg::X20, Reg::X2);
        c.sw(Reg::X30, Reg::X6, 20);

        // rs1==x7, rs2==x24, rd==x13, rs2_val == 0, rs1_val==858993459 and rs2_val==0
        // opcode: and ; op1:x7; op2:x24; dest:x13; op1val:0x33333333;  op2val:0x0
        c.li(Reg::X7, MASK_XLEN(0x33333333));
        c.li(Reg::X24, MASK_XLEN(0x0));
        c.and_(Reg::X13, Reg::X7, Reg::X24);
        c.sw(Reg::X13, Reg::X6, 24);

        // rs1==x30, rs2==x27, rd==x10, rs2_val == 1, rs1_val == -1073741825
        // opcode: and ; op1:x30; op2:x27; dest:x10; op1val:-0x40000001;  op2val:0x1
        c.li(Reg::X30, MASK_XLEN(-0x40000001));
        c.li(Reg::X27, MASK_XLEN(0x1));
        c.and_(Reg::X10, Reg::X30, Reg::X27);
        c.sw(Reg::X10, Reg::X6, 28);

        // rs1==x28, rs2==x18, rd==x22, rs1_val == (-2**(xlen-1)), rs2_val == 2048, rs1_val == -2147483648
        // opcode: and ; op1:x28; op2:x18; dest:x22; op1val:-0x80000000;  op2val:0x800
        c.li(Reg::X28, MASK_XLEN(-0x80000000));
        c.li(Reg::X18, MASK_XLEN(0x800));
        c.and_(Reg::X22, Reg::X28, Reg::X18);
        c.sw(Reg::X22, Reg::X6, 32);

        // rs1==x2, rs2==x15, rd==x0, rs1_val == 0, rs2_val == 512
        // opcode: and ; op1:x2; op2:x15; dest:x0; op1val:0x0;  op2val:0x200
        c.li(Reg::X2, MASK_XLEN(0x0));
        c.li(Reg::X15, MASK_XLEN(0x200));
        c.and_(Reg::X0, Reg::X2, Reg::X15);
        c.sw(Reg::X0, Reg::X6, 36);

        // rs1==x25, rs2==x26, rd==x12, rs1_val == (2**(xlen-1)-1), rs2_val == 1431655765, rs1_val == 2147483647
        // opcode: and ; op1:x25; op2:x26; dest:x12; op1val:0x7fffffff;  op2val:0x55555555
        c.li(Reg::X25, MASK_XLEN(0x7fffffff));
        c.li(Reg::X26, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X25, Reg::X26);
        c.sw(Reg::X12, Reg::X6, 40);

        // rs1==x1, rs2==x31, rd==x2, rs1_val == 1, 
        // opcode: and ; op1:x1; op2:x31; dest:x2; op1val:0x1;  op2val:0x55555554
        c.li(Reg::X1, MASK_XLEN(0x1));
        c.li(Reg::X31, MASK_XLEN(0x55555554));
        c.and_(Reg::X2, Reg::X1, Reg::X31);
        c.sw(Reg::X2, Reg::X6, 44);

        // rs1==x27, rs2==x11, rd==x14, rs2_val == 2, rs1_val == 262144
        // opcode: and ; op1:x27; op2:x11; dest:x14; op1val:0x40000;  op2val:0x2
        c.li(Reg::X27, MASK_XLEN(0x40000));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X14, Reg::X27, Reg::X11);
        c.sw(Reg::X14, Reg::X6, 48);

        // rs1==x31, rs2==x23, rd==x4, rs2_val == 4, rs1_val == -131073
        // opcode: and ; op1:x31; op2:x23; dest:x4; op1val:-0x20001;  op2val:0x4
        c.li(Reg::X31, MASK_XLEN(-0x20001));
        c.li(Reg::X23, MASK_XLEN(0x4));
        c.and_(Reg::X4, Reg::X31, Reg::X23);
        c.sw(Reg::X4, Reg::X6, 52);

        // rs1==x21, rs2==x9, rd==x27, rs2_val == 8, 
        // opcode: and ; op1:x21; op2:x9; dest:x27; op1val:-0x55555555;  op2val:0x8
        c.li(Reg::X21, MASK_XLEN(-0x55555555));
        c.li(Reg::X9, MASK_XLEN(0x8));
        c.and_(Reg::X27, Reg::X21, Reg::X9);
        c.sw(Reg::X27, Reg::X6, 56);

        // rs1==x26, rs2==x7, rd==x23, rs2_val == 16, rs1_val == 1024
        // opcode: and ; op1:x26; op2:x7; dest:x23; op1val:0x400;  op2val:0x10
        c.li(Reg::X26, MASK_XLEN(0x400));
        c.li(Reg::X7, MASK_XLEN(0x10));
        c.and_(Reg::X23, Reg::X26, Reg::X7);
        c.sw(Reg::X23, Reg::X6, 60);

        // rs1==x9, rs2==x20, rd==x24, rs2_val == 32, 
        // opcode: and ; op1:x9; op2:x20; dest:x24; op1val:-0x8;  op2val:0x20
        c.li(Reg::X9, MASK_XLEN(-0x8));
        c.li(Reg::X20, MASK_XLEN(0x20));
        c.and_(Reg::X24, Reg::X9, Reg::X20);
        c.sw(Reg::X24, Reg::X6, 64);

        // rs1==x15, rs2==x13, rd==x26, rs2_val == 64, rs1_val == -257
        // opcode: and ; op1:x15; op2:x13; dest:x26; op1val:-0x101;  op2val:0x40
        c.li(Reg::X15, MASK_XLEN(-0x101));
        c.li(Reg::X13, MASK_XLEN(0x40));
        c.and_(Reg::X26, Reg::X15, Reg::X13);
        c.sw(Reg::X26, Reg::X6, 68);

        // rs1==x12, rs2==x4, rd==x17, rs2_val == 128, rs1_val == -33554433
        // opcode: and ; op1:x12; op2:x4; dest:x17; op1val:-0x2000001;  op2val:0x80
        c.li(Reg::X12, MASK_XLEN(-0x2000001));
        c.li(Reg::X4, MASK_XLEN(0x80));
        c.and_(Reg::X17, Reg::X12, Reg::X4);
        c.sw(Reg::X17, Reg::X6, 72);

        c.li(Reg::X2, 76);
        // rs1==x4, rs2==x17, rd==x8, rs2_val == 256, 
        // opcode: and ; op1:x4; op2:x17; dest:x8; op1val:0x66666665;  op2val:0x100
        c.li(Reg::X4, MASK_XLEN(0x66666665));
        c.li(Reg::X17, MASK_XLEN(0x100));
        c.and_(Reg::X8, Reg::X4, Reg::X17);
        c.sw(Reg::X8, Reg::X2, 0);

        // rs1==x13, rs2==x22, rd==x1, rs2_val == 1024, rs1_val == -129
        // opcode: and ; op1:x13; op2:x22; dest:x1; op1val:-0x81;  op2val:0x400
        c.li(Reg::X13, MASK_XLEN(-0x81));
        c.li(Reg::X22, MASK_XLEN(0x400));
        c.and_(Reg::X1, Reg::X13, Reg::X22);
        c.sw(Reg::X1, Reg::X2, 4);

        // rs1==x8, rs2==x12, rd==x7, rs2_val == 4096, rs1_val == 4
        // opcode: and ; op1:x8; op2:x12; dest:x7; op1val:0x4;  op2val:0x1000
        c.li(Reg::X8, MASK_XLEN(0x4));
        c.li(Reg::X12, MASK_XLEN(0x1000));
        c.and_(Reg::X7, Reg::X8, Reg::X12);
        c.sw(Reg::X7, Reg::X2, 8);

        // rs1==x11, rs2==x5, rd==x21, rs2_val == 8192, rs1_val == -32769
        // opcode: and ; op1:x11; op2:x5; dest:x21; op1val:-0x8001;  op2val:0x2000
        c.li(Reg::X11, MASK_XLEN(-0x8001));
        c.li(Reg::X5, MASK_XLEN(0x2000));
        c.and_(Reg::X21, Reg::X11, Reg::X5);
        c.sw(Reg::X21, Reg::X2, 12);

        // rs1==x14, rs2==x21, rd==x29, rs2_val == 16384, rs1_val == 64
        // opcode: and ; op1:x14; op2:x21; dest:x29; op1val:0x40;  op2val:0x4000
        c.li(Reg::X14, MASK_XLEN(0x40));
        c.li(Reg::X21, MASK_XLEN(0x4000));
        c.and_(Reg::X29, Reg::X14, Reg::X21);
        c.sw(Reg::X29, Reg::X2, 16);

        // rs1==x17, rs2==x10, rd==x11, rs2_val == 32768, 
        // opcode: and ; op1:x17; op2:x10; dest:x11; op1val:0x5;  op2val:0x8000
        c.li(Reg::X17, MASK_XLEN(0x5));
        c.li(Reg::X10, MASK_XLEN(0x8000));
        c.and_(Reg::X11, Reg::X17, Reg::X10);
        c.sw(Reg::X11, Reg::X2, 20);

        // rs1==x22, rs2==x6, rd==x3, rs2_val == 65536, rs1_val == 8388608
        // opcode: and ; op1:x22; op2:x6; dest:x3; op1val:0x800000;  op2val:0x10000
        c.li(Reg::X22, MASK_XLEN(0x800000));
        c.li(Reg::X6, MASK_XLEN(0x10000));
        c.and_(Reg::X3, Reg::X22, Reg::X6);
        c.sw(Reg::X3, Reg::X2, 24);

        // rs1==x0, rs2==x29, rd==x9, rs2_val == 131072, 
        // opcode: and ; op1:x0; op2:x29; dest:x9; op1val:0x0;  op2val:0x20000
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.li(Reg::X29, MASK_XLEN(0x20000));
        c.and_(Reg::X9, Reg::X0, Reg::X29);
        c.sw(Reg::X9, Reg::X2, 28);

        // rs1==x29, rs2==x30, rd==x28, rs2_val == 262144, 
        // opcode: and ; op1:x29; op2:x30; dest:x28; op1val:0x5;  op2val:0x40000
        c.li(Reg::X29, MASK_XLEN(0x5));
        c.li(Reg::X30, MASK_XLEN(0x40000));
        c.and_(Reg::X28, Reg::X29, Reg::X30);
        c.sw(Reg::X28, Reg::X2, 32);

        // rs1==x18, rs2==x1, rd==x6, rs2_val == 524288, rs1_val == 65536
        // opcode: and ; op1:x18; op2:x1; dest:x6; op1val:0x10000;  op2val:0x80000
        c.li(Reg::X18, MASK_XLEN(0x10000));
        c.li(Reg::X1, MASK_XLEN(0x80000));
        c.and_(Reg::X6, Reg::X18, Reg::X1);
        c.sw(Reg::X6, Reg::X2, 36);

        // rs1==x6, rs2==x0, rd==x31, rs2_val == 1048576, rs1_val == 134217728
        // opcode: and ; op1:x6; op2:x0; dest:x31; op1val:0x8000000;  op2val:0x0
        c.li(Reg::X6, MASK_XLEN(0x8000000));
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.and_(Reg::X31, Reg::X6, Reg::X0);
        c.sw(Reg::X31, Reg::X2, 40);

        // rs1==x16, rs2==x8, rd==x15, rs2_val == 2097152, 
        // opcode: and ; op1:x16; op2:x8; dest:x15; op1val:-0x3;  op2val:0x200000
        c.li(Reg::X16, MASK_XLEN(-0x3));
        c.li(Reg::X8, MASK_XLEN(0x200000));
        c.and_(Reg::X15, Reg::X16, Reg::X8);
        c.sw(Reg::X15, Reg::X2, 44);

        // rs1==x10, rs2==x28, rd==x16, rs2_val == 4194304, 
        // opcode: and ; op1:x10; op2:x28; dest:x16; op1val:0x800;  op2val:0x400000
        c.li(Reg::X10, MASK_XLEN(0x800));
        c.li(Reg::X28, MASK_XLEN(0x400000));
        c.and_(Reg::X16, Reg::X10, Reg::X28);
        c.sw(Reg::X16, Reg::X2, 48);

        // rs2_val == 8388608, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x800000
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x800000));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 52);

        // rs2_val == 16777216, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x1000000
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x1000000));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 56);

        // rs2_val == 33554432, rs1_val == 1431655765
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x2000000
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x2000000));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 60);

        // rs2_val == 67108864, rs1_val == 131072
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x20000;  op2val:0x4000000
        c.li(Reg::X10, MASK_XLEN(0x20000));
        c.li(Reg::X11, MASK_XLEN(0x4000000));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 64);

        // rs2_val == 134217728, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x8001;  op2val:0x8000000
        c.li(Reg::X10, MASK_XLEN(-0x8001));
        c.li(Reg::X11, MASK_XLEN(0x8000000));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 68);

        // rs2_val == 268435456, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x10000000
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x10000000));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 72);

        // rs2_val == 536870912, rs1_val == -65537
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x10001;  op2val:0x20000000
        c.li(Reg::X10, MASK_XLEN(-0x10001));
        c.li(Reg::X11, MASK_XLEN(0x20000000));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 76);

        // rs2_val == 1073741824, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x8000000;  op2val:0x40000000
        c.li(Reg::X10, MASK_XLEN(0x8000000));
        c.li(Reg::X11, MASK_XLEN(0x40000000));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 80);

        // rs2_val == -2, rs1_val == -513
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x201;  op2val:-0x2
        c.li(Reg::X10, MASK_XLEN(-0x201));
        c.li(Reg::X11, MASK_XLEN(-0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 84);

        // rs2_val == -3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x40000001;  op2val:-0x3
        c.li(Reg::X10, MASK_XLEN(-0x40000001));
        c.li(Reg::X11, MASK_XLEN(-0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 88);

        // rs2_val == -5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x800000;  op2val:-0x5
        c.li(Reg::X10, MASK_XLEN(0x800000));
        c.li(Reg::X11, MASK_XLEN(-0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 92);

        // rs2_val == -9, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x201;  op2val:-0x9
        c.li(Reg::X10, MASK_XLEN(-0x201));
        c.li(Reg::X11, MASK_XLEN(-0x9));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 96);

        // rs2_val == -17, rs1_val == -134217729
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x8000001;  op2val:-0x11
        c.li(Reg::X10, MASK_XLEN(-0x8000001));
        c.li(Reg::X11, MASK_XLEN(-0x11));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 100);

        // rs2_val == -33, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x21
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x21));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 104);

        // rs2_val == -65, rs1_val == 128
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x80;  op2val:-0x41
        c.li(Reg::X10, MASK_XLEN(0x80));
        c.li(Reg::X11, MASK_XLEN(-0x41));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 108);

        // rs2_val == -129, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x7;  op2val:-0x81
        c.li(Reg::X10, MASK_XLEN(-0x7));
        c.li(Reg::X11, MASK_XLEN(-0x81));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 112);

        // rs2_val == -257, rs1_val == -9
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x9;  op2val:-0x101
        c.li(Reg::X10, MASK_XLEN(-0x9));
        c.li(Reg::X11, MASK_XLEN(-0x101));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 116);

        // rs2_val == -513, rs1_val == -1431655766
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x201
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x201));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 120);

        // rs2_val == -1025, rs1_val == -8388609
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x800001;  op2val:-0x401
        c.li(Reg::X10, MASK_XLEN(-0x800001));
        c.li(Reg::X11, MASK_XLEN(-0x401));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 124);

        // rs2_val == -2049, rs1_val == -1048577
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x100001;  op2val:-0x801
        c.li(Reg::X10, MASK_XLEN(-0x100001));
        c.li(Reg::X11, MASK_XLEN(-0x801));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 128);

        // rs2_val == -4097, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x400;  op2val:-0x1001
        c.li(Reg::X10, MASK_XLEN(0x400));
        c.li(Reg::X11, MASK_XLEN(-0x1001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 132);

        // rs2_val == -8193, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x2001
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0x2001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 136);

        // rs2_val == -16385, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x7;  op2val:-0x4001
        c.li(Reg::X10, MASK_XLEN(0x7));
        c.li(Reg::X11, MASK_XLEN(-0x4001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 140);

        // rs2_val == -32769, rs1_val == 33554432
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2000000;  op2val:-0x8001
        c.li(Reg::X10, MASK_XLEN(0x2000000));
        c.li(Reg::X11, MASK_XLEN(-0x8001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 144);

        // rs2_val == -65537, rs1_val == 1073741824
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x40000000;  op2val:-0x10001
        c.li(Reg::X10, MASK_XLEN(0x40000000));
        c.li(Reg::X11, MASK_XLEN(-0x10001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 148);

        // rs2_val == -131073, rs1_val == -5
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x5;  op2val:-0x20001
        c.li(Reg::X10, MASK_XLEN(-0x5));
        c.li(Reg::X11, MASK_XLEN(-0x20001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 152);

        // rs2_val == -262145, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x40000;  op2val:-0x40001
        c.li(Reg::X10, MASK_XLEN(0x40000));
        c.li(Reg::X11, MASK_XLEN(-0x40001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 156);

        // rs2_val == -524289, rs1_val == 16
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x10;  op2val:-0x80001
        c.li(Reg::X10, MASK_XLEN(0x10));
        c.li(Reg::X11, MASK_XLEN(-0x80001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 160);

        // rs2_val == -2097153, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x400;  op2val:-0x200001
        c.li(Reg::X10, MASK_XLEN(0x400));
        c.li(Reg::X11, MASK_XLEN(-0x200001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 164);

        // rs2_val == -4194305, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x1;  op2val:-0x400001
        c.li(Reg::X10, MASK_XLEN(-0x1));
        c.li(Reg::X11, MASK_XLEN(-0x400001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 168);

        // rs2_val == -8388609, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x20000;  op2val:-0x800001
        c.li(Reg::X10, MASK_XLEN(0x20000));
        c.li(Reg::X11, MASK_XLEN(-0x800001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 172);

        // rs2_val == -16777217, rs1_val == -262145
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x40001;  op2val:-0x1000001
        c.li(Reg::X10, MASK_XLEN(-0x40001));
        c.li(Reg::X11, MASK_XLEN(-0x1000001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 176);

        // rs2_val == -33554433, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x2000001
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0x2000001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 180);

        // rs2_val == -67108865, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x4000001
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x4000001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 184);

        // rs2_val == -134217729, rs1_val == -17
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x11;  op2val:-0x8000001
        c.li(Reg::X10, MASK_XLEN(-0x11));
        c.li(Reg::X11, MASK_XLEN(-0x8000001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 188);

        // rs2_val == -268435457, rs1_val == -536870913
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x20000001;  op2val:-0x10000001
        c.li(Reg::X10, MASK_XLEN(-0x20000001));
        c.li(Reg::X11, MASK_XLEN(-0x10000001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 192);

        // rs2_val == -536870913, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x20000001
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x20000001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 196);

        // rs2_val == -1073741825, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x80000000;  op2val:-0x40000001
        c.li(Reg::X10, MASK_XLEN(-0x80000000));
        c.li(Reg::X11, MASK_XLEN(-0x40000001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 200);

        // rs2_val == -1431655766, rs1_val==-46340 and rs2_val==-1431655766
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 204);

        // rs1_val == 8, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x8;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x8));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 208);

        // rs1_val == 32, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x20;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x20));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 212);

        // rs1_val == 256, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x100;  op2val:-0x101
        c.li(Reg::X10, MASK_XLEN(0x100));
        c.li(Reg::X11, MASK_XLEN(-0x101));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 216);

        // rs1_val == 512, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x200;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x200));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 220);

        // rs1_val == 4096, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x1000;  op2val:0x8
        c.li(Reg::X10, MASK_XLEN(0x1000));
        c.li(Reg::X11, MASK_XLEN(0x8));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 224);

        // rs1_val == 8192, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2000;  op2val:0x40000
        c.li(Reg::X10, MASK_XLEN(0x2000));
        c.li(Reg::X11, MASK_XLEN(0x40000));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 228);

        // rs1_val == 32768, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x8000;  op2val:-0x4
        c.li(Reg::X10, MASK_XLEN(0x8000));
        c.li(Reg::X11, MASK_XLEN(-0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 232);

        // rs1_val == 524288, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x80000;  op2val:0x3fffffff
        c.li(Reg::X10, MASK_XLEN(0x80000));
        c.li(Reg::X11, MASK_XLEN(0x3fffffff));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 236);

        // rs1_val == 1048576, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x100000;  op2val:-0x2001
        c.li(Reg::X10, MASK_XLEN(0x100000));
        c.li(Reg::X11, MASK_XLEN(-0x2001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 240);

        // rs1_val == 2097152, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x200000;  op2val:0x8000
        c.li(Reg::X10, MASK_XLEN(0x200000));
        c.li(Reg::X11, MASK_XLEN(0x8000));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 244);

        // rs1_val == 4194304, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x400000;  op2val:-0x11
        c.li(Reg::X10, MASK_XLEN(0x400000));
        c.li(Reg::X11, MASK_XLEN(-0x11));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 248);

        // rs1_val == 16777216, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x1000000;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x1000000));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 252);

        // rs1_val == 67108864, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4000000;  op2val:0x20000
        c.li(Reg::X10, MASK_XLEN(0x4000000));
        c.li(Reg::X11, MASK_XLEN(0x20000));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 256);

        // rs1_val == 268435456, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x10000000;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x10000000));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 260);

        // rs1_val == 536870912, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x20000000;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x20000000));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 264);

        // rs1_val == -2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x2;  op2val:-0x10000001
        c.li(Reg::X10, MASK_XLEN(-0x2));
        c.li(Reg::X11, MASK_XLEN(-0x10000001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 268);

        // rs1_val == -33, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x21;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0x21));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 272);

        // rs1_val == -65, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x41;  op2val:-0x400001
        c.li(Reg::X10, MASK_XLEN(-0x41));
        c.li(Reg::X11, MASK_XLEN(-0x400001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 276);

        // rs1_val == -1025, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x401;  op2val:-0x4001
        c.li(Reg::X10, MASK_XLEN(-0x401));
        c.li(Reg::X11, MASK_XLEN(-0x4001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 280);

        // rs1_val == -2049, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x801;  op2val:-0x7
        c.li(Reg::X10, MASK_XLEN(-0x801));
        c.li(Reg::X11, MASK_XLEN(-0x7));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 284);

        // rs1_val == -4097, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x1001;  op2val:-0x81
        c.li(Reg::X10, MASK_XLEN(-0x1001));
        c.li(Reg::X11, MASK_XLEN(-0x81));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 288);

        // rs1_val == -8193, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x2001;  op2val:-0x5
        c.li(Reg::X10, MASK_XLEN(-0x2001));
        c.li(Reg::X11, MASK_XLEN(-0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 292);

        // rs1_val == -16385, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x4001;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x4001));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 296);

        // rs1_val == -524289, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x80001;  op2val:-0x200001
        c.li(Reg::X10, MASK_XLEN(-0x80001));
        c.li(Reg::X11, MASK_XLEN(-0x200001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 300);

        // rs1_val == -2097153, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x200001;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x200001));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 304);

        // rs1_val == -4194305, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x400001;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0x400001));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 308);

        // rs1_val == -16777217, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x1000001;  op2val:0x7fffffff
        c.li(Reg::X10, MASK_XLEN(-0x1000001));
        c.li(Reg::X11, MASK_XLEN(0x7fffffff));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 312);

        // rs1_val == -67108865, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x4000001;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0x4000001));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 316);

        // rs1_val == -268435457, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x10000001;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0x10000001));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 320);

        // rs1_val==3 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 324);

        // rs1_val==3 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 328);

        // rs1_val==3 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 332);

        // rs1_val==3 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 336);

        // rs1_val==3 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 340);

        // rs1_val==3 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 344);

        // rs1_val==3 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 348);

        // rs1_val==3 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 352);

        // rs1_val==3 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 356);

        // rs1_val==3 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 360);

        // rs1_val==3 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 364);

        // rs1_val==3 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 368);

        // rs1_val==3 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 372);

        // rs1_val==3 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 376);

        // rs1_val==3 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 380);

        // rs1_val==3 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 384);

        // rs1_val==3 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 388);

        // rs1_val==3 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 392);

        // rs1_val==3 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 396);

        // rs1_val==3 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 400);

        // rs1_val==3 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 404);

        // rs1_val==3 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x3;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 408);

        // rs1_val==1431655765 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 412);

        // rs1_val==1431655765 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 416);

        // rs1_val==1431655765 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 420);

        // rs1_val==1431655765 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 424);

        // rs1_val==1431655765 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 428);

        // rs1_val==1431655765 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 432);

        // rs1_val==1431655765 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 436);

        // rs1_val==1431655765 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 440);

        // rs1_val==1431655765 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 444);

        // rs1_val==1431655765 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 448);

        // rs1_val==1431655765 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 452);

        // rs1_val==1431655765 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 456);

        // rs1_val==1431655765 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 460);

        // rs1_val==1431655765 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 464);

        // rs1_val==1431655765 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 468);

        // rs1_val==1431655765 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 472);

        // rs1_val==1431655765 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 476);

        // rs1_val==1431655765 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 480);

        // rs1_val==1431655765 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 484);

        // rs1_val==1431655765 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 488);

        // rs1_val==1431655765 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 492);

        // rs1_val==1431655765 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555555;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 496);

        // rs1_val==-1431655766 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 500);

        // rs1_val==-1431655766 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 504);

        // rs1_val==-1431655766 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 508);

        // rs1_val==-1431655766 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 512);

        // rs1_val==-1431655766 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 516);

        // rs1_val==-1431655766 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 520);

        // rs1_val==-1431655766 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 524);

        // rs1_val==-1431655766 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 528);

        // rs1_val==-1431655766 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 532);

        // rs1_val==-1431655766 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 536);

        // rs1_val==-1431655766 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 540);

        // rs1_val==-1431655766 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 544);

        // rs1_val==-1431655766 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 548);

        // rs1_val==-1431655766 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 552);

        // rs1_val==-1431655766 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 556);

        // rs1_val==-1431655766 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 560);

        // rs1_val==-1431655766 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 564);

        // rs1_val==-1431655766 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 568);

        // rs1_val==-1431655766 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 572);

        // rs1_val==-1431655766 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 576);

        // rs1_val==-1431655766 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 580);

        // rs1_val==-1431655766 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555556;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 584);

        // rs1_val==5 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 588);

        // rs1_val==5 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 592);

        // rs1_val==5 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 596);

        // rs1_val==5 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 600);

        // rs1_val==5 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 604);

        // rs1_val==5 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 608);

        // rs1_val==5 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 612);

        // rs1_val==5 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 616);

        // rs1_val==5 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 620);

        // rs1_val==5 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 624);

        // rs1_val==5 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 628);

        // rs1_val==5 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 632);

        // rs1_val==5 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 636);

        // rs1_val==5 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 640);

        // rs1_val==5 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 644);

        // rs1_val==5 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 648);

        // rs1_val==5 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 652);

        // rs1_val==5 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 656);

        // rs1_val==5 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 660);

        // rs1_val==5 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 664);

        // rs1_val==5 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x5;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 668);

        // rs1_val==858993459 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 672);

        // rs1_val==858993459 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 676);

        // rs1_val==858993459 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 680);

        // rs1_val==858993459 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 684);

        // rs1_val==858993459 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 688);

        // rs1_val==858993459 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 692);

        // rs1_val==858993459 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 696);

        // rs1_val==858993459 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 700);

        // rs1_val==858993459 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 704);

        // rs1_val==858993459 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 708);

        // rs1_val==858993459 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 712);

        // rs1_val==858993459 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 716);

        // rs1_val==858993459 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 720);

        // rs1_val==858993459 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 724);

        // rs1_val==858993459 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 728);

        // rs1_val==858993459 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 732);

        // rs1_val==858993459 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 736);

        // rs1_val==858993459 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 740);

        // rs1_val==858993459 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 744);

        // rs1_val==858993459 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 748);

        // rs1_val==858993459 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333333;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 752);

        // rs1_val==1717986918 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 756);

        // rs1_val==1717986918 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 760);

        // rs1_val==1717986918 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 764);

        // rs1_val==1717986918 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 768);

        // rs1_val==1717986918 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 772);

        // rs1_val==1717986918 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 776);

        // rs1_val==1717986918 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 780);

        // rs1_val==1717986918 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 784);

        // rs1_val==1717986918 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 788);

        // rs1_val==1717986918 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 792);

        // rs1_val==1717986918 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 796);

        // rs1_val==1717986918 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 800);

        // rs1_val==1717986918 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 804);

        // rs1_val==1717986918 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 808);

        // rs1_val==1717986918 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 812);

        // rs1_val==1717986918 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 816);

        // rs1_val==1717986918 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 820);

        // rs1_val==1717986918 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 824);

        // rs1_val==1717986918 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 828);

        // rs1_val==1717986918 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 832);

        // rs1_val==1717986918 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 836);

        // rs1_val==1717986918 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666666;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 840);

        // rs1_val==-46340 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 844);

        // rs1_val==-46340 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 848);

        // rs1_val==-46340 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 852);

        // rs1_val==-46340 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 856);

        // rs1_val==-46340 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 860);

        // rs1_val==-46340 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 864);

        // rs1_val==-46340 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 868);

        // rs1_val==-46340 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 872);

        // rs1_val==-46340 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 876);

        // rs1_val==-46340 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 880);

        // rs1_val==-46340 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 884);

        // rs1_val==-46340 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 888);

        // rs1_val==-46340 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 892);

        // rs1_val==-46340 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 896);

        // rs1_val==-46340 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 900);

        // rs1_val==-46340 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 904);

        // rs1_val==-46340 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 908);

        // rs1_val==-46340 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 912);

        // rs1_val==-46340 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 916);

        // rs1_val==-46340 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 920);

        // rs1_val==-46340 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb504;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 924);

        // rs1_val==46340 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 928);

        // rs1_val==46340 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 932);

        // rs1_val==46340 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 936);

        // rs1_val==46340 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 940);

        // rs1_val==46340 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 944);

        // rs1_val==46340 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 948);

        // rs1_val==46340 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 952);

        // rs1_val==46340 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 956);

        // rs1_val==46340 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 960);

        // rs1_val==46340 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 964);

        // rs1_val==46340 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 968);

        // rs1_val==46340 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 972);

        // rs1_val==46340 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 976);

        // rs1_val==46340 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 980);

        // rs1_val==46340 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 984);

        // rs1_val==46340 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 988);

        // rs1_val==46340 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 992);

        // rs1_val==46340 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 996);

        // rs1_val==46340 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1000);

        // rs1_val==46340 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1004);

        // rs1_val==46340 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1008);

        // rs1_val==46340 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb504;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1012);

        // rs1_val==2 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1016);

        // rs1_val==2 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1020);

        // rs1_val==2 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1024);

        // rs1_val==2 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1028);

        // rs1_val==2 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1032);

        // rs1_val==2 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1036);

        // rs1_val==2 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1040);

        // rs1_val==2 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1044);

        // rs1_val==2 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1048);

        // rs1_val==2 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1052);

        // rs1_val==2 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1056);

        // rs1_val==2 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1060);

        // rs1_val==2 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1064);

        // rs1_val==2 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1068);

        // rs1_val==2 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1072);

        // rs1_val==2 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1076);

        // rs1_val==2 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1080);

        // rs1_val==2 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1084);

        // rs1_val==2 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1088);

        // rs1_val==2 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1092);

        // rs1_val==2 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1096);

        // rs1_val==2 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x2;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1100);

        // rs1_val==1431655764 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1104);

        // rs1_val==1431655764 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1108);

        // rs1_val==1431655764 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1112);

        // rs1_val==1431655764 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1116);

        // rs1_val==1431655764 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1120);

        // rs1_val==1431655764 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1124);

        // rs1_val==1431655764 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1128);

        // rs1_val==1431655764 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1132);

        // rs1_val==1431655764 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1136);

        // rs1_val==1431655764 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1140);

        // rs1_val==1431655764 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1144);

        // rs1_val==1431655764 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1148);

        // rs1_val==1431655764 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1152);

        // rs1_val==1431655764 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1156);

        // rs1_val==1431655764 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1160);

        // rs1_val==1431655764 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1164);

        // rs1_val==1431655764 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1168);

        // rs1_val==1431655764 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1172);

        // rs1_val==1431655764 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1176);

        // rs1_val==1431655764 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1180);

        // rs1_val==1431655764 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1184);

        // rs1_val==1431655764 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555554;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1188);

        // rs1_val==0 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1192);

        // rs1_val==0 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1196);

        // rs1_val==0 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1200);

        // rs1_val==0 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1204);

        // rs1_val==0 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1208);

        // rs1_val==0 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1212);

        // rs1_val==0 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1216);

        // rs1_val==0 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1220);

        // rs1_val==0 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1224);

        // rs1_val==-1431655765 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1228);

        // rs1_val==-1431655765 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1232);

        // rs1_val==-1431655765 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1236);

        // rs1_val==-1431655765 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1240);

        // rs1_val==-1431655765 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1244);

        // rs1_val==-1431655765 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1248);

        // rs1_val==6 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1252);

        // rs1_val==6 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1256);

        // rs1_val==6 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1260);

        // rs1_val==6 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1264);

        // rs1_val==6 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1268);

        // rs1_val==6 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1272);

        // rs1_val==6 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1276);

        // rs1_val==6 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1280);

        // rs1_val==6 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1284);

        // rs1_val==6 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1288);

        // rs1_val==6 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1292);

        // rs1_val==6 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1296);

        // rs1_val==6 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1300);

        // rs1_val==6 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1304);

        // rs1_val==6 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1308);

        // rs1_val==6 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1312);

        // rs1_val==6 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1316);

        // rs1_val==6 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1320);

        // rs1_val==6 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1324);

        // rs1_val==6 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1328);

        // rs1_val==6 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1332);

        // rs1_val==6 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x6;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1336);

        // rs1_val==858993460 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1340);

        // rs1_val==858993460 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1344);

        // rs1_val==858993460 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1348);

        // rs1_val==858993460 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1352);

        // rs1_val==858993460 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1356);

        // rs1_val==858993460 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1360);

        // rs1_val==858993460 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1364);

        // rs1_val==858993460 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1368);

        // rs1_val==858993460 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1372);

        // rs1_val==858993460 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1376);

        // rs1_val==858993460 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1380);

        // rs1_val==858993460 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1384);

        // rs1_val==858993460 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1388);

        // rs1_val==858993460 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1392);

        // rs1_val==858993460 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1396);

        // rs1_val==858993460 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1400);

        // rs1_val==858993460 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1404);

        // rs1_val==858993460 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1408);

        // rs1_val==858993460 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1412);

        // rs1_val==858993460 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1416);

        // rs1_val==858993460 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1420);

        // rs1_val==858993460 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333334;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1424);

        // rs1_val==1717986919 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1428);

        // rs1_val==1717986919 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1432);

        // rs1_val==1717986919 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1436);

        // rs1_val==1717986919 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1440);

        // rs1_val==1717986919 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1444);

        // rs1_val==1717986919 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1448);

        // rs1_val==1717986919 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1452);

        // rs1_val==1717986919 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1456);

        // rs1_val==1717986919 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1460);

        // rs1_val==1717986919 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1464);

        // rs1_val==1717986919 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1468);

        // rs1_val==1717986919 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1472);

        // rs1_val==1717986919 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1476);

        // rs1_val==1717986919 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1480);

        // rs1_val==1717986919 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1484);

        // rs1_val==1717986919 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1488);

        // rs1_val==1717986919 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1492);

        // rs1_val==1717986919 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1496);

        // rs1_val==1717986919 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1500);

        // rs1_val==1717986919 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1504);

        // rs1_val==1717986919 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1508);

        // rs1_val==1717986919 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666667;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1512);

        // rs1_val==-46339 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1516);

        // rs1_val==-46339 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1520);

        // rs1_val==-46339 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1524);

        // rs1_val==-46339 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1528);

        // rs1_val==-46339 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1532);

        // rs1_val==-46339 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1536);

        // rs1_val==-46339 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1540);

        // rs1_val==-46339 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1544);

        // rs1_val==-46339 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1548);

        // rs1_val==-46339 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1552);

        // rs1_val==-46339 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1556);

        // rs1_val==-46339 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1560);

        // rs1_val==-46339 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1564);

        // rs1_val==-46339 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1568);

        // rs1_val==-46339 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1572);

        // rs1_val==-46339 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1576);

        // rs1_val==-46339 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1580);

        // rs1_val==-46339 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1584);

        // rs1_val==-46339 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1588);

        // rs1_val==-46339 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1592);

        // rs1_val==-46339 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1596);

        // rs1_val==-46339 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0xb503;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1600);

        // rs1_val==46341 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1604);

        // rs1_val==46341 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1608);

        // rs1_val==46341 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1612);

        // rs1_val==46341 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1616);

        // rs1_val==46341 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1620);

        // rs1_val==46341 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1624);

        // rs1_val==46341 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1628);

        // rs1_val==46341 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1632);

        // rs1_val==46341 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1636);

        // rs1_val==46341 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1640);

        // rs1_val==46341 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1644);

        // rs1_val==46341 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1648);

        // rs1_val==46341 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1652);

        // rs1_val==46341 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1656);

        // rs1_val==46341 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1660);

        // rs1_val==46341 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1664);

        // rs1_val==46341 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1668);

        // rs1_val==46341 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1672);

        // rs1_val==46341 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1676);

        // rs1_val==46341 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1680);

        // rs1_val==46341 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1684);

        // rs1_val==46341 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb505;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1688);

        // rs1_val==0 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1692);

        // rs1_val==0 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1696);

        // rs1_val==0 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1700);

        // rs1_val==0 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1704);

        // rs1_val==0 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1708);

        // rs1_val==0 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1712);

        // rs1_val==0 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1716);

        // rs1_val==0 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1720);

        // rs1_val==0 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1724);

        // rs1_val==0 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1728);

        // rs1_val==0 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1732);

        // rs1_val==0 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1736);

        // rs1_val==0 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1740);

        // rs1_val==4 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1744);

        // rs1_val==4 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1748);

        // rs1_val==4 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1752);

        // rs1_val==4 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1756);

        // rs1_val==4 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1760);

        // rs1_val==4 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1764);

        // rs1_val==4 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1768);

        // rs1_val==4 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1772);

        // rs1_val==4 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1776);

        // rs1_val==4 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1780);

        // rs1_val==4 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1784);

        // rs1_val==4 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1788);

        // rs1_val==4 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1792);

        // rs1_val==4 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1796);

        // rs1_val==4 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1800);

        // rs1_val==4 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1804);

        // rs1_val==4 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1808);

        // rs1_val==4 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1812);

        // rs1_val==4 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1816);

        // rs1_val==4 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1820);

        // rs1_val==4 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1824);

        // rs1_val==4 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x4;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1828);

        // rs1_val==858993458 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1832);

        // rs1_val==858993458 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1836);

        // rs1_val==858993458 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1840);

        // rs1_val==858993458 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1844);

        // rs1_val==858993458 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1848);

        // rs1_val==858993458 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1852);

        // rs1_val==858993458 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1856);

        // rs1_val==858993458 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1860);

        // rs1_val==858993458 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1864);

        // rs1_val==858993458 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1868);

        // rs1_val==858993458 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1872);

        // rs1_val==858993458 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1876);

        // rs1_val==858993458 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1880);

        // rs1_val==858993458 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1884);

        // rs1_val==858993458 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1888);

        // rs1_val==858993458 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1892);

        // rs1_val==858993458 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1896);

        // rs1_val==858993458 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1900);

        // rs1_val==858993458 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1904);

        // rs1_val==858993458 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1908);

        // rs1_val==858993458 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1912);

        // rs1_val==858993458 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x33333332;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1916);

        // rs1_val==1717986917 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1920);

        // rs1_val==1717986917 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1924);

        // rs1_val==1717986917 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1928);

        // rs1_val==1717986917 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1932);

        // rs1_val==1717986917 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1936);

        // rs1_val==1717986917 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1940);

        // rs1_val==1717986917 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1944);

        // rs1_val==1717986917 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1948);

        // rs1_val==1717986917 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1952);

        // rs1_val==1717986917 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1956);

        // rs1_val==1717986917 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1960);

        // rs1_val==1717986917 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1964);

        // rs1_val==1717986917 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1968);

        // rs1_val==1717986917 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1972);

        // rs1_val==1717986917 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1976);

        // rs1_val==1717986917 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1980);

        // rs1_val==1717986917 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1984);

        // rs1_val==1717986917 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1988);

        // rs1_val==1717986917 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1992);

        // rs1_val==1717986917 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 1996);

        // rs1_val==1717986917 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 2000);

        // rs1_val==1717986917 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x66666665;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 2004);

        // rs1_val==46339 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 2008);

        // rs1_val==46339 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 2012);

        // rs1_val==46339 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 2016);

        // rs1_val==46339 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 2020);

        // rs1_val==46339 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 2024);

        // rs1_val==46339 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 2028);

        // rs1_val==46339 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 2032);

        // rs1_val==46339 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 2036);

        // rs1_val==46339 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 2040);

        // rs1_val==46339 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 2044);

        c.li(Reg::X2, 2124);
        // rs1_val==46339 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 0);

        // rs1_val==46339 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 4);

        // rs1_val==46339 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 8);

        // rs1_val==46339 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 12);

        // rs1_val==46339 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 16);

        // rs1_val==46339 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 20);

        // rs1_val==46339 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 24);

        // rs1_val==46339 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 28);

        // rs1_val==46339 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 32);

        // rs1_val==46339 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 36);

        // rs1_val==46339 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 40);

        // rs1_val==46339 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0xb503;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 44);

        // rs1_val==1431655766 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 48);

        // rs1_val==1431655766 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 52);

        // rs1_val==1431655766 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 56);

        // rs1_val==1431655766 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 60);

        // rs1_val==1431655766 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 64);

        // rs1_val==1431655766 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 68);

        // rs1_val==1431655766 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 72);

        // rs1_val==1431655766 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 76);

        // rs1_val==1431655766 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 80);

        // rs1_val==1431655766 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 84);

        // rs1_val==1431655766 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 88);

        // rs1_val==1431655766 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 92);

        // rs1_val==1431655766 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 96);

        // rs1_val==1431655766 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 100);

        // rs1_val==1431655766 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 104);

        // rs1_val==1431655766 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 108);

        // rs1_val==1431655766 and rs2_val==-1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0x55555555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 112);

        // rs1_val==1431655766 and rs2_val==6, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x6));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 116);

        // rs1_val==1431655766 and rs2_val==858993460, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x33333334
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x33333334));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 120);

        // rs1_val==1431655766 and rs2_val==1717986919, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0x66666667
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0x66666667));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 124);

        // rs1_val==1431655766 and rs2_val==-46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:-0xb503
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(-0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 128);

        // rs1_val==1431655766 and rs2_val==46341, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x55555556;  op2val:0xb505
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.li(Reg::X11, MASK_XLEN(0xb505));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 132);

        // rs1_val==-1431655765 and rs2_val==3, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x3));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 136);

        // rs1_val==-1431655765 and rs2_val==1431655765, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555555));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 140);

        // rs1_val==-1431655765 and rs2_val==-1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 144);

        // rs1_val==-1431655765 and rs2_val==5, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x5));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 148);

        // rs1_val==-1431655765 and rs2_val==858993459, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333333
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333333));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 152);

        // rs1_val==-1431655765 and rs2_val==1717986918, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666666
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666666));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 156);

        // rs1_val==-1431655765 and rs2_val==-46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:-0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(-0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 160);

        // rs1_val==-1431655765 and rs2_val==46340, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb504
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb504));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 164);

        // rs1_val==-1431655765 and rs2_val==2, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 168);

        // rs1_val==-1431655765 and rs2_val==1431655764, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555554
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555554));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 172);

        // rs1_val==-1431655765 and rs2_val==0, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x0));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 176);

        // rs1_val==-1431655765 and rs2_val==4, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x4));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 180);

        // rs1_val==-1431655765 and rs2_val==858993458, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x33333332
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x33333332));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 184);

        // rs1_val==-1431655765 and rs2_val==1717986917, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x66666665
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x66666665));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 188);

        // rs1_val==-1431655765 and rs2_val==46339, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0xb503
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0xb503));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 192);

        // rs1_val==-1431655765 and rs2_val==1431655766, 
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:-0x55555555;  op2val:0x55555556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.li(Reg::X11, MASK_XLEN(0x55555556));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 196);

        // rs1_val > 0 and rs2_val < 0, rs1_val == 2048, rs2_val == -1048577
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x800;  op2val:-0x100001
        c.li(Reg::X10, MASK_XLEN(0x800));
        c.li(Reg::X11, MASK_XLEN(-0x100001));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 200);

        // rs1_val == 0, rs2_val == 512
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x0;  op2val:0x200
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.li(Reg::X11, MASK_XLEN(0x200));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 204);

        // rs2_val == 1048576, rs1_val == 134217728
        // opcode: and ; op1:x10; op2:x11; dest:x12; op1val:0x8000000;  op2val:0x100000
        c.li(Reg::X10, MASK_XLEN(0x8000000));
        c.li(Reg::X11, MASK_XLEN(0x100000));
        c.and_(Reg::X12, Reg::X10, Reg::X11);
        c.sw(Reg::X12, Reg::X2, 208);


                
        // End
        c.ecall();
        return c;
    }

    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> and_01::s_ScalarData;
}


// Generate google tests

TEST_F(and_01, opcode__and___op1_x24__op2_x25__dest_x25__op1val_0x4000___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[0], 0);
}


TEST_F(and_01, opcode__and___op1_x3__op2_x3__dest_x18__op1val_0x800___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[1], 2048);
}


TEST_F(and_01, opcode__and___op1_x19__op2_x19__dest_x19__op1val__0x3___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[2], 4294967293);
}


TEST_F(and_01, opcode__and___op1_x5__op2_x14__dest_x5__op1val__0x1___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[3], 2147483647);
}


TEST_F(and_01, opcode__and___op1_x23__op2_x16__dest_x20__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[4], 5);
}


TEST_F(and_01, opcode__and___op1_x20__op2_x2__dest_x30__op1val_0x2___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(and_01, opcode__and___op1_x7__op2_x24__dest_x13__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[6], 0);
}


TEST_F(and_01, opcode__and___op1_x30__op2_x27__dest_x10__op1val__0x40000001___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[7], 1);
}


TEST_F(and_01, opcode__and___op1_x28__op2_x18__dest_x22__op1val__0x80000000___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[8], 0);
}


TEST_F(and_01, opcode__and___op1_x2__op2_x15__dest_x0__op1val_0x0___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(and_01, opcode__and___op1_x25__op2_x26__dest_x12__op1val_0x7fffffff___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[10], 1431655765);
}


TEST_F(and_01, opcode__and___op1_x1__op2_x31__dest_x2__op1val_0x1___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[11], 0);
}


TEST_F(and_01, opcode__and___op1_x27__op2_x11__dest_x14__op1val_0x40000___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(and_01, opcode__and___op1_x31__op2_x23__dest_x4__op1val__0x20001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[13], 4);
}


TEST_F(and_01, opcode__and___op1_x21__op2_x9__dest_x27__op1val__0x55555555___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[14], 8);
}


TEST_F(and_01, opcode__and___op1_x26__op2_x7__dest_x23__op1val_0x400___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(and_01, opcode__and___op1_x9__op2_x20__dest_x24__op1val__0x8___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[16], 32);
}


TEST_F(and_01, opcode__and___op1_x15__op2_x13__dest_x26__op1val__0x101___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[17], 64);
}


TEST_F(and_01, opcode__and___op1_x12__op2_x4__dest_x17__op1val__0x2000001___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[18], 128);
}


TEST_F(and_01, opcode__and___op1_x4__op2_x17__dest_x8__op1val_0x66666665___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[19], 0);
}


TEST_F(and_01, opcode__and___op1_x13__op2_x22__dest_x1__op1val__0x81___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[20], 1024);
}


TEST_F(and_01, opcode__and___op1_x8__op2_x12__dest_x7__op1val_0x4___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[21], 0);
}


TEST_F(and_01, opcode__and___op1_x11__op2_x5__dest_x21__op1val__0x8001___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[22], 8192);
}


TEST_F(and_01, opcode__and___op1_x14__op2_x21__dest_x29__op1val_0x40___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[23], 0);
}


TEST_F(and_01, opcode__and___op1_x17__op2_x10__dest_x11__op1val_0x5___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[24], 0);
}


TEST_F(and_01, opcode__and___op1_x22__op2_x6__dest_x3__op1val_0x800000___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[25], 0);
}


TEST_F(and_01, opcode__and___op1_x0__op2_x29__dest_x9__op1val_0x0___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[26], 0);
}


TEST_F(and_01, opcode__and___op1_x29__op2_x30__dest_x28__op1val_0x5___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[27], 0);
}


TEST_F(and_01, opcode__and___op1_x18__op2_x1__dest_x6__op1val_0x10000___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[28], 0);
}


TEST_F(and_01, opcode__and___op1_x6__op2_x0__dest_x31__op1val_0x8000000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[29], 0);
}


TEST_F(and_01, opcode__and___op1_x16__op2_x8__dest_x15__op1val__0x3___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[30], 2097152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x28__dest_x16__op1val_0x800___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[31], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[32], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[33], 16777216);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[34], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[35], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[36], 134217728);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[37], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[38], 536870912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[39], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[40], 4294966782);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[41], 3221225469);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[42], 8388608);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[43], 4294966775);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[44], 4160749551);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[45], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[46], 128);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x7___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[47], 4294967161);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[48], 4294967031);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[49], 2863311018);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[50], 4286577663);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[51], 4293916671);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[52], 1024);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[53], 1717978726);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x7___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[54], 7);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[55], 33554432);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[56], 1073741824);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[57], 4294836219);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[58], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[59], 16);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[60], 1024);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x1___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[61], 4290772991);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[62], 131072);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[63], 4277927935);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[64], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[65], 2863311531);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[66], 4160749551);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[67], 3489660927);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[68], 3758050045);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[69], 2147483648);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[70], 2863270568);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[71], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[72], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[73], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[74], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[75], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[76], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val__0x4) {
  ASSERT_EQ(getScalarWordData()[77], 32768);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val_0x3fffffff) {
  ASSERT_EQ(getScalarWordData()[78], 524288);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[79], 1048576);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[80], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[81], 4194304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[82], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[83], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[84], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[85], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[86], 4026531838);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[87], 858993426);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[88], 4290772927);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[89], 4294949887);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val__0x7) {
  ASSERT_EQ(getScalarWordData()[90], 4294965241);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[91], 4294963071);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[92], 4294959099);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[93], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[94], 4292345855);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[95], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[96], 858993460);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[97], 2130706431);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[98], 1650878053);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[99], 4026485501);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[100], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[101], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[102], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[103], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[104], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[105], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[106], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[107], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[108], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[109], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[110], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[111], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[112], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[113], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[114], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[115], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[116], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[117], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[118], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[119], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[120], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[121], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[122], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[123], 1431655765);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[124], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[125], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[126], 286331153);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[127], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[128], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[129], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[130], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[131], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[132], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[133], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[134], 286331152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[135], 1145324613);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[136], 5377);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[137], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[138], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[139], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[140], 286331156);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[141], 1145324613);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[142], 1431650389);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[143], 5381);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[144], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[145], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[146], 2863311530);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[147], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[148], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[149], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[150], 2863270568);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[151], 40960);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[152], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[153], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[154], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[155], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[156], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[157], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[158], 40962);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[159], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[160], 2863311530);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[161], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[162], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[163], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[164], 2863270568);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[165], 40960);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[166], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[167], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[168], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[169], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[170], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[171], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[172], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[173], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[174], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[175], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[176], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[177], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[178], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[179], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[180], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[181], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[182], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[183], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[184], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[185], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[186], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[187], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[188], 286331153);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[189], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[190], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[191], 858993459);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[192], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[193], 858980912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[194], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[195], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[196], 286331152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[197], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[198], 858993458);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[199], 572662305);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[200], 12547);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[201], 286331154);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[202], 572662307);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[203], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[204], 858993456);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[205], 572662307);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[206], 858980913);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[207], 12545);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[208], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[209], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[210], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[211], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[212], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[213], 1717986918);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[214], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[215], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[216], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[217], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[218], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[219], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[220], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[221], 1717986916);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[222], 9218);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[223], 1145324614);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[224], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[225], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[226], 572662308);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[227], 1717986918);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[228], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[229], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[230], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[231], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[232], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[233], 858980912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[234], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[235], 4294920956);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[236], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[237], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[238], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[239], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[240], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[241], 858980912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[242], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[243], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[244], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[245], 2863270568);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[246], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[247], 858980916);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[248], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[249], 4294920956);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[250], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[251], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[252], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[253], 40960);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[254], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[255], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[256], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[257], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[258], 46340);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[259], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[260], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[261], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[262], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[263], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[264], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[265], 46336);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[266], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[267], 40960);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[268], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[269], 12548);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[270], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[271], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[272], 46340);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[273], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[274], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[275], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[276], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[277], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[278], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[279], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[280], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[281], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[282], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[283], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[284], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[285], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[286], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[287], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[288], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[289], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[290], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[291], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[292], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[293], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[294], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[295], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[296], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[297], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[298], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[299], 286331152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[300], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[301], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[302], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[303], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[304], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[305], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[306], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[307], 286331152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[308], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[309], 5376);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[310], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[311], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[312], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[313], 286331156);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[314], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[315], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[316], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[317], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[318], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[319], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[320], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[321], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[322], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[323], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[324], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[325], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[326], 2863311531);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[327], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[328], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[329], 572662307);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[330], 2863270569);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[331], 40961);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[332], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[333], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[334], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[335], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[336], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[337], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[338], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[339], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[340], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[341], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[342], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[343], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[344], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[345], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[346], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[347], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[348], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[349], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[350], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[351], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[352], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[353], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[354], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[355], 286331156);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[356], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[357], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[358], 858993456);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[359], 572662308);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[360], 858980916);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[361], 12548);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[362], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[363], 286331156);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[364], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[365], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[366], 858993456);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[367], 572662308);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[368], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[369], 286331156);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[370], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[371], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[372], 858993460);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[373], 572662308);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[374], 858980916);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[375], 12548);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[376], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[377], 1145324613);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[378], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[379], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[380], 572662307);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[381], 1717986918);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[382], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[383], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[384], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[385], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[386], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[387], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[388], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[389], 1717986917);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[390], 9219);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[391], 1145324614);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[392], 572662307);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[393], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[394], 572662308);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[395], 1717986919);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[396], 1717977701);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[397], 9221);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[398], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[399], 1431650389);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[400], 2863270568);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[401], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[402], 858980913);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[403], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[404], 4294920956);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[405], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[406], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[407], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[408], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[409], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[410], 858980912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[411], 1717977701);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[412], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[413], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[414], 2863270569);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[415], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[416], 858980916);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[417], 1717977701);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[418], 4294920957);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[419], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[420], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[421], 5381);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[422], 40960);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[423], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[424], 12545);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[425], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[426], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[427], 46340);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[428], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[429], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[430], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[431], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[432], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[433], 9221);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[434], 46337);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[435], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[436], 40961);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[437], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[438], 12548);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[439], 9221);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[440], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[441], 46341);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[442], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[443], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[444], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[445], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[446], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[447], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[448], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[449], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[450], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[451], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[452], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[453], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[454], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[455], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[456], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[457], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[458], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[459], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[460], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[461], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[462], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[463], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[464], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[465], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[466], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[467], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[468], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[469], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[470], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[471], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[472], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[473], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[474], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[475], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[476], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[477], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[478], 286331152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[479], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[480], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[481], 858993458);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[482], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[483], 858980912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[484], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[485], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[486], 286331152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[487], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[488], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[489], 858993458);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[490], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[491], 12546);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[492], 286331154);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[493], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[494], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[495], 858993456);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[496], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[497], 858980912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[498], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[499], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[500], 1145324613);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[501], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[502], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[503], 572662305);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[504], 1717986916);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[505], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[506], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[507], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[508], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[509], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[510], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[511], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[512], 1717986917);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[513], 9217);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[514], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[515], 572662305);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[516], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[517], 572662308);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[518], 1717986917);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[519], 1717977701);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[520], 9221);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[521], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[522], 5377);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[523], 40962);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[524], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[525], 12547);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[526], 9218);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[527], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[528], 46336);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[529], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[530], 5376);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[531], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[532], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[533], 12546);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[534], 9217);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[535], 46339);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[536], 5378);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[537], 40963);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[538], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[539], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[540], 9219);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[541], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[542], 46337);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[543], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[544], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[545], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[546], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[547], 286331154);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[548], 1145324614);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[549], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[550], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[551], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[552], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[553], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[554], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[555], 286331154);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[556], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[557], 5378);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[558], 1431655766);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[559], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[560], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[561], 286331156);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[562], 1145324614);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[563], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[564], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[565], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[566], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[567], 2863311530);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[568], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[569], 572662307);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[570], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[571], 2863270568);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[572], 40960);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[573], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[574], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[575], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[576], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[577], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[578], 572662305);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[579], 40963);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[580], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[581], 2048);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[582], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[583], 0);
}

