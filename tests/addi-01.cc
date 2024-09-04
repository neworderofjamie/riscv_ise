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
class addi_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        // Generate word-aligned vector to initialise memory
        const std::vector<uint32_t> scalarWordInitData{
        3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,};
        
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
        
        c.li(Reg::X14, 0);
        // rs1 != rd, rs1==x20, rd==x7, imm_val == (-2**(12-1)), imm_val == -2048, rs1_val != imm_val, rs1_val == 536870912, rs1_val > 0 and imm_val < 0
        // opcode: addi ; op1:x20; dest:x7; op1val:0x20000000;  immval:-0x800
        c.li(Reg::X20, MASK_XLEN(0x20000000));
        c.addi(Reg::X7, Reg::X20, SEXT_IMM(-0x800));
        c.sw(Reg::X7, Reg::X14, 0);

        // rs1 == rd, rs1==x3, rd==x3, imm_val == 0, rs1_val == 1024
        // opcode: addi ; op1:x3; dest:x3; op1val:0x400;  immval:0x0
        c.li(Reg::X3, MASK_XLEN(0x400));
        c.addi(Reg::X3, Reg::X3, SEXT_IMM(0x0));
        c.sw(Reg::X3, Reg::X14, 4);

        // rs1==x4, rd==x22, imm_val == (2**(12-1)-1), rs1_val < 0 and imm_val > 0, imm_val == 2047, rs1_val == -513
        // opcode: addi ; op1:x4; dest:x22; op1val:-0x201;  immval:0x7ff
        c.li(Reg::X4, MASK_XLEN(-0x201));
        c.addi(Reg::X22, Reg::X4, SEXT_IMM(0x7ff));
        c.sw(Reg::X22, Reg::X14, 8);

        // rs1==x30, rd==x11, imm_val == 1, rs1_val == 0
        // opcode: addi ; op1:x30; dest:x11; op1val:0x0;  immval:0x1
        c.li(Reg::X30, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X30, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X14, 12);

        // rs1==x27, rd==x31, rs1_val == (-2**(xlen-1)), imm_val == 16, rs1_val == -2147483648
        // opcode: addi ; op1:x27; dest:x31; op1val:-0x80000000;  immval:0x10
        c.li(Reg::X27, MASK_XLEN(-0x80000000));
        c.addi(Reg::X31, Reg::X27, SEXT_IMM(0x10));
        c.sw(Reg::X31, Reg::X14, 16);

        // rs1==x17, rd==x30, rs1_val == (2**(xlen-1)-1), rs1_val > 0 and imm_val > 0, rs1_val == 2147483647
        // opcode: addi ; op1:x17; dest:x30; op1val:0x7fffffff;  immval:0x6
        c.li(Reg::X17, MASK_XLEN(0x7fffffff));
        c.addi(Reg::X30, Reg::X17, SEXT_IMM(0x6));
        c.sw(Reg::X30, Reg::X14, 20);

        // rs1==x18, rd==x28, rs1_val == 1, imm_val == 4
        // opcode: addi ; op1:x18; dest:x28; op1val:0x1;  immval:0x4
        c.li(Reg::X18, MASK_XLEN(0x1));
        c.addi(Reg::X28, Reg::X18, SEXT_IMM(0x4));
        c.sw(Reg::X28, Reg::X14, 24);

        // rs1==x13, rd==x6, rs1_val == imm_val, rs1_val==5 and imm_val==5
        // opcode: addi ; op1:x13; dest:x6; op1val:0x5;  immval:0x5
        c.li(Reg::X13, MASK_XLEN(0x5));
        c.addi(Reg::X6, Reg::X13, SEXT_IMM(0x5));
        c.sw(Reg::X6, Reg::X14, 28);

        // rs1==x10, rd==x16, rs1_val < 0 and imm_val < 0, imm_val == -33
        // opcode: addi ; op1:x10; dest:x16; op1val:-0x55555555;  immval:-0x21
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X16, Reg::X10, SEXT_IMM(-0x21));
        c.sw(Reg::X16, Reg::X14, 32);

        // rs1==x9, rd==x21, imm_val == 2, rs1_val == -17
        // opcode: addi ; op1:x9; dest:x21; op1val:-0x11;  immval:0x2
        c.li(Reg::X9, MASK_XLEN(-0x11));
        c.addi(Reg::X21, Reg::X9, SEXT_IMM(0x2));
        c.sw(Reg::X21, Reg::X14, 36);

        // rs1==x7, rd==x2, imm_val == 8, 
        // opcode: addi ; op1:x7; dest:x2; op1val:0xb505;  immval:0x8
        c.li(Reg::X7, MASK_XLEN(0xb505));
        c.addi(Reg::X2, Reg::X7, SEXT_IMM(0x8));
        c.sw(Reg::X2, Reg::X14, 40);

        // rs1==x22, rd==x18, imm_val == 32, 
        // opcode: addi ; op1:x22; dest:x18; op1val:-0xb504;  immval:0x20
        c.li(Reg::X22, MASK_XLEN(-0xb504));
        c.addi(Reg::X18, Reg::X22, SEXT_IMM(0x20));
        c.sw(Reg::X18, Reg::X14, 44);

        // rs1==x29, rd==x0, imm_val == 64, rs1_val == -2097153
        // opcode: addi ; op1:x29; dest:x0; op1val:-0x200001;  immval:0x40
        c.li(Reg::X29, MASK_XLEN(-0x200001));
        c.addi(Reg::X0, Reg::X29, SEXT_IMM(0x40));
        c.sw(Reg::X0, Reg::X14, 48);

        // rs1==x25, rd==x13, imm_val == 128, 
        // opcode: addi ; op1:x25; dest:x13; op1val:0x5;  immval:0x80
        c.li(Reg::X25, MASK_XLEN(0x5));
        c.addi(Reg::X13, Reg::X25, SEXT_IMM(0x80));
        c.sw(Reg::X13, Reg::X14, 52);

        // rs1==x11, rd==x29, imm_val == 256, rs1_val == -33554433
        // opcode: addi ; op1:x11; dest:x29; op1val:-0x2000001;  immval:0x100
        c.li(Reg::X11, MASK_XLEN(-0x2000001));
        c.addi(Reg::X29, Reg::X11, SEXT_IMM(0x100));
        c.sw(Reg::X29, Reg::X14, 56);

        // rs1==x6, rd==x8, imm_val == 512, rs1_val == 16
        // opcode: addi ; op1:x6; dest:x8; op1val:0x10;  immval:0x200
        c.li(Reg::X6, MASK_XLEN(0x10));
        c.addi(Reg::X8, Reg::X6, SEXT_IMM(0x200));
        c.sw(Reg::X8, Reg::X14, 60);

        // rs1==x19, rd==x4, imm_val == 1024, rs1_val == 2
        // opcode: addi ; op1:x19; dest:x4; op1val:0x2;  immval:0x400
        c.li(Reg::X19, MASK_XLEN(0x2));
        c.addi(Reg::X4, Reg::X19, SEXT_IMM(0x400));
        c.sw(Reg::X4, Reg::X14, 64);

        // rs1==x12, rd==x10, imm_val == -2, 
        // opcode: addi ; op1:x12; dest:x10; op1val:0x55555554;  immval:-0x2
        c.li(Reg::X12, MASK_XLEN(0x55555554));
        c.addi(Reg::X10, Reg::X12, SEXT_IMM(-0x2));
        c.sw(Reg::X10, Reg::X14, 68);

        // rs1==x31, rd==x26, imm_val == -3, rs1_val == 1431655765
        // opcode: addi ; op1:x31; dest:x26; op1val:0x55555555;  immval:-0x3
        c.li(Reg::X31, MASK_XLEN(0x55555555));
        c.addi(Reg::X26, Reg::X31, SEXT_IMM(-0x3));
        c.sw(Reg::X26, Reg::X14, 72);

        // rs1==x26, rd==x15, imm_val == -5, 
        // opcode: addi ; op1:x26; dest:x15; op1val:0x55555554;  immval:-0x5
        c.li(Reg::X26, MASK_XLEN(0x55555554));
        c.addi(Reg::X15, Reg::X26, SEXT_IMM(-0x5));
        c.sw(Reg::X15, Reg::X14, 76);

        // rs1==x5, rd==x19, imm_val == -9, 
        // opcode: addi ; op1:x5; dest:x19; op1val:0x3;  immval:-0x9
        c.li(Reg::X5, MASK_XLEN(0x3));
        c.addi(Reg::X19, Reg::X5, SEXT_IMM(-0x9));
        c.sw(Reg::X19, Reg::X14, 80);

        // rs1==x0, rd==x1, imm_val == -17, 
        // opcode: addi ; op1:x0; dest:x1; op1val:0x0;  immval:-0x11
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.addi(Reg::X1, Reg::X0, SEXT_IMM(-0x11));
        c.sw(Reg::X1, Reg::X14, 84);

        c.li(Reg::X3, 88);
        // rs1==x24, rd==x23, imm_val == -65, rs1_val == 8388608
        // opcode: addi ; op1:x24; dest:x23; op1val:0x800000;  immval:-0x41
        c.li(Reg::X24, MASK_XLEN(0x800000));
        c.addi(Reg::X23, Reg::X24, SEXT_IMM(-0x41));
        c.sw(Reg::X23, Reg::X3, 0);

        // rs1==x15, rd==x27, imm_val == -129, 
        // opcode: addi ; op1:x15; dest:x27; op1val:-0x11;  immval:-0x81
        c.li(Reg::X15, MASK_XLEN(-0x11));
        c.addi(Reg::X27, Reg::X15, SEXT_IMM(-0x81));
        c.sw(Reg::X27, Reg::X3, 4);

        // rs1==x14, rd==x17, imm_val == -257, 
        // opcode: addi ; op1:x14; dest:x17; op1val:0x55555555;  immval:-0x101
        c.li(Reg::X14, MASK_XLEN(0x55555555));
        c.addi(Reg::X17, Reg::X14, SEXT_IMM(-0x101));
        c.sw(Reg::X17, Reg::X3, 8);

        // rs1==x21, rd==x25, imm_val == -513, 
        // opcode: addi ; op1:x21; dest:x25; op1val:0x55555556;  immval:-0x201
        c.li(Reg::X21, MASK_XLEN(0x55555556));
        c.addi(Reg::X25, Reg::X21, SEXT_IMM(-0x201));
        c.sw(Reg::X25, Reg::X3, 12);

        // rs1==x1, rd==x24, imm_val == -1025, rs1_val == 8192
        // opcode: addi ; op1:x1; dest:x24; op1val:0x2000;  immval:-0x401
        c.li(Reg::X1, MASK_XLEN(0x2000));
        c.addi(Reg::X24, Reg::X1, SEXT_IMM(-0x401));
        c.sw(Reg::X24, Reg::X3, 16);

        // rs1==x16, rd==x12, imm_val == 1365, rs1_val==-1431655766 and imm_val==1365, rs1_val == -1431655766
        // opcode: addi ; op1:x16; dest:x12; op1val:-0x55555556;  immval:0x555
        c.li(Reg::X16, MASK_XLEN(-0x55555556));
        c.addi(Reg::X12, Reg::X16, SEXT_IMM(0x555));
        c.sw(Reg::X12, Reg::X3, 20);

        // rs1==x2, rd==x14, imm_val == -1366, rs1_val==0 and imm_val==-1366
        // opcode: addi ; op1:x2; dest:x14; op1val:0x0;  immval:-0x556
        c.li(Reg::X2, MASK_XLEN(0x0));
        c.addi(Reg::X14, Reg::X2, SEXT_IMM(-0x556));
        c.sw(Reg::X14, Reg::X3, 24);

        // rs1==x23, rd==x20, rs1_val == 4, rs1_val==4 and imm_val==4
        // opcode: addi ; op1:x23; dest:x20; op1val:0x4;  immval:0x4
        c.li(Reg::X23, MASK_XLEN(0x4));
        c.addi(Reg::X20, Reg::X23, SEXT_IMM(0x4));
        c.sw(Reg::X20, Reg::X3, 28);

        // rs1==x28, rd==x5, rs1_val == 8, 
        // opcode: addi ; op1:x28; dest:x5; op1val:0x8;  immval:0x10
        c.li(Reg::X28, MASK_XLEN(0x8));
        c.addi(Reg::X5, Reg::X28, SEXT_IMM(0x10));
        c.sw(Reg::X5, Reg::X3, 32);

        // rs1==x8, rd==x9, rs1_val == 32, 
        // opcode: addi ; op1:x8; dest:x9; op1val:0x20;  immval:-0x2c
        c.li(Reg::X8, MASK_XLEN(0x20));
        c.addi(Reg::X9, Reg::X8, SEXT_IMM(-0x2c));
        c.sw(Reg::X9, Reg::X3, 36);

        // rs1_val == 64, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x40;  immval:-0x41
        c.li(Reg::X10, MASK_XLEN(0x40));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x41));
        c.sw(Reg::X11, Reg::X3, 40);

        // rs1_val == 128, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x80;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x80));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 44);

        // rs1_val == 256, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x100;  immval:-0x400
        c.li(Reg::X10, MASK_XLEN(0x100));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x400));
        c.sw(Reg::X11, Reg::X3, 48);

        // rs1_val == 512, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x200;  immval:0x100
        c.li(Reg::X10, MASK_XLEN(0x200));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x100));
        c.sw(Reg::X11, Reg::X3, 52);

        // rs1_val == 2048, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x800;  immval:-0xa
        c.li(Reg::X10, MASK_XLEN(0x800));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0xa));
        c.sw(Reg::X11, Reg::X3, 56);

        // rs1_val == 4096, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x1000;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x1000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X3, 60);

        // rs1_val == 16384, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4000;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x4000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 64);

        // rs1_val == 32768, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x8000;  immval:-0x400
        c.li(Reg::X10, MASK_XLEN(0x8000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x400));
        c.sw(Reg::X11, Reg::X3, 68);

        // rs1_val == 65536, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x10000;  immval:-0x101
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x101));
        c.sw(Reg::X11, Reg::X3, 72);

        // rs1_val == 131072, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x20000;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x20000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 76);

        // rs1_val == 262144, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x40000;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x40000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 80);

        // rs1_val == 524288, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x80000;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x80000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 84);

        // rs1_val == 1048576, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x100000;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x100000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 88);

        // rs1_val == 2097152, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x200000;  immval:-0x4
        c.li(Reg::X10, MASK_XLEN(0x200000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x4));
        c.sw(Reg::X11, Reg::X3, 92);

        // rs1_val == 4194304, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x400000;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x400000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 96);

        // rs1_val == 16777216, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x1000000;  immval:-0x400
        c.li(Reg::X10, MASK_XLEN(0x1000000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x400));
        c.sw(Reg::X11, Reg::X3, 100);

        // rs1_val == 33554432, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2000000;  immval:-0x11
        c.li(Reg::X10, MASK_XLEN(0x2000000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x11));
        c.sw(Reg::X11, Reg::X3, 104);

        // rs1_val == 67108864, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4000000;  immval:-0x2
        c.li(Reg::X10, MASK_XLEN(0x4000000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2));
        c.sw(Reg::X11, Reg::X3, 108);

        // rs1_val == 134217728, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x8000000;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x8000000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 112);

        // rs1_val == 268435456, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x10000000;  immval:-0x81
        c.li(Reg::X10, MASK_XLEN(0x10000000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x81));
        c.sw(Reg::X11, Reg::X3, 116);

        // rs1_val == 1073741824, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x40000000;  immval:-0x2
        c.li(Reg::X10, MASK_XLEN(0x40000000));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2));
        c.sw(Reg::X11, Reg::X3, 120);

        // rs1_val == -2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x2;  immval:-0x201
        c.li(Reg::X10, MASK_XLEN(-0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x201));
        c.sw(Reg::X11, Reg::X3, 124);

        // rs1_val == -3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x3;  immval:-0x9
        c.li(Reg::X10, MASK_XLEN(-0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x9));
        c.sw(Reg::X11, Reg::X3, 128);

        // rs1_val == -5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x5;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 132);

        // rs1_val == -9, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x9;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0x9));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 136);

        // rs1_val == -33, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x21;  immval:-0x5
        c.li(Reg::X10, MASK_XLEN(-0x21));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x5));
        c.sw(Reg::X11, Reg::X3, 140);

        // rs1_val == -65, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x41;  immval:-0x9
        c.li(Reg::X10, MASK_XLEN(-0x41));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x9));
        c.sw(Reg::X11, Reg::X3, 144);

        // rs1_val == -129, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x81;  immval:-0x11
        c.li(Reg::X10, MASK_XLEN(-0x81));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x11));
        c.sw(Reg::X11, Reg::X3, 148);

        // rs1_val == -257, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x101;  immval:0x200
        c.li(Reg::X10, MASK_XLEN(-0x101));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x200));
        c.sw(Reg::X11, Reg::X3, 152);

        // rs1_val == -1025, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x401;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0x401));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 156);

        // rs1_val == -2049, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x801;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0x801));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 160);

        // rs1_val == -4097, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x1001;  immval:0x8
        c.li(Reg::X10, MASK_XLEN(-0x1001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x8));
        c.sw(Reg::X11, Reg::X3, 164);

        // rs1_val == -8193, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x2001;  immval:-0x201
        c.li(Reg::X10, MASK_XLEN(-0x2001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x201));
        c.sw(Reg::X11, Reg::X3, 168);

        // rs1_val == -16385, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x4001;  immval:0x400
        c.li(Reg::X10, MASK_XLEN(-0x4001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x400));
        c.sw(Reg::X11, Reg::X3, 172);

        // rs1_val == -32769, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x8001;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0x8001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 176);

        // rs1_val == -65537, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x10001;  immval:0x7
        c.li(Reg::X10, MASK_XLEN(-0x10001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x7));
        c.sw(Reg::X11, Reg::X3, 180);

        // rs1_val == -131073, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x20001;  immval:0x80
        c.li(Reg::X10, MASK_XLEN(-0x20001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x80));
        c.sw(Reg::X11, Reg::X3, 184);

        // rs1_val == -262145, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x40001;  immval:0x8
        c.li(Reg::X10, MASK_XLEN(-0x40001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x8));
        c.sw(Reg::X11, Reg::X3, 188);

        // rs1_val == -524289, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x80001;  immval:-0x800
        c.li(Reg::X10, MASK_XLEN(-0x80001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x800));
        c.sw(Reg::X11, Reg::X3, 192);

        // rs1_val == -1048577, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x100001;  immval:-0x21
        c.li(Reg::X10, MASK_XLEN(-0x100001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x21));
        c.sw(Reg::X11, Reg::X3, 196);

        // rs1_val == -4194305, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x400001;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(-0x400001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 200);

        // rs1_val == -8388609, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x800001;  immval:-0xa
        c.li(Reg::X10, MASK_XLEN(-0x800001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0xa));
        c.sw(Reg::X11, Reg::X3, 204);

        // rs1_val == -16777217, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x1000001;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(-0x1000001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 208);

        // rs1_val == -67108865, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x4000001;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0x4000001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 212);

        // rs1_val == -134217729, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x8000001;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0x8000001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 216);

        // rs1_val == -268435457, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x10000001;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0x10000001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 220);

        // rs1_val == -536870913, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x20000001;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0x20000001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 224);

        // rs1_val == -1073741825, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x40000001;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0x40000001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 228);

        // rs1_val==3 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 232);

        // rs1_val==3 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 236);

        // rs1_val==3 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 240);

        // rs1_val==3 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 244);

        // rs1_val==3 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 248);

        // rs1_val==3 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 252);

        // rs1_val==3 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 256);

        // rs1_val==3 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 260);

        // rs1_val==3 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 264);

        // rs1_val==3 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 268);

        // rs1_val==3 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 272);

        // rs1_val==3 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 276);

        // rs1_val==3 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 280);

        // rs1_val==3 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 284);

        // rs1_val==3 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 288);

        // rs1_val==3 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 292);

        // rs1_val==3 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 296);

        // rs1_val==3 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 300);

        // rs1_val==3 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 304);

        // rs1_val==3 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 308);

        // rs1_val==3 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 312);

        // rs1_val==3 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x3;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 316);

        // rs1_val==1431655765 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 320);

        // rs1_val==1431655765 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 324);

        // rs1_val==1431655765 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 328);

        // rs1_val==1431655765 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 332);

        // rs1_val==1431655765 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 336);

        // rs1_val==1431655765 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 340);

        // rs1_val==1431655765 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 344);

        // rs1_val==1431655765 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 348);

        // rs1_val==1431655765 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 352);

        // rs1_val==1431655765 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 356);

        // rs1_val==1431655765 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 360);

        // rs1_val==1431655765 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 364);

        // rs1_val==1431655765 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 368);

        // rs1_val==1431655765 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 372);

        // rs1_val==1431655765 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 376);

        // rs1_val==1431655765 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 380);

        // rs1_val==1431655765 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 384);

        // rs1_val==1431655765 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 388);

        // rs1_val==1431655765 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 392);

        // rs1_val==1431655765 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 396);

        // rs1_val==1431655765 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 400);

        // rs1_val==1431655765 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 404);

        // rs1_val==-1431655766 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 408);

        // rs1_val==-1431655766 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 412);

        // rs1_val==-1431655766 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 416);

        // rs1_val==-1431655766 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 420);

        // rs1_val==-1431655766 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 424);

        // rs1_val==-1431655766 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 428);

        // rs1_val==-1431655766 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 432);

        // rs1_val==-1431655766 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 436);

        // rs1_val==-1431655766 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 440);

        // rs1_val==-1431655766 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 444);

        // rs1_val==-1431655766 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 448);

        // rs1_val==-1431655766 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 452);

        // rs1_val==-1431655766 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 456);

        // rs1_val==-1431655766 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 460);

        // rs1_val==-1431655766 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 464);

        // rs1_val==-1431655766 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 468);

        // rs1_val==-1431655766 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 472);

        // rs1_val==-1431655766 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 476);

        // rs1_val==-1431655766 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 480);

        // rs1_val==-1431655766 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 484);

        // rs1_val==-1431655766 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 488);

        // rs1_val==5 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 492);

        // rs1_val==5 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 496);

        // rs1_val==5 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 500);

        // rs1_val==5 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 504);

        // rs1_val==5 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 508);

        // rs1_val==5 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 512);

        // rs1_val==5 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 516);

        // rs1_val==5 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 520);

        // rs1_val==5 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 524);

        // rs1_val==5 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 528);

        // rs1_val==5 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 532);

        // rs1_val==5 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 536);

        // rs1_val==5 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 540);

        // rs1_val==5 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 544);

        // rs1_val==5 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 548);

        // rs1_val==5 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 552);

        // rs1_val==5 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 556);

        // rs1_val==5 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 560);

        // rs1_val==5 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 564);

        // rs1_val==5 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 568);

        // rs1_val==5 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x5;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 572);

        // rs1_val==858993459 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 576);

        // rs1_val==858993459 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 580);

        // rs1_val==858993459 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 584);

        // rs1_val==858993459 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 588);

        // rs1_val==858993459 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 592);

        // rs1_val==858993459 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 596);

        // rs1_val==858993459 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 600);

        // rs1_val==858993459 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 604);

        // rs1_val==858993459 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 608);

        // rs1_val==858993459 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 612);

        // rs1_val==858993459 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 616);

        // rs1_val==858993459 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 620);

        // rs1_val==858993459 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 624);

        // rs1_val==858993459 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 628);

        // rs1_val==858993459 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 632);

        // rs1_val==858993459 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 636);

        // rs1_val==858993459 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 640);

        // rs1_val==858993459 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 644);

        // rs1_val==858993459 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 648);

        // rs1_val==858993459 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 652);

        // rs1_val==858993459 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 656);

        // rs1_val==858993459 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 660);

        // rs1_val==1717986918 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 664);

        // rs1_val==1717986918 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 668);

        // rs1_val==1717986918 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 672);

        // rs1_val==1717986918 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 676);

        // rs1_val==1717986918 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 680);

        // rs1_val==1717986918 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 684);

        // rs1_val==1717986918 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 688);

        // rs1_val==1717986918 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 692);

        // rs1_val==1717986918 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 696);

        // rs1_val==1717986918 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 700);

        // rs1_val==1717986918 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 704);

        // rs1_val==1717986918 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 708);

        // rs1_val==1717986918 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 712);

        // rs1_val==1717986918 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 716);

        // rs1_val==1717986918 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 720);

        // rs1_val==1717986918 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 724);

        // rs1_val==1717986918 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 728);

        // rs1_val==1717986918 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 732);

        // rs1_val==1717986918 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 736);

        // rs1_val==1717986918 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 740);

        // rs1_val==1717986918 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 744);

        // rs1_val==1717986918 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 748);

        // rs1_val==-46340 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 752);

        // rs1_val==-46340 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 756);

        // rs1_val==-46340 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 760);

        // rs1_val==-46340 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 764);

        // rs1_val==-46340 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 768);

        // rs1_val==-46340 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 772);

        // rs1_val==-46340 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 776);

        // rs1_val==-46340 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 780);

        // rs1_val==-46340 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 784);

        // rs1_val==-46340 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 788);

        // rs1_val==-46340 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 792);

        // rs1_val==-46340 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 796);

        // rs1_val==-46340 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 800);

        // rs1_val==-46340 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 804);

        // rs1_val==-46340 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 808);

        // rs1_val==-46340 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 812);

        // rs1_val==-46340 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 816);

        // rs1_val==-46340 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 820);

        // rs1_val==-46340 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 824);

        // rs1_val==-46340 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 828);

        // rs1_val==-46340 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 832);

        // rs1_val==-46340 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 836);

        // rs1_val==46340 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 840);

        // rs1_val==46340 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 844);

        // rs1_val==46340 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 848);

        // rs1_val==46340 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 852);

        // rs1_val==46340 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 856);

        // rs1_val==46340 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 860);

        // rs1_val==46340 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 864);

        // rs1_val==46340 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 868);

        // rs1_val==46340 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 872);

        // rs1_val==46340 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 876);

        // rs1_val==46340 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 880);

        // rs1_val==46340 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 884);

        // rs1_val==46340 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 888);

        // rs1_val==46340 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 892);

        // rs1_val==46340 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 896);

        // rs1_val==46340 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 900);

        // rs1_val==46340 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 904);

        // rs1_val==46340 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 908);

        // rs1_val==46340 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 912);

        // rs1_val==46340 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 916);

        // rs1_val==46340 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 920);

        // rs1_val==46340 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 924);

        // rs1_val==2 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 928);

        // rs1_val==2 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 932);

        // rs1_val==2 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 936);

        // rs1_val==2 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 940);

        // rs1_val==2 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 944);

        // rs1_val==2 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 948);

        // rs1_val==2 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 952);

        // rs1_val==2 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 956);

        // rs1_val==2 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 960);

        // rs1_val==2 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 964);

        // rs1_val==2 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 968);

        // rs1_val==2 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 972);

        // rs1_val==2 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 976);

        // rs1_val==2 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 980);

        // rs1_val==2 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 984);

        // rs1_val==2 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 988);

        // rs1_val==2 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 992);

        // rs1_val==2 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 996);

        // rs1_val==2 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1000);

        // rs1_val==2 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1004);

        // rs1_val==2 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1008);

        // rs1_val==2 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x2;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1012);

        // rs1_val==1431655764 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1016);

        // rs1_val==1431655764 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1020);

        // rs1_val==1431655764 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1024);

        // rs1_val==1431655764 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1028);

        // rs1_val==1431655764 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1032);

        // rs1_val==1431655764 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1036);

        // rs1_val==1431655764 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1040);

        // rs1_val==1431655764 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1044);

        // rs1_val==1431655764 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1048);

        // rs1_val==1431655764 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1052);

        // rs1_val==1431655764 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1056);

        // rs1_val==1431655764 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1060);

        // rs1_val==1431655764 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1064);

        // rs1_val==1431655764 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1068);

        // rs1_val==1431655764 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1072);

        // rs1_val==1431655764 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1076);

        // rs1_val==1431655764 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1080);

        // rs1_val==1431655764 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1084);

        // rs1_val==1431655764 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1088);

        // rs1_val==1431655764 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1092);

        // rs1_val==1431655764 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1096);

        // rs1_val==1431655764 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1100);

        // rs1_val==0 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1104);

        // rs1_val==0 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1108);

        // rs1_val==0 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1112);

        // rs1_val==0 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1116);

        // rs1_val==0 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1120);

        // rs1_val==0 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1124);

        // rs1_val==0 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1128);

        // rs1_val==858993460 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1132);

        // rs1_val==858993460 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1136);

        // rs1_val==858993460 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1140);

        // rs1_val==858993460 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1144);

        // rs1_val==858993460 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1148);

        // rs1_val==858993460 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1152);

        // rs1_val==858993460 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1156);

        // rs1_val==858993460 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1160);

        // rs1_val==858993460 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1164);

        // rs1_val==858993460 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1168);

        // rs1_val==1717986919 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1172);

        // rs1_val==1717986919 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1176);

        // rs1_val==1717986919 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1180);

        // rs1_val==1717986919 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1184);

        // rs1_val==1717986919 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1188);

        // rs1_val==1717986919 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1192);

        // rs1_val==1717986919 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1196);

        // rs1_val==1717986919 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1200);

        // rs1_val==1717986919 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1204);

        // rs1_val==1717986919 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1208);

        // rs1_val==1717986919 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1212);

        // rs1_val==1717986919 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1216);

        // rs1_val==1717986919 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1220);

        // rs1_val==1717986919 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1224);

        // rs1_val==1717986919 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1228);

        // rs1_val==1717986919 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1232);

        // rs1_val==1717986919 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1236);

        // rs1_val==1717986919 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1240);

        // rs1_val==1717986919 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1244);

        // rs1_val==1717986919 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1248);

        // rs1_val==1717986919 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1252);

        // rs1_val==1717986919 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1256);

        // rs1_val==-46339 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1260);

        // rs1_val==-46339 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1264);

        // rs1_val==-46339 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1268);

        // rs1_val==-46339 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1272);

        // rs1_val==-46339 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1276);

        // rs1_val==-46339 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1280);

        // rs1_val==-46339 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1284);

        // rs1_val==-46339 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1288);

        // rs1_val==-46339 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1292);

        // rs1_val==-46339 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1296);

        // rs1_val==-46339 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1300);

        // rs1_val==-46339 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1304);

        // rs1_val==-46339 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1308);

        // rs1_val==-46339 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1312);

        // rs1_val==-46339 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1316);

        // rs1_val==-46339 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1320);

        // rs1_val==-46339 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1324);

        // rs1_val==-46339 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1328);

        // rs1_val==-46339 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1332);

        // rs1_val==-46339 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1336);

        // rs1_val==-46339 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1340);

        // rs1_val==-46339 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1344);

        // rs1_val==46341 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1348);

        // rs1_val==46341 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1352);

        // rs1_val==46341 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1356);

        // rs1_val==46341 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1360);

        // rs1_val==46341 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1364);

        // rs1_val==46341 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1368);

        // rs1_val==46341 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1372);

        // rs1_val==46341 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1376);

        // rs1_val==46341 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1380);

        // rs1_val==46341 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1384);

        // rs1_val==46341 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1388);

        // rs1_val==46341 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1392);

        // rs1_val==46341 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1396);

        // rs1_val==46341 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1400);

        // rs1_val==46341 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1404);

        // rs1_val==46341 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1408);

        // rs1_val==46341 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1412);

        // rs1_val==46341 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1416);

        // rs1_val==46341 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1420);

        // rs1_val==46341 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1424);

        // rs1_val==46341 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1428);

        // rs1_val==46341 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1432);

        // rs1_val==0 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1436);

        // rs1_val==0 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1440);

        // rs1_val==0 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1444);

        // rs1_val==0 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1448);

        // rs1_val==0 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1452);

        // rs1_val==0 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1456);

        // rs1_val==0 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1460);

        // rs1_val==0 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1464);

        // rs1_val==0 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1468);

        // rs1_val==0 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1472);

        // rs1_val==0 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1476);

        // rs1_val==0 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1480);

        // rs1_val==0 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1484);

        // rs1_val==0 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x0;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1488);

        // rs1_val==4 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1492);

        // rs1_val==4 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1496);

        // rs1_val==4 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1500);

        // rs1_val==4 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1504);

        // rs1_val==4 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1508);

        // rs1_val==4 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1512);

        // rs1_val==4 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1516);

        // rs1_val==4 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1520);

        // rs1_val==4 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1524);

        // rs1_val==4 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1528);

        // rs1_val==4 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1532);

        // rs1_val==4 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1536);

        // rs1_val==4 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1540);

        // rs1_val==4 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1544);

        // rs1_val==4 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1548);

        // rs1_val==4 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1552);

        // rs1_val==4 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1556);

        // rs1_val==4 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1560);

        // rs1_val==4 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1564);

        // rs1_val==4 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1568);

        // rs1_val==4 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x4;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1572);

        // rs1_val==858993458 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1576);

        // rs1_val==858993458 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1580);

        // rs1_val==858993458 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1584);

        // rs1_val==858993458 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1588);

        // rs1_val==858993458 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1592);

        // rs1_val==858993458 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1596);

        // rs1_val==858993458 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1600);

        // rs1_val==858993458 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1604);

        // rs1_val==858993458 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1608);

        // rs1_val==858993458 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1612);

        // rs1_val==858993458 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1616);

        // rs1_val==858993458 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1620);

        // rs1_val==858993458 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1624);

        // rs1_val==858993458 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1628);

        // rs1_val==858993458 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1632);

        // rs1_val==858993458 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1636);

        // rs1_val==858993458 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1640);

        // rs1_val==858993458 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1644);

        // rs1_val==858993458 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1648);

        // rs1_val==858993458 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1652);

        // rs1_val==858993458 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1656);

        // rs1_val==858993458 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1660);

        // rs1_val==1717986917 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1664);

        // rs1_val==1717986917 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1668);

        // rs1_val==1717986917 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1672);

        // rs1_val==1717986917 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1676);

        // rs1_val==1717986917 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1680);

        // rs1_val==1717986917 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1684);

        // rs1_val==1717986917 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1688);

        // rs1_val==1717986917 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1692);

        // rs1_val==1717986917 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1696);

        // rs1_val==1717986917 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1700);

        // rs1_val==1717986917 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1704);

        // rs1_val==1717986917 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1708);

        // rs1_val==1717986917 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1712);

        // rs1_val==1717986917 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1716);

        // rs1_val==1717986917 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1720);

        // rs1_val==1717986917 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1724);

        // rs1_val==1717986917 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1728);

        // rs1_val==1717986917 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1732);

        // rs1_val==1717986917 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1736);

        // rs1_val==1717986917 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1740);

        // rs1_val==1717986917 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1744);

        // rs1_val==1717986917 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1748);

        // rs1_val==46339 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1752);

        // rs1_val==46339 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1756);

        // rs1_val==46339 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1760);

        // rs1_val==46339 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1764);

        // rs1_val==46339 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1768);

        // rs1_val==46339 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1772);

        // rs1_val==46339 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1776);

        // rs1_val==46339 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1780);

        // rs1_val==46339 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1784);

        // rs1_val==46339 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1788);

        // rs1_val==46339 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1792);

        // rs1_val==46339 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1796);

        // rs1_val==46339 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1800);

        // rs1_val==46339 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1804);

        // rs1_val==46339 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1808);

        // rs1_val==46339 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1812);

        // rs1_val==46339 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1816);

        // rs1_val==46339 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1820);

        // rs1_val==46339 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1824);

        // rs1_val==46339 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1828);

        // rs1_val==46339 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1832);

        // rs1_val==46339 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1836);

        // rs1_val==1431655766 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1840);

        // rs1_val==1431655766 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1844);

        // rs1_val==1431655766 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1848);

        // rs1_val==1431655766 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1852);

        // rs1_val==1431655766 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1856);

        // rs1_val==1431655766 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1860);

        // rs1_val==1431655766 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1864);

        // rs1_val==1431655766 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1868);

        // rs1_val==1431655766 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1872);

        // rs1_val==1431655766 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1876);

        // rs1_val==1431655766 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1880);

        // rs1_val==1431655766 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1884);

        // rs1_val==1431655766 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1888);

        // rs1_val==1431655766 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1892);

        // rs1_val==1431655766 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1896);

        // rs1_val==1431655766 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1900);

        // rs1_val==1431655766 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1904);

        // rs1_val==1431655766 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1908);

        // rs1_val==1431655766 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1912);

        // rs1_val==1431655766 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1916);

        // rs1_val==1431655766 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1920);

        // rs1_val==1431655766 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1924);

        // rs1_val==-1431655765 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1928);

        // rs1_val==-1431655765 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1932);

        // rs1_val==-1431655765 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1936);

        // rs1_val==-1431655765 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1940);

        // rs1_val==-1431655765 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1944);

        // rs1_val==-1431655765 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1948);

        // rs1_val==-1431655765 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1952);

        // rs1_val==-1431655765 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1956);

        // rs1_val==-1431655765 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1960);

        // rs1_val==-1431655765 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1964);

        // rs1_val==-1431655765 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1968);

        // rs1_val==-1431655765 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1972);

        // rs1_val==-1431655765 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1976);

        // rs1_val==-1431655765 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1980);

        // rs1_val==-1431655765 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1984);

        // rs1_val==-1431655765 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1988);

        // rs1_val==-1431655765 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1992);

        // rs1_val==-1431655765 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1996);

        // rs1_val==-1431655765 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 2000);

        // rs1_val==-1431655765 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 2004);

        // rs1_val==-1431655765 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 2008);

        // rs1_val==-1431655765 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 2012);

        // rs1_val==6 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 2016);

        // rs1_val==6 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 2020);

        // rs1_val==6 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 2024);

        // rs1_val==6 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 2028);

        // rs1_val==6 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 2032);

        // rs1_val==6 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 2036);

        // rs1_val==6 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 2040);

        // rs1_val==6 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 2044);

        c.li(Reg::X3, 2136);
        // rs1_val==6 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 0);

        // rs1_val==6 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 4);

        // rs1_val==6 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 8);

        // rs1_val==6 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 12);

        // rs1_val==6 and imm_val==818, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 16);

        // rs1_val==6 and imm_val==1637, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 20);

        // rs1_val==6 and imm_val==44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 24);

        // rs1_val==6 and imm_val==1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 28);

        // rs1_val==6 and imm_val==-1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 32);

        // rs1_val==6 and imm_val==6, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 36);

        // rs1_val==6 and imm_val==820, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 40);

        // rs1_val==6 and imm_val==1639, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 44);

        // rs1_val==6 and imm_val==-44, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 48);

        // rs1_val==6 and imm_val==46, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x6;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 52);

        // rs1_val==858993460 and imm_val==3, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 56);

        // rs1_val==858993460 and imm_val==1365, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 60);

        // rs1_val==858993460 and imm_val==-1366, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 64);

        // rs1_val==858993460 and imm_val==5, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 68);

        // rs1_val==858993460 and imm_val==819, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 72);

        // rs1_val==858993460 and imm_val==1638, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 76);

        // rs1_val==858993460 and imm_val==-45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 80);

        // rs1_val==858993460 and imm_val==45, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 84);

        // rs1_val==858993460 and imm_val==2, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 88);

        // rs1_val==858993460 and imm_val==1364, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 92);

        // rs1_val==858993460 and imm_val==0, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 96);

        // rs1_val==858993460 and imm_val==4, 
        // opcode: addi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 100);

        // imm_val == 64, rs1_val == -2097153
        // opcode: addi ; op1:x10; dest:x11; op1val:-0x200001;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(-0x200001));
        c.addi(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X3, 104);


                
        // End
        c.ecall();
        return c;
    }

    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> addi_01::s_ScalarData;
}


// Generate google tests

TEST_F(addi_01, opcode__addi___op1_x20__dest_x7__op1val_0x20000000___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[0], 536868864);
}


TEST_F(addi_01, opcode__addi___op1_x3__dest_x3__op1val_0x400___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[1], 1024);
}


TEST_F(addi_01, opcode__addi___op1_x4__dest_x22__op1val__0x201___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[2], 1534);
}


TEST_F(addi_01, opcode__addi___op1_x30__dest_x11__op1val_0x0___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[3], 1);
}


TEST_F(addi_01, opcode__addi___op1_x27__dest_x31__op1val__0x80000000___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[4], 2147483664);
}


TEST_F(addi_01, opcode__addi___op1_x17__dest_x30__op1val_0x7fffffff___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[5], 2147483653);
}


TEST_F(addi_01, opcode__addi___op1_x18__dest_x28__op1val_0x1___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[6], 5);
}


TEST_F(addi_01, opcode__addi___op1_x13__dest_x6__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[7], 10);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x16__op1val__0x55555555___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[8], 2863311498);
}


TEST_F(addi_01, opcode__addi___op1_x9__dest_x21__op1val__0x11___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[9], 4294967281);
}


TEST_F(addi_01, opcode__addi___op1_x7__dest_x2__op1val_0xb505___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[10], 46349);
}


TEST_F(addi_01, opcode__addi___op1_x22__dest_x18__op1val__0xb504___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[11], 4294920988);
}


TEST_F(addi_01, opcode__addi___op1_x29__dest_x0__op1val__0x200001___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(addi_01, opcode__addi___op1_x25__dest_x13__op1val_0x5___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[13], 133);
}


TEST_F(addi_01, opcode__addi___op1_x11__dest_x29__op1val__0x2000001___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[14], 4261413119);
}


TEST_F(addi_01, opcode__addi___op1_x6__dest_x8__op1val_0x10___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[15], 528);
}


TEST_F(addi_01, opcode__addi___op1_x19__dest_x4__op1val_0x2___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[16], 1026);
}


TEST_F(addi_01, opcode__addi___op1_x12__dest_x10__op1val_0x55555554___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[17], 1431655762);
}


TEST_F(addi_01, opcode__addi___op1_x31__dest_x26__op1val_0x55555555___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[18], 1431655762);
}


TEST_F(addi_01, opcode__addi___op1_x26__dest_x15__op1val_0x55555554___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[19], 1431655759);
}


TEST_F(addi_01, opcode__addi___op1_x5__dest_x19__op1val_0x3___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[20], 4294967290);
}


TEST_F(addi_01, opcode__addi___op1_x0__dest_x1__op1val_0x0___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[21], 4294967279);
}


TEST_F(addi_01, opcode__addi___op1_x24__dest_x23__op1val_0x800000___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[22], 8388543);
}


TEST_F(addi_01, opcode__addi___op1_x15__dest_x27__op1val__0x11___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[23], 4294967150);
}


TEST_F(addi_01, opcode__addi___op1_x14__dest_x17__op1val_0x55555555___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[24], 1431655508);
}


TEST_F(addi_01, opcode__addi___op1_x21__dest_x25__op1val_0x55555556___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[25], 1431655253);
}


TEST_F(addi_01, opcode__addi___op1_x1__dest_x24__op1val_0x2000___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[26], 7167);
}


TEST_F(addi_01, opcode__addi___op1_x16__dest_x12__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[27], 2863312895);
}


TEST_F(addi_01, opcode__addi___op1_x2__dest_x14__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[28], 4294965930);
}


TEST_F(addi_01, opcode__addi___op1_x23__dest_x20__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[29], 8);
}


TEST_F(addi_01, opcode__addi___op1_x28__dest_x5__op1val_0x8___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[30], 24);
}


TEST_F(addi_01, opcode__addi___op1_x8__dest_x9__op1val_0x20___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[31], 4294967284);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x40___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[32], 4294967295);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x80___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[33], 133);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x100___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[34], 4294966528);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x200___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[35], 768);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x800___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[36], 2038);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x1000___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[37], 4160);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4000___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[38], 17202);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x8000___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[39], 31744);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x10000___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[40], 65279);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x20000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[41], 131076);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x40000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[42], 262144);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x80000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[43], 524293);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x100000___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[44], 1048620);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x200000___immval__0x4) {
  ASSERT_EQ(getScalarWordData()[45], 2097148);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x400000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[46], 4194350);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x1000000___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[47], 16776192);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2000000___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[48], 33554415);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4000000___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[49], 67108862);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x8000000___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[50], 134217731);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x10000000___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[51], 268435327);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x40000000___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[52], 1073741822);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x2___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[53], 4294966781);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x3___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[54], 4294967284);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[55], 4294967246);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x9___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[56], 811);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x21___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[57], 4294967258);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x41___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[58], 4294967222);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x81___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[59], 4294967150);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x101___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[60], 255);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x401___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[61], 4294966227);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x801___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[62], 4294965251);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x1001___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[63], 4294963207);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x2001___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[64], 4294958590);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x4001___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[65], 4294951935);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x8001___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[66], 4294933161);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x10001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[67], 4294901766);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x20001___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[68], 4294836351);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x40001___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[69], 4294705159);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x80001___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[70], 4294440959);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x100001___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[71], 4293918686);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x400001___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[72], 4290774355);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x800001___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[73], 4286578677);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x1000001___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[74], 4278190123);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x4000001___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[75], 4227858437);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x8000001___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[76], 4160751205);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x10000001___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[77], 4026531885);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x20000001___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[78], 3758096386);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x40000001___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[79], 3221227109);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[80], 6);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[81], 1368);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[82], 4294965933);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[83], 8);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[84], 822);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[85], 1641);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[86], 4294967254);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[87], 48);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[88], 5);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[89], 1367);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[90], 3);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[91], 7);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[92], 821);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[93], 1640);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[94], 47);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[95], 1369);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[96], 4294965934);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[97], 9);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[98], 823);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[99], 1642);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[100], 4294967255);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[101], 49);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[102], 1431655768);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[103], 1431657130);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[104], 1431654399);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[105], 1431655770);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[106], 1431656584);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[107], 1431657403);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[108], 1431655720);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[109], 1431655810);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[110], 1431655767);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[111], 1431657129);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[112], 1431655765);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[113], 1431655769);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[114], 1431656583);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[115], 1431657402);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[116], 1431655809);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[117], 1431657131);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[118], 1431654400);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[119], 1431655771);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[120], 1431656585);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[121], 1431657404);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[122], 1431655721);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[123], 1431655811);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[124], 2863311533);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[125], 2863310164);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[126], 2863311535);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[127], 2863312349);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[128], 2863313168);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[129], 2863311485);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[130], 2863311575);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[131], 2863311532);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[132], 2863312894);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[133], 2863311530);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[134], 2863311534);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[135], 2863312348);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[136], 2863313167);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[137], 2863311574);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[138], 2863312896);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[139], 2863310165);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[140], 2863311536);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[141], 2863312350);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[142], 2863313169);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[143], 2863311486);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[144], 2863311576);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[145], 8);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[146], 1370);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[147], 4294965935);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[148], 824);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[149], 1643);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[150], 4294967256);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[151], 50);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[152], 7);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[153], 1369);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[154], 5);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[155], 9);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[156], 823);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[157], 1642);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[158], 49);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[159], 1371);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[160], 4294965936);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[161], 11);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[162], 825);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[163], 1644);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[164], 4294967257);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[165], 51);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[166], 858993462);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[167], 858994824);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[168], 858992093);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[169], 858993464);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[170], 858994278);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[171], 858995097);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[172], 858993414);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[173], 858993504);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[174], 858993461);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[175], 858994823);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[176], 858993459);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[177], 858993463);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[178], 858994277);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[179], 858995096);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[180], 858993503);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[181], 858994825);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[182], 858992094);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[183], 858993465);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[184], 858994279);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[185], 858995098);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[186], 858993415);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[187], 858993505);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[188], 1717986921);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[189], 1717988283);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[190], 1717985552);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[191], 1717986923);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[192], 1717987737);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[193], 1717988556);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[194], 1717986873);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[195], 1717986963);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[196], 1717986920);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[197], 1717988282);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[198], 1717986918);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[199], 1717986922);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[200], 1717987736);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[201], 1717988555);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[202], 1717986962);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[203], 1717988284);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[204], 1717985553);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[205], 1717986924);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[206], 1717987738);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[207], 1717988557);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[208], 1717986874);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[209], 1717986964);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[210], 4294920959);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[211], 4294922321);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[212], 4294919590);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[213], 4294920961);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[214], 4294921775);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[215], 4294922594);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[216], 4294920911);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[217], 4294921001);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[218], 4294920958);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[219], 4294922320);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[220], 4294920956);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[221], 4294920960);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[222], 4294921774);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[223], 4294922593);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[224], 4294921000);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[225], 4294922322);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[226], 4294919591);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[227], 4294920962);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[228], 4294921776);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[229], 4294922595);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[230], 4294920912);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[231], 4294921002);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[232], 46343);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[233], 47705);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[234], 44974);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[235], 46345);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[236], 47159);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[237], 47978);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[238], 46295);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[239], 46385);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[240], 46342);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[241], 47704);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[242], 46340);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[243], 46344);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[244], 47158);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[245], 47977);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[246], 46384);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[247], 47706);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[248], 44975);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[249], 46346);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[250], 47160);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[251], 47979);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[252], 46296);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[253], 46386);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[254], 5);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[255], 1367);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[256], 4294965932);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[257], 7);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[258], 821);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[259], 1640);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[260], 4294967253);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[261], 47);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[262], 4);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[263], 1366);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[264], 2);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[265], 6);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[266], 820);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[267], 1639);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[268], 46);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[269], 1368);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[270], 4294965933);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[271], 8);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[272], 822);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[273], 1641);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[274], 4294967254);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[275], 48);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[276], 1431655767);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[277], 1431657129);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[278], 1431654398);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[279], 1431655769);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[280], 1431656583);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[281], 1431657402);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[282], 1431655719);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[283], 1431655809);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[284], 1431655766);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[285], 1431657128);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[286], 1431655764);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[287], 1431655768);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[288], 1431656582);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[289], 1431657401);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[290], 1431655808);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[291], 1431657130);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[292], 1431654399);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[293], 1431655770);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[294], 1431656584);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[295], 1431657403);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[296], 1431655720);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[297], 1431655810);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[298], 3);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[299], 1365);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[300], 5);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[301], 819);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[302], 1638);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[303], 4294967251);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[304], 45);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[305], 858994278);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[306], 858995097);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[307], 858993504);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[308], 858994826);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[309], 858992095);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[310], 858993466);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[311], 858994280);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[312], 858995099);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[313], 858993416);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[314], 858993506);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[315], 1717986922);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[316], 1717988284);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[317], 1717985553);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[318], 1717986924);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[319], 1717987738);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[320], 1717988557);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[321], 1717986874);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[322], 1717986964);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[323], 1717986921);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[324], 1717988283);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[325], 1717986919);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[326], 1717986923);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[327], 1717987737);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[328], 1717988556);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[329], 1717986963);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[330], 1717988285);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[331], 1717985554);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[332], 1717986925);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[333], 1717987739);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[334], 1717988558);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[335], 1717986875);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[336], 1717986965);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[337], 4294920960);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[338], 4294922322);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[339], 4294919591);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[340], 4294920962);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[341], 4294921776);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[342], 4294922595);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[343], 4294920912);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[344], 4294921002);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[345], 4294920959);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[346], 4294922321);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[347], 4294920957);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[348], 4294920961);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[349], 4294921775);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[350], 4294922594);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[351], 4294921001);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[352], 4294922323);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[353], 4294919592);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[354], 4294920963);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[355], 4294921777);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[356], 4294922596);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[357], 4294920913);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[358], 4294921003);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[359], 46344);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[360], 47706);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[361], 44975);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[362], 46346);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[363], 47160);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[364], 47979);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[365], 46296);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[366], 46386);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[367], 46343);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[368], 47705);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[369], 46341);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[370], 46345);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[371], 47159);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[372], 47978);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[373], 46385);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[374], 47707);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[375], 44976);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[376], 46347);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[377], 47161);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[378], 47980);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[379], 46297);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[380], 46387);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[381], 2);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[382], 1364);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[383], 0);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[384], 4);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[385], 818);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[386], 1637);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[387], 44);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[388], 1366);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[389], 4294965931);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[390], 6);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[391], 820);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[392], 1639);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[393], 4294967252);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[394], 46);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[395], 7);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[396], 1369);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[397], 4294965934);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[398], 9);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[399], 823);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[400], 1642);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[401], 4294967255);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[402], 49);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[403], 6);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[404], 1368);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[405], 4);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[406], 822);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[407], 1641);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[408], 48);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[409], 1370);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[410], 4294965935);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[411], 10);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[412], 824);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[413], 1643);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[414], 4294967256);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[415], 50);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[416], 858993461);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[417], 858994823);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[418], 858992092);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[419], 858993463);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[420], 858994277);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[421], 858995096);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[422], 858993413);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[423], 858993503);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[424], 858993460);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[425], 858994822);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[426], 858993458);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[427], 858993462);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[428], 858994276);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[429], 858995095);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[430], 858993502);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[431], 858994824);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[432], 858992093);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[433], 858993464);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[434], 858994278);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[435], 858995097);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[436], 858993414);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[437], 858993504);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[438], 1717986920);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[439], 1717988282);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[440], 1717985551);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[441], 1717986922);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[442], 1717987736);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[443], 1717988555);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[444], 1717986872);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[445], 1717986962);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[446], 1717986919);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[447], 1717988281);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[448], 1717986917);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[449], 1717986921);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[450], 1717987735);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[451], 1717988554);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[452], 1717986961);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[453], 1717988283);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[454], 1717985552);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[455], 1717986923);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[456], 1717987737);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[457], 1717988556);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[458], 1717986873);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[459], 1717986963);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[460], 46342);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[461], 47704);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[462], 44973);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[463], 46344);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[464], 47158);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[465], 47977);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[466], 46294);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[467], 46384);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[468], 46341);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[469], 47703);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[470], 46339);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[471], 46343);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[472], 47157);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[473], 47976);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[474], 46383);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[475], 47705);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[476], 44974);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[477], 46345);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[478], 47159);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[479], 47978);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[480], 46295);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[481], 46385);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[482], 1431655769);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[483], 1431657131);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[484], 1431654400);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[485], 1431655771);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[486], 1431656585);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[487], 1431657404);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[488], 1431655721);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[489], 1431655811);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[490], 1431655768);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[491], 1431657130);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[492], 1431655766);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[493], 1431655770);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[494], 1431656584);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[495], 1431657403);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[496], 1431655810);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[497], 1431657132);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[498], 1431654401);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[499], 1431655772);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[500], 1431656586);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[501], 1431657405);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[502], 1431655722);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[503], 1431655812);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[504], 2863311534);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[505], 2863312896);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[506], 2863310165);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[507], 2863311536);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[508], 2863312350);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[509], 2863313169);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[510], 2863311486);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[511], 2863311576);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[512], 2863311533);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[513], 2863312895);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[514], 2863311531);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[515], 2863311535);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[516], 2863312349);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[517], 2863313168);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[518], 2863311575);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[519], 2863312897);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[520], 2863310166);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[521], 2863311537);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[522], 2863312351);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[523], 2863313170);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[524], 2863311487);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[525], 2863311577);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[526], 9);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[527], 1371);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[528], 4294965936);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[529], 11);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[530], 825);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[531], 1644);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[532], 4294967257);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[533], 51);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[534], 8);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[535], 1370);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[536], 6);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[537], 10);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[538], 824);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[539], 1643);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[540], 50);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[541], 1372);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[542], 4294965937);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[543], 12);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[544], 826);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[545], 1645);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[546], 4294967258);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[547], 52);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[548], 858993463);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[549], 858994825);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[550], 858992094);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[551], 858993465);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[552], 858994279);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[553], 858995098);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[554], 858993415);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[555], 858993505);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[556], 858993462);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[557], 858994824);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[558], 858993460);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[559], 858993464);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x200001___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[560], 4292870207);
}

