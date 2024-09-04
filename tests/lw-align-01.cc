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
class lw_align_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        // Generate word-aligned vector to initialise memory
        const std::vector<uint32_t> scalarWordInitData{
        3133065982,3133065982,3133065982,3133065982,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,};
        
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
        
        c.li(Reg::X12, 16);
        // rs1 != rd, rs1==x6, rd==x24, ea_align == 0 and (imm_val % 4) == 0, imm_val < 0
        // opcode:lw op1:x6; dest:x24; immval:-0x4; align:0
        c.li(Reg::X6, 0+(0*4)+(0)-(-0x4));
        c.lw(Reg::X24, Reg::X6, -0x4);
        c.sw(Reg::X24, Reg::X12, 0);

        // rs1 == rd, rs1==x4, rd==x4, ea_align == 0 and (imm_val % 4) == 1, 
        // opcode:lw op1:x4; dest:x4; immval:-0x3; align:0
        c.li(Reg::X4, 0+(0*4)+(0)-(-0x3));
        c.lw(Reg::X4, Reg::X4, -0x3);
        c.sw(Reg::X4, Reg::X12, 4);

        // rs1==x21, rd==x15, ea_align == 0 and (imm_val % 4) == 2, 
        // opcode:lw op1:x21; dest:x15; immval:-0xa; align:0
        c.li(Reg::X21, 0+(0*4)+(0)-(-0xa));
        c.lw(Reg::X15, Reg::X21, -0xa);
        c.sw(Reg::X15, Reg::X12, 8);

        // rs1==x26, rd==x8, ea_align == 0 and (imm_val % 4) == 3, imm_val > 0
        // opcode:lw op1:x26; dest:x8; immval:0x7; align:0
        c.li(Reg::X26, 0+(0*4)+(0)-(0x7));
        c.lw(Reg::X8, Reg::X26, 0x7);
        c.sw(Reg::X8, Reg::X12, 12);

        // rs1==x27, rd==x26, imm_val == 0, 
        // opcode:lw op1:x27; dest:x26; immval:0x0; align:0
        c.li(Reg::X27, 0+(0*4)+(0)-(0x0));
        c.lw(Reg::X26, Reg::X27, 0x0);
        c.sw(Reg::X26, Reg::X12, 16);

        // rs1==x20, rd==x18, 
        // opcode:lw op1:x20; dest:x18; immval:-0x800; align:0
        c.li(Reg::X20, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X18, Reg::X20, -0x800);
        c.sw(Reg::X18, Reg::X12, 20);

        // rs1==x30, rd==x9, 
        // opcode:lw op1:x30; dest:x9; immval:-0x800; align:0
        c.li(Reg::X30, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X9, Reg::X30, -0x800);
        c.sw(Reg::X9, Reg::X12, 24);

        // rs1==x18, rd==x11, 
        // opcode:lw op1:x18; dest:x11; immval:-0x800; align:0
        c.li(Reg::X18, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X11, Reg::X18, -0x800);
        c.sw(Reg::X11, Reg::X12, 28);

        // rs1==x13, rd==x6, 
        // opcode:lw op1:x13; dest:x6; immval:-0x800; align:0
        c.li(Reg::X13, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X6, Reg::X13, -0x800);
        c.sw(Reg::X6, Reg::X12, 32);

        // rs1==x14, rd==x10, 
        // opcode:lw op1:x14; dest:x10; immval:-0x800; align:0
        c.li(Reg::X14, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X10, Reg::X14, -0x800);
        c.sw(Reg::X10, Reg::X12, 36);

        // rs1==x9, rd==x3, 
        // opcode:lw op1:x9; dest:x3; immval:-0x800; align:0
        c.li(Reg::X9, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X3, Reg::X9, -0x800);
        c.sw(Reg::X3, Reg::X12, 40);

        // rs1==x2, rd==x31, 
        // opcode:lw op1:x2; dest:x31; immval:-0x800; align:0
        c.li(Reg::X2, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X31, Reg::X2, -0x800);
        c.sw(Reg::X31, Reg::X12, 44);

        // rs1==x23, rd==x0, 
        // opcode:lw op1:x23; dest:x0; immval:-0x800; align:0
        c.li(Reg::X23, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X0, Reg::X23, -0x800);
        c.sw(Reg::X0, Reg::X12, 48);

        // rs1==x3, rd==x19, 
        // opcode:lw op1:x3; dest:x19; immval:-0x800; align:0
        c.li(Reg::X3, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X19, Reg::X3, -0x800);
        c.sw(Reg::X19, Reg::X12, 52);

        // rs1==x7, rd==x13, 
        // opcode:lw op1:x7; dest:x13; immval:-0x800; align:0
        c.li(Reg::X7, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X13, Reg::X7, -0x800);
        c.sw(Reg::X13, Reg::X12, 56);

        // rs1==x1, rd==x7, 
        // opcode:lw op1:x1; dest:x7; immval:-0x800; align:0
        c.li(Reg::X1, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X7, Reg::X1, -0x800);
        c.sw(Reg::X7, Reg::X12, 60);

        // rs1==x19, rd==x14, 
        // opcode:lw op1:x19; dest:x14; immval:-0x800; align:0
        c.li(Reg::X19, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X14, Reg::X19, -0x800);
        c.sw(Reg::X14, Reg::X12, 64);

        // rs1==x8, rd==x29, 
        // opcode:lw op1:x8; dest:x29; immval:-0x800; align:0
        c.li(Reg::X8, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X29, Reg::X8, -0x800);
        c.sw(Reg::X29, Reg::X12, 68);

        // rs1==x24, rd==x5, 
        // opcode:lw op1:x24; dest:x5; immval:-0x800; align:0
        c.li(Reg::X24, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X5, Reg::X24, -0x800);
        c.sw(Reg::X5, Reg::X12, 72);

        // rs1==x31, rd==x16, 
        // opcode:lw op1:x31; dest:x16; immval:-0x800; align:0
        c.li(Reg::X31, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X16, Reg::X31, -0x800);
        c.sw(Reg::X16, Reg::X12, 76);

        // rs1==x10, rd==x25, 
        // opcode:lw op1:x10; dest:x25; immval:-0x800; align:0
        c.li(Reg::X10, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X25, Reg::X10, -0x800);
        c.sw(Reg::X25, Reg::X12, 80);

        // rs1==x5, rd==x1, 
        // opcode:lw op1:x5; dest:x1; immval:-0x800; align:0
        c.li(Reg::X5, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X1, Reg::X5, -0x800);
        c.sw(Reg::X1, Reg::X12, 84);

        // rs1==x15, rd==x22, 
        // opcode:lw op1:x15; dest:x22; immval:-0x800; align:0
        c.li(Reg::X15, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X22, Reg::X15, -0x800);
        c.sw(Reg::X22, Reg::X12, 88);

        c.li(Reg::X1, 108);
        // rs1==x17, rd==x27, 
        // opcode:lw op1:x17; dest:x27; immval:-0x800; align:0
        c.li(Reg::X17, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X27, Reg::X17, -0x800);
        c.sw(Reg::X27, Reg::X1, 0);

        // rs1==x12, rd==x20, 
        // opcode:lw op1:x12; dest:x20; immval:-0x800; align:0
        c.li(Reg::X12, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X20, Reg::X12, -0x800);
        c.sw(Reg::X20, Reg::X1, 4);

        // rs1==x28, rd==x30, 
        // opcode:lw op1:x28; dest:x30; immval:-0x800; align:0
        c.li(Reg::X28, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X30, Reg::X28, -0x800);
        c.sw(Reg::X30, Reg::X1, 8);

        // rs1==x25, rd==x28, 
        // opcode:lw op1:x25; dest:x28; immval:-0x800; align:0
        c.li(Reg::X25, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X28, Reg::X25, -0x800);
        c.sw(Reg::X28, Reg::X1, 12);

        // rs1==x22, rd==x12, 
        // opcode:lw op1:x22; dest:x12; immval:-0x800; align:0
        c.li(Reg::X22, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X12, Reg::X22, -0x800);
        c.sw(Reg::X12, Reg::X1, 16);

        // rs1==x16, rd==x2, 
        // opcode:lw op1:x16; dest:x2; immval:-0x800; align:0
        c.li(Reg::X16, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X2, Reg::X16, -0x800);
        c.sw(Reg::X2, Reg::X1, 20);

        // rs1==x29, rd==x21, 
        // opcode:lw op1:x29; dest:x21; immval:-0x800; align:0
        c.li(Reg::X29, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X21, Reg::X29, -0x800);
        c.sw(Reg::X21, Reg::X1, 24);

        // rs1==x11, rd==x17, 
        // opcode:lw op1:x11; dest:x17; immval:-0x800; align:0
        c.li(Reg::X11, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X17, Reg::X11, -0x800);
        c.sw(Reg::X17, Reg::X1, 28);

        // rd==x23, 
        // opcode:lw op1:x12; dest:x23; immval:-0x800; align:0
        c.li(Reg::X12, 0+(0*4)+(0)-(-0x800));
        c.lw(Reg::X23, Reg::X12, -0x800);
        c.sw(Reg::X23, Reg::X1, 32);


                
        // End
        c.ecall();
        return c;
    }

    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> lw_align_01::s_ScalarData;
}


// Generate google tests

TEST_F(lw_align_01, opcode_lw_op1_x6__dest_x24__immval__0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[4], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x4__dest_x4__immval__0x3__align_0) {
  ASSERT_EQ(getScalarWordData()[5], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x21__dest_x15__immval__0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[6], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x26__dest_x8__immval_0x7__align_0) {
  ASSERT_EQ(getScalarWordData()[7], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x27__dest_x26__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[8], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x20__dest_x18__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[9], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x30__dest_x9__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[10], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x18__dest_x11__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[11], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x13__dest_x6__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[12], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x14__dest_x10__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[13], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x9__dest_x3__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[14], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x2__dest_x31__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[15], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x23__dest_x0__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[16], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x3__dest_x19__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[17], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x7__dest_x13__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[18], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x1__dest_x7__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[19], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x19__dest_x14__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[20], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x8__dest_x29__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[21], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x24__dest_x5__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[22], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x31__dest_x16__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[23], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x10__dest_x25__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[24], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x5__dest_x1__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[25], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x15__dest_x22__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[26], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x17__dest_x27__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[27], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x12__dest_x20__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[28], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x28__dest_x30__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[29], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x25__dest_x28__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[30], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x22__dest_x12__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[31], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x16__dest_x2__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[32], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x29__dest_x21__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[33], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x11__dest_x17__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[34], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x12__dest_x23__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[35], 3133065982);
}

