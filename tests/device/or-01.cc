// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

// RISC-V architecture test includes
#include "riscv_arch_test/or-01.h"

namespace
{
class or_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::or_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::or_01::generateCode(false));
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

std::vector<uint8_t> or_01::s_ScalarData;
}


// Generate google tests

TEST_F(or_01, opcode__or___op1_x10__op2_x3__dest_x2__op1val__0x200001___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[5], 4292870143);
}


TEST_F(or_01, opcode__or___op1_x14__op2_x10__dest_x14__op1val_0x5___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[6], 2147483647);
}


TEST_F(or_01, opcode__or___op1_x1__op2_x6__dest_x6__op1val__0xb504___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[7], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x11__op2_x11__dest_x9__op1val__0x40000000___op2val__0x40000000) {
  ASSERT_EQ(getScalarWordData()[8], 3221225472);
}


TEST_F(or_01, opcode__or___op1_x8__op2_x8__dest_x8__op1val_0x4000___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[9], 16384);
}


TEST_F(or_01, opcode__or___op1_x7__op2_x9__dest_x12__op1val__0x2001___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[10], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x4__op2_x1__dest_x15__op1val__0x3___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[11], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x6__op2_x13__dest_x10__op1val_0x400___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[12], 4261412863);
}


TEST_F(or_01, opcode__or___op1_x5__op2_x14__dest_x0__op1val_0x0___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(or_01, opcode__or___op1_x9__op2_x0__dest_x1__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[14], 6);
}


TEST_F(or_01, opcode__or___op1_x13__op2_x5__dest_x11__op1val_0x6___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[15], 4290772991);
}


TEST_F(or_01, opcode__or___op1_x12__op2_x2__dest_x5__op1val_0x8000000___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[16], 4292870143);
}


TEST_F(or_01, opcode__or___op1_x3__op2_x12__dest_x7__op1val_0xb504___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[17], 4293918719);
}


TEST_F(or_01, opcode__or___op1_x0__op2_x15__dest_x13__op1val_0x0___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[18], 4294443007);
}


TEST_F(or_01, opcode__or___op1_x15__op2_x7__dest_x3__op1val__0x55555556___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[19], 4294705151);
}


TEST_F(or_01, opcode__or___op1_x2__op2_x12__dest_x1__op1val__0x55555555___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[20], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x9__op2_x4__dest_x10__op1val_0x9___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[21], 4294901759);
}


TEST_F(or_01, opcode__or___op1_x3__op2_x8__dest_x4__op1val__0xb504___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[22], 4294934527);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[23], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[24], 4294959103);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[25], 4294963199);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[26], 4294965247);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[27], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[28], 4294966783);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[29], 4294967039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[30], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[31], 4294967231);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[32], 4294967263);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[33], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[34], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x8___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[35], 4294967291);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[36], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[37], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[38], 3221225471);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[39], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[40], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[41], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[42], 4261412863);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[43], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[44], 4286578687);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[45], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[46], 4293918719);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[47], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[48], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[49], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[50], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[51], 4294934527);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[52], 4294950911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[53], 4294963199);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val__0x6) {
  ASSERT_EQ(getScalarWordData()[54], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[55], 4294966271);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[56], 4294967039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val__0x1) {
  ASSERT_EQ(getScalarWordData()[57], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val__0x1) {
  ASSERT_EQ(getScalarWordData()[58], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[59], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val__0x6) {
  ASSERT_EQ(getScalarWordData()[60], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[61], 4294967291);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[62], 2147483653);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[63], 1073774592);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[64], 4294965247);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[65], 134217736);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[66], 2930420394);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[67], 33600772);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[68], 4294959103);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x7___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[69], 4294967289);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[70], 4194320);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[71], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[72], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[73], 308483);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[74], 131080);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[75], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[76], 1431663958);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[77], 4098);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[78], 2052);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[79], 2147484160);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3fffffff___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[80], 1073741823);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[81], 96);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[82], 4294965247);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[83], 524292);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[84], 4294966783);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[85], 1);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[86], 4294967167);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[87], 570425344);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[88], 268435456);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[89], 603979776);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val__0x6) {
  ASSERT_EQ(getScalarWordData()[90], 4294967290);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[91], 20971520);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[92], 1440044373);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val__0xa) {
  ASSERT_EQ(getScalarWordData()[93], 4294967286);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[94], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[95], 1048704);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[96], 327680);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[97], 135168);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[98], 65542);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[99], 8200);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[100], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[101], 1431657813);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[102], 544);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[103], 4294967167);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[104], 33554433);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[105], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[106], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[107], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[108], 859027253);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[109], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[110], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[111], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[112], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[113], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[114], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[115], 1718024037);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[116], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[117], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[118], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[119], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[120], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[121], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[122], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[123], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[124], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[125], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[126], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[127], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[128], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[129], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[130], 4294933501);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[131], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[132], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[133], 4294926335);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[134], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[135], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[136], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[137], 4294930173);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[138], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[139], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[140], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[141], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[142], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[143], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[144], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[145], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[146], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[147], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[148], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[149], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[150], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[151], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[152], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[153], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[154], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[155], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[156], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[157], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[158], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[159], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[160], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[161], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[162], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[163], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[164], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[165], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[166], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[167], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[168], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[169], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[170], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[171], 859027253);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[172], 4294933501);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[173], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[174], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[175], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[176], 3149642687);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[177], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[178], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[179], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[180], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[181], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[182], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[183], 2004318068);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[184], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[185], 859027252);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[186], 4294933500);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[187], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[188], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[189], 858993461);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[190], 3149642686);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[191], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[192], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[193], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[194], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[195], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[196], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[197], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[198], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[199], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[200], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[201], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[202], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[203], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[204], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[205], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[206], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[207], 46342);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[208], 4294920958);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[209], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[210], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[211], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[212], 2863311534);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[213], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[214], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[215], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[216], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[217], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[218], 3149642687);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[219], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[220], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[221], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[222], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[223], 2863316907);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[224], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[225], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[226], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[227], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[228], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[229], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[230], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[231], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[232], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[233], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[234], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[235], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[236], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[237], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[238], 4294926335);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[239], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[240], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[241], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[242], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[243], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[244], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[245], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[246], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[247], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[248], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[249], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[250], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[251], 1431696726);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[252], 4294926334);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[253], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[254], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[255], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[256], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[257], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[258], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[259], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[260], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[261], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[262], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[263], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[264], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[265], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[266], 4);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[267], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[268], 4);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[269], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[270], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[271], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[272], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[273], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[274], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[275], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[276], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[277], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[278], 2863311534);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[279], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[280], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[281], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[282], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[283], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[284], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[285], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[286], 2863316907);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[287], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[288], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[289], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[290], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[291], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[292], 859027251);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[293], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[294], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[295], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[296], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[297], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[298], 859027251);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[299], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[300], 2863316907);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[301], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[302], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[303], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[304], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[305], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[306], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[307], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[308], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[309], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[310], 4);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[311], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[312], 0);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[313], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[314], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[315], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[316], 2);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[317], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[318], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[319], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[320], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[321], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[322], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[323], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[324], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[325], 1718024037);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[326], 4294930173);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[327], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[328], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[329], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[330], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[331], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[332], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[333], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[334], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[335], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[336], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[337], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[338], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[339], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[340], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[341], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[342], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[343], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[344], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[345], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[346], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[347], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[348], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[349], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[350], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[351], 858993461);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[352], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[353], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[354], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[355], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[356], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[357], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[358], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[359], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[360], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[361], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[362], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[363], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[364], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[365], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[366], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[367], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[368], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[369], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[370], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[371], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[372], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[373], 3149642686);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[374], 2863311534);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[375], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[376], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[377], 2863311534);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[378], 2863316907);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[379], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[380], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[381], 3149642682);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[382], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[383], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[384], 2863316910);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[385], 4294961918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[386], 4008636142);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[387], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[388], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[389], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[390], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[391], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[392], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[393], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[394], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[395], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[396], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[397], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[398], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[399], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[400], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[401], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[402], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[403], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[404], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[405], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[406], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[407], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[408], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[409], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[410], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[411], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[412], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[413], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[414], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[415], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[416], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[417], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[418], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[419], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[420], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[421], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[422], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[423], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[424], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[425], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[426], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[427], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[428], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[429], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[430], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[431], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[432], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[433], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[434], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[435], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[436], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[437], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[438], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[439], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[440], 1718024037);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[441], 4294930173);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[442], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[443], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[444], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[445], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[446], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[447], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[448], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[449], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[450], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[451], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[452], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[453], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[454], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[455], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[456], 859027251);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[457], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[458], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[459], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[460], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[461], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[462], 859027254);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[463], 4294933502);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[464], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[465], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[466], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[467], 3149642682);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[468], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[469], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[470], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[471], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[472], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[473], 2004318068);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[474], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[475], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[476], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[477], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[478], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[479], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[480], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[481], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[482], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[483], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[484], 1431696724);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[485], 4294926332);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[486], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[487], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[488], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[489], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[490], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[491], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[492], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[493], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[494], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[495], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[496], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[497], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[498], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[499], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[500], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[501], 2);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[502], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[503], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[504], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[505], 2);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[506], 46342);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[507], 4294920958);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[508], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[509], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[510], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[511], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[512], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[513], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[514], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[515], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[516], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[517], 859027252);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[518], 46342);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[519], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[520], 1431696726);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[521], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[522], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[523], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[524], 1718024037);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[525], 859027254);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[526], 1431696724);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[527], 46342);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[528], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[529], 4294967292);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[530], 1718024038);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[531], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[532], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[533], 2863316910);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[534], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[535], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[536], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[537], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[538], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[539], 4294933500);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[540], 4294920958);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[541], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[542], 4294926334);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[543], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[544], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[545], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[546], 4294930173);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[547], 4294933502);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[548], 4294926332);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[549], 4294920958);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[550], 4294967292);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[551], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[552], 4294930174);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[553], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[554], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[555], 4294961918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[556], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[557], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[558], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[559], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[560], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[561], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[562], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[563], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[564], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[565], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[566], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[567], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[568], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[569], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[570], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[571], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[572], 1718024038);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[573], 4294930174);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[574], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[575], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[576], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[577], 4008636142);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[578], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[579], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[580], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[581], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[582], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[583], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[584], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[585], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[586], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[587], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[588], 859027251);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x40000000___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[589], 3758096383);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[590], 4026531839);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[591], 4278190079);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[592], 4286578687);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[593], 4294967295);
}

