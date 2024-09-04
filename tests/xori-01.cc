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
class xori_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        // Generate word-aligned vector to initialise memory
        const std::vector<uint32_t> scalarWordInitData{
        3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,};
        
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
        // rs1 != rd, rs1==x24, rd==x10, imm_val == (-2**(12-1)), imm_val == -2048, rs1_val != imm_val, rs1_val > 0 and imm_val < 0
        // opcode: xori ; op1:x24; dest:x10; op1val:0x33333334;  immval:-0x800
        c.li(Reg::X24, MASK_XLEN(0x33333334));
        c.xori(Reg::X10, Reg::X24, SEXT_IMM(-0x800));
        c.sw(Reg::X10, Reg::X2, 0);

        // rs1 == rd, rs1==x18, rd==x18, imm_val == 0, rs1_val == 4, rs1_val==4 and imm_val==0
        // opcode: xori ; op1:x18; dest:x18; op1val:0x4;  immval:0x0
        c.li(Reg::X18, MASK_XLEN(0x4));
        c.xori(Reg::X18, Reg::X18, SEXT_IMM(0x0));
        c.sw(Reg::X18, Reg::X2, 4);

        // rs1==x15, rd==x24, imm_val == (2**(12-1)-1), rs1_val < 0 and imm_val > 0, imm_val == 2047
        // opcode: xori ; op1:x15; dest:x24; op1val:-0x4;  immval:0x7ff
        c.li(Reg::X15, MASK_XLEN(-0x4));
        c.xori(Reg::X24, Reg::X15, SEXT_IMM(0x7ff));
        c.sw(Reg::X24, Reg::X2, 8);

        // rs1==x11, rd==x20, imm_val == 1, rs1_val > 0 and imm_val > 0, rs1_val == 2
        // opcode: xori ; op1:x11; dest:x20; op1val:0x2;  immval:0x1
        c.li(Reg::X11, MASK_XLEN(0x2));
        c.xori(Reg::X20, Reg::X11, SEXT_IMM(0x1));
        c.sw(Reg::X20, Reg::X2, 12);

        // rs1==x7, rd==x21, rs1_val == (-2**(xlen-1)), rs1_val == -2147483648
        // opcode: xori ; op1:x7; dest:x21; op1val:-0x80000000;  immval:0x554
        c.li(Reg::X7, MASK_XLEN(-0x80000000));
        c.xori(Reg::X21, Reg::X7, SEXT_IMM(0x554));
        c.sw(Reg::X21, Reg::X2, 16);

        // rs1==x17, rd==x27, rs1_val == 0, imm_val == -1025
        // opcode: xori ; op1:x17; dest:x27; op1val:0x0;  immval:-0x401
        c.li(Reg::X17, MASK_XLEN(0x0));
        c.xori(Reg::X27, Reg::X17, SEXT_IMM(-0x401));
        c.sw(Reg::X27, Reg::X2, 20);

        // rs1==x22, rd==x1, rs1_val == (2**(xlen-1)-1), rs1_val == 2147483647
        // opcode: xori ; op1:x22; dest:x1; op1val:0x7fffffff;  immval:-0xa
        c.li(Reg::X22, MASK_XLEN(0x7fffffff));
        c.xori(Reg::X1, Reg::X22, SEXT_IMM(-0xa));
        c.sw(Reg::X1, Reg::X2, 24);

        // rs1==x20, rd==x22, rs1_val == 1, imm_val == 4
        // opcode: xori ; op1:x20; dest:x22; op1val:0x1;  immval:0x4
        c.li(Reg::X20, MASK_XLEN(0x1));
        c.xori(Reg::X22, Reg::X20, SEXT_IMM(0x4));
        c.sw(Reg::X22, Reg::X2, 28);

        // rs1==x19, rd==x31, rs1_val == imm_val, imm_val == -513, rs1_val < 0 and imm_val < 0, rs1_val == -513
        // opcode: xori ; op1:x19; dest:x31; op1val:-0x201;  immval:-0x201
        c.li(Reg::X19, MASK_XLEN(-0x201));
        c.xori(Reg::X31, Reg::X19, SEXT_IMM(-0x201));
        c.sw(Reg::X31, Reg::X2, 32);

        // rs1==x9, rd==x5, imm_val == 2, rs1_val == -33
        // opcode: xori ; op1:x9; dest:x5; op1val:-0x21;  immval:0x2
        c.li(Reg::X9, MASK_XLEN(-0x21));
        c.xori(Reg::X5, Reg::X9, SEXT_IMM(0x2));
        c.sw(Reg::X5, Reg::X2, 36);

        // rs1==x28, rd==x29, imm_val == 8, 
        // opcode: xori ; op1:x28; dest:x29; op1val:-0x80000000;  immval:0x8
        c.li(Reg::X28, MASK_XLEN(-0x80000000));
        c.xori(Reg::X29, Reg::X28, SEXT_IMM(0x8));
        c.sw(Reg::X29, Reg::X2, 40);

        // rs1==x30, rd==x4, imm_val == 16, rs1_val == -1073741825
        // opcode: xori ; op1:x30; dest:x4; op1val:-0x40000001;  immval:0x10
        c.li(Reg::X30, MASK_XLEN(-0x40000001));
        c.xori(Reg::X4, Reg::X30, SEXT_IMM(0x10));
        c.sw(Reg::X4, Reg::X2, 44);

        // rs1==x27, rd==x8, imm_val == 32, 
        // opcode: xori ; op1:x27; dest:x8; op1val:0x7fffffff;  immval:0x20
        c.li(Reg::X27, MASK_XLEN(0x7fffffff));
        c.xori(Reg::X8, Reg::X27, SEXT_IMM(0x20));
        c.sw(Reg::X8, Reg::X2, 48);

        // rs1==x3, rd==x25, imm_val == 64, 
        // opcode: xori ; op1:x3; dest:x25; op1val:0x66666666;  immval:0x40
        c.li(Reg::X3, MASK_XLEN(0x66666666));
        c.xori(Reg::X25, Reg::X3, SEXT_IMM(0x40));
        c.sw(Reg::X25, Reg::X2, 52);

        // rs1==x31, rd==x17, imm_val == 128, rs1_val == -524289
        // opcode: xori ; op1:x31; dest:x17; op1val:-0x80001;  immval:0x80
        c.li(Reg::X31, MASK_XLEN(-0x80001));
        c.xori(Reg::X17, Reg::X31, SEXT_IMM(0x80));
        c.sw(Reg::X17, Reg::X2, 56);

        // rs1==x29, rd==x16, imm_val == 256, 
        // opcode: xori ; op1:x29; dest:x16; op1val:-0xb504;  immval:0x100
        c.li(Reg::X29, MASK_XLEN(-0xb504));
        c.xori(Reg::X16, Reg::X29, SEXT_IMM(0x100));
        c.sw(Reg::X16, Reg::X2, 60);

        // rs1==x4, rd==x6, imm_val == 512, 
        // opcode: xori ; op1:x4; dest:x6; op1val:0x0;  immval:0x200
        c.li(Reg::X4, MASK_XLEN(0x0));
        c.xori(Reg::X6, Reg::X4, SEXT_IMM(0x200));
        c.sw(Reg::X6, Reg::X2, 64);

        // rs1==x14, rd==x3, imm_val == 1024, rs1_val == -1048577
        // opcode: xori ; op1:x14; dest:x3; op1val:-0x100001;  immval:0x400
        c.li(Reg::X14, MASK_XLEN(-0x100001));
        c.xori(Reg::X3, Reg::X14, SEXT_IMM(0x400));
        c.sw(Reg::X3, Reg::X2, 68);

        // rs1==x12, rd==x15, imm_val == -2, 
        // opcode: xori ; op1:x12; dest:x15; op1val:-0x7;  immval:-0x2
        c.li(Reg::X12, MASK_XLEN(-0x7));
        c.xori(Reg::X15, Reg::X12, SEXT_IMM(-0x2));
        c.sw(Reg::X15, Reg::X2, 72);

        // rs1==x21, rd==x9, imm_val == -3, 
        // opcode: xori ; op1:x21; dest:x9; op1val:0x5;  immval:-0x3
        c.li(Reg::X21, MASK_XLEN(0x5));
        c.xori(Reg::X9, Reg::X21, SEXT_IMM(-0x3));
        c.sw(Reg::X9, Reg::X2, 76);

        // rs1==x10, rd==x19, imm_val == -5, 
        // opcode: xori ; op1:x10; dest:x19; op1val:-0xb504;  immval:-0x5
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X19, Reg::X10, SEXT_IMM(-0x5));
        c.sw(Reg::X19, Reg::X2, 80);

        // rs1==x16, rd==x26, imm_val == -9, 
        // opcode: xori ; op1:x16; dest:x26; op1val:0x1;  immval:-0x9
        c.li(Reg::X16, MASK_XLEN(0x1));
        c.xori(Reg::X26, Reg::X16, SEXT_IMM(-0x9));
        c.sw(Reg::X26, Reg::X2, 84);

        c.li(Reg::X3, 88);
        // rs1==x26, rd==x30, imm_val == -17, 
        // opcode: xori ; op1:x26; dest:x30; op1val:0x66666667;  immval:-0x11
        c.li(Reg::X26, MASK_XLEN(0x66666667));
        c.xori(Reg::X30, Reg::X26, SEXT_IMM(-0x11));
        c.sw(Reg::X30, Reg::X3, 0);

        // rs1==x1, rd==x23, imm_val == -33, rs1_val == -65
        // opcode: xori ; op1:x1; dest:x23; op1val:-0x41;  immval:-0x21
        c.li(Reg::X1, MASK_XLEN(-0x41));
        c.xori(Reg::X23, Reg::X1, SEXT_IMM(-0x21));
        c.sw(Reg::X23, Reg::X3, 4);

        // rs1==x6, rd==x0, imm_val == -65, 
        // opcode: xori ; op1:x6; dest:x0; op1val:-0x1;  immval:-0x41
        c.li(Reg::X6, MASK_XLEN(-0x1));
        c.xori(Reg::X0, Reg::X6, SEXT_IMM(-0x41));
        c.sw(Reg::X0, Reg::X3, 8);

        // rs1==x2, rd==x7, imm_val == -129, rs1_val == 1431655765
        // opcode: xori ; op1:x2; dest:x7; op1val:0x55555555;  immval:-0x81
        c.li(Reg::X2, MASK_XLEN(0x55555555));
        c.xori(Reg::X7, Reg::X2, SEXT_IMM(-0x81));
        c.sw(Reg::X7, Reg::X3, 12);

        // rs1==x0, rd==x14, imm_val == -257, 
        // opcode: xori ; op1:x0; dest:x14; op1val:0x0;  immval:-0x101
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.xori(Reg::X14, Reg::X0, SEXT_IMM(-0x101));
        c.sw(Reg::X14, Reg::X3, 16);

        // rs1==x8, rd==x13, imm_val == 1365, 
        // opcode: xori ; op1:x8; dest:x13; op1val:0x3fffffff;  immval:0x555
        c.li(Reg::X8, MASK_XLEN(0x3fffffff));
        c.xori(Reg::X13, Reg::X8, SEXT_IMM(0x555));
        c.sw(Reg::X13, Reg::X3, 20);

        // rs1==x25, rd==x12, imm_val == -1366, 
        // opcode: xori ; op1:x25; dest:x12; op1val:-0x7;  immval:-0x556
        c.li(Reg::X25, MASK_XLEN(-0x7));
        c.xori(Reg::X12, Reg::X25, SEXT_IMM(-0x556));
        c.sw(Reg::X12, Reg::X3, 24);

        // rs1==x13, rd==x11, rs1_val == 8, 
        // opcode: xori ; op1:x13; dest:x11; op1val:0x8;  immval:0x0
        c.li(Reg::X13, MASK_XLEN(0x8));
        c.xori(Reg::X11, Reg::X13, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 28);

        // rs1==x23, rd==x2, rs1_val == 16, 
        // opcode: xori ; op1:x23; dest:x2; op1val:0x10;  immval:0x1
        c.li(Reg::X23, MASK_XLEN(0x10));
        c.xori(Reg::X2, Reg::X23, SEXT_IMM(0x1));
        c.sw(Reg::X2, Reg::X3, 32);

        // rs1==x5, rd==x28, rs1_val == 32, 
        // opcode: xori ; op1:x5; dest:x28; op1val:0x20;  immval:0x555
        c.li(Reg::X5, MASK_XLEN(0x20));
        c.xori(Reg::X28, Reg::X5, SEXT_IMM(0x555));
        c.sw(Reg::X28, Reg::X3, 36);

        // rs1_val == 64, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x40;  immval:0x100
        c.li(Reg::X10, MASK_XLEN(0x40));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x100));
        c.sw(Reg::X11, Reg::X3, 40);

        // rs1_val == 128, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x80;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x80));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 44);

        // rs1_val == 256, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x100;  immval:-0x41
        c.li(Reg::X10, MASK_XLEN(0x100));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x41));
        c.sw(Reg::X11, Reg::X3, 48);

        // rs1_val == 512, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x200;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x200));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 52);

        // rs1_val == 1024, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x400;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x400));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 56);

        // rs1_val == 2048, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x800;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x800));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 60);

        // rs1_val == 4096, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x1000;  immval:0x8
        c.li(Reg::X10, MASK_XLEN(0x1000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x8));
        c.sw(Reg::X11, Reg::X3, 64);

        // rs1_val == 8192, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2000;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x2000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 68);

        // rs1_val == 16384, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4000;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x4000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 72);

        // rs1_val == 32768, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x8000;  immval:0x7ff
        c.li(Reg::X10, MASK_XLEN(0x8000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x7ff));
        c.sw(Reg::X11, Reg::X3, 76);

        // rs1_val == 65536, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x10000;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X3, 80);

        // rs1_val == 131072, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x20000;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x20000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 84);

        // rs1_val == 262144, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x40000;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x40000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 88);

        // rs1_val == 524288, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x80000;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x80000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 92);

        // rs1_val == 1048576, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x100000;  immval:-0x6
        c.li(Reg::X10, MASK_XLEN(0x100000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x6));
        c.sw(Reg::X11, Reg::X3, 96);

        // rs1_val == 2097152, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x200000;  immval:-0x3
        c.li(Reg::X10, MASK_XLEN(0x200000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x3));
        c.sw(Reg::X11, Reg::X3, 100);

        // rs1_val == 4194304, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x400000;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x400000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 104);

        // rs1_val == 8388608, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x800000;  immval:-0x9
        c.li(Reg::X10, MASK_XLEN(0x800000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x9));
        c.sw(Reg::X11, Reg::X3, 108);

        // rs1_val == 16777216, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x1000000;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x1000000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 112);

        // rs1_val == 33554432, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2000000;  immval:-0x201
        c.li(Reg::X10, MASK_XLEN(0x2000000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x201));
        c.sw(Reg::X11, Reg::X3, 116);

        // rs1_val == 67108864, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4000000;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x4000000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 120);

        // rs1_val == 134217728, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x8000000;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x8000000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 124);

        // rs1_val == 268435456, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x10000000;  immval:-0x41
        c.li(Reg::X10, MASK_XLEN(0x10000000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x41));
        c.sw(Reg::X11, Reg::X3, 128);

        // rs1_val == 536870912, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x20000000;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x20000000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 132);

        // rs1_val == 1073741824, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x40000000;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x40000000));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 136);

        // rs1_val == -2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x2;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 140);

        // rs1_val == -3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x3;  immval:-0x3
        c.li(Reg::X10, MASK_XLEN(-0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x3));
        c.sw(Reg::X11, Reg::X3, 144);

        // rs1_val == -5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x5;  immval:-0x400
        c.li(Reg::X10, MASK_XLEN(-0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x400));
        c.sw(Reg::X11, Reg::X3, 148);

        // rs1_val == -9, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x9;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(-0x9));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 152);

        // rs1_val == -17, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x11;  immval:0x80
        c.li(Reg::X10, MASK_XLEN(-0x11));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x80));
        c.sw(Reg::X11, Reg::X3, 156);

        // rs1_val == -129, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x81;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0x81));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 160);

        // rs1_val == -257, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x101;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0x101));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 164);

        // rs1_val == -1025, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x401;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0x401));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 168);

        // rs1_val == -2049, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x801;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0x801));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 172);

        // rs1_val == -4097, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x1001;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0x1001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 176);

        // rs1_val == -8193, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x2001;  immval:0x7ff
        c.li(Reg::X10, MASK_XLEN(-0x2001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x7ff));
        c.sw(Reg::X11, Reg::X3, 180);

        // rs1_val == -16385, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x4001;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0x4001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 184);

        // rs1_val == -32769, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x8001;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0x8001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 188);

        // rs1_val == -65537, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x10001;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(-0x10001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 192);

        // rs1_val == -131073, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x20001;  immval:-0x101
        c.li(Reg::X10, MASK_XLEN(-0x20001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x101));
        c.sw(Reg::X11, Reg::X3, 196);

        // rs1_val == -262145, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x40001;  immval:-0x800
        c.li(Reg::X10, MASK_XLEN(-0x40001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x800));
        c.sw(Reg::X11, Reg::X3, 200);

        // rs1_val == -2097153, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x200001;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0x200001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 204);

        // rs1_val == -4194305, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x400001;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0x400001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 208);

        // rs1_val == -8388609, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x800001;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0x800001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 212);

        // rs1_val == -16777217, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x1000001;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0x1000001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 216);

        // rs1_val == -33554433, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x2000001;  immval:-0x8
        c.li(Reg::X10, MASK_XLEN(-0x2000001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x8));
        c.sw(Reg::X11, Reg::X3, 220);

        // rs1_val == -67108865, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x4000001;  immval:-0x2
        c.li(Reg::X10, MASK_XLEN(-0x4000001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2));
        c.sw(Reg::X11, Reg::X3, 224);

        // rs1_val == -134217729, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x8000001;  immval:-0x400
        c.li(Reg::X10, MASK_XLEN(-0x8000001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x400));
        c.sw(Reg::X11, Reg::X3, 228);

        // rs1_val == -268435457, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x10000001;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0x10000001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 232);

        // rs1_val == -536870913, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x20000001;  immval:-0x101
        c.li(Reg::X10, MASK_XLEN(-0x20000001));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x101));
        c.sw(Reg::X11, Reg::X3, 236);

        // rs1_val == -1431655766, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x200
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x200));
        c.sw(Reg::X11, Reg::X3, 240);

        // rs1_val==3 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 244);

        // rs1_val==3 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 248);

        // rs1_val==3 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 252);

        // rs1_val==3 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 256);

        // rs1_val==3 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 260);

        // rs1_val==3 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 264);

        // rs1_val==3 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 268);

        // rs1_val==3 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 272);

        // rs1_val==3 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 276);

        // rs1_val==3 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 280);

        // rs1_val==3 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 284);

        // rs1_val==3 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 288);

        // rs1_val==3 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 292);

        // rs1_val==3 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 296);

        // rs1_val==3 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 300);

        // rs1_val==3 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 304);

        // rs1_val==3 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 308);

        // rs1_val==3 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 312);

        // rs1_val==3 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 316);

        // rs1_val==3 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 320);

        // rs1_val==3 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 324);

        // rs1_val==3 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x3;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 328);

        // rs1_val==1431655765 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 332);

        // rs1_val==1431655765 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 336);

        // rs1_val==1431655765 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 340);

        // rs1_val==1431655765 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 344);

        // rs1_val==1431655765 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 348);

        // rs1_val==1431655765 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 352);

        // rs1_val==1431655765 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 356);

        // rs1_val==1431655765 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 360);

        // rs1_val==1431655765 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 364);

        // rs1_val==1431655765 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 368);

        // rs1_val==1431655765 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 372);

        // rs1_val==1431655765 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 376);

        // rs1_val==1431655765 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 380);

        // rs1_val==1431655765 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 384);

        // rs1_val==1431655765 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 388);

        // rs1_val==1431655765 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 392);

        // rs1_val==1431655765 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 396);

        // rs1_val==1431655765 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 400);

        // rs1_val==1431655765 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 404);

        // rs1_val==1431655765 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 408);

        // rs1_val==1431655765 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 412);

        // rs1_val==1431655765 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 416);

        // rs1_val==-1431655766 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 420);

        // rs1_val==-1431655766 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 424);

        // rs1_val==-1431655766 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 428);

        // rs1_val==-1431655766 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 432);

        // rs1_val==-1431655766 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 436);

        // rs1_val==-1431655766 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 440);

        // rs1_val==-1431655766 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 444);

        // rs1_val==-1431655766 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 448);

        // rs1_val==-1431655766 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 452);

        // rs1_val==-1431655766 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 456);

        // rs1_val==-1431655766 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 460);

        // rs1_val==-1431655766 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 464);

        // rs1_val==-1431655766 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 468);

        // rs1_val==-1431655766 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 472);

        // rs1_val==-1431655766 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 476);

        // rs1_val==-1431655766 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 480);

        // rs1_val==-1431655766 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 484);

        // rs1_val==-1431655766 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 488);

        // rs1_val==-1431655766 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 492);

        // rs1_val==-1431655766 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 496);

        // rs1_val==-1431655766 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 500);

        // rs1_val==-1431655766 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555556;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 504);

        // rs1_val==5 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 508);

        // rs1_val==5 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 512);

        // rs1_val==5 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 516);

        // rs1_val==5 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 520);

        // rs1_val==5 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 524);

        // rs1_val==5 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 528);

        // rs1_val==5 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 532);

        // rs1_val==5 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 536);

        // rs1_val==5 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 540);

        // rs1_val==5 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 544);

        // rs1_val==5 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 548);

        // rs1_val==5 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 552);

        // rs1_val==5 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 556);

        // rs1_val==5 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 560);

        // rs1_val==5 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 564);

        // rs1_val==5 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 568);

        // rs1_val==5 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 572);

        // rs1_val==5 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 576);

        // rs1_val==5 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 580);

        // rs1_val==5 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 584);

        // rs1_val==5 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 588);

        // rs1_val==5 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x5;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 592);

        // rs1_val==858993459 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 596);

        // rs1_val==858993459 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 600);

        // rs1_val==858993459 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 604);

        // rs1_val==858993459 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 608);

        // rs1_val==858993459 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 612);

        // rs1_val==858993459 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 616);

        // rs1_val==858993459 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 620);

        // rs1_val==858993459 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 624);

        // rs1_val==858993459 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 628);

        // rs1_val==858993459 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 632);

        // rs1_val==858993459 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 636);

        // rs1_val==858993459 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 640);

        // rs1_val==858993459 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 644);

        // rs1_val==858993459 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 648);

        // rs1_val==858993459 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 652);

        // rs1_val==858993459 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 656);

        // rs1_val==858993459 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 660);

        // rs1_val==858993459 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 664);

        // rs1_val==858993459 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 668);

        // rs1_val==858993459 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 672);

        // rs1_val==858993459 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 676);

        // rs1_val==858993459 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 680);

        // rs1_val==1717986918 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 684);

        // rs1_val==1717986918 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 688);

        // rs1_val==1717986918 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 692);

        // rs1_val==1717986918 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 696);

        // rs1_val==1717986918 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 700);

        // rs1_val==1717986918 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 704);

        // rs1_val==1717986918 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 708);

        // rs1_val==1717986918 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 712);

        // rs1_val==1717986918 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 716);

        // rs1_val==1717986918 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 720);

        // rs1_val==1717986918 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 724);

        // rs1_val==1717986918 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 728);

        // rs1_val==1717986918 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 732);

        // rs1_val==1717986918 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 736);

        // rs1_val==1717986918 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 740);

        // rs1_val==1717986918 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 744);

        // rs1_val==1717986918 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 748);

        // rs1_val==1717986918 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 752);

        // rs1_val==1717986918 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 756);

        // rs1_val==1717986918 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 760);

        // rs1_val==1717986918 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 764);

        // rs1_val==1717986918 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 768);

        // rs1_val==-46340 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 772);

        // rs1_val==-46340 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 776);

        // rs1_val==-46340 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 780);

        // rs1_val==-46340 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 784);

        // rs1_val==-46340 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 788);

        // rs1_val==-46340 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 792);

        // rs1_val==-46340 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 796);

        // rs1_val==-46340 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 800);

        // rs1_val==-46340 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 804);

        // rs1_val==-46340 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 808);

        // rs1_val==-46340 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 812);

        // rs1_val==-46340 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 816);

        // rs1_val==-46340 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 820);

        // rs1_val==-46340 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 824);

        // rs1_val==-46340 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 828);

        // rs1_val==-46340 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 832);

        // rs1_val==-46340 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 836);

        // rs1_val==-46340 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 840);

        // rs1_val==-46340 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 844);

        // rs1_val==-46340 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 848);

        // rs1_val==-46340 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 852);

        // rs1_val==-46340 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb504;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 856);

        // rs1_val==46340 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 860);

        // rs1_val==46340 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 864);

        // rs1_val==46340 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 868);

        // rs1_val==46340 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 872);

        // rs1_val==46340 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 876);

        // rs1_val==46340 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 880);

        // rs1_val==46340 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 884);

        // rs1_val==46340 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 888);

        // rs1_val==46340 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 892);

        // rs1_val==46340 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 896);

        // rs1_val==46340 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 900);

        // rs1_val==46340 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 904);

        // rs1_val==46340 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 908);

        // rs1_val==46340 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 912);

        // rs1_val==46340 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 916);

        // rs1_val==46340 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 920);

        // rs1_val==46340 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 924);

        // rs1_val==46340 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 928);

        // rs1_val==46340 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 932);

        // rs1_val==46340 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 936);

        // rs1_val==46340 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 940);

        // rs1_val==46340 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 944);

        // rs1_val==2 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 948);

        // rs1_val==2 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 952);

        // rs1_val==2 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 956);

        // rs1_val==2 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 960);

        // rs1_val==2 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 964);

        // rs1_val==2 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 968);

        // rs1_val==2 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 972);

        // rs1_val==2 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 976);

        // rs1_val==2 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 980);

        // rs1_val==2 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 984);

        // rs1_val==2 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 988);

        // rs1_val==2 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 992);

        // rs1_val==2 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 996);

        // rs1_val==2 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1000);

        // rs1_val==2 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1004);

        // rs1_val==2 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1008);

        // rs1_val==2 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1012);

        // rs1_val==2 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1016);

        // rs1_val==2 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1020);

        // rs1_val==2 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1024);

        // rs1_val==2 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1028);

        // rs1_val==2 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x2;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1032);

        // rs1_val==1431655764 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1036);

        // rs1_val==1431655764 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1040);

        // rs1_val==1431655764 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1044);

        // rs1_val==1431655764 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1048);

        // rs1_val==1431655764 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1052);

        // rs1_val==1431655764 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1056);

        // rs1_val==1431655764 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1060);

        // rs1_val==1431655764 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1064);

        // rs1_val==1431655764 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1068);

        // rs1_val==1431655764 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1072);

        // rs1_val==1431655764 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1076);

        // rs1_val==1431655764 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1080);

        // rs1_val==1431655764 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1084);

        // rs1_val==1431655764 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1088);

        // rs1_val==1431655764 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1092);

        // rs1_val==1431655764 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1096);

        // rs1_val==1431655764 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1100);

        // rs1_val==1431655764 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1104);

        // rs1_val==1431655764 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1108);

        // rs1_val==1431655764 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1112);

        // rs1_val==1431655764 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1116);

        // rs1_val==1431655764 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1120);

        // rs1_val==0 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1124);

        // rs1_val==0 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1128);

        // rs1_val==0 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1132);

        // rs1_val==0 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1136);

        // rs1_val==0 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1140);

        // rs1_val==0 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1144);

        // rs1_val==0 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1148);

        // rs1_val==0 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1152);

        // rs1_val==858993460 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1156);

        // rs1_val==858993460 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1160);

        // rs1_val==858993460 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1164);

        // rs1_val==858993460 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1168);

        // rs1_val==858993460 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1172);

        // rs1_val==858993460 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1176);

        // rs1_val==858993460 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1180);

        // rs1_val==858993460 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1184);

        // rs1_val==858993460 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1188);

        // rs1_val==858993460 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1192);

        // rs1_val==1717986919 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1196);

        // rs1_val==1717986919 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1200);

        // rs1_val==1717986919 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1204);

        // rs1_val==1717986919 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1208);

        // rs1_val==1717986919 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1212);

        // rs1_val==1717986919 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1216);

        // rs1_val==1717986919 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1220);

        // rs1_val==1717986919 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1224);

        // rs1_val==1717986919 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1228);

        // rs1_val==1717986919 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1232);

        // rs1_val==1717986919 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1236);

        // rs1_val==1717986919 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1240);

        // rs1_val==1717986919 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1244);

        // rs1_val==1717986919 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1248);

        // rs1_val==1717986919 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1252);

        // rs1_val==1717986919 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1256);

        // rs1_val==1717986919 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1260);

        // rs1_val==1717986919 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1264);

        // rs1_val==1717986919 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1268);

        // rs1_val==1717986919 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1272);

        // rs1_val==1717986919 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1276);

        // rs1_val==1717986919 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1280);

        // rs1_val==-46339 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1284);

        // rs1_val==-46339 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1288);

        // rs1_val==-46339 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1292);

        // rs1_val==-46339 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1296);

        // rs1_val==-46339 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1300);

        // rs1_val==-46339 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1304);

        // rs1_val==-46339 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1308);

        // rs1_val==-46339 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1312);

        // rs1_val==-46339 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1316);

        // rs1_val==-46339 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1320);

        // rs1_val==-46339 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1324);

        // rs1_val==-46339 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1328);

        // rs1_val==-46339 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1332);

        // rs1_val==-46339 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1336);

        // rs1_val==-46339 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1340);

        // rs1_val==-46339 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1344);

        // rs1_val==-46339 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1348);

        // rs1_val==-46339 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1352);

        // rs1_val==-46339 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1356);

        // rs1_val==-46339 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1360);

        // rs1_val==-46339 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1364);

        // rs1_val==-46339 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0xb503;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1368);

        // rs1_val==46341 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1372);

        // rs1_val==46341 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1376);

        // rs1_val==46341 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1380);

        // rs1_val==46341 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1384);

        // rs1_val==46341 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1388);

        // rs1_val==46341 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1392);

        // rs1_val==46341 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1396);

        // rs1_val==46341 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1400);

        // rs1_val==46341 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1404);

        // rs1_val==46341 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1408);

        // rs1_val==46341 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1412);

        // rs1_val==46341 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1416);

        // rs1_val==46341 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1420);

        // rs1_val==46341 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1424);

        // rs1_val==46341 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1428);

        // rs1_val==46341 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1432);

        // rs1_val==46341 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1436);

        // rs1_val==46341 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1440);

        // rs1_val==46341 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1444);

        // rs1_val==46341 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1448);

        // rs1_val==46341 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1452);

        // rs1_val==46341 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1456);

        // rs1_val==0 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1460);

        // rs1_val==0 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1464);

        // rs1_val==0 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1468);

        // rs1_val==0 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1472);

        // rs1_val==0 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1476);

        // rs1_val==0 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1480);

        // rs1_val==0 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1484);

        // rs1_val==0 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1488);

        // rs1_val==0 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1492);

        // rs1_val==0 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1496);

        // rs1_val==0 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1500);

        // rs1_val==0 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1504);

        // rs1_val==0 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1508);

        // rs1_val==0 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x0;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1512);

        // rs1_val==4 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1516);

        // rs1_val==4 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1520);

        // rs1_val==4 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1524);

        // rs1_val==4 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1528);

        // rs1_val==4 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1532);

        // rs1_val==4 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1536);

        // rs1_val==4 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1540);

        // rs1_val==4 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1544);

        // rs1_val==4 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1548);

        // rs1_val==4 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1552);

        // rs1_val==4 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1556);

        // rs1_val==4 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1560);

        // rs1_val==4 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1564);

        // rs1_val==4 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1568);

        // rs1_val==4 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1572);

        // rs1_val==4 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1576);

        // rs1_val==4 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1580);

        // rs1_val==4 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1584);

        // rs1_val==4 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1588);

        // rs1_val==4 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1592);

        // rs1_val==4 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x4;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1596);

        // rs1_val==858993458 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1600);

        // rs1_val==858993458 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1604);

        // rs1_val==858993458 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1608);

        // rs1_val==858993458 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1612);

        // rs1_val==858993458 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1616);

        // rs1_val==858993458 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1620);

        // rs1_val==858993458 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1624);

        // rs1_val==858993458 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1628);

        // rs1_val==858993458 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1632);

        // rs1_val==858993458 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1636);

        // rs1_val==858993458 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1640);

        // rs1_val==858993458 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1644);

        // rs1_val==858993458 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1648);

        // rs1_val==858993458 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1652);

        // rs1_val==858993458 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1656);

        // rs1_val==858993458 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1660);

        // rs1_val==858993458 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1664);

        // rs1_val==858993458 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1668);

        // rs1_val==858993458 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1672);

        // rs1_val==858993458 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1676);

        // rs1_val==858993458 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1680);

        // rs1_val==858993458 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1684);

        // rs1_val==1717986917 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1688);

        // rs1_val==1717986917 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1692);

        // rs1_val==1717986917 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1696);

        // rs1_val==1717986917 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1700);

        // rs1_val==1717986917 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1704);

        // rs1_val==1717986917 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1708);

        // rs1_val==1717986917 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1712);

        // rs1_val==1717986917 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1716);

        // rs1_val==1717986917 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1720);

        // rs1_val==1717986917 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1724);

        // rs1_val==1717986917 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1728);

        // rs1_val==1717986917 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1732);

        // rs1_val==1717986917 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1736);

        // rs1_val==1717986917 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1740);

        // rs1_val==1717986917 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1744);

        // rs1_val==1717986917 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1748);

        // rs1_val==1717986917 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1752);

        // rs1_val==1717986917 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1756);

        // rs1_val==1717986917 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1760);

        // rs1_val==1717986917 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1764);

        // rs1_val==1717986917 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1768);

        // rs1_val==1717986917 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1772);

        // rs1_val==46339 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1776);

        // rs1_val==46339 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1780);

        // rs1_val==46339 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1784);

        // rs1_val==46339 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1788);

        // rs1_val==46339 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1792);

        // rs1_val==46339 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1796);

        // rs1_val==46339 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1800);

        // rs1_val==46339 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1804);

        // rs1_val==46339 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1808);

        // rs1_val==46339 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1812);

        // rs1_val==46339 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1816);

        // rs1_val==46339 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1820);

        // rs1_val==46339 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1824);

        // rs1_val==46339 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1828);

        // rs1_val==46339 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1832);

        // rs1_val==46339 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1836);

        // rs1_val==46339 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1840);

        // rs1_val==46339 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1844);

        // rs1_val==46339 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1848);

        // rs1_val==46339 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1852);

        // rs1_val==46339 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1856);

        // rs1_val==46339 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1860);

        // rs1_val==1431655766 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1864);

        // rs1_val==1431655766 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1868);

        // rs1_val==1431655766 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1872);

        // rs1_val==1431655766 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1876);

        // rs1_val==1431655766 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1880);

        // rs1_val==1431655766 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1884);

        // rs1_val==1431655766 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1888);

        // rs1_val==1431655766 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1892);

        // rs1_val==1431655766 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1896);

        // rs1_val==1431655766 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1900);

        // rs1_val==1431655766 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1904);

        // rs1_val==1431655766 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1908);

        // rs1_val==1431655766 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 1912);

        // rs1_val==1431655766 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 1916);

        // rs1_val==1431655766 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 1920);

        // rs1_val==1431655766 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 1924);

        // rs1_val==1431655766 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 1928);

        // rs1_val==1431655766 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 1932);

        // rs1_val==1431655766 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 1936);

        // rs1_val==1431655766 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 1940);

        // rs1_val==1431655766 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 1944);

        // rs1_val==1431655766 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 1948);

        // rs1_val==-1431655765 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 1952);

        // rs1_val==-1431655765 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 1956);

        // rs1_val==-1431655765 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 1960);

        // rs1_val==-1431655765 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 1964);

        // rs1_val==-1431655765 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 1968);

        // rs1_val==-1431655765 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 1972);

        // rs1_val==-1431655765 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 1976);

        // rs1_val==-1431655765 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 1980);

        // rs1_val==-1431655765 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 1984);

        // rs1_val==-1431655765 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 1988);

        // rs1_val==-1431655765 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 1992);

        // rs1_val==-1431655765 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 1996);

        // rs1_val==-1431655765 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 2000);

        // rs1_val==-1431655765 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 2004);

        // rs1_val==-1431655765 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 2008);

        // rs1_val==-1431655765 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 2012);

        // rs1_val==-1431655765 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 2016);

        // rs1_val==-1431655765 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 2020);

        // rs1_val==-1431655765 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 2024);

        // rs1_val==-1431655765 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 2028);

        // rs1_val==-1431655765 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 2032);

        // rs1_val==-1431655765 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:-0x55555555;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(-0x55555555));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 2036);

        // rs1_val==6 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 2040);

        // rs1_val==6 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 2044);

        c.li(Reg::X3, 2136);
        // rs1_val==6 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 0);

        // rs1_val==6 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 4);

        // rs1_val==6 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 8);

        // rs1_val==6 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 12);

        // rs1_val==6 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 16);

        // rs1_val==6 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 20);

        // rs1_val==6 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 24);

        // rs1_val==6 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 28);

        // rs1_val==6 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 32);

        // rs1_val==6 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 36);

        // rs1_val==6 and imm_val==818, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X3, 40);

        // rs1_val==6 and imm_val==1637, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X3, 44);

        // rs1_val==6 and imm_val==44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X3, 48);

        // rs1_val==6 and imm_val==1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X3, 52);

        // rs1_val==6 and imm_val==-1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:-0x555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x555));
        c.sw(Reg::X11, Reg::X3, 56);

        // rs1_val==6 and imm_val==6, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X3, 60);

        // rs1_val==6 and imm_val==820, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X3, 64);

        // rs1_val==6 and imm_val==1639, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X3, 68);

        // rs1_val==6 and imm_val==-44, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:-0x2c
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2c));
        c.sw(Reg::X11, Reg::X3, 72);

        // rs1_val==6 and imm_val==46, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x6;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X3, 76);

        // rs1_val==858993460 and imm_val==3, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X3, 80);

        // rs1_val==858993460 and imm_val==1365, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X3, 84);

        // rs1_val==858993460 and imm_val==-1366, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x556));
        c.sw(Reg::X11, Reg::X3, 88);

        // rs1_val==858993460 and imm_val==5, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X3, 92);

        // rs1_val==858993460 and imm_val==819, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X3, 96);

        // rs1_val==858993460 and imm_val==1638, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X3, 100);

        // rs1_val==858993460 and imm_val==-45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:-0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(-0x2d));
        c.sw(Reg::X11, Reg::X3, 104);

        // rs1_val==858993460 and imm_val==45, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X3, 108);

        // rs1_val==858993460 and imm_val==2, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X3, 112);

        // rs1_val==858993460 and imm_val==1364, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X3, 116);

        // rs1_val==858993460 and imm_val==0, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X3, 120);

        // rs1_val==858993460 and imm_val==4, 
        // opcode: xori ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.xori(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X3, 124);


                
        // End
        c.ecall();
        return c;
    }

    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> xori_01::s_ScalarData;
}


// Generate google tests

TEST_F(xori_01, opcode__xori___op1_x24__dest_x10__op1val_0x33333334___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[0], 3435973428);
}


TEST_F(xori_01, opcode__xori___op1_x18__dest_x18__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[1], 4);
}


TEST_F(xori_01, opcode__xori___op1_x15__dest_x24__op1val__0x4___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[2], 4294965251);
}


TEST_F(xori_01, opcode__xori___op1_x11__dest_x20__op1val_0x2___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[3], 3);
}


TEST_F(xori_01, opcode__xori___op1_x7__dest_x21__op1val__0x80000000___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[4], 2147485012);
}


TEST_F(xori_01, opcode__xori___op1_x17__dest_x27__op1val_0x0___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[5], 4294966271);
}


TEST_F(xori_01, opcode__xori___op1_x22__dest_x1__op1val_0x7fffffff___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[6], 2147483657);
}


TEST_F(xori_01, opcode__xori___op1_x20__dest_x22__op1val_0x1___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[7], 5);
}


TEST_F(xori_01, opcode__xori___op1_x19__dest_x31__op1val__0x201___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[8], 0);
}


TEST_F(xori_01, opcode__xori___op1_x9__dest_x5__op1val__0x21___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[9], 4294967261);
}


TEST_F(xori_01, opcode__xori___op1_x28__dest_x29__op1val__0x80000000___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[10], 2147483656);
}


TEST_F(xori_01, opcode__xori___op1_x30__dest_x4__op1val__0x40000001___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[11], 3221225455);
}


TEST_F(xori_01, opcode__xori___op1_x27__dest_x8__op1val_0x7fffffff___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[12], 2147483615);
}


TEST_F(xori_01, opcode__xori___op1_x3__dest_x25__op1val_0x66666666___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[13], 1717986854);
}


TEST_F(xori_01, opcode__xori___op1_x31__dest_x17__op1val__0x80001___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[14], 4294442879);
}


TEST_F(xori_01, opcode__xori___op1_x29__dest_x16__op1val__0xb504___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[15], 4294921212);
}


TEST_F(xori_01, opcode__xori___op1_x4__dest_x6__op1val_0x0___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[16], 512);
}


TEST_F(xori_01, opcode__xori___op1_x14__dest_x3__op1val__0x100001___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[17], 4293917695);
}


TEST_F(xori_01, opcode__xori___op1_x12__dest_x15__op1val__0x7___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[18], 7);
}


TEST_F(xori_01, opcode__xori___op1_x21__dest_x9__op1val_0x5___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[19], 4294967288);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x19__op1val__0xb504___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[20], 46343);
}


TEST_F(xori_01, opcode__xori___op1_x16__dest_x26__op1val_0x1___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[21], 4294967286);
}


TEST_F(xori_01, opcode__xori___op1_x26__dest_x30__op1val_0x66666667___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[22], 2576980360);
}


TEST_F(xori_01, opcode__xori___op1_x1__dest_x23__op1val__0x41___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[23], 96);
}


TEST_F(xori_01, opcode__xori___op1_x6__dest_x0__op1val__0x1___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[24], 0);
}


TEST_F(xori_01, opcode__xori___op1_x2__dest_x7__op1val_0x55555555___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[25], 2863311402);
}


TEST_F(xori_01, opcode__xori___op1_x0__dest_x14__op1val_0x0___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[26], 4294967039);
}


TEST_F(xori_01, opcode__xori___op1_x8__dest_x13__op1val_0x3fffffff___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[27], 1073740458);
}


TEST_F(xori_01, opcode__xori___op1_x25__dest_x12__op1val__0x7___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[28], 1363);
}


TEST_F(xori_01, opcode__xori___op1_x13__dest_x11__op1val_0x8___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[29], 8);
}


TEST_F(xori_01, opcode__xori___op1_x23__dest_x2__op1val_0x10___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[30], 17);
}


TEST_F(xori_01, opcode__xori___op1_x5__dest_x28__op1val_0x20___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[31], 1397);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x40___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[32], 320);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x80___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[33], 133);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x100___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[34], 4294966975);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x200___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[35], 514);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x400___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[36], 4294966955);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x800___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[37], 2052);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x1000___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[38], 4104);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2000___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[39], 9556);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[40], 16388);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x8000___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[41], 34815);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x10000___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[42], 65600);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x20000___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[43], 131116);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x40000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[44], 4294703786);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x80000___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[45], 525107);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x100000___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[46], 4293918714);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x200000___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[47], 4292870141);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x400000___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[48], 4195669);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x800000___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[49], 4286578679);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x1000000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[50], 16777221);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2000000___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[51], 4261412351);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[52], 67108864);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x8000000___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[53], 134218548);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x10000000___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[54], 4026531775);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x20000000___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[55], 536872277);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x40000000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[56], 3221224106);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[57], 4294967290);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x3___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[58], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x5___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[59], 1019);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x9___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[60], 1372);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x11___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[61], 4294967151);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x81___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[62], 4294965529);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x101___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[63], 4294967033);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x401___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[64], 4294966265);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x801___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[65], 4294965245);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x1001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[66], 4294963199);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x2001___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[67], 4294957056);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x4001___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[68], 16427);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x8001___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[69], 4294932890);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x10001___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[70], 4294900941);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x20001___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[71], 131328);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x40001___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[72], 264191);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x200001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[73], 4292870143);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x400001___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[74], 4290771354);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x800001___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[75], 4286577048);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x1000001___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[76], 16778581);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x2000001___immval__0x8) {
  ASSERT_EQ(getScalarWordData()[77], 33554439);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x4000001___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[78], 67108865);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x8000001___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[79], 134218751);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x10000001___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[80], 268435499);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x20000001___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[81], 536871168);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[82], 2863311018);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[83], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[84], 1366);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[85], 4294965929);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[86], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[87], 816);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[88], 1637);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[89], 4294967248);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[90], 46);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[91], 1);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[92], 1367);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[93], 3);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[94], 7);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[95], 817);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[96], 1638);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[97], 47);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[98], 1365);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[99], 4294965928);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[100], 5);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[101], 823);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[102], 1636);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[103], 4294967255);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[104], 45);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[105], 1431655766);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[106], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[107], 2863312895);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[108], 1431655760);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[109], 1431656038);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[110], 1431655219);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[111], 2863311494);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[112], 1431655800);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[113], 1431655767);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[114], 1431654401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[115], 1431655765);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[116], 1431655761);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[117], 1431656039);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[118], 1431655216);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[119], 1431655801);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[120], 1431654403);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[121], 2863312894);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[122], 1431655763);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[123], 1431656033);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[124], 1431655218);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[125], 2863311489);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[126], 1431655803);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[127], 2863311529);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[128], 2863312895);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[129], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[130], 2863311535);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[131], 2863311257);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[132], 2863312076);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[133], 1431655801);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[134], 2863311495);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[135], 2863311528);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[136], 2863312894);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[137], 2863311530);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[138], 2863311534);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[139], 2863311256);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[140], 2863312079);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[141], 2863311494);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[142], 2863312892);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[143], 1431654401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[144], 2863311532);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[145], 2863311262);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[146], 2863312077);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[147], 1431655806);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[148], 2863311492);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[149], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[150], 1360);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[151], 4294965935);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[152], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[153], 822);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[154], 1635);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[155], 4294967254);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[156], 40);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[157], 7);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[158], 1361);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[159], 5);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[160], 1);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[161], 823);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[162], 1632);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[163], 41);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[164], 1363);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[165], 4294965934);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[166], 3);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[167], 817);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[168], 1634);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[169], 4294967249);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[170], 43);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[171], 858993456);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[172], 858994278);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[173], 3435973017);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[174], 858993462);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[175], 858992640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[176], 858994005);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[177], 3435973856);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[178], 858993438);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[179], 858993457);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[180], 858994279);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[181], 858993459);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[182], 858993463);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[183], 858992641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[184], 858994006);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[185], 858993439);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[186], 858994277);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[187], 3435973016);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[188], 858993461);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[189], 858992647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[190], 858994004);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[191], 3435973863);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[192], 858993437);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[193], 1717986917);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[194], 1717986099);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[195], 2576981196);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[196], 1717986915);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[197], 1717986645);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[198], 1717985280);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[199], 2576980405);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[200], 1717986891);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[201], 1717986916);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[202], 1717986098);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[203], 1717986918);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[204], 1717986914);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[205], 1717986644);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[206], 1717985283);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[207], 1717986890);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[208], 1717986096);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[209], 2576981197);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[210], 1717986912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[211], 1717986642);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[212], 1717985281);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[213], 2576980402);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[214], 1717986888);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[215], 4294920959);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[216], 4294922153);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[217], 45142);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[218], 4294920953);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[219], 4294920655);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[220], 4294921370);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[221], 46383);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[222], 4294920913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[223], 4294920958);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[224], 4294922152);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[225], 4294920956);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[226], 4294920952);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[227], 4294920654);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[228], 4294921369);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[229], 4294920912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[230], 4294922154);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[231], 45143);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[232], 4294920954);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[233], 4294920648);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[234], 4294921371);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[235], 46376);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[236], 4294920914);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[237], 46343);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[238], 45137);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[239], 4294922158);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[240], 46337);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[241], 46647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[242], 45922);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[243], 4294920919);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[244], 46377);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[245], 46342);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[246], 45136);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[247], 46340);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[248], 46336);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[249], 46646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[250], 45921);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[251], 46376);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[252], 45138);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[253], 4294922159);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[254], 46338);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[255], 46640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[256], 45923);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[257], 4294920912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[258], 46378);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[259], 1);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[260], 1367);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[261], 4294965928);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[262], 7);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[263], 817);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[264], 1636);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[265], 4294967249);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[266], 47);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[267], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[268], 1366);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[269], 2);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[270], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[271], 816);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[272], 1639);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[273], 46);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[274], 1364);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[275], 4294965929);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[276], 4);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[277], 822);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[278], 1637);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[279], 4294967254);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[280], 44);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[281], 1431655767);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[282], 1431654401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[283], 2863312894);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[284], 1431655761);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[285], 1431656039);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[286], 1431655218);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[287], 2863311495);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[288], 1431655801);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[289], 1431655766);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[290], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[291], 1431655764);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[292], 1431655760);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[293], 1431656038);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[294], 1431655217);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[295], 1431655800);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[296], 1431654402);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[297], 2863312895);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[298], 1431655762);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[299], 1431656032);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[300], 1431655219);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[301], 2863311488);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[302], 1431655802);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[303], 3);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[304], 1365);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[305], 4294965930);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[306], 5);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[307], 819);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[308], 1638);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[309], 4294967251);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[310], 45);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[311], 858992646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[312], 858994001);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[313], 858993432);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[314], 858994274);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[315], 3435973023);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[316], 858993458);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[317], 858992640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[318], 858994003);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[319], 3435973856);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[320], 858993434);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[321], 1717986916);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[322], 1717986098);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[323], 2576981197);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[324], 1717986914);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[325], 1717986644);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[326], 1717985281);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[327], 2576980404);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[328], 1717986890);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[329], 1717986917);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[330], 1717986099);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[331], 1717986919);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[332], 1717986915);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[333], 1717986645);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[334], 1717985282);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[335], 1717986891);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[336], 1717986097);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[337], 2576981196);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[338], 1717986913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[339], 1717986643);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[340], 1717985280);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[341], 2576980403);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[342], 1717986889);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[343], 4294920958);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[344], 4294922152);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[345], 45143);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[346], 4294920952);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[347], 4294920654);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[348], 4294921371);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[349], 46382);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[350], 4294920912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[351], 4294920959);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[352], 4294922153);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[353], 4294920957);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[354], 4294920953);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[355], 4294920655);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[356], 4294921368);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[357], 4294920913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[358], 4294922155);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[359], 45142);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[360], 4294920955);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[361], 4294920649);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[362], 4294921370);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[363], 46377);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[364], 4294920915);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[365], 46342);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[366], 45136);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[367], 4294922159);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[368], 46336);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[369], 46646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[370], 45923);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[371], 4294920918);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[372], 46376);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[373], 46343);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[374], 45137);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[375], 46341);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[376], 46337);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[377], 46647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[378], 45920);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[379], 46377);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[380], 45139);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[381], 4294922158);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[382], 46339);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[383], 46641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[384], 45922);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[385], 4294920913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[386], 46379);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[387], 2);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[388], 1364);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[389], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[390], 4);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[391], 818);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[392], 1637);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[393], 44);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[394], 1366);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[395], 4294965931);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[396], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[397], 820);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[398], 1639);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[399], 4294967252);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[400], 46);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[401], 7);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[402], 1361);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[403], 4294965934);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[404], 1);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[405], 823);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[406], 1634);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[407], 4294967255);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[408], 41);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[409], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[410], 1360);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[411], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[412], 822);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[413], 1633);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[414], 40);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[415], 1362);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[416], 4294965935);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[417], 2);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[418], 816);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[419], 1635);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[420], 4294967248);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[421], 42);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[422], 858993457);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[423], 858994279);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[424], 3435973016);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[425], 858993463);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[426], 858992641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[427], 858994004);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[428], 3435973857);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[429], 858993439);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[430], 858993456);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[431], 858994278);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[432], 858993458);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[433], 858993462);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[434], 858992640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[435], 858994007);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[436], 858993438);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[437], 858994276);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[438], 3435973017);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[439], 858993460);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[440], 858992646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[441], 858994005);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[442], 3435973862);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[443], 858993436);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[444], 1717986918);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[445], 1717986096);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[446], 2576981199);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[447], 1717986912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[448], 1717986646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[449], 1717985283);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[450], 2576980406);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[451], 1717986888);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[452], 1717986919);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[453], 1717986097);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[454], 1717986917);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[455], 1717986913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[456], 1717986647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[457], 1717985280);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[458], 1717986889);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[459], 1717986099);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[460], 2576981198);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[461], 1717986915);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[462], 1717986641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[463], 1717985282);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[464], 2576980401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[465], 1717986891);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[466], 46336);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[467], 45142);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[468], 4294922153);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[469], 46342);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[470], 46640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[471], 45925);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[472], 4294920912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[473], 46382);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[474], 46337);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[475], 45143);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[476], 46339);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[477], 46343);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[478], 46641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[479], 45926);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[480], 46383);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[481], 45141);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[482], 4294922152);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[483], 46341);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[484], 46647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[485], 45924);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[486], 4294920919);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[487], 46381);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[488], 1431655765);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[489], 1431654403);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[490], 2863312892);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[491], 1431655763);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[492], 1431656037);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[493], 1431655216);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[494], 2863311493);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[495], 1431655803);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[496], 1431655764);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[497], 1431654402);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[498], 1431655766);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[499], 1431655762);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[500], 1431656036);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[501], 1431655219);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[502], 1431655802);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[503], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[504], 2863312893);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[505], 1431655760);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[506], 1431656034);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[507], 1431655217);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[508], 2863311490);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[509], 1431655800);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[510], 2863311528);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[511], 2863312894);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[512], 1431654401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[513], 2863311534);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[514], 2863311256);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[515], 2863312077);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[516], 1431655800);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[517], 2863311494);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[518], 2863311529);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[519], 2863312895);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[520], 2863311531);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[521], 2863311535);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[522], 2863311257);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[523], 2863312078);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[524], 2863311495);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[525], 2863312893);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[526], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[527], 2863311533);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[528], 2863311263);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[529], 2863312076);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[530], 1431655807);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[531], 2863311493);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[532], 5);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[533], 1363);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[534], 4294965932);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[535], 3);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[536], 821);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[537], 1632);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[538], 4294967253);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[539], 43);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[540], 4);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[541], 1362);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[542], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[543], 2);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[544], 820);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[545], 1635);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[546], 42);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[547], 1360);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[548], 4294965933);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[549], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[550], 818);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[551], 1633);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[552], 4294967250);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[553], 40);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[554], 858993463);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[555], 858994273);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[556], 3435973022);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[557], 858993457);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[558], 858992647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[559], 858994002);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[560], 3435973863);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[561], 858993433);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[562], 858993462);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[563], 858994272);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[564], 858993460);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[565], 858993456);
}

