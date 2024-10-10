// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

// RISC-V architecture test includes
#include "riscv_arch_test/beq-01.h"

namespace
{
class beq_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::beq_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::beq_01::generateCode(false));
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

std::vector<uint8_t> beq_01::s_ScalarData;
}


// Generate google tests

TEST_F(beq_01, opcode__beq__op1_x6__op2_x6__op1val__0x2000001__op2val__0x2000001__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[5], 3);
}


TEST_F(beq_01, opcode__beq__op1_x2__op2_x12__op1val_0x0__op2val_0x7fffffff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[6], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x7__op1val__0x55555556__op2val__0x40000001__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[7], 1);
}


TEST_F(beq_01, opcode__beq__op1_x7__op2_x11__op1val_0x7fffffff__op2val__0x20000001__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[8], 3);
}


TEST_F(beq_01, opcode__beq__op1_x9__op2_x14__op1val_0x3fffffff__op2val__0x10000001__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[9], 1);
}


TEST_F(beq_01, opcode__beq__op1_x8__op2_x2__op1val__0xa__op2val__0x8000001__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[10], 1);
}


TEST_F(beq_01, opcode__beq__op1_x12__op2_x1__op1val_0xb504__op2val__0x2000001__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[11], 3);
}


TEST_F(beq_01, opcode__beq__op1_x3__op2_x4__op1val__0x200001__op2val__0x1000001__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[12], 3);
}


TEST_F(beq_01, opcode__beq__op1_x0__op2_x13__op1val_0x0__op2val__0x800001__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[13], 3);
}


TEST_F(beq_01, opcode__beq__op1_x14__op2_x5__op1val__0x10000001__op2val__0x400001__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[14], 3);
}


TEST_F(beq_01, opcode__beq__op1_x15__op2_x9__op1val_0x4__op2val__0x200001__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[15], 3);
}


TEST_F(beq_01, opcode__beq__op1_x5__op2_x15__op1val_0x200__op2val__0x100001__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[16], 1);
}


TEST_F(beq_01, opcode__beq__op1_x4__op2_x0__op1val__0x20000001__op2val_0x0__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[17], 1);
}


TEST_F(beq_01, opcode__beq__op1_x1__op2_x8__op1val__0x41__op2val__0x40001__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[18], 3);
}


TEST_F(beq_01, opcode__beq__op1_x11__op2_x3__op1val_0x4__op2val__0x20001__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[19], 3);
}


TEST_F(beq_01, opcode__beq__op1_x13__op2_x10__op1val__0x2001__op2val__0x10001__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[20], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x20__op2val__0x8001__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[21], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x4__op2val__0x4001__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[22], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x200__op2val__0x2001__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[23], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x6__op2val__0x1001__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[24], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x8__op2val__0x801__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[25], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x81__op2val__0x401__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[26], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val__0x201__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[27], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x3__op2val__0x101__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[28], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2000__op2val__0x81__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[29], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3fffffff__op2val__0x41__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[30], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x40001__op2val__0x21__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[31], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x21__op2val__0x11__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[32], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x7fffffff__op2val__0x9__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[33], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val__0x5__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[34], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x1__op2val__0x3__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[35], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x40001__op2val__0x2__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[36], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x40000001__op2val_0x6__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[37], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x8000001__op2val__0x10000001__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[38], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x4000001__op2val_0xb504__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[39], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x1000001__op2val__0x1000001__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[40], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x800001__op2val__0x9__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[41], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x400001__op2val_0x80__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[42], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x100001__op2val__0x55555555__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[43], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x80001__op2val_0x20000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[44], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x20001__op2val_0x66666667__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[45], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x10001__op2val_0x33333334__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[46], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x8001__op2val_0x0__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[47], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x4001__op2val__0x1001__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[48], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x1001__op2val__0x800001__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[49], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x801__op2val_0x2__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[50], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x401__op2val__0x81__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[51], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x201__op2val__0x401__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[52], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x101__op2val_0xb503__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[53], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x11__op2val__0xb503__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[54], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x9__op2val__0xb504__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[55], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x5__op2val_0x55555554__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[56], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x2__op2val__0x801__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[57], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x8000__op2val__0x80000000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[58], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x40000000__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[59], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x20000000__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[60], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x401__op2val_0x10000000__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[61], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x8000000__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[62], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x20__op2val_0x4000000__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[63], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x9__op2val_0x2000000__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[64], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x8000000__op2val_0x1000000__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[65], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x800000__op2val_0x800000__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[66], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x400000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[67], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x401__op2val_0x200000__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[68], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x8000001__op2val_0x100000__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[69], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x8__op2val_0x80000__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[70], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x100__op2val_0x40000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[71], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xa__op2val_0x10000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[72], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x8000__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[73], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x40001__op2val_0x4000__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[74], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x8001__op2val_0x2000__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[75], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x1000__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[76], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x5__op2val_0x800__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[77], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x400001__op2val_0x400__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[78], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x101__op2val_0x200__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[79], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x100__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[80], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x40__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[81], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x20__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[82], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x200001__op2val_0x10__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[83], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x8__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[84], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4000__op2val_0x4__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[85], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x20001__op2val_0x1__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[86], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x80000000__op2val_0x40000000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[87], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x40000000__op2val__0x81__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[88], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x20000000__op2val__0x40000001__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[89], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x10000000__op2val__0x40000001__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[90], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4000000__op2val__0x8__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[91], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2000000__op2val_0x10__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[92], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x1000000__op2val_0x6__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[93], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x400000__op2val_0x66666666__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[94], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x200000__op2val__0x2__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[95], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x100000__op2val_0x66666667__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[96], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x80000__op2val__0x101__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[97], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x40000__op2val_0x55555555__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[98], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x20000__op2val__0x10000001__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[99], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x10000__op2val__0x55555555__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[100], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x1000__op2val_0x66666665__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[101], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x800__op2val__0x200001__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[102], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x400__op2val_0x3__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[103], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x80__op2val_0x2000000__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[104], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x40__op2val_0x3__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[105], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x10__op2val__0x80000000__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[106], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x8__op2val_0x33333332__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[107], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x7__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[108], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0xb505__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[109], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val__0xb503__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[110], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x66666667__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[111], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x33333334__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[112], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x6__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[113], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val__0x55555555__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[114], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x55555556__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[115], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x4__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[116], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0xb503__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[117], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x0__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[118], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x66666665__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[119], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x33333332__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[120], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x55555554__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[121], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x2__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[122], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0xb504__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[123], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val__0xb504__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[124], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x66666666__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[125], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x33333333__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[126], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x5__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[127], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val__0x55555556__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[128], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x55555555__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[129], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb505__op2val_0x3__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[130], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0xb505__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[131], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val__0xb503__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[132], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x66666667__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[133], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x33333334__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[134], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x6__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[135], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val__0x55555555__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[136], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[137], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x4__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[138], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0xb503__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[139], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x0__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[140], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x66666665__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[141], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x33333332__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[142], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x55555554__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[143], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x2__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[144], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0xb504__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[145], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val__0xb504__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[146], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x66666666__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[147], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x33333333__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[148], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x5__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[149], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val__0x55555556__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[150], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x55555555__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[151], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb503__op2val_0x3__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[152], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0xb505__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[153], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val__0xb503__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[154], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x66666667__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[155], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x33333334__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[156], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x6__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[157], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val__0x55555555__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[158], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x55555556__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[159], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x4__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[160], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0xb503__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[161], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x0__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[162], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x66666665__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[163], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x33333332__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[164], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x55555554__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[165], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x2__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[166], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0xb504__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[167], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val__0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[168], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x66666666__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[169], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x33333333__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[170], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x5__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[171], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val__0x55555556__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[172], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[173], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666667__op2val_0x3__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[174], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0xb505__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[175], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val__0xb503__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[176], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x66666667__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[177], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x33333334__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[178], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x6__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[179], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val__0x55555555__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[180], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x55555556__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[181], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x4__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[182], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0xb503__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[183], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x0__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[184], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x66666665__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[185], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x33333332__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[186], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x55555554__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[187], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x2__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[188], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0xb504__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[189], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val__0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[190], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x66666666__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[191], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x33333333__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[192], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x5__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[193], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val__0x55555556__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[194], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x55555555__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[195], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333334__op2val_0x3__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[196], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0xb505__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[197], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val__0xb503__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[198], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x66666667__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[199], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x33333334__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[200], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x6__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[201], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val__0x55555555__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[202], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x55555556__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[203], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x4__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[204], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0xb503__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[205], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x0__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[206], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x66666665__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[207], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x33333332__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[208], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x55555554__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[209], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x2__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[210], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0xb504__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[211], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val__0xb504__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[212], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x66666666__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[213], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x33333333__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[214], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x5__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[215], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val__0x55555556__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[216], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x55555555__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[217], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x6__op2val_0x3__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[218], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0xb505__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[219], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val__0xb503__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[220], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x66666667__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[221], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x33333334__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[222], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x6__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[223], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val__0x55555555__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[224], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x55555556__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[225], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x4__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[226], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0xb503__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[227], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x0__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[228], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x66666665__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[229], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x33333332__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[230], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x55555554__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[231], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x2__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[232], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0xb504__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[233], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val__0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[234], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x66666666__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[235], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x33333333__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[236], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x5__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[237], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val__0x55555556__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[238], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x55555555__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[239], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555555__op2val_0x3__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[240], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0xb505__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[241], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val__0xb503__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[242], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x66666667__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[243], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x33333334__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[244], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x6__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[245], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val__0x55555555__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[246], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x55555556__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[247], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x4__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[248], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0xb503__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[249], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x0__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[250], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x66666665__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[251], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x33333332__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[252], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x55555554__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[253], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x2__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[254], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0xb504__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[255], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val__0xb504__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[256], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x66666666__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[257], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x33333333__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[258], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x5__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[259], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val__0x55555556__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[260], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x55555555__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[261], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555556__op2val_0x3__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[262], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0xb505__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[263], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val__0xb503__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[264], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x66666667__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[265], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x33333334__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[266], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x6__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[267], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val__0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[268], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x55555556__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[269], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x4__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[270], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0xb503__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[271], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x0__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[272], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x66666665__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[273], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x33333332__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[274], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x55555554__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[275], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x2__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[276], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[277], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val__0xb504__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[278], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x66666666__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[279], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x33333333__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[280], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x5__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[281], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val__0x55555556__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[282], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x55555555__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[283], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x4__op2val_0x3__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[284], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0xb505__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[285], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val__0xb503__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[286], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x66666667__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[287], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x33333334__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[288], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x6__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[289], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val__0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[290], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[291], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x4__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[292], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0xb503__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[293], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x0__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[294], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x66666665__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[295], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x33333332__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[296], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x55555554__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[297], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x2__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[298], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0xb504__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[299], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val__0xb504__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[300], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x66666666__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[301], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x33333333__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[302], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x5__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[303], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val__0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[304], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x55555555__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[305], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb503__op2val_0x3__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[306], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0xb505__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[307], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val__0xb503__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[308], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x66666667__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[309], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x33333334__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[310], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x6__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[311], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val__0x55555555__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[312], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[313], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x4__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[314], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0xb503__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[315], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x0__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[316], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x66666665__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[317], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x33333332__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[318], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x55555554__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[319], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x2__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[320], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0xb504__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[321], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val__0xb504__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[322], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x66666666__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[323], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x33333333__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[324], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x5__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[325], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val__0x55555556__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[326], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x55555555__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[327], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x0__op2val_0x3__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[328], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0xb505__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[329], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val__0xb503__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[330], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x66666667__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[331], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x33333334__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[332], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x6__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[333], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val__0x55555555__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[334], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[335], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x4__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[336], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0xb503__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[337], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x0__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[338], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x66666665__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[339], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[340], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x55555554__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[341], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x2__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[342], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0xb504__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[343], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val__0xb504__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[344], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x66666666__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[345], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x33333333__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[346], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x5__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[347], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val__0x55555556__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[348], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x55555555__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[349], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x3__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[350], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0xb505__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[351], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val__0xb503__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[352], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x66666667__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[353], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x33333334__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[354], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x6__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[355], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val__0x55555555__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[356], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x55555556__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[357], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x4__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[358], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0xb503__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[359], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x0__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[360], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x66666665__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[361], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[362], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x55555554__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[363], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x2__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[364], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[365], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val__0xb504__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[366], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x66666666__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[367], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x33333333__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[368], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x5__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[369], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val__0x55555556__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[370], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[371], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x5__op2val_0x3__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[372], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0xb505__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[373], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val__0xb503__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[374], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x66666667__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[375], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x33333334__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[376], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x6__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[377], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val__0x55555555__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[378], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x55555556__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[379], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x4__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[380], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0xb503__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[381], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x0__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[382], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x66666665__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[383], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x33333332__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[384], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x55555554__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[385], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x2__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[386], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0xb504__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[387], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val__0xb504__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[388], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x66666666__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[389], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x33333333__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[390], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x5__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[391], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val__0x55555556__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[392], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x55555555__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[393], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x55555556__op2val_0x3__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[394], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0xb505__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[395], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val__0xb503__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[396], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x66666667__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[397], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x33333334__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[398], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x6__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[399], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val__0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[400], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x55555556__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[401], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x4__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[402], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0xb503__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[403], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x0__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[404], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x66666665__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[405], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x33333332__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[406], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x55555554__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[407], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x2__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[408], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0xb504__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[409], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val__0xb504__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[410], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x66666666__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[411], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x33333333__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[412], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x5__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[413], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val__0x55555556__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[414], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x55555555__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[415], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555555__op2val_0x3__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[416], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0xb505__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[417], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val__0xb503__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[418], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x66666667__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[419], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x33333334__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[420], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x6__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[421], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val__0x55555555__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[422], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x55555556__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[423], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x4__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[424], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0xb503__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[425], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x0__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[426], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x66666665__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[427], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[428], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x55555554__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[429], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x2__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[430], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0xb504__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[431], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val__0xb504__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[432], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x66666666__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[433], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x33333333__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[434], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x5__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[435], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val__0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[436], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x55555555__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[437], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x3__op2val_0x3__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[438], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x0__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[439], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x66666665__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[440], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x33333332__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[441], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x55555554__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[442], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x2__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[443], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[444], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val__0xb504__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[445], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x66666666__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[446], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x33333333__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[447], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x5__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[448], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val__0x55555556__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[449], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x55555555__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[450], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666665__op2val_0x3__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[451], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0xb505__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[452], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val__0xb503__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[453], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x66666667__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[454], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x33333334__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[455], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x6__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[456], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val__0x55555555__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[457], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x55555556__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[458], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x4__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[459], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0xb503__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[460], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x0__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[461], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x66666665__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[462], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x33333332__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[463], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x55555554__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[464], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x2__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[465], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0xb504__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[466], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val__0xb504__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[467], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x66666666__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[468], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x33333333__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[469], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x5__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[470], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val__0x55555556__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[471], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x55555555__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[472], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333332__op2val_0x3__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[473], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0xb505__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[474], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val__0xb503__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[475], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x66666667__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[476], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x33333334__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[477], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x6__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[478], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val__0x55555555__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[479], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x55555556__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[480], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x4__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[481], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0xb503__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[482], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x0__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[483], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x66666665__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[484], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x33333332__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[485], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x55555554__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[486], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x2__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[487], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[488], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val__0xb504__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[489], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x66666666__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[490], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x33333333__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[491], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x5__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[492], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val__0x55555556__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[493], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x55555555__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[494], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x55555554__op2val_0x3__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[495], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0xb505__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[496], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val__0xb503__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[497], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x66666667__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[498], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x33333334__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[499], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x6__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[500], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val__0x55555555__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[501], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x55555556__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[502], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x4__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[503], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0xb503__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[504], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x0__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[505], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x66666665__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[506], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x33333332__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[507], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x55555554__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[508], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x2__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[509], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0xb504__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[510], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val__0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[511], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x66666666__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[512], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x33333333__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[513], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x5__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[514], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val__0x55555556__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[515], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x55555555__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[516], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x2__op2val_0x3__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[517], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0xb505__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[518], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val__0xb503__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[519], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x66666667__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[520], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x33333334__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[521], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x6__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[522], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val__0x55555555__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[523], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x55555556__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[524], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x4__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[525], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0xb503__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[526], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x0__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[527], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x66666665__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[528], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[529], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x55555554__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[530], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x2__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[531], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[532], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val__0xb504__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[533], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x66666666__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[534], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x33333333__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[535], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x5__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[536], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val__0x55555556__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[537], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x55555555__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[538], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0xb504__op2val_0x3__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[539], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0xb505__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[540], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val__0xb503__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[541], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x66666667__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[542], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x33333334__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[543], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x6__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[544], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val__0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[545], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x55555556__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[546], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x4__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[547], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0xb503__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[548], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x0__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[549], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x66666665__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[550], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x33333332__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[551], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x55555554__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[552], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x2__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[553], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0xb504__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[554], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val__0xb504__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[555], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x66666666__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[556], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x33333333__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[557], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x5__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[558], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val__0x55555556__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[559], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x55555555__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[560], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0xb504__op2val_0x3__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[561], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0xb505__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[562], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val__0xb503__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[563], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x66666667__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[564], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x33333334__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[565], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x6__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[566], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val__0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[567], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x55555556__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[568], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x4__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[569], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0xb503__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[570], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x0__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[571], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x66666665__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[572], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x33333332__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[573], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x55555554__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[574], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x2__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[575], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0xb504__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[576], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val__0xb504__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[577], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x66666666__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[578], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x33333333__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[579], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x5__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[580], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val__0x55555556__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[581], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x55555555__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[582], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x66666666__op2val_0x3__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[583], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0xb505__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[584], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val__0xb503__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[585], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x66666667__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[586], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x33333334__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[587], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x6__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[588], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val__0x55555555__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[589], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x55555556__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[590], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0x4__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[591], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val_0x33333333__op2val_0xb503__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[592], 1);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x2000001__op2val__0x4000001__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[593], 3);
}


TEST_F(beq_01, opcode__beq__op1_x10__op2_x11__op1val__0x20000001__op2val__0x80001__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[594], 1);
}

