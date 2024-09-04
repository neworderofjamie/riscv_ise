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
class andi_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        // Generate word-aligned vector to initialise memory
        const std::vector<uint32_t> scalarWordInitData{
        3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,};
        
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
        
        c.li(Reg::X18, 0);
        // rs1 != rd, rs1==x22, rd==x10, imm_val == (-2**(12-1)), imm_val == -2048, rs1_val != imm_val, rs1_val < 0 and imm_val < 0, rs1_val == -2
        // opcode: andi ; op1:x22; dest:x10; op1val:-0x2;  immval:-0x800
        c.li(Reg::X22, MASK_XLEN(-0x2));
        c.andi(Reg::X10, Reg::X22, SEXT_IMM(-0x800));
        c.sw(Reg::X10, Reg::X18, 0);

        // rs1 == rd, rs1==x25, rd==x25, imm_val == 0, rs1_val == -4097
        // opcode: andi ; op1:x25; dest:x25; op1val:-0x1001;  immval:0x0
        c.li(Reg::X25, MASK_XLEN(-0x1001));
        c.andi(Reg::X25, Reg::X25, SEXT_IMM(0x0));
        c.sw(Reg::X25, Reg::X18, 4);

        // rs1==x16, rd==x17, imm_val == (2**(12-1)-1), rs1_val < 0 and imm_val > 0, imm_val == 2047, rs1_val == -33554433
        // opcode: andi ; op1:x16; dest:x17; op1val:-0x2000001;  immval:0x7ff
        c.li(Reg::X16, MASK_XLEN(-0x2000001));
        c.andi(Reg::X17, Reg::X16, SEXT_IMM(0x7ff));
        c.sw(Reg::X17, Reg::X18, 8);

        // rs1==x2, rd==x8, imm_val == 1, rs1_val == -131073
        // opcode: andi ; op1:x2; dest:x8; op1val:-0x20001;  immval:0x1
        c.li(Reg::X2, MASK_XLEN(-0x20001));
        c.andi(Reg::X8, Reg::X2, SEXT_IMM(0x1));
        c.sw(Reg::X8, Reg::X18, 12);

        // rs1==x28, rd==x30, rs1_val == (-2**(xlen-1)), rs1_val == -2147483648, imm_val == 4
        // opcode: andi ; op1:x28; dest:x30; op1val:-0x80000000;  immval:0x4
        c.li(Reg::X28, MASK_XLEN(-0x80000000));
        c.andi(Reg::X30, Reg::X28, SEXT_IMM(0x4));
        c.sw(Reg::X30, Reg::X18, 16);

        // rs1==x4, rd==x19, rs1_val == 0, 
        // opcode: andi ; op1:x4; dest:x19; op1val:0x0;  immval:-0x800
        c.li(Reg::X4, MASK_XLEN(0x0));
        c.andi(Reg::X19, Reg::X4, SEXT_IMM(-0x800));
        c.sw(Reg::X19, Reg::X18, 20);

        // rs1==x10, rd==x2, rs1_val == (2**(xlen-1)-1), rs1_val > 0 and imm_val > 0, rs1_val == 2147483647
        // opcode: andi ; op1:x10; dest:x2; op1val:0x7fffffff;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x7fffffff));
        c.andi(Reg::X2, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X2, Reg::X18, 24);

        // rs1==x7, rd==x13, rs1_val == 1, 
        // opcode: andi ; op1:x7; dest:x13; op1val:0x1;  immval:0x554
        c.li(Reg::X7, MASK_XLEN(0x1));
        c.andi(Reg::X13, Reg::X7, SEXT_IMM(0x554));
        c.sw(Reg::X13, Reg::X18, 28);

        // rs1==x27, rd==x9, rs1_val == imm_val, imm_val == 128, rs1_val == 128
        // opcode: andi ; op1:x27; dest:x9; op1val:0x80;  immval:0x80
        c.li(Reg::X27, MASK_XLEN(0x80));
        c.andi(Reg::X9, Reg::X27, SEXT_IMM(0x80));
        c.sw(Reg::X9, Reg::X18, 32);

        // rs1==x17, rd==x3, rs1_val > 0 and imm_val < 0, 
        // opcode: andi ; op1:x17; dest:x3; op1val:0x7fffffff;  immval:-0x2c
        c.li(Reg::X17, MASK_XLEN(0x7fffffff));
        c.andi(Reg::X3, Reg::X17, SEXT_IMM(-0x2c));
        c.sw(Reg::X3, Reg::X18, 36);

        // rs1==x0, rd==x26, imm_val == 2, rs1_val == 16777216
        // opcode: andi ; op1:x0; dest:x26; op1val:0x0;  immval:0x2
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.andi(Reg::X26, Reg::X0, SEXT_IMM(0x2));
        c.sw(Reg::X26, Reg::X18, 40);

        // rs1==x23, rd==x21, imm_val == 8, 
        // opcode: andi ; op1:x23; dest:x21; op1val:0x66666666;  immval:0x8
        c.li(Reg::X23, MASK_XLEN(0x66666666));
        c.andi(Reg::X21, Reg::X23, SEXT_IMM(0x8));
        c.sw(Reg::X21, Reg::X18, 44);

        // rs1==x6, rd==x14, imm_val == 16, 
        // opcode: andi ; op1:x6; dest:x14; op1val:0x0;  immval:0x10
        c.li(Reg::X6, MASK_XLEN(0x0));
        c.andi(Reg::X14, Reg::X6, SEXT_IMM(0x10));
        c.sw(Reg::X14, Reg::X18, 48);

        // rs1==x5, rd==x22, imm_val == 32, rs1_val == 256
        // opcode: andi ; op1:x5; dest:x22; op1val:0x100;  immval:0x20
        c.li(Reg::X5, MASK_XLEN(0x100));
        c.andi(Reg::X22, Reg::X5, SEXT_IMM(0x20));
        c.sw(Reg::X22, Reg::X18, 52);

        // rs1==x8, rd==x29, imm_val == 64, rs1_val == -5
        // opcode: andi ; op1:x8; dest:x29; op1val:-0x5;  immval:0x40
        c.li(Reg::X8, MASK_XLEN(-0x5));
        c.andi(Reg::X29, Reg::X8, SEXT_IMM(0x40));
        c.sw(Reg::X29, Reg::X18, 56);

        // rs1==x12, rd==x23, imm_val == 256, 
        // opcode: andi ; op1:x12; dest:x23; op1val:0x1;  immval:0x100
        c.li(Reg::X12, MASK_XLEN(0x1));
        c.andi(Reg::X23, Reg::X12, SEXT_IMM(0x100));
        c.sw(Reg::X23, Reg::X18, 60);

        // rs1==x15, rd==x6, imm_val == 512, 
        // opcode: andi ; op1:x15; dest:x6; op1val:-0x55555555;  immval:0x200
        c.li(Reg::X15, MASK_XLEN(-0x55555555));
        c.andi(Reg::X6, Reg::X15, SEXT_IMM(0x200));
        c.sw(Reg::X6, Reg::X18, 64);

        // rs1==x29, rd==x11, imm_val == 1024, 
        // opcode: andi ; op1:x29; dest:x11; op1val:0x0;  immval:0x400
        c.li(Reg::X29, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X29, SEXT_IMM(0x400));
        c.sw(Reg::X11, Reg::X18, 68);

        // rs1==x20, rd==x1, imm_val == -2, 
        // opcode: andi ; op1:x20; dest:x1; op1val:0x66666667;  immval:-0x2
        c.li(Reg::X20, MASK_XLEN(0x66666667));
        c.andi(Reg::X1, Reg::X20, SEXT_IMM(-0x2));
        c.sw(Reg::X1, Reg::X18, 72);

        c.li(Reg::X2, 76);
        // rs1==x31, rd==x5, imm_val == -3, rs1_val == -1048577
        // opcode: andi ; op1:x31; dest:x5; op1val:-0x100001;  immval:-0x3
        c.li(Reg::X31, MASK_XLEN(-0x100001));
        c.andi(Reg::X5, Reg::X31, SEXT_IMM(-0x3));
        c.sw(Reg::X5, Reg::X2, 0);

        // rs1==x3, rd==x20, imm_val == -5, rs1_val == 512
        // opcode: andi ; op1:x3; dest:x20; op1val:0x200;  immval:-0x5
        c.li(Reg::X3, MASK_XLEN(0x200));
        c.andi(Reg::X20, Reg::X3, SEXT_IMM(-0x5));
        c.sw(Reg::X20, Reg::X2, 4);

        // rs1==x9, rd==x7, imm_val == -9, rs1_val == 16384
        // opcode: andi ; op1:x9; dest:x7; op1val:0x4000;  immval:-0x9
        c.li(Reg::X9, MASK_XLEN(0x4000));
        c.andi(Reg::X7, Reg::X9, SEXT_IMM(-0x9));
        c.sw(Reg::X7, Reg::X2, 8);

        // rs1==x13, rd==x16, imm_val == -17, rs1_val == 2097152
        // opcode: andi ; op1:x13; dest:x16; op1val:0x200000;  immval:-0x11
        c.li(Reg::X13, MASK_XLEN(0x200000));
        c.andi(Reg::X16, Reg::X13, SEXT_IMM(-0x11));
        c.sw(Reg::X16, Reg::X2, 12);

        // rs1==x18, rd==x24, imm_val == -33, rs1_val == -3
        // opcode: andi ; op1:x18; dest:x24; op1val:-0x3;  immval:-0x21
        c.li(Reg::X18, MASK_XLEN(-0x3));
        c.andi(Reg::X24, Reg::X18, SEXT_IMM(-0x21));
        c.sw(Reg::X24, Reg::X2, 16);

        // rs1==x26, rd==x4, imm_val == -65, 
        // opcode: andi ; op1:x26; dest:x4; op1val:-0x6;  immval:-0x41
        c.li(Reg::X26, MASK_XLEN(-0x6));
        c.andi(Reg::X4, Reg::X26, SEXT_IMM(-0x41));
        c.sw(Reg::X4, Reg::X2, 20);

        // rs1==x30, rd==x28, imm_val == -129, rs1_val == 64
        // opcode: andi ; op1:x30; dest:x28; op1val:0x40;  immval:-0x81
        c.li(Reg::X30, MASK_XLEN(0x40));
        c.andi(Reg::X28, Reg::X30, SEXT_IMM(-0x81));
        c.sw(Reg::X28, Reg::X2, 24);

        // rs1==x19, rd==x0, imm_val == -257, 
        // opcode: andi ; op1:x19; dest:x0; op1val:-0xb504;  immval:-0x101
        c.li(Reg::X19, MASK_XLEN(-0xb504));
        c.andi(Reg::X0, Reg::X19, SEXT_IMM(-0x101));
        c.sw(Reg::X0, Reg::X2, 28);

        // rs1==x21, rd==x18, imm_val == -513, rs1_val == -32769
        // opcode: andi ; op1:x21; dest:x18; op1val:-0x8001;  immval:-0x201
        c.li(Reg::X21, MASK_XLEN(-0x8001));
        c.andi(Reg::X18, Reg::X21, SEXT_IMM(-0x201));
        c.sw(Reg::X18, Reg::X2, 32);

        // rs1==x1, rd==x12, imm_val == -1025, 
        // opcode: andi ; op1:x1; dest:x12; op1val:0x40;  immval:-0x401
        c.li(Reg::X1, MASK_XLEN(0x40));
        c.andi(Reg::X12, Reg::X1, SEXT_IMM(-0x401));
        c.sw(Reg::X12, Reg::X2, 36);

        // rs1==x24, rd==x27, imm_val == 1365, rs1_val == -16385
        // opcode: andi ; op1:x24; dest:x27; op1val:-0x4001;  immval:0x555
        c.li(Reg::X24, MASK_XLEN(-0x4001));
        c.andi(Reg::X27, Reg::X24, SEXT_IMM(0x555));
        c.sw(Reg::X27, Reg::X2, 40);

        // rs1==x11, rd==x31, imm_val == -1366, rs1_val == 1073741824
        // opcode: andi ; op1:x11; dest:x31; op1val:0x40000000;  immval:-0x556
        c.li(Reg::X11, MASK_XLEN(0x40000000));
        c.andi(Reg::X31, Reg::X11, SEXT_IMM(-0x556));
        c.sw(Reg::X31, Reg::X2, 44);

        // rs1==x14, rd==x15, rs1_val == 2, 
        // opcode: andi ; op1:x14; dest:x15; op1val:0x2;  immval:0x8
        c.li(Reg::X14, MASK_XLEN(0x2));
        c.andi(Reg::X15, Reg::X14, SEXT_IMM(0x8));
        c.sw(Reg::X15, Reg::X2, 48);

        // rs1_val == 4, rs1_val==4 and imm_val==1639
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 52);

        // rs1_val == 8, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x8;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x8));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 56);

        // rs1_val == 16, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x10;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x10));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 60);

        // rs1_val == 32, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x20;  immval:0x10
        c.li(Reg::X10, MASK_XLEN(0x20));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x10));
        c.sw(Reg::X11, Reg::X2, 64);

        // rs1_val == 1024, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x400;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x400));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 68);

        // rs1_val == 2048, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x800;  immval:-0x41
        c.li(Reg::X10, MASK_XLEN(0x800));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x41));
        c.sw(Reg::X11, Reg::X2, 72);

        // rs1_val == 4096, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x1000;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x1000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 76);

        // rs1_val == 8192, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2000;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x2000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 80);

        // rs1_val == 32768, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x8000;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x8000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 84);

        // rs1_val == 65536, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x10000;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 88);

        // rs1_val == 131072, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x20000;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x20000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 92);

        // rs1_val == 262144, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x40000;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x40000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 96);

        // rs1_val == 524288, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x80000;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x80000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 100);

        // rs1_val == 1048576, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x100000;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x100000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 104);

        // rs1_val == 4194304, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x400000;  immval:-0x1
        c.li(Reg::X10, MASK_XLEN(0x400000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x1));
        c.sw(Reg::X11, Reg::X2, 108);

        // rs1_val == 8388608, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x800000;  immval:-0x6
        c.li(Reg::X10, MASK_XLEN(0x800000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x6));
        c.sw(Reg::X11, Reg::X2, 112);

        // rs1_val == 33554432, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2000000;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x2000000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 116);

        // rs1_val == 67108864, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4000000;  immval:-0x81
        c.li(Reg::X10, MASK_XLEN(0x4000000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x81));
        c.sw(Reg::X11, Reg::X2, 120);

        // rs1_val == 134217728, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x8000000;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x8000000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 124);

        // rs1_val == 268435456, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x10000000;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x10000000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 128);

        // rs1_val == 536870912, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x20000000;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x20000000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 132);

        // rs1_val == -9, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x9;  immval:-0x3
        c.li(Reg::X10, MASK_XLEN(-0x9));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x3));
        c.sw(Reg::X11, Reg::X2, 136);

        // rs1_val == -17, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x11;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0x11));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 140);

        // rs1_val == -33, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x21;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0x21));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 144);

        // rs1_val == -65, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x41;  immval:0x9
        c.li(Reg::X10, MASK_XLEN(-0x41));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x9));
        c.sw(Reg::X11, Reg::X2, 148);

        // rs1_val == -129, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x81;  immval:-0x3
        c.li(Reg::X10, MASK_XLEN(-0x81));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x3));
        c.sw(Reg::X11, Reg::X2, 152);

        // rs1_val == -257, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x101;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0x101));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 156);

        // rs1_val == -513, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x201;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0x201));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 160);

        // rs1_val == -1025, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x401;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(-0x401));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 164);

        // rs1_val == -2049, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x801;  immval:0x9
        c.li(Reg::X10, MASK_XLEN(-0x801));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x9));
        c.sw(Reg::X11, Reg::X2, 168);

        // rs1_val == -8193, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x2001;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0x2001));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 172);

        // rs1_val == -65537, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x10001;  immval:0x7ff
        c.li(Reg::X10, MASK_XLEN(-0x10001));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x7ff));
        c.sw(Reg::X11, Reg::X2, 176);

        // rs1_val == -262145, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x40001;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0x40001));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 180);

        // rs1_val == -524289, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x80001;  immval:0x200
        c.li(Reg::X10, MASK_XLEN(-0x80001));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x200));
        c.sw(Reg::X11, Reg::X2, 184);

        // rs1_val == -2097153, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x200001;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0x200001));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 188);

        // rs1_val == -4194305, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x400001;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0x400001));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 192);

        // rs1_val == -8388609, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x800001;  immval:0x7
        c.li(Reg::X10, MASK_XLEN(-0x800001));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x7));
        c.sw(Reg::X11, Reg::X2, 196);

        // rs1_val == -16777217, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x1000001;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0x1000001));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 200);

        // rs1_val == -67108865, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x4000001;  immval:0x8
        c.li(Reg::X10, MASK_XLEN(-0x4000001));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x8));
        c.sw(Reg::X11, Reg::X2, 204);

        // rs1_val == -134217729, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x8000001;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0x8000001));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 208);

        // rs1_val == -268435457, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x10000001;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(-0x10000001));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 212);

        // rs1_val == -536870913, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x20000001;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0x20000001));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 216);

        // rs1_val == -1073741825, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x40000001;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0x40000001));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 220);

        // rs1_val == 1431655765, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x5
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x5));
        c.sw(Reg::X11, Reg::X2, 224);

        // rs1_val == -1431655766, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x80
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x80));
        c.sw(Reg::X11, Reg::X2, 228);

        // rs1_val==3 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 232);

        // rs1_val==3 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 236);

        // rs1_val==3 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 240);

        // rs1_val==3 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 244);

        // rs1_val==3 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 248);

        // rs1_val==3 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 252);

        // rs1_val==3 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 256);

        // rs1_val==3 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 260);

        // rs1_val==3 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 264);

        // rs1_val==3 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 268);

        // rs1_val==3 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 272);

        // rs1_val==3 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 276);

        // rs1_val==3 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 280);

        // rs1_val==3 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 284);

        // rs1_val==3 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 288);

        // rs1_val==3 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 292);

        // rs1_val==3 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 296);

        // rs1_val==3 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 300);

        // rs1_val==3 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 304);

        // rs1_val==3 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 308);

        // rs1_val==3 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 312);

        // rs1_val==3 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x3;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 316);

        // rs1_val==1431655765 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 320);

        // rs1_val==1431655765 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 324);

        // rs1_val==1431655765 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 328);

        // rs1_val==1431655765 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 332);

        // rs1_val==1431655765 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 336);

        // rs1_val==1431655765 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 340);

        // rs1_val==1431655765 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 344);

        // rs1_val==1431655765 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 348);

        // rs1_val==1431655765 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 352);

        // rs1_val==1431655765 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 356);

        // rs1_val==1431655765 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 360);

        // rs1_val==1431655765 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 364);

        // rs1_val==1431655765 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 368);

        // rs1_val==1431655765 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 372);

        // rs1_val==1431655765 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 376);

        // rs1_val==1431655765 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 380);

        // rs1_val==1431655765 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 384);

        // rs1_val==1431655765 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 388);

        // rs1_val==1431655765 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 392);

        // rs1_val==1431655765 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 396);

        // rs1_val==1431655765 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 400);

        // rs1_val==1431655765 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 404);

        // rs1_val==-1431655766 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 408);

        // rs1_val==-1431655766 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 412);

        // rs1_val==-1431655766 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 416);

        // rs1_val==-1431655766 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 420);

        // rs1_val==-1431655766 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 424);

        // rs1_val==-1431655766 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 428);

        // rs1_val==-1431655766 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 432);

        // rs1_val==-1431655766 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 436);

        // rs1_val==-1431655766 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 440);

        // rs1_val==-1431655766 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 444);

        // rs1_val==-1431655766 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 448);

        // rs1_val==-1431655766 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 452);

        // rs1_val==-1431655766 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 456);

        // rs1_val==-1431655766 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 460);

        // rs1_val==-1431655766 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 464);

        // rs1_val==-1431655766 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 468);

        // rs1_val==-1431655766 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 472);

        // rs1_val==-1431655766 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 476);

        // rs1_val==-1431655766 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 480);

        // rs1_val==-1431655766 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 484);

        // rs1_val==-1431655766 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 488);

        // rs1_val==-1431655766 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 492);

        // rs1_val==5 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 496);

        // rs1_val==5 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 500);

        // rs1_val==5 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 504);

        // rs1_val==5 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 508);

        // rs1_val==5 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 512);

        // rs1_val==5 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 516);

        // rs1_val==5 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 520);

        // rs1_val==5 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 524);

        // rs1_val==5 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 528);

        // rs1_val==5 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 532);

        // rs1_val==5 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 536);

        // rs1_val==5 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 540);

        // rs1_val==5 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 544);

        // rs1_val==5 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 548);

        // rs1_val==5 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 552);

        // rs1_val==5 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 556);

        // rs1_val==5 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 560);

        // rs1_val==5 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 564);

        // rs1_val==5 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 568);

        // rs1_val==5 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 572);

        // rs1_val==5 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 576);

        // rs1_val==5 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x5;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 580);

        // rs1_val==858993459 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 584);

        // rs1_val==858993459 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 588);

        // rs1_val==858993459 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 592);

        // rs1_val==858993459 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 596);

        // rs1_val==858993459 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 600);

        // rs1_val==858993459 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 604);

        // rs1_val==858993459 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 608);

        // rs1_val==858993459 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 612);

        // rs1_val==858993459 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 616);

        // rs1_val==858993459 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 620);

        // rs1_val==858993459 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 624);

        // rs1_val==858993459 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 628);

        // rs1_val==858993459 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 632);

        // rs1_val==858993459 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 636);

        // rs1_val==858993459 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 640);

        // rs1_val==858993459 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 644);

        // rs1_val==858993459 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 648);

        // rs1_val==858993459 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 652);

        // rs1_val==858993459 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 656);

        // rs1_val==858993459 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 660);

        // rs1_val==858993459 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 664);

        // rs1_val==858993459 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 668);

        // rs1_val==1717986918 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 672);

        // rs1_val==1717986918 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 676);

        // rs1_val==1717986918 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 680);

        // rs1_val==1717986918 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 684);

        // rs1_val==1717986918 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 688);

        // rs1_val==1717986918 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 692);

        // rs1_val==1717986918 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 696);

        // rs1_val==1717986918 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 700);

        // rs1_val==1717986918 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 704);

        // rs1_val==1717986918 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 708);

        // rs1_val==1717986918 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 712);

        // rs1_val==1717986918 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 716);

        // rs1_val==1717986918 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 720);

        // rs1_val==1717986918 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 724);

        // rs1_val==1717986918 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 728);

        // rs1_val==1717986918 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 732);

        // rs1_val==1717986918 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 736);

        // rs1_val==1717986918 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 740);

        // rs1_val==1717986918 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 744);

        // rs1_val==1717986918 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 748);

        // rs1_val==1717986918 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 752);

        // rs1_val==1717986918 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 756);

        // rs1_val==-46340 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 760);

        // rs1_val==-46340 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 764);

        // rs1_val==-46340 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 768);

        // rs1_val==-46340 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 772);

        // rs1_val==-46340 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 776);

        // rs1_val==-46340 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 780);

        // rs1_val==-46340 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 784);

        // rs1_val==-46340 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 788);

        // rs1_val==-46340 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 792);

        // rs1_val==-46340 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 796);

        // rs1_val==-46340 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 800);

        // rs1_val==-46340 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 804);

        // rs1_val==-46340 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 808);

        // rs1_val==-46340 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 812);

        // rs1_val==-46340 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 816);

        // rs1_val==-46340 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 820);

        // rs1_val==-46340 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 824);

        // rs1_val==-46340 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 828);

        // rs1_val==-46340 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 832);

        // rs1_val==-46340 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 836);

        // rs1_val==-46340 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 840);

        // rs1_val==-46340 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 844);

        // rs1_val==46340 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 848);

        // rs1_val==46340 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 852);

        // rs1_val==46340 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 856);

        // rs1_val==46340 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 860);

        // rs1_val==46340 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 864);

        // rs1_val==46340 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 868);

        // rs1_val==46340 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 872);

        // rs1_val==46340 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 876);

        // rs1_val==46340 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 880);

        // rs1_val==46340 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 884);

        // rs1_val==46340 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 888);

        // rs1_val==46340 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 892);

        // rs1_val==46340 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 896);

        // rs1_val==46340 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 900);

        // rs1_val==46340 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 904);

        // rs1_val==46340 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 908);

        // rs1_val==46340 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 912);

        // rs1_val==46340 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 916);

        // rs1_val==46340 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 920);

        // rs1_val==46340 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 924);

        // rs1_val==46340 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 928);

        // rs1_val==46340 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 932);

        // rs1_val==2 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 936);

        // rs1_val==2 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 940);

        // rs1_val==2 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 944);

        // rs1_val==2 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 948);

        // rs1_val==2 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 952);

        // rs1_val==2 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 956);

        // rs1_val==2 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 960);

        // rs1_val==2 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 964);

        // rs1_val==2 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 968);

        // rs1_val==2 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 972);

        // rs1_val==2 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 976);

        // rs1_val==2 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 980);

        // rs1_val==2 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 984);

        // rs1_val==2 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 988);

        // rs1_val==2 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 992);

        // rs1_val==2 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 996);

        // rs1_val==2 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 1000);

        // rs1_val==2 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1004);

        // rs1_val==2 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1008);

        // rs1_val==2 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1012);

        // rs1_val==2 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 1016);

        // rs1_val==2 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x2;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1020);

        // rs1_val==1431655764 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1024);

        // rs1_val==1431655764 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1028);

        // rs1_val==1431655764 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 1032);

        // rs1_val==1431655764 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1036);

        // rs1_val==1431655764 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1040);

        // rs1_val==1431655764 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1044);

        // rs1_val==1431655764 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 1048);

        // rs1_val==1431655764 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1052);

        // rs1_val==1431655764 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1056);

        // rs1_val==1431655764 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1060);

        // rs1_val==1431655764 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1064);

        // rs1_val==1431655764 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1068);

        // rs1_val==1431655764 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1072);

        // rs1_val==1431655764 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1076);

        // rs1_val==1431655764 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1080);

        // rs1_val==1431655764 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1084);

        // rs1_val==1431655764 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 1088);

        // rs1_val==1431655764 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1092);

        // rs1_val==1431655764 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1096);

        // rs1_val==1431655764 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1100);

        // rs1_val==1431655764 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 1104);

        // rs1_val==1431655764 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1108);

        // rs1_val==0 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1112);

        // rs1_val==0 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1116);

        // rs1_val==0 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 1120);

        // rs1_val==0 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1124);

        // rs1_val==0 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1128);

        // rs1_val==0 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1132);

        // rs1_val==0 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 1136);

        // rs1_val==0 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1140);

        // rs1_val==858993460 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1144);

        // rs1_val==858993460 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1148);

        // rs1_val==858993460 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1152);

        // rs1_val==858993460 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1156);

        // rs1_val==858993460 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 1160);

        // rs1_val==858993460 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1164);

        // rs1_val==858993460 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1168);

        // rs1_val==858993460 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1172);

        // rs1_val==858993460 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 1176);

        // rs1_val==858993460 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1180);

        // rs1_val==1717986919 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1184);

        // rs1_val==1717986919 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1188);

        // rs1_val==1717986919 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 1192);

        // rs1_val==1717986919 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1196);

        // rs1_val==1717986919 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1200);

        // rs1_val==1717986919 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1204);

        // rs1_val==1717986919 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 1208);

        // rs1_val==1717986919 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1212);

        // rs1_val==1717986919 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1216);

        // rs1_val==1717986919 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1220);

        // rs1_val==1717986919 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1224);

        // rs1_val==1717986919 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1228);

        // rs1_val==1717986919 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1232);

        // rs1_val==1717986919 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1236);

        // rs1_val==1717986919 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1240);

        // rs1_val==1717986919 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1244);

        // rs1_val==1717986919 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 1248);

        // rs1_val==1717986919 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1252);

        // rs1_val==1717986919 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1256);

        // rs1_val==1717986919 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1260);

        // rs1_val==1717986919 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 1264);

        // rs1_val==1717986919 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1268);

        // rs1_val==-46339 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1272);

        // rs1_val==-46339 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1276);

        // rs1_val==-46339 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 1280);

        // rs1_val==-46339 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1284);

        // rs1_val==-46339 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1288);

        // rs1_val==-46339 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1292);

        // rs1_val==-46339 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 1296);

        // rs1_val==-46339 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1300);

        // rs1_val==-46339 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1304);

        // rs1_val==-46339 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1308);

        // rs1_val==-46339 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1312);

        // rs1_val==-46339 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1316);

        // rs1_val==-46339 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1320);

        // rs1_val==-46339 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1324);

        // rs1_val==-46339 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1328);

        // rs1_val==-46339 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1332);

        // rs1_val==-46339 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 1336);

        // rs1_val==-46339 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1340);

        // rs1_val==-46339 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1344);

        // rs1_val==-46339 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1348);

        // rs1_val==-46339 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 1352);

        // rs1_val==-46339 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1356);

        // rs1_val==46341 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1360);

        // rs1_val==46341 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1364);

        // rs1_val==46341 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 1368);

        // rs1_val==46341 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1372);

        // rs1_val==46341 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1376);

        // rs1_val==46341 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1380);

        // rs1_val==46341 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 1384);

        // rs1_val==46341 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1388);

        // rs1_val==46341 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1392);

        // rs1_val==46341 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1396);

        // rs1_val==46341 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1400);

        // rs1_val==46341 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1404);

        // rs1_val==46341 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1408);

        // rs1_val==46341 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1412);

        // rs1_val==46341 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1416);

        // rs1_val==46341 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1420);

        // rs1_val==46341 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 1424);

        // rs1_val==46341 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1428);

        // rs1_val==46341 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1432);

        // rs1_val==46341 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1436);

        // rs1_val==46341 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 1440);

        // rs1_val==46341 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1444);

        // rs1_val==0 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1448);

        // rs1_val==0 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1452);

        // rs1_val==0 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1456);

        // rs1_val==0 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1460);

        // rs1_val==0 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1464);

        // rs1_val==0 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1468);

        // rs1_val==0 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1472);

        // rs1_val==0 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1476);

        // rs1_val==0 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 1480);

        // rs1_val==0 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1484);

        // rs1_val==0 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1488);

        // rs1_val==0 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1492);

        // rs1_val==0 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 1496);

        // rs1_val==0 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x0;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1500);

        // rs1_val==4 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1504);

        // rs1_val==4 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1508);

        // rs1_val==4 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 1512);

        // rs1_val==4 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1516);

        // rs1_val==4 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1520);

        // rs1_val==4 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1524);

        // rs1_val==4 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 1528);

        // rs1_val==4 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1532);

        // rs1_val==4 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1536);

        // rs1_val==4 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1540);

        // rs1_val==4 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1544);

        // rs1_val==4 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1548);

        // rs1_val==4 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1552);

        // rs1_val==4 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1556);

        // rs1_val==4 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1560);

        // rs1_val==4 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1564);

        // rs1_val==4 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 1568);

        // rs1_val==4 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1572);

        // rs1_val==4 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1576);

        // rs1_val==4 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 1580);

        // rs1_val==4 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x4;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1584);

        // rs1_val==858993458 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1588);

        // rs1_val==858993458 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1592);

        // rs1_val==858993458 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 1596);

        // rs1_val==858993458 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1600);

        // rs1_val==858993458 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1604);

        // rs1_val==858993458 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1608);

        // rs1_val==858993458 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 1612);

        // rs1_val==858993458 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1616);

        // rs1_val==858993458 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1620);

        // rs1_val==858993458 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1624);

        // rs1_val==858993458 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1628);

        // rs1_val==858993458 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1632);

        // rs1_val==858993458 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1636);

        // rs1_val==858993458 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1640);

        // rs1_val==858993458 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1644);

        // rs1_val==858993458 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1648);

        // rs1_val==858993458 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 1652);

        // rs1_val==858993458 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1656);

        // rs1_val==858993458 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1660);

        // rs1_val==858993458 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1664);

        // rs1_val==858993458 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 1668);

        // rs1_val==858993458 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1672);

        // rs1_val==1717986917 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1676);

        // rs1_val==1717986917 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1680);

        // rs1_val==1717986917 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 1684);

        // rs1_val==1717986917 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1688);

        // rs1_val==1717986917 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1692);

        // rs1_val==1717986917 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1696);

        // rs1_val==1717986917 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 1700);

        // rs1_val==1717986917 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1704);

        // rs1_val==1717986917 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1708);

        // rs1_val==1717986917 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1712);

        // rs1_val==1717986917 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1716);

        // rs1_val==1717986917 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1720);

        // rs1_val==1717986917 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1724);

        // rs1_val==1717986917 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1728);

        // rs1_val==1717986917 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1732);

        // rs1_val==1717986917 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1736);

        // rs1_val==1717986917 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 1740);

        // rs1_val==1717986917 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1744);

        // rs1_val==1717986917 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1748);

        // rs1_val==1717986917 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1752);

        // rs1_val==1717986917 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 1756);

        // rs1_val==1717986917 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1760);

        // rs1_val==46339 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1764);

        // rs1_val==46339 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1768);

        // rs1_val==46339 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 1772);

        // rs1_val==46339 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1776);

        // rs1_val==46339 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1780);

        // rs1_val==46339 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1784);

        // rs1_val==46339 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 1788);

        // rs1_val==46339 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1792);

        // rs1_val==46339 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1796);

        // rs1_val==46339 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1800);

        // rs1_val==46339 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1804);

        // rs1_val==46339 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1808);

        // rs1_val==46339 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1812);

        // rs1_val==46339 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1816);

        // rs1_val==46339 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1820);

        // rs1_val==46339 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1824);

        // rs1_val==46339 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 1828);

        // rs1_val==46339 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1832);

        // rs1_val==46339 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1836);

        // rs1_val==46339 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1840);

        // rs1_val==46339 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 1844);

        // rs1_val==46339 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1848);

        // rs1_val==1431655766 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1852);

        // rs1_val==1431655766 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1856);

        // rs1_val==1431655766 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 1860);

        // rs1_val==1431655766 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1864);

        // rs1_val==1431655766 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1868);

        // rs1_val==1431655766 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1872);

        // rs1_val==1431655766 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 1876);

        // rs1_val==1431655766 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1880);

        // rs1_val==1431655766 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1884);

        // rs1_val==1431655766 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1888);

        // rs1_val==1431655766 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1892);

        // rs1_val==1431655766 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1896);

        // rs1_val==1431655766 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1900);

        // rs1_val==1431655766 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1904);

        // rs1_val==1431655766 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1908);

        // rs1_val==1431655766 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1912);

        // rs1_val==1431655766 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 1916);

        // rs1_val==1431655766 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1920);

        // rs1_val==1431655766 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1924);

        // rs1_val==1431655766 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1928);

        // rs1_val==1431655766 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 1932);

        // rs1_val==1431655766 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1936);

        // rs1_val==-1431655765 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1940);

        // rs1_val==-1431655765 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1944);

        // rs1_val==-1431655765 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 1948);

        // rs1_val==-1431655765 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1952);

        // rs1_val==-1431655765 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1956);

        // rs1_val==-1431655765 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1960);

        // rs1_val==-1431655765 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 1964);

        // rs1_val==-1431655765 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1968);

        // rs1_val==-1431655765 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1972);

        // rs1_val==-1431655765 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1976);

        // rs1_val==-1431655765 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1980);

        // rs1_val==-1431655765 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1984);

        // rs1_val==-1431655765 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1988);

        // rs1_val==-1431655765 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1992);

        // rs1_val==-1431655765 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1996);

        // rs1_val==-1431655765 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 2000);

        // rs1_val==-1431655765 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 2004);

        // rs1_val==-1431655765 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 2008);

        // rs1_val==-1431655765 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 2012);

        // rs1_val==-1431655765 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 2016);

        // rs1_val==-1431655765 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 2020);

        // rs1_val==-1431655765 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 2024);

        // rs1_val==6 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 2028);

        // rs1_val==6 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 2032);

        // rs1_val==6 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 2036);

        // rs1_val==6 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 2040);

        // rs1_val==6 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 2044);

        c.li(Reg::X2, 2124);
        // rs1_val==6 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 0);

        // rs1_val==6 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 4);

        // rs1_val==6 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 8);

        // rs1_val==6 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 12);

        // rs1_val==6 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 16);

        // rs1_val==6 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 20);

        // rs1_val==6 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 24);

        // rs1_val==6 and imm_val==818, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 28);

        // rs1_val==6 and imm_val==1637, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 32);

        // rs1_val==6 and imm_val==44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 36);

        // rs1_val==6 and imm_val==1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 40);

        // rs1_val==6 and imm_val==-1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X2, 44);

        // rs1_val==6 and imm_val==6, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 48);

        // rs1_val==6 and imm_val==820, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 52);

        // rs1_val==6 and imm_val==1639, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 56);

        // rs1_val==6 and imm_val==-44, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X2, 60);

        // rs1_val==6 and imm_val==46, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x6;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 64);

        // rs1_val==858993460 and imm_val==3, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 68);

        // rs1_val==858993460 and imm_val==1365, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 72);

        // rs1_val==858993460 and imm_val==-1366, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X2, 76);

        // rs1_val==858993460 and imm_val==5, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 80);

        // rs1_val==858993460 and imm_val==819, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 84);

        // rs1_val==858993460 and imm_val==1638, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 88);

        // rs1_val==858993460 and imm_val==-45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X2, 92);

        // rs1_val==858993460 and imm_val==45, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 96);

        // rs1_val==858993460 and imm_val==2, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 100);

        // rs1_val==858993460 and imm_val==1364, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 104);

        // rs1_val==858993460 and imm_val==0, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 108);

        // rs1_val==858993460 and imm_val==4, 
        // opcode: andi ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 112);

        // imm_val == 2, rs1_val == 16777216
        // opcode: andi ; op1:x10; dest:x11; op1val:0x1000000;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x1000000));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 116);

        // imm_val == -257, 
        // opcode: andi ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x101
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.andi(Reg::X11, Reg::X10, SEXT_IMM(-0x101));
        c.sw(Reg::X11, Reg::X2, 120);


                
        // End
        c.ecall();
        return c;
    }

    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> andi_01::s_ScalarData;
}


// Generate google tests

TEST_F(andi_01, opcode__andi___op1_x22__dest_x10__op1val__0x2___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[0], 4294965248);
}


TEST_F(andi_01, opcode__andi___op1_x25__dest_x25__op1val__0x1001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[1], 0);
}


TEST_F(andi_01, opcode__andi___op1_x16__dest_x17__op1val__0x2000001___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[2], 2047);
}


TEST_F(andi_01, opcode__andi___op1_x2__dest_x8__op1val__0x20001___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[3], 1);
}


TEST_F(andi_01, opcode__andi___op1_x28__dest_x30__op1val__0x80000000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[4], 0);
}


TEST_F(andi_01, opcode__andi___op1_x4__dest_x19__op1val_0x0___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x2__op1val_0x7fffffff___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[6], 6);
}


TEST_F(andi_01, opcode__andi___op1_x7__dest_x13__op1val_0x1___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[7], 0);
}


TEST_F(andi_01, opcode__andi___op1_x27__dest_x9__op1val_0x80___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[8], 128);
}


TEST_F(andi_01, opcode__andi___op1_x17__dest_x3__op1val_0x7fffffff___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[9], 2147483604);
}


TEST_F(andi_01, opcode__andi___op1_x0__dest_x26__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(andi_01, opcode__andi___op1_x23__dest_x21__op1val_0x66666666___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[11], 0);
}


TEST_F(andi_01, opcode__andi___op1_x6__dest_x14__op1val_0x0___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(andi_01, opcode__andi___op1_x5__dest_x22__op1val_0x100___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(andi_01, opcode__andi___op1_x8__dest_x29__op1val__0x5___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[14], 64);
}


TEST_F(andi_01, opcode__andi___op1_x12__dest_x23__op1val_0x1___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(andi_01, opcode__andi___op1_x15__dest_x6__op1val__0x55555555___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[16], 512);
}


TEST_F(andi_01, opcode__andi___op1_x29__dest_x11__op1val_0x0___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(andi_01, opcode__andi___op1_x20__dest_x1__op1val_0x66666667___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[18], 1717986918);
}


TEST_F(andi_01, opcode__andi___op1_x31__dest_x5__op1val__0x100001___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[19], 4293918717);
}


TEST_F(andi_01, opcode__andi___op1_x3__dest_x20__op1val_0x200___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[20], 512);
}


TEST_F(andi_01, opcode__andi___op1_x9__dest_x7__op1val_0x4000___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[21], 16384);
}


TEST_F(andi_01, opcode__andi___op1_x13__dest_x16__op1val_0x200000___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[22], 2097152);
}


TEST_F(andi_01, opcode__andi___op1_x18__dest_x24__op1val__0x3___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[23], 4294967261);
}


TEST_F(andi_01, opcode__andi___op1_x26__dest_x4__op1val__0x6___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[24], 4294967226);
}


TEST_F(andi_01, opcode__andi___op1_x30__dest_x28__op1val_0x40___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[25], 64);
}


TEST_F(andi_01, opcode__andi___op1_x19__dest_x0__op1val__0xb504___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[26], 0);
}


TEST_F(andi_01, opcode__andi___op1_x21__dest_x18__op1val__0x8001___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[27], 4294934015);
}


TEST_F(andi_01, opcode__andi___op1_x1__dest_x12__op1val_0x40___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[28], 64);
}


TEST_F(andi_01, opcode__andi___op1_x24__dest_x27__op1val__0x4001___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[29], 1365);
}


TEST_F(andi_01, opcode__andi___op1_x11__dest_x31__op1val_0x40000000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[30], 1073741824);
}


TEST_F(andi_01, opcode__andi___op1_x14__dest_x15__op1val_0x2___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[31], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[32], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x8___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[33], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x10___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[34], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x20___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[35], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x400___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[36], 1024);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x800___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[37], 2048);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x1000___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[38], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2000___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[39], 8192);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x8000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[40], 32768);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x10000___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[41], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x20000___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[42], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x40000___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[43], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x80000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[44], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x100000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[45], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x400000___immval__0x1) {
  ASSERT_EQ(getScalarWordData()[46], 4194304);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x800000___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[47], 8388608);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2000000___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[48], 33554432);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4000000___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[49], 67108864);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x8000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[50], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x10000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[51], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x20000000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[52], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x9___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[53], 4294967285);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x11___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[54], 1350);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x21___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[55], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x41___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[56], 9);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x81___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[57], 4294967165);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x101___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[58], 564);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x201___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[59], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x401___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[60], 64);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x801___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[61], 9);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x2001___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[62], 1639);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x10001___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[63], 2047);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x40001___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[64], 4294705107);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x80001___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[65], 512);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x200001___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[66], 4292870100);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x400001___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[67], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x800001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[68], 7);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x1000001___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[69], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x4000001___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[70], 8);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x8000001___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[71], 46);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x10000001___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[72], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x20000001___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[73], 3758096340);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x40000001___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[74], 1366);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[75], 1431655761);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[76], 128);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[77], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[78], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[79], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[80], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[81], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[82], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[83], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[84], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[85], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[86], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[87], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[88], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[89], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[90], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[91], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[92], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[93], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[94], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[95], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[96], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[97], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[98], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[99], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[100], 1365);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[101], 1431654400);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[102], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[103], 273);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[104], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[105], 1431655761);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[106], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[107], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[108], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[109], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[110], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[111], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[112], 1093);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[113], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[114], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[115], 1431654401);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[116], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[117], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[118], 1093);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[119], 1431655764);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[120], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[121], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[122], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[123], 2863311530);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[124], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[125], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[126], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[127], 2863311490);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[128], 40);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[129], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[130], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[131], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[132], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[133], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[134], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[135], 40);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[136], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[137], 2863311530);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[138], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[139], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[140], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[141], 2863311488);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[142], 42);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[143], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[144], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[145], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[146], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[147], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[148], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[149], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[150], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[151], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[152], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[153], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[154], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[155], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[156], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[157], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[158], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[159], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[160], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[161], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[162], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[163], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[164], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[165], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[166], 273);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[167], 858993186);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[168], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[169], 819);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[170], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[171], 858993427);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[172], 33);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[173], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[174], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[175], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[176], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[177], 818);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[178], 545);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[179], 32);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[180], 274);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[181], 858993187);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[182], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[183], 816);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[184], 547);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[185], 858993424);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[186], 34);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[187], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[188], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[189], 1717985826);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[190], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[191], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[192], 1638);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[193], 1717986882);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[194], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[195], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[196], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[197], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[198], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[199], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[200], 1636);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[201], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[202], 1094);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[203], 1717985826);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[204], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[205], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[206], 1638);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[207], 1717986884);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[208], 38);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[209], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[210], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[211], 4294920872);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[212], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[213], 560);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[214], 612);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[215], 4294920912);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[216], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[217], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[218], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[219], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[220], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[221], 560);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[222], 612);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[223], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[224], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[225], 4294920872);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[226], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[227], 564);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[228], 612);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[229], 4294920916);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[230], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[231], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[232], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[233], 45056);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[234], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[235], 256);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[236], 1028);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[237], 46336);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[238], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[239], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[240], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[241], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[242], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[243], 256);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[244], 1028);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[245], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[246], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[247], 45056);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[248], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[249], 260);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[250], 1028);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[251], 46340);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[252], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[253], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[254], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[255], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[256], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[257], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[258], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[259], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[260], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[261], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[262], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[263], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[264], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[265], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[266], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[267], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[268], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[269], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[270], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[271], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[272], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[273], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[274], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[275], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[276], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[277], 1431654400);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[278], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[279], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[280], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[281], 1431655760);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[282], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[283], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[284], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[285], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[286], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[287], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[288], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[289], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[290], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[291], 1431654400);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[292], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[293], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[294], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[295], 1431655764);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[296], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[297], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[298], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[299], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[300], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[301], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[302], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[303], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[304], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[305], 816);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[306], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[307], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[308], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[309], 858993184);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[310], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[311], 820);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[312], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[313], 858993428);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[314], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[315], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[316], 1093);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[317], 1717985826);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[318], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[319], 547);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[320], 1638);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[321], 1717986883);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[322], 37);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[323], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[324], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[325], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[326], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[327], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[328], 1637);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[329], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[330], 1094);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[331], 1717985827);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[332], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[333], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[334], 1639);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[335], 1717986884);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[336], 38);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[337], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[338], 85);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[339], 4294920872);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[340], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[341], 561);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[342], 612);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[343], 4294920913);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[344], 45);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[345], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[346], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[347], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[348], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[349], 560);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[350], 613);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[351], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[352], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[353], 4294920873);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[354], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[355], 564);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[356], 613);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[357], 4294920916);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[358], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[359], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[360], 1285);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[361], 45056);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[362], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[363], 257);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[364], 1028);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[365], 46337);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[366], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[367], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[368], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[369], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[370], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[371], 256);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[372], 1029);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[373], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[374], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[375], 45057);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[376], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[377], 260);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[378], 1029);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[379], 46340);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[380], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[381], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[382], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[383], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[384], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[385], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[386], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[387], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[388], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[389], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[390], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[391], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[392], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[393], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[394], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[395], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[396], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[397], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[398], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[399], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[400], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[401], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[402], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[403], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[404], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[405], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[406], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[407], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[408], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[409], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[410], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[411], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[412], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[413], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[414], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[415], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[416], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[417], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[418], 858993186);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[419], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[420], 818);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[421], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[422], 858993426);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[423], 32);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[424], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[425], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[426], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[427], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[428], 818);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[429], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[430], 32);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[431], 274);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[432], 858993186);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[433], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[434], 816);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[435], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[436], 858993424);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[437], 34);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[438], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[439], 1093);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[440], 1717985824);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[441], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[442], 545);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[443], 1636);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[444], 1717986881);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[445], 37);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[446], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[447], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[448], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[449], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[450], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[451], 1637);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[452], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[453], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[454], 1717985825);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[455], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[456], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[457], 1637);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[458], 1717986884);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[459], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[460], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[461], 1281);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[462], 45058);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[463], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[464], 259);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[465], 1026);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[466], 46339);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[467], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[468], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[469], 1280);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[470], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[471], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[472], 258);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[473], 1025);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[474], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[475], 1282);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[476], 45059);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[477], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[478], 256);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[479], 1027);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[480], 46336);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[481], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[482], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[483], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[484], 1431654402);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[485], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[486], 274);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[487], 1094);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[488], 1431655762);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[489], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[490], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[491], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[492], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[493], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[494], 274);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[495], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[496], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[497], 1366);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[498], 1431654402);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[499], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[500], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[501], 1094);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[502], 1431655764);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[503], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[504], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[505], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[506], 2863311530);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[507], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[508], 547);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[509], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[510], 2863311491);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[511], 41);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[512], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[513], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[514], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[515], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[516], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[517], 545);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[518], 40);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[519], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[520], 2863311531);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[521], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[522], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[523], 547);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[524], 2863311488);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[525], 42);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[526], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[527], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[528], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[529], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[530], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[531], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[532], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[533], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[534], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[535], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[536], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[537], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[538], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[539], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[540], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[541], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[542], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[543], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[544], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[545], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[546], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[547], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[548], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[549], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[550], 858993184);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[551], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[552], 816);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[553], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[554], 858993424);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[555], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[556], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[557], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[558], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[559], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x1000000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[560], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[561], 4294920956);
}

