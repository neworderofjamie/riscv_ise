// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

// RISC-V architecture test includes
#include "riscv_arch_test/slli-01.h"

namespace
{
class slli_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::slli_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::slli_01::generateCode(false));
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

std::vector<uint8_t> slli_01::s_ScalarData;
}


// Generate google tests

TEST_F(slli_01, opcode__slli___op1_x2__dest_x10__op1val__0x1___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[5], 2147483648);
}


TEST_F(slli_01, opcode__slli___op1_x3__dest_x3__op1val_0x7fffffff___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[6], 4160749568);
}


TEST_F(slli_01, opcode__slli___op1_x9__dest_x14__op1val__0x40000001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[7], 4294967168);
}


TEST_F(slli_01, opcode__slli___op1_x7__dest_x8__op1val__0x20000001___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[8], 3221225470);
}


TEST_F(slli_01, opcode__slli___op1_x4__dest_x6__op1val__0x10000001___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[9], 4294705152);
}


TEST_F(slli_01, opcode__slli___op1_x11__dest_x7__op1val__0x8000001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[10], 4160749567);
}


TEST_F(slli_01, opcode__slli___op1_x0__dest_x1__op1val_0x0___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[11], 0);
}


TEST_F(slli_01, opcode__slli___op1_x1__dest_x11__op1val__0x2000001___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[12], 4294934528);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x12__op1val__0x1000001___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[13], 4294443008);
}


TEST_F(slli_01, opcode__slli___op1_x13__dest_x9__op1val__0x800001___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[14], 4294901760);
}


TEST_F(slli_01, opcode__slli___op1_x5__dest_x13__op1val__0x400001___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[15], 2147483648);
}


TEST_F(slli_01, opcode__slli___op1_x8__dest_x15__op1val__0x200001___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[16], 4160749568);
}


TEST_F(slli_01, opcode__slli___op1_x15__dest_x4__op1val__0x100001___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[17], 4294963200);
}


TEST_F(slli_01, opcode__slli___op1_x14__dest_x5__op1val__0x80001___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[18], 4026531328);
}


TEST_F(slli_01, opcode__slli___op1_x6__dest_x2__op1val__0x40001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[19], 4294705151);
}


TEST_F(slli_01, opcode__slli___op1_x12__dest_x0__op1val__0x20001___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x10001___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[21], 4160747520);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x8001___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[22], 4294836224);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x4001___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[23], 4160749568);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x2001___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[24], 3221094400);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x1001___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[25], 3758096384);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x801___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[26], 4261396480);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x401___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[27], 4286578688);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x201___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[28], 4286562304);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x101___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[29], 4261281792);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x81___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[30], 4292853760);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x41___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[31], 4293902336);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x21___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[32], 4294958848);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x11___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[33], 4294966208);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x9___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[34], 4294967152);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x5___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[35], 4294639616);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x3___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[36], 4294965760);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x2___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[37], 4294967040);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x0___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[38], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x80000000___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[39], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x40000000___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[40], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x20000000___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[41], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x10000000___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[42], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x8000000___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[43], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x4000000___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[44], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x2000000___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[45], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x1000000___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[46], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x800000___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[47], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x400000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[48], 4194304);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x200000___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[49], 134217728);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x100000___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[50], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x80000___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[51], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x40000___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[52], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x20000___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[53], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x10000___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[54], 1073741824);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x8000___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[55], 134217728);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x4000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[56], 16384);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x2000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[57], 8192);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x1000___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[58], 8192);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x800___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[59], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x400___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[60], 4194304);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x200___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[61], 4096);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x100___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[62], 536870912);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x80___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[63], 0);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x40___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[64], 4096);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x20___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[65], 65536);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x10___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[66], 16);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x8___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[67], 4194304);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[68], 64);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x2___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[69], 4096);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x1___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[70], 16384);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x81___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[71], 4294966780);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0xb505___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[72], 1518501888);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0xb503___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[73], 2122317824);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x66666667___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[74], 939524096);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x33333334___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[75], 2577399808);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x6___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[76], 12288);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x55555555___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[77], 2863398912);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x55555556___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[78], 2147483648);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x3___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[79], 786432);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x55555556___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[80], 2863267840);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x55555555___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[81], 2863311488);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x1001___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[82], 4290771968);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[83], 2965696);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x66666665___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[84], 3435962368);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x33333332___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[85], 1717986916);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x55555554___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[86], 2147483648);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0xb504___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[87], 1778909184);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0xb504___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[88], 1474297856);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x66666666___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[89], 3221225472);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x33333333___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[90], 3435973632);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val_0x5___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[91], 671088640);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x4000001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[92], 4294967168);
}


TEST_F(slli_01, opcode__slli___op1_x10__dest_x11__op1val__0x20001___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[93], 4294901760);
}

