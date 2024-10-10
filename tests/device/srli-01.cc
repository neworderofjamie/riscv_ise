// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

// RISC-V architecture test includes
#include "riscv_arch_test/srli-01.h"

namespace
{
class srli_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::srli_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::srli_01::generateCode(false));
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

std::vector<uint8_t> srli_01::s_ScalarData;
}


// Generate google tests

TEST_F(srli_01, opcode__srli___op1_x4__dest_x5__op1val__0x41___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[5], 1);
}


TEST_F(srli_01, opcode__srli___op1_x9__dest_x9__op1val_0x7fffffff___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[6], 8388607);
}


TEST_F(srli_01, opcode__srli___op1_x0__dest_x6__op1val_0x0___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[7], 0);
}


TEST_F(srli_01, opcode__srli___op1_x12__dest_x4__op1val__0x20000001___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[8], 58720255);
}


TEST_F(srli_01, opcode__srli___op1_x8__dest_x14__op1val__0x10000001___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[9], 3932159);
}


TEST_F(srli_01, opcode__srli___op1_x7__dest_x3__op1val__0x8000001___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[10], 1040187391);
}


TEST_F(srli_01, opcode__srli___op1_x6__dest_x2__op1val__0x4000001___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[11], 16127);
}


TEST_F(srli_01, opcode__srli___op1_x11__dest_x15__op1val__0x2000001___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[12], 65023);
}


TEST_F(srli_01, opcode__srli___op1_x5__dest_x13__op1val__0x1000001___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[13], 2139095039);
}


TEST_F(srli_01, opcode__srli___op1_x15__dest_x11__op1val__0x800001___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[14], 32703);
}


TEST_F(srli_01, opcode__srli___op1_x2__dest_x12__op1val__0x400001___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[15], 130943);
}


TEST_F(srli_01, opcode__srli___op1_x14__dest_x7__op1val__0x200001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[16], 33538047);
}


TEST_F(srli_01, opcode__srli___op1_x13__dest_x0__op1val__0x100001___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(srli_01, opcode__srli___op1_x3__dest_x10__op1val__0x80001___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[18], 2047);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x1__op1val__0x40001___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[19], 65531);
}


TEST_F(srli_01, opcode__srli___op1_x1__dest_x8__op1val__0x20001___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[20], 32766);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x10001___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[21], 2097119);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x8001___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[22], 16383);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x4001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[23], 4294950911);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x2001___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[24], 8191);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x1001___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[25], 4194299);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x801___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[26], 2047);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x401___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[27], 67108847);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x201___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[28], 8191);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x101___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[29], 16777214);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x81___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[30], 16777215);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x21___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[31], 1073741815);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x11___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[32], 16383);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x9___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[33], 1073741821);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[34], 67108863);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x3___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[35], 131071);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x2___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[36], 1048575);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x40000___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[37], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x40000000___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[38], 6);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x40___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[39], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x80000000___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[40], 2097152);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x40000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[41], 1073741824);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x20000000___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[42], 4096);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x10000000___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[43], 2048);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x8000000___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[44], 65536);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x4000000___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[45], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x2000000___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[46], 64);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x1000000___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[47], 2048);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x800000___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[48], 65536);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x400000___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[49], 65536);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x200000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[50], 131072);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x100000___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[51], 2048);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x80000___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[52], 256);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x20000___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[53], 2);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x10000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[54], 65536);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x8000___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[55], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x4000___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[56], 256);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x2000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[57], 512);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x1000___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[58], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x800___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[59], 8);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x400___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[60], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x200___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[61], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x100___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[62], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x80___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[63], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x20___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[64], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x10___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[65], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x8___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[66], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x4___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[67], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x2___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[68], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x1___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[69], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0xb505___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[70], 5);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0xb503___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[71], 33554069);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x66666667___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[72], 838860);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x33333334___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[73], 52428);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x6___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[74], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x55555555___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[75], 1431655765);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x55555556___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[76], 21845);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0xb503___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[77], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x0___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[78], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x3___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[79], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[80], 715827882);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x55555555___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[81], 699050);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[82], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x66666665___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[83], 819);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[84], 13421772);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[85], 22369621);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0xb504___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[86], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0xb504___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[87], 16777034);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x66666666___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[88], 3355443);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x33333333___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[89], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val_0x5___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[90], 0);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x40000001___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[91], 383);
}


TEST_F(srli_01, opcode__srli___op1_x10__dest_x11__op1val__0x100001___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[92], 1048319);
}

