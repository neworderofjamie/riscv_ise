// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

// RISC-V architecture test includes
#include "riscv_arch_test/add-01.h"

namespace
{
class add_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::add_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::add_01::generateCode(false));
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

std::vector<uint8_t> add_01::s_ScalarData;
}


// Generate google tests

TEST_F(add_01, opcode__add___op1_x7__op2_x9__dest_x5__op1val__0x801___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[5], 4294948862);
}


TEST_F(add_01, opcode__add___op1_x12__op2_x3__dest_x12__op1val__0x40001___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[6], 2147221502);
}


TEST_F(add_01, opcode__add___op1_x6__op2_x1__dest_x1__op1val__0x400001___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[7], 3217031166);
}


TEST_F(add_01, opcode__add___op1_x2__op2_x2__dest_x7__op1val_0x20___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[8], 64);
}


TEST_F(add_01, opcode__add___op1_x0__op2_x0__dest_x0__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(add_01, opcode__add___op1_x8__op2_x4__dest_x14__op1val__0x1000001___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[10], 4143972350);
}


TEST_F(add_01, opcode__add___op1_x4__op2_x5__dest_x15__op1val_0x7___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[11], 4227858438);
}


TEST_F(add_01, opcode__add___op1_x14__op2_x11__dest_x6__op1val_0x33333333___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[12], 825439026);
}


TEST_F(add_01, opcode__add___op1_x3__op2_x10__dest_x2__op1val_0x800___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[13], 4278192127);
}


TEST_F(add_01, opcode__add___op1_x13__op2_x6__dest_x9__op1val__0x11___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[14], 4286578670);
}


TEST_F(add_01, opcode__add___op1_x15__op2_x14__dest_x13__op1val__0x5___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[15], 4290772986);
}


TEST_F(add_01, opcode__add___op1_x11__op2_x12__dest_x3__op1val__0x2000001___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[16], 4259315710);
}


TEST_F(add_01, opcode__add___op1_x5__op2_x15__dest_x11__op1val__0x20001___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[17], 4293787646);
}


TEST_F(add_01, opcode__add___op1_x1__op2_x13__dest_x10__op1val__0x200001___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[18], 4292345854);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x8__dest_x4__op1val_0x80___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[19], 4294705279);
}


TEST_F(add_01, opcode__add___op1_x9__op2_x7__dest_x8__op1val__0x2000001___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[20], 4261281790);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[21], 2863245993);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[22], 858960689);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[23], 4294959106);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x9___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[24], 4294963208);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[25], 4294440958);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[26], 4294966271);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[27], 4294964734);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[28], 1431655508);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[29], 4294967164);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[30], 4292870078);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[31], 4294967134);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x9___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[32], 4294967288);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[33], 4294950902);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[34], 134217723);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[35], 4292870140);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[36], 4294950909);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[37], 2147614719);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[38], 644245092);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[39], 3758063614);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[40], 4026531871);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[41], 4160749438);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[42], 4228907007);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val__0x4) {
  ASSERT_EQ(getScalarWordData()[43], 4286578683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[44], 4293918719);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[45], 4294897662);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[46], 491519);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[47], 4294959103);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[48], 258047);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[49], 4294966527);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[50], 4294958590);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[51], 4294962942);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[52], 4294967214);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[53], 4294967270);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[54], 4261412854);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val__0x6) {
  ASSERT_EQ(getScalarWordData()[55], 4294967288);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[56], 2147483654);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[57], 1610612736);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x40000000___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[58], 3758096384);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[59], 3131746986);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[60], 134217729);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[61], 1498764628);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[62], 33554496);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[63], 16777219);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x6___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[64], 8388602);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[65], 4194320);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[66], 1048575);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[67], 65536);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[68], 4294934527);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[69], 4294937341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[70], 1431663957);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[71], 69632);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[72], 2048);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[73], 525312);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[74], 509);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[75], 4294966911);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[76], 66);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[77], 4292870159);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[78], 4294967287);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[79], 4294967291);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[80], 4294967233);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3fffffff___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[81], 1073741824);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[82], 2147483654);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[83], 1073741829);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[84], 268389116);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[85], 67108870);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[86], 33554423);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val__0x7) {
  ASSERT_EQ(getScalarWordData()[87], 16777209);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[88], 8388606);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[89], 4193791);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val__0x8) {
  ASSERT_EQ(getScalarWordData()[90], 2097144);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[91], 3222274047);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[92], 262015);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[93], 3221356543);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[94], 16809984);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val__0x40000000) {
  ASSERT_EQ(getScalarWordData()[95], 3221241856);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[96], 270336);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[97], 2863315626);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[98], 4160750591);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[99], 513);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[100], 4294902015);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[101], 4294443015);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[102], 92682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[103], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[104], 1718033260);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[105], 859039801);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[106], 46347);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[107], 2863357872);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[108], 1431702107);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[109], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[110], 92680);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[111], 46341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[112], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[113], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[114], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[115], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[116], 92681);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[117], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[118], 1718033259);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[119], 859039800);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[120], 46346);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[121], 2863357871);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[122], 1431702106);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[123], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[124], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[125], 4294874618);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[126], 1717940580);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[127], 858947121);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[128], 4294920963);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[129], 2863265192);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[130], 1431609427);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[131], 4294920961);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[132], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[133], 4294920957);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[134], 1717940578);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[135], 858947119);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[136], 1431609425);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[137], 4294920959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[138], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[139], 4294874617);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[140], 1717940579);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[141], 858947120);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[142], 4294920962);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[143], 2863265191);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[144], 1431609426);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[145], 4294920960);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[146], 1718033260);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[147], 1717940580);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[148], 3435973838);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[149], 2576980379);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[150], 1717986925);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[151], 286331154);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[152], 3149642685);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[153], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[154], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[155], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[156], 3435973836);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[157], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[158], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[159], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[160], 1718033259);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[161], 1717940579);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[162], 3435973837);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[163], 2576980378);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[164], 1717986924);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[165], 286331153);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[166], 3149642684);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[167], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[168], 859039801);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[169], 858947121);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[170], 2576980379);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[171], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[172], 858993466);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[173], 3722304991);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[174], 2290649226);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[175], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[176], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[177], 858993460);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[178], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[179], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[180], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[181], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[182], 859039800);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[183], 858947120);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[184], 2576980378);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[185], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[186], 858993465);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[187], 3722304990);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[188], 2290649225);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[189], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[190], 46347);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[191], 4294920963);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[192], 1717986925);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[193], 858993466);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[194], 12);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[195], 2863311537);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[196], 1431655772);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[197], 10);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[198], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[199], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[200], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[201], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[202], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[203], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[204], 46346);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[205], 4294920962);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[206], 1717986924);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[207], 858993465);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[208], 11);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[209], 2863311536);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[210], 1431655771);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[211], 9);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[212], 2863357872);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[213], 2863265192);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[214], 286331154);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[215], 3722304991);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[216], 2863311537);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[217], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[218], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[219], 2863311535);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[220], 2863357870);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[221], 2863311531);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[222], 286331152);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[223], 3722304989);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[224], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[225], 2863311533);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[226], 2863357871);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[227], 2863265191);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[228], 286331153);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[229], 3722304990);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[230], 2863311536);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[231], 1431655765);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[232], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[233], 2863311534);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[234], 1431702107);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[235], 1431609427);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[236], 3149642685);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[237], 2290649226);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[238], 1431655772);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[239], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[240], 2863311532);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[241], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[242], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[243], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[244], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[245], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[246], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[247], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[248], 1431702106);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[249], 1431609426);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[250], 3149642684);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[251], 2290649225);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[252], 1431655771);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[253], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[254], 2863311531);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[255], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[256], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[257], 4294920961);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[258], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[259], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[260], 10);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[261], 2863311535);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[262], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[263], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[264], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[265], 4);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[266], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[267], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[268], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[269], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[270], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[271], 4294920960);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[272], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[273], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[274], 9);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[275], 2863311534);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[276], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[277], 7);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[278], 92680);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[279], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[280], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[281], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[282], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[283], 2863357870);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[284], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[285], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[286], 92678);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[287], 46339);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[288], 1718033256);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[289], 859039797);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[290], 1431702103);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[291], 46341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[292], 92679);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[293], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[294], 1718033257);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[295], 859039798);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[296], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[297], 2863357869);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[298], 1431702104);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[299], 46342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[300], 46341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[301], 4294920957);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[302], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[303], 858993460);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[304], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[305], 2863311531);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[306], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[307], 4);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[308], 46339);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[309], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[310], 1717986917);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[311], 858993458);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[312], 1431655764);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[313], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[314], 46340);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[315], 4294920956);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[316], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[317], 858993459);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[318], 5);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[319], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[320], 1431655765);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[321], 3);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[322], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[323], 1717940578);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[324], 3435973836);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[325], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[326], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[327], 286331152);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[328], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[329], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[330], 1718033256);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[331], 858993459);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[332], 2576980376);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[333], 1717986917);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[334], 2290649223);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[335], 858993461);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[336], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[337], 858947119);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[338], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[339], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[340], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[341], 3722304989);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[342], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[343], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[344], 46346);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[345], 4294920962);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[346], 1717986924);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[347], 858993465);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[348], 11);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[349], 2863311536);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[350], 1431655771);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[351], 9);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[352], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[353], 5);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[354], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[355], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[356], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[357], 7);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[358], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[359], 4294920961);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[360], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[361], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[362], 10);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[363], 2863311535);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[364], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[365], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[366], 2863357871);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[367], 2863265191);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[368], 286331153);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[369], 3722304990);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[370], 2863311536);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[371], 1431655765);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[372], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[373], 2863311534);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[374], 2863357869);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[375], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[376], 286331151);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[377], 3722304988);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[378], 4294967294);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[379], 2863311532);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[380], 2863357870);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[381], 2863265190);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[382], 286331152);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[383], 3722304989);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[384], 2863311535);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[385], 1431655764);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[386], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[387], 2863311533);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[388], 1431702106);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[389], 1431609426);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[390], 3149642684);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[391], 2290649225);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[392], 1431655771);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[393], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[394], 2863311531);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[395], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[396], 1431702104);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[397], 1431655765);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[398], 3149642682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[399], 2290649223);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[400], 2863311529);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[401], 1431655767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[402], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[403], 1431609425);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[404], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[405], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[406], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[407], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[408], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[409], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[410], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[411], 4294920960);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[412], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[413], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[414], 9);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[415], 2863311534);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[416], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[417], 7);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[418], 46342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[419], 3);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[420], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[421], 858993461);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[422], 1431655767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[423], 5);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[424], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[425], 4294920959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[426], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[427], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[428], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[429], 2863311533);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[430], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[431], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[432], 1717986917);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[433], 3435973834);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[434], 2576980375);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[435], 3149642681);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[436], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[437], 1718033257);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[438], 1717940577);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[439], 3435973835);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[440], 2576980376);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[441], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[442], 286331151);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[443], 3149642682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[444], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[445], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[446], 858947119);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[447], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[448], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[449], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[450], 3722304989);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[451], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[452], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[453], 859039797);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[454], 858993458);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[455], 2576980375);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[456], 1717986916);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[457], 2290649222);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[458], 858993460);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[459], 859039798);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[460], 858947118);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[461], 2576980376);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[462], 1717986917);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[463], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[464], 3722304988);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[465], 2290649223);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[466], 858993461);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[467], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[468], 1431609425);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[469], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[470], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[471], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[472], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[473], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[474], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[475], 1431702103);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[476], 1431655764);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[477], 3149642681);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[478], 2290649222);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[479], 2863311528);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[480], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[481], 1431702104);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[482], 1431609424);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[483], 3149642682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[484], 2290649223);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[485], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[486], 4294967294);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[487], 2863311529);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[488], 1431655767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[489], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[490], 4294920959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[491], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[492], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[493], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[494], 2863311533);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[495], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[496], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[497], 46341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[498], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[499], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[500], 858993460);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[501], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[502], 4);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[503], 46342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[504], 4294920958);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[505], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[506], 858993461);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[507], 7);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[508], 2863311532);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[509], 1431655767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[510], 5);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[511], 92681);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[512], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[513], 1718033259);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[514], 859039800);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[515], 46346);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[516], 2863357871);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[517], 1431702106);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[518], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[519], 92679);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[520], 46340);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[521], 1718033257);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[522], 859039798);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[523], 1431702104);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[524], 46342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[525], 92680);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[526], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[527], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[528], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[529], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[530], 2863357870);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[531], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[532], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[533], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[534], 4294874617);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[535], 1717940579);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[536], 858947120);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[537], 4294920962);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[538], 2863265191);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[539], 1431609426);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[540], 4294920960);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[541], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[542], 4294920956);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[543], 1717940577);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[544], 858947118);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[545], 1431609424);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[546], 4294920958);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[547], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[548], 4294874616);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[549], 1717940578);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[550], 858947119);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[551], 4294920961);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[552], 2863265190);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[553], 1431609425);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[554], 4294920959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[555], 1718033259);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[556], 1717940579);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[557], 3435973837);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[558], 2576980378);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[559], 1717986924);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[560], 286331153);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[561], 3149642684);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[562], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[563], 1718033257);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[564], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[565], 3435973835);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[566], 2576980376);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[567], 3149642682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[568], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[569], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[570], 1717940578);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[571], 3435973836);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[572], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[573], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[574], 286331152);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[575], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[576], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[577], 859039800);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[578], 858947120);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[579], 2576980378);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[580], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[581], 858993465);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[582], 3722304990);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[583], 2290649225);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[584], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[585], 859039798);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[586], 3758096415);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[587], 4026531843);
}

