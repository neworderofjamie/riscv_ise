// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

// RISC-V architecture test includes
#include "riscv_arch_test/sub-01.h"

namespace
{
class sub_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::sub_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::sub_01::generateCode(false));
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

std::vector<uint8_t> sub_01::s_ScalarData;
}


// Generate google tests

TEST_F(sub_01, opcode__sub___op1_x14__op2_x9__dest_x4__op1val_0x2000000___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[5], 29360128);
}


TEST_F(sub_01, opcode__sub___op1_x1__op2_x0__dest_x1__op1val__0x400001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[6], 4290772991);
}


TEST_F(sub_01, opcode__sub___op1_x11__op2_x6__dest_x6__op1val__0x6___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[7], 1073741819);
}


TEST_F(sub_01, opcode__sub___op1_x12__op2_x12__dest_x15__op1val__0x800001___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[8], 0);
}


TEST_F(sub_01, opcode__sub___op1_x13__op2_x13__dest_x13__op1val__0x2000001___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x1__dest_x12__op1val__0x5___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[10], 134217724);
}


TEST_F(sub_01, opcode__sub___op1_x7__op2_x2__dest_x8__op1val_0x0___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[11], 67108865);
}


TEST_F(sub_01, opcode__sub___op1_x5__op2_x11__dest_x9__op1val__0x401___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[12], 33553408);
}


TEST_F(sub_01, opcode__sub___op1_x8__op2_x3__dest_x14__op1val_0x10___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[13], 16777233);
}


TEST_F(sub_01, opcode__sub___op1_x4__op2_x15__dest_x5__op1val_0x80___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[14], 8388737);
}


TEST_F(sub_01, opcode__sub___op1_x15__op2_x4__dest_x2__op1val_0x2___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[15], 4194307);
}


TEST_F(sub_01, opcode__sub___op1_x2__op2_x14__dest_x7__op1val_0x4___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[16], 2097157);
}


TEST_F(sub_01, opcode__sub___op1_x6__op2_x7__dest_x3__op1val__0x201___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[17], 1048064);
}


TEST_F(sub_01, opcode__sub___op1_x3__op2_x8__dest_x0__op1val__0x2___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[18], 0);
}


TEST_F(sub_01, opcode__sub___op1_x0__op2_x10__dest_x11__op1val_0x0___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[19], 262145);
}


TEST_F(sub_01, opcode__sub___op1_x9__op2_x5__dest_x10__op1val__0x200001___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[20], 4293001216);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[21], 65532);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[22], 32833);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[23], 4026548224);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[24], 859001651);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[25], 536875009);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[26], 4290775040);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[27], 1057);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[28], 4294705664);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[29], 4292870400);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[30], 4294921086);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[31], 1431655831);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[32], 35);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[33], 2863311548);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[34], 65545);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[35], 262149);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[36], 46344);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[37], 4294920959);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[38], 3579139413);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[39], 3154116607);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[40], 3221225471);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[41], 4160782336);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[42], 4227858431);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[43], 3419196619);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[44], 3434925259);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[45], 4286054399);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[46], 4294836217);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[47], 4294901888);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[48], 4294888188);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[49], 4294950903);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val__0x4) {
  ASSERT_EQ(getScalarWordData()[50], 4294959107);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[51], 4294701055);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[52], 536868864);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[53], 134217472);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[54], 4294965119);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[55], 65472);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[56], 524256);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[57], 2097136);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[58], 134217720);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[59], 4294959101);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[60], 715827883);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[61], 3221192703);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[62], 4060086272);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[63], 2729093803);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[64], 4261404671);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[65], 4278190063);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[66], 4292870160);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[67], 857944883);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[68], 4277665791);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[69], 393216);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[70], 134152192);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[71], 2064384);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[72], 4160733183);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[73], 536866816);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[74], 1073740800);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[75], 4294934015);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[76], 1431655508);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[77], 858993331);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[78], 4294967236);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[79], 4294966239);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[80], 4294967282);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[81], 4290772987);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[82], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[83], 536870911);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[84], 2147482624);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[85], 268435454);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[86], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[87], 17039361);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[88], 142606337);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[89], 2581174682);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[90], 2148532224);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[91], 33685505);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[92], 32768);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[93], 16379);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[94], 4294929147);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[95], 3968);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[96], 2147485696);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[97], 4286579712);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val__0x4) {
  ASSERT_EQ(getScalarWordData()[98], 516);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[99], 3758096640);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[100], 4294950920);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[101], 131074);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[102], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[103], 92680);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[104], 2577026718);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[105], 3436020177);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[106], 46335);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[107], 1431702106);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[108], 2863357871);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[109], 46337);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[110], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[111], 46341);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[112], 2577026720);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[113], 3436020179);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[114], 2863357873);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[115], 46339);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[116], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[117], 92681);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[118], 2577026719);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[119], 3436020178);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[120], 46336);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[121], 1431702107);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[122], 2863357872);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[123], 46338);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[124], 4294874616);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[125], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[126], 2576934038);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[127], 3435927497);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[128], 4294920951);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[129], 1431609426);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[130], 2863265191);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[131], 4294920953);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[132], 4294874618);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[133], 4294920957);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[134], 2576934040);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[135], 3435927499);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[136], 2863265193);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[137], 4294920955);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[138], 4294874617);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[139], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[140], 2576934039);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[141], 3435927498);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[142], 4294920952);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[143], 1431609427);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[144], 2863265192);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[145], 4294920954);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[146], 1717940578);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[147], 1718033258);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[148], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[149], 858993459);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[150], 1717986913);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[151], 3149642684);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[152], 286331153);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[153], 1717986915);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[154], 1717940580);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[155], 1717986919);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[156], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[157], 858993461);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[158], 286331155);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[159], 1717986917);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[160], 1717940579);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[161], 1718033259);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[162], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[163], 858993460);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[164], 1717986914);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[165], 3149642685);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[166], 286331154);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[167], 1717986916);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[168], 858947119);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[169], 859039799);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[170], 3435973837);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[171], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[172], 858993454);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[173], 2290649225);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[174], 3722304990);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[175], 858993456);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[176], 858947121);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[177], 858993460);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[178], 3435973839);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[179], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[180], 3722304992);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[181], 858993458);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[182], 858947120);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[183], 859039800);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[184], 3435973838);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[185], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[186], 858993455);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[187], 2290649226);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[188], 3722304991);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[189], 858993457);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[190], 4294920961);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[191], 46345);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[192], 2576980383);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[193], 3435973842);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[194], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[195], 1431655771);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[196], 2863311536);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[197], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[198], 4294920963);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[199], 6);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[200], 2576980385);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[201], 3435973844);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[202], 2863311538);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[203], 4);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[204], 4294920962);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[205], 46346);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[206], 2576980384);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[207], 3435973843);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[208], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[209], 1431655772);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[210], 2863311537);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[211], 3);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[212], 2863265190);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[213], 2863357870);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[214], 1145324612);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[215], 2004318071);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[216], 2863311525);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[217], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[218], 1431655765);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[219], 2863311527);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[220], 2863265192);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[221], 2863311531);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[222], 1145324614);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[223], 2004318073);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[224], 1431655767);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[225], 2863311529);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[226], 2863265191);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[227], 2863357871);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[228], 1145324613);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[229], 2004318072);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[230], 2863311526);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[231], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[232], 1431655766);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[233], 2863311528);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[234], 1431609425);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[235], 1431702105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[236], 4008636143);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[237], 572662306);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[238], 1431655760);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[239], 2863311531);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[240], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[241], 1431655762);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[242], 1431609427);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[243], 1431655766);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[244], 4008636145);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[245], 572662308);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[246], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[247], 1431655764);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[248], 1431609426);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[249], 1431702106);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[250], 4008636144);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[251], 572662307);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[252], 1431655761);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[253], 2863311532);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[254], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[255], 1431655763);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[256], 4294920959);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[257], 46343);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[258], 2576980381);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[259], 3435973840);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[260], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[261], 1431655769);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[262], 2863311534);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[263], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[264], 4294920961);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[265], 4);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[266], 2576980383);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[267], 3435973842);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[268], 2863311536);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[269], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[270], 4294920960);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[271], 46344);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[272], 2576980382);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[273], 3435973841);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[274], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[275], 1431655770);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[276], 2863311535);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[277], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[278], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[279], 92678);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[280], 2577026716);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[281], 3436020175);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[282], 46333);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[283], 1431702104);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[284], 2863357869);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[285], 46335);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[286], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[287], 46339);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[288], 2577026718);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[289], 3436020177);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[290], 2863357871);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[291], 46337);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[292], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[293], 92679);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[294], 2577026717);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[295], 3436020176);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[296], 46334);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[297], 1431702105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[298], 2863357870);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[299], 46336);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[300], 4294920955);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[301], 46339);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[302], 2576980377);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[303], 3435973836);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[304], 4294967290);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[305], 1431655765);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[306], 2863311530);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[307], 4294967292);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[308], 4294920957);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[309], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[310], 2576980379);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[311], 3435973838);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[312], 2863311532);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[313], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[314], 4294920956);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[315], 46340);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[316], 2576980378);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[317], 3435973837);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[318], 4294967291);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[319], 1431655766);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[320], 2863311531);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[321], 4294967293);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[322], 1717940576);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[323], 1718033256);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[324], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[325], 858993457);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[326], 1717986911);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[327], 3149642682);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[328], 286331151);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[329], 1717986913);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[330], 1717940578);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[331], 1717986917);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[332], 858993459);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[333], 3435973838);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[334], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[335], 3722304991);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[336], 858993457);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[337], 858947119);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[338], 859039799);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[339], 3435973837);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[340], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[341], 858993454);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[342], 2290649225);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[343], 3722304990);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[344], 858993456);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[345], 4294920960);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[346], 46344);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[347], 2576980382);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[348], 3435973841);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[349], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[350], 1431655770);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[351], 2863311535);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[352], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[353], 4294920962);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[354], 5);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[355], 2576980384);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[356], 3435973843);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[357], 2863311537);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[358], 3);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[359], 4294920961);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[360], 46345);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[361], 2576980383);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[362], 3435973842);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[363], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[364], 1431655771);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[365], 2863311536);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[366], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[367], 2863265189);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[368], 2863357869);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[369], 1145324611);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[370], 2004318070);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[371], 2863311524);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[372], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[373], 1431655764);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[374], 2863311526);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[375], 2863265191);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[376], 2863311530);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[377], 1145324613);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[378], 2004318072);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[379], 1431655766);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[380], 2863311528);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[381], 2863265190);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[382], 2863357870);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[383], 1145324612);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[384], 2004318071);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[385], 2863311525);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[386], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[387], 1431655765);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[388], 2863311527);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[389], 1431609424);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[390], 1431702104);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[391], 4008636142);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[392], 572662305);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[393], 1431655759);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[394], 2863311530);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[395], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[396], 1431655761);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[397], 1431609426);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[398], 1431655765);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[399], 4008636144);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[400], 572662307);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[401], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[402], 1431655763);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[403], 1431609425);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[404], 1431702105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[405], 4008636143);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[406], 572662306);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[407], 1431655760);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[408], 2863311531);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[409], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[410], 1431655762);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[411], 4294920958);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[412], 46342);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[413], 2576980380);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[414], 3435973839);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[415], 4294967293);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[416], 1431655768);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[417], 2863311533);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[418], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[419], 4294920960);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[420], 3);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[421], 2576980382);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[422], 3435973841);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[423], 2863311535);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[424], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[425], 4294920959);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[426], 46343);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[427], 2576980381);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[428], 3435973840);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[429], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[430], 1431655769);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[431], 2863311534);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[432], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[433], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[434], 858993459);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[435], 286331153);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[436], 1717986915);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[437], 1717940577);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[438], 1718033257);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[439], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[440], 858993458);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[441], 1717986912);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[442], 3149642683);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[443], 286331152);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[444], 1717986914);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[445], 858947117);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[446], 859039797);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[447], 3435973835);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[448], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[449], 858993452);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[450], 2290649223);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[451], 3722304988);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[452], 858993454);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[453], 858947119);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[454], 858993458);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[455], 3435973837);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[456], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[457], 3722304990);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[458], 858993456);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[459], 858947118);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[460], 859039798);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[461], 3435973836);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[462], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[463], 858993453);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[464], 2290649224);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[465], 3722304989);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[466], 858993455);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[467], 1431609423);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[468], 1431702103);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[469], 4008636141);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[470], 572662304);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[471], 1431655758);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[472], 2863311529);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[473], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[474], 1431655760);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[475], 1431609425);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[476], 1431655764);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[477], 4008636143);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[478], 572662306);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[479], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[480], 1431655762);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[481], 1431609424);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[482], 1431702104);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[483], 4008636142);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[484], 572662305);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[485], 1431655759);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[486], 2863311530);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[487], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[488], 1431655761);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[489], 4294920957);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[490], 46341);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[491], 2576980379);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[492], 3435973838);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[493], 4294967292);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[494], 1431655767);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[495], 2863311532);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[496], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[497], 4294920959);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[498], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[499], 2576980381);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[500], 3435973840);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[501], 2863311534);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[502], 4294920958);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[503], 46342);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[504], 2576980380);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[505], 3435973839);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[506], 4294967293);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[507], 1431655768);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[508], 2863311533);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[509], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[510], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[511], 92679);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[512], 2577026717);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[513], 3436020176);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[514], 46334);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[515], 1431702105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[516], 2863357870);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[517], 46336);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[518], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[519], 46340);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[520], 2577026719);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[521], 3436020178);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[522], 2863357872);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[523], 46338);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[524], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[525], 92680);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[526], 2577026718);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[527], 3436020177);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[528], 46335);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[529], 1431702106);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[530], 2863357871);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[531], 46337);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[532], 4294874615);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[533], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[534], 2576934037);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[535], 3435927496);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[536], 4294920950);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[537], 1431609425);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[538], 2863265190);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[539], 4294920952);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[540], 4294874617);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[541], 4294920956);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[542], 2576934039);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[543], 3435927498);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[544], 2863265192);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[545], 4294920954);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[546], 4294874616);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[547], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[548], 2576934038);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[549], 3435927497);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[550], 4294920951);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[551], 1431609426);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[552], 2863265191);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[553], 4294920953);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[554], 1717940577);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[555], 1718033257);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[556], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[557], 858993458);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[558], 1717986912);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[559], 3149642683);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[560], 286331152);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[561], 1717986914);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[562], 1717940579);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[563], 1717986918);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[564], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[565], 858993460);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[566], 286331154);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[567], 1717986916);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[568], 1717940578);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[569], 1718033258);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[570], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[571], 858993459);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[572], 1717986913);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[573], 3149642684);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[574], 286331153);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[575], 1717986915);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[576], 858947118);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[577], 859039798);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[578], 3435973836);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[579], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[580], 858993453);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[581], 2290649224);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[582], 3722304989);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[583], 858993455);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[584], 858947120);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[585], 2143289344);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[586], 528482304);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[587], 234881024);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[588], 524287);
}

