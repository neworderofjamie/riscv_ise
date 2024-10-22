// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/lw-align-01.h"

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
        const auto &scalarWordInitData = ArchTest::lw_align_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::lw_align_01::generateCode(true), scalarInitData);
        
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
    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> lw_align_01::s_ScalarData;
}


// Generate google tests

TEST_F(lw_align_01, opcode_lw_op1_x6__dest_x24__immval__0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[5], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x4__dest_x4__immval__0x3__align_0) {
  ASSERT_EQ(getScalarWordData()[6], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x21__dest_x15__immval__0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[7], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x26__dest_x8__immval_0x7__align_0) {
  ASSERT_EQ(getScalarWordData()[8], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x27__dest_x26__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[9], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x20__dest_x18__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[10], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x30__dest_x9__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[11], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x18__dest_x11__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[12], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x13__dest_x6__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[13], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x14__dest_x10__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[14], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x9__dest_x3__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[15], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x2__dest_x31__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[16], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x23__dest_x0__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[17], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x3__dest_x19__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[18], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x7__dest_x13__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[19], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x1__dest_x7__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[20], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x19__dest_x14__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[21], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x8__dest_x29__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[22], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x24__dest_x5__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[23], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x31__dest_x16__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[24], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x10__dest_x25__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[25], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x5__dest_x1__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[26], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x15__dest_x22__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[27], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x17__dest_x27__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[28], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x12__dest_x20__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[29], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x28__dest_x30__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[30], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x25__dest_x28__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[31], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x22__dest_x12__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[32], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x16__dest_x2__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[33], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x29__dest_x21__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[34], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x11__dest_x17__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[35], (uint32_t)3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x12__dest_x23__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[36], (uint32_t)3133065982);
}

