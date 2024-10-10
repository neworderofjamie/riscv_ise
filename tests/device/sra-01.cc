// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

// RISC-V architecture test includes
#include "riscv_arch_test/sra-01.h"

namespace
{
class sra_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::sra_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::sra_01::generateCode(false));
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

std::vector<uint8_t> sra_01::s_ScalarData;
}


// Generate google tests

TEST_F(sra_01, opcode__sra___op1_x11__op2_x9__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[5], -1431655766);
}


TEST_F(sra_01, opcode__sra___op1_x8__op2_x12__dest_x8__op1val__0x80001___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[6], -17);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x2__dest_x2__op1val_0x3___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[7], 0);
}


TEST_F(sra_01, opcode__sra___op1_x6__op2_x6__dest_x4__op1val__0x8___op2val__0x8) {
  ASSERT_EQ(getScalarWordData()[8], -1);
}


TEST_F(sra_01, opcode__sra___op1_x13__op2_x13__dest_x13__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[9], -699051);
}


TEST_F(sra_01, opcode__sra___op1_x0__op2_x10__dest_x7__op1val_0x0___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(sra_01, opcode__sra___op1_x1__op2_x15__dest_x14__op1val_0x7fffffff___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[11], 1023);
}


TEST_F(sra_01, opcode__sra___op1_x3__op2_x7__dest_x5__op1val__0x40000001___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[12], -536870913);
}


TEST_F(sra_01, opcode__sra___op1_x2__op2_x8__dest_x15__op1val__0x20000001___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[13], -4194305);
}


TEST_F(sra_01, opcode__sra___op1_x7__op2_x11__dest_x1__op1val__0x10000001___op2val_0x9) {
  ASSERT_EQ(getScalarWordData()[14], -524289);
}


TEST_F(sra_01, opcode__sra___op1_x12__op2_x0__dest_x9__op1val__0x8000001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[15], -134217729);
}


TEST_F(sra_01, opcode__sra___op1_x5__op2_x14__dest_x10__op1val__0x4000001___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[16], -9);
}


TEST_F(sra_01, opcode__sra___op1_x4__op2_x5__dest_x0__op1val__0x2000001___op2val_0x9) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(sra_01, opcode__sra___op1_x15__op2_x4__dest_x3__op1val__0x1000001___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[18], -1025);
}


TEST_F(sra_01, opcode__sra___op1_x9__op2_x3__dest_x11__op1val__0x800001___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[19], -65);
}


TEST_F(sra_01, opcode__sra___op1_x14__op2_x1__dest_x6__op1val__0x400001___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[20], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[21], -65);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[22], -262145);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val_0x9) {
  ASSERT_EQ(getScalarWordData()[23], -513);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[24], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[25], -2);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[26], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[27], -4097);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[28], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[29], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[30], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[31], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[32], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val_0x9) {
  ASSERT_EQ(getScalarWordData()[33], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[34], -3);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[35], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[36], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[37], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x9) {
  ASSERT_EQ(getScalarWordData()[38], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[39], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[40], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[41], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[42], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[43], -4096);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[44], 8);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0x9) {
  ASSERT_EQ(getScalarWordData()[45], 1048576);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[46], 131072);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[47], 16);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[48], 16384);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[49], 256);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[50], 128);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[51], 8388608);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[52], 2048);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val_0x9) {
  ASSERT_EQ(getScalarWordData()[53], 4096);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[54], 524288);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[55], 512);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[56], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[57], 64);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[58], 64);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[59], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[60], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[61], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[62], 8);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[63], 32);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[64], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[65], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[66], 16);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[67], 2);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[68], 1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[69], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[70], 1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[71], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[72], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[73], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[74], -182);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[75], 26214);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[76], 838860);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[77], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[78], 682);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[79], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x7___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[80], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[81], 2730);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[82], 362);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[83], 12);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[84], 409);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[85], 11184810);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[86], 1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[87], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[88], 1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[89], 1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[90], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[91], -3);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[92], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[93], -65537);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0x9) {
  ASSERT_EQ(getScalarWordData()[94], -65537);
}

