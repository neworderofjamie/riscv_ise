// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

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

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::lw_align_01::generateCode(false));
        device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());
        
        // Re-enable 
        device.setEnabled(true);
        
        // Wait for tests to complete
        device.waitOnNonZero(0);
        
        // Disable and copy data
        device.setEnabled(false);
        s_ScalarData.resize(scalarInitData.size());
        device.memcpyDataFromDevice(s_ScalarData.data(), 0, scalarInitData.size());
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

TEST_F(lw_align_01, opcode_lw_op1_x11__dest_x1__immval__0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[5], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x14__dest_x14__immval__0x3__align_0) {
  ASSERT_EQ(getScalarWordData()[6], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x4__dest_x8__immval__0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[7], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x1__dest_x10__immval__0x5__align_0) {
  ASSERT_EQ(getScalarWordData()[8], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x12__dest_x15__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[9], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x7__dest_x13__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[10], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x5__dest_x3__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[11], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x8__dest_x0__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[12], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x3__dest_x12__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[13], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x13__dest_x4__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[14], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x15__dest_x7__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[15], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x9__dest_x5__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[16], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x6__dest_x9__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[17], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x10__dest_x2__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[18], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x2__dest_x6__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[19], 3133065982);
}


TEST_F(lw_align_01, opcode_lw_op1_x5__dest_x11__immval__0x800__align_0) {
  ASSERT_EQ(getScalarWordData()[20], 3133065982);
}

