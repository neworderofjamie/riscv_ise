// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

// RISC-V architecture test includes
#include "riscv_arch_test/ori-01.h"

namespace
{
class ori_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::ori_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::ori_01::generateCode(false));
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

std::vector<uint8_t> ori_01::s_ScalarData;
}


// Generate google tests

TEST_F(ori_01, opcode__ori___op1_x12__dest_x9__op1val_0x200000___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[5], 4294965248);
}


TEST_F(ori_01, opcode__ori___op1_x0__dest_x0__op1val_0x0___immval_0x3ff) {
  ASSERT_EQ(getScalarWordData()[6], 0);
}


TEST_F(ori_01, opcode__ori___op1_x6__dest_x7__op1val__0x40000001___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[7], 3221225471);
}


TEST_F(ori_01, opcode__ori___op1_x9__dest_x3__op1val__0x20000001___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[8], 3758096383);
}


TEST_F(ori_01, opcode__ori___op1_x5__dest_x14__op1val__0x10000001___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[9], 4026531839);
}


TEST_F(ori_01, opcode__ori___op1_x13__dest_x11__op1val__0x8000001___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[10], 4160749567);
}


TEST_F(ori_01, opcode__ori___op1_x3__dest_x5__op1val__0x4000001___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[11], 4227858431);
}


TEST_F(ori_01, opcode__ori___op1_x11__dest_x12__op1val__0x2000001___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[12], 4261412863);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x15__op1val__0x1000001___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[13], 4278190079);
}


TEST_F(ori_01, opcode__ori___op1_x4__dest_x13__op1val__0x800001___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[14], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x15__dest_x10__op1val__0x400001___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[15], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x7__dest_x4__op1val__0x200001___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[16], 4292870143);
}


TEST_F(ori_01, opcode__ori___op1_x2__dest_x6__op1val__0x100001___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[17], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x8__dest_x1__op1val__0x80001___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[18], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x1__dest_x2__op1val__0x40001___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[19], 4294705151);
}


TEST_F(ori_01, opcode__ori___op1_x14__dest_x8__op1val__0x20001___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[20], 4294836223);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x10001___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[21], 4294901759);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x8001___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[22], 4294934527);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x4001___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[23], 4294950911);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x2001___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[24], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x1001___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[25], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x801___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[26], 4294965247);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x401___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[27], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x201___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[28], 4294966783);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x101___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[29], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x81___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[30], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x41___immval__0x4) {
  ASSERT_EQ(getScalarWordData()[31], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x21___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[32], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x11___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[33], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x9___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[34], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x5___immval__0x7) {
  ASSERT_EQ(getScalarWordData()[35], 4294967291);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x3___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[36], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x2___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[37], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2000___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[38], 10239);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[39], 4294966271);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x20000___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[40], 4294966783);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[41], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[42], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x200001___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[43], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x80000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[44], 2147483648);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x40000000___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[45], 4294967294);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x20000000___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[46], 536872277);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x10000000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[47], 4294965930);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x8000000___immval__0x1) {
  ASSERT_EQ(getScalarWordData()[48], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4000000___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[49], 67109682);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2000000___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[50], 33555798);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x1000000___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[51], 4294966271);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x800000___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[52], 4294967039);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x400000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[53], 4194304);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x100000___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[54], 4294967263);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x80000___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[55], 4294966271);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x40000___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[56], 263510);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x10000___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[57], 65537);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x8000___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[58], 33280);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4000___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[59], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x1000___immval__0x8) {
  ASSERT_EQ(getScalarWordData()[60], 4294967288);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x800___immval__0x8) {
  ASSERT_EQ(getScalarWordData()[61], 4294967288);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x400___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[62], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x200___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[63], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x100___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[64], 258);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x80___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[65], 128);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x40___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[66], 70);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x20___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[67], 4294967284);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x10___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[68], 1365);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x8___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[69], 4294967294);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x3ff) {
  ASSERT_EQ(getScalarWordData()[70], 1023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[71], 2);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x1___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[72], 1);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x20000000___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[73], 536871168);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[74], 68);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[75], 858993460);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[76], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x8___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[77], 4294967288);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[78], 46383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[79], 4294967253);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[80], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[81], 46901);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[82], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[83], 4294967215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[84], 46423);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[85], 46341);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[86], 46381);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[87], 46341);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[88], 46949);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[89], 46903);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[90], 46421);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[91], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[92], 46381);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[93], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[94], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[95], 46903);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[96], 46341);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[97], 4294967215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[98], 46421);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[99], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[100], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[101], 4294967293);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[102], 4294921983);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[103], 4294921213);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[104], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[105], 4294966015);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[106], 4294922239);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[107], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[108], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[109], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[110], 4294921981);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[111], 4294921215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[112], 4294922237);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[113], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[114], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[115], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[116], 4294921983);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[117], 4294921215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[118], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[119], 4294966015);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[120], 4294922237);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[121], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[122], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[123], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[124], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[125], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[126], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[127], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[128], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[129], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[130], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[131], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[132], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[133], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[134], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[135], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[136], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[137], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[138], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[139], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[140], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[141], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[142], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[143], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[144], 858993470);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[145], 4294967284);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[146], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[147], 858993460);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[148], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[149], 4294966207);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[150], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[151], 858993460);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[152], 858993468);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[153], 858993460);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[154], 858994549);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[155], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[156], 858994548);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[157], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[158], 858993469);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[159], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[160], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[161], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[162], 858993461);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[163], 4294966206);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[164], 858994549);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[165], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[166], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[167], 4294967254);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[168], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[169], 822);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[170], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[171], 4294965935);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[172], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[173], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[174], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[175], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[176], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[177], 822);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[178], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[179], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[180], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[181], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[182], 1638);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[183], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[184], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[185], 4294965934);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[186], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[187], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[188], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[189], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[190], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[191], 2863311807);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[192], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[193], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[194], 2863312895);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[195], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[196], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[197], 2863311531);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[198], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[199], 2863311803);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[200], 2863312895);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[201], 2863311531);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[202], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[203], 4294967291);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[204], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[205], 2863311803);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[206], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[207], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[208], 2863312895);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[209], 2863311531);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[210], 1431655806);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[211], 4294967254);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[212], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[213], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[214], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[215], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[216], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[217], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[218], 1431655806);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[219], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[220], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[221], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[222], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[223], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[224], 1431655807);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[225], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[226], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[227], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[228], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[229], 4294967294);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[230], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[231], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[232], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[233], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[234], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[235], 820);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[236], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[237], 4294965935);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[238], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[239], 4);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[240], 44);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[241], 4);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[242], 1637);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[243], 822);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[244], 1364);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[245], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[246], 45);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[247], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[248], 1638);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[249], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[250], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[251], 4294965934);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[252], 1365);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[253], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[254], 46383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[255], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[256], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[257], 46903);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[258], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[259], 4294967211);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[260], 46423);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[261], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[262], 46383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[263], 46339);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[264], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[265], 46899);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[266], 46423);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[267], 46339);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[268], 46383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[269], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[270], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[271], 46899);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[272], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[273], 4294967211);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[274], 46423);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[275], 46339);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[276], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[277], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[278], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[279], 820);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[280], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[281], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[282], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[283], 4);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[284], 44);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[285], 0);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[286], 1637);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[287], 818);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[288], 1364);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[289], 2);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[290], 45);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[291], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[292], 1638);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[293], 819);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[294], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[295], 4294965930);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[296], 1365);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[297], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[298], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[299], 4294967285);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[300], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[301], 1717987189);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[302], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[303], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[304], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[305], 1717986917);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[306], 1717986925);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[307], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[308], 2863312894);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[309], 2863311534);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[310], 2863311534);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[311], 2863311530);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[312], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[313], 2863311802);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[314], 2863312894);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[315], 2863311530);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[316], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[317], 4294967291);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[318], 2863312622);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[319], 2863311803);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[320], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[321], 4294965930);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[322], 2863312895);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[323], 2863311531);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[324], 1431655807);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[325], 4294967253);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[326], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[327], 1431656309);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[328], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[329], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[330], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[331], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[332], 1431655805);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[333], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[334], 1431656309);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[335], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[336], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[337], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[338], 1431655805);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[339], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[340], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[341], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[342], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[343], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[344], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[345], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[346], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[347], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[348], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[349], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[350], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[351], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[352], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[353], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[354], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[355], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[356], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[357], 819);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[358], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[359], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[360], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[361], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[362], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[363], 819);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[364], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[365], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[366], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[367], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[368], 1717986917);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[369], 1717986917);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[370], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[371], 1717987189);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[372], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[373], 1717986925);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[374], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[375], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[376], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[377], 1717986917);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[378], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[379], 1717987189);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[380], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[381], 858993470);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[382], 4294967286);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[383], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[384], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[385], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[386], 4294966203);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[387], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[388], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[389], 858993470);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[390], 858993458);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[391], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[392], 858993458);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[393], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[394], 858993458);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[395], 858993471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[396], 4294967283);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[397], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[398], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[399], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[400], 4294966202);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[401], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[402], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[403], 1431655806);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[404], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[405], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[406], 1431656308);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[407], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[408], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[409], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[410], 1431655764);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[411], 1431655804);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[412], 1431655764);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[413], 1431656309);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[414], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[415], 1431655764);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[416], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[417], 1431655805);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[418], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[419], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[420], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[421], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[422], 4294967294);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[423], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[424], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[425], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[426], 4294967254);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[427], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[428], 822);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[429], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[430], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[431], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[432], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[433], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[434], 2);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[435], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[436], 818);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[437], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[438], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[439], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[440], 1638);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[441], 819);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[442], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[443], 4294965930);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[444], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[445], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[446], 46382);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[447], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[448], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[449], 46900);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[450], 46342);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[451], 4294967215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[452], 46422);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[453], 46340);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[454], 46380);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[455], 46340);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[456], 46949);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[457], 46902);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[458], 46420);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[459], 46342);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[460], 46381);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[461], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[462], 46950);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[463], 46903);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[464], 46341);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[465], 4294967214);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[466], 46421);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[467], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[468], 4294920958);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[469], 4294967292);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[470], 4294921983);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[471], 4294921212);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[472], 4294920958);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[473], 4294966015);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[474], 4294922238);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[475], 4294920956);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[476], 4294920956);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[477], 4294920956);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[478], 4294921981);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[479], 4294921214);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[480], 4294922236);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[481], 4294920958);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[482], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[483], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[484], 4294921982);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[485], 4294921215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[486], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[487], 4294966014);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[488], 4294922237);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[489], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[490], 1717986926);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[491], 4294967286);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[492], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[493], 1717987190);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[494], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[495], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[496], 1717987190);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[497], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[498], 1717986926);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[499], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[500], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[501], 1717987190);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[502], 1717987190);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[503], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[504], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[505], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[506], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[507], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[508], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[509], 4294967022);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[510], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[511], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[512], 858993471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[513], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[514], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[515], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[516], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[517], 4294966203);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[518], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[519], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[520], 858993471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[521], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[522], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[523], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[524], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[525], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[526], 858993471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[527], 4294967283);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[528], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[529], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[530], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[531], 4294966203);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[532], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[533], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[534], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[535], 4294967253);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[536], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[537], 821);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[538], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[539], 4294965935);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[540], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[541], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[542], 45);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[543], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[544], 1637);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[545], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[546], 1365);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[547], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[548], 45);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[549], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[550], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[551], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[552], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[553], 4294965935);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[554], 1365);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[555], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[556], 2863311534);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[557], 4294967294);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[558], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[559], 2863311806);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[560], 2863311534);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x7fffffff___immval_0x3ff) {
  ASSERT_EQ(getScalarWordData()[561], 2147483647);
}

