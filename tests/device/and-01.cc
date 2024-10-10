// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

// RISC-V architecture test includes
#include "riscv_arch_test/and-01.h"

namespace
{
class and_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::and_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::and_01::generateCode(false));
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

std::vector<uint8_t> and_01::s_ScalarData;
}


// Generate google tests

TEST_F(and_01, opcode__and___op1_x11__op2_x3__dest_x10__op1val_0x10___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(and_01, opcode__and___op1_x13__op2_x11__dest_x13__op1val_0x3___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[6], 3);
}


TEST_F(and_01, opcode__and___op1_x12__op2_x6__dest_x6__op1val_0x100___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[7], 256);
}


TEST_F(and_01, opcode__and___op1_x8__op2_x8__dest_x5__op1val_0x40000___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[8], 262144);
}


TEST_F(and_01, opcode__and___op1_x4__op2_x4__dest_x4__op1val_0x100000___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[9], 1048576);
}


TEST_F(and_01, opcode__and___op1_x15__op2_x2__dest_x12__op1val__0x4000001___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[10], 4093640703);
}


TEST_F(and_01, opcode__and___op1_x6__op2_x15__dest_x8__op1val__0x55555556___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[11], 2863311530);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x5__dest_x9__op1val_0x400000___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[12], 4194304);
}


TEST_F(and_01, opcode__and___op1_x2__op2_x0__dest_x1__op1val_0x40000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(and_01, opcode__and___op1_x5__op2_x1__dest_x15__op1val__0x41___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[14], 4286578623);
}


TEST_F(and_01, opcode__and___op1_x7__op2_x10__dest_x2__op1val_0x1000000___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[15], 16777216);
}


TEST_F(and_01, opcode__and___op1_x1__op2_x9__dest_x14__op1val__0x4001___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[16], 4292853759);
}


TEST_F(and_01, opcode__and___op1_x14__op2_x13__dest_x7__op1val__0x401___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[17], 4293917695);
}


TEST_F(and_01, opcode__and___op1_x3__op2_x14__dest_x0__op1val__0x3___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[18], 0);
}


TEST_F(and_01, opcode__and___op1_x9__op2_x7__dest_x11__op1val_0x2000000___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[19], 33554432);
}


TEST_F(and_01, opcode__and___op1_x0__op2_x12__dest_x3__op1val_0x0___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[21], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[22], 1431655765);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[23], 1073741824);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[24], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[25], 256);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[26], 2863309482);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[27], 4096);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[28], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x7___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[29], 7);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[30], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[31], 46340);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[32], 128);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[33], 46341);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[34], 1048576);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[35], 16384);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[36], 858993456);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[37], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[38], 2139095039);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[39], 644245095);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[40], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val__0x8) {
  ASSERT_EQ(getScalarWordData()[41], 4026531832);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[42], 2097152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[43], 4261412607);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[44], 4269801471);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[45], 4286578431);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[46], 4288675839);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[47], 1715889766);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[48], 4293872381);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[49], 46340);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[50], 4294672383);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[51], 1431655766);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[52], 524288);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[53], 4294918143);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[54], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[55], 32);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[56], 4294918908);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[57], 4294442495);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[58], 4160749311);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[59], 1431655765);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[60], 4294966751);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[61], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[62], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[63], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[64], 4294920956);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[65], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[66], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[67], 536870912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[68], 268435456);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[69], 134217728);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x1___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[70], 67108864);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[71], 33554432);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[72], 16777216);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[73], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[74], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[75], 1048576);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x9___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[76], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[77], 131072);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[78], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[79], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[80], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xa___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[81], 8192);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[82], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[83], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[84], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[85], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[86], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[87], 128);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[88], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x7___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[89], 16);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[90], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[91], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[92], 536870912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[93], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[94], 67108864);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val_0x3fffffff) {
  ASSERT_EQ(getScalarWordData()[95], 8388608);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[96], 2097152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[97], 524288);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x4) {
  ASSERT_EQ(getScalarWordData()[98], 131072);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[99], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[100], 8192);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[101], 1024);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[102], 512);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[103], 64);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[104], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[105], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[106], 46341);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[107], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[108], 9221);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[109], 12548);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[110], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[111], 40961);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[112], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[113], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[114], 46337);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[115], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[116], 9221);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[117], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[118], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[119], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[120], 46340);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[121], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[122], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[123], 12545);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[124], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[125], 40960);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[126], 5381);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[127], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[128], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[129], 4294920957);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[130], 1717977701);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[131], 858980916);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[132], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[133], 2863270569);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[134], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[135], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[136], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[137], 1717977701);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[138], 858980912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[139], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[140], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[141], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[142], 4294920956);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[143], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[144], 858980913);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[145], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[146], 2863270568);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[147], 1431650389);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[148], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[149], 9221);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[150], 1717977701);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[151], 1717986919);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[152], 572662308);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[153], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[154], 572662307);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[155], 1145324614);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[156], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[157], 9219);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[158], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[159], 1717986917);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[160], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[161], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[162], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[163], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[164], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[165], 1717986918);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[166], 572662307);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[167], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[168], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[169], 1145324613);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[170], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[171], 12548);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[172], 858980916);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[173], 572662308);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[174], 858993460);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[175], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[176], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[177], 286331156);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[178], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[179], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[180], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[181], 572662308);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[182], 858993456);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[183], 286331156);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[184], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[185], 12548);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[186], 858980916);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[187], 572662308);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[188], 858993456);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[189], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[190], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[191], 286331156);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[192], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[193], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[194], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[195], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[196], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[197], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[198], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[199], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[200], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[201], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[202], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[203], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[204], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[205], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[206], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[207], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[208], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[209], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[210], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[211], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[212], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[213], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[214], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[215], 40961);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[216], 2863270569);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[217], 572662307);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[218], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[219], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[220], 2863311531);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[221], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[222], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[223], 40963);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[224], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[225], 572662305);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[226], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[227], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[228], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[229], 40960);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[230], 2863270568);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[231], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[232], 572662307);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[233], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[234], 2863311530);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[235], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[236], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[237], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[238], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[239], 1145324614);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[240], 286331156);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[241], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[242], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[243], 1431655766);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[244], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[245], 5378);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[246], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[247], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[248], 286331154);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[249], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[250], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[251], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[252], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[253], 1145324614);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[254], 286331154);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[255], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[256], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[257], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[258], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[259], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[260], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[261], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[262], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[263], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[264], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[265], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[266], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[267], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[268], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[269], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[270], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[271], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[272], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[273], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[274], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[275], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[276], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[277], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[278], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[279], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[280], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[281], 46337);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[282], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[283], 9219);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[284], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[285], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[286], 40963);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[287], 5378);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[288], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[289], 46339);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[290], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[291], 9217);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[292], 12546);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[293], 5376);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[294], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[295], 46336);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[296], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[297], 9218);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[298], 12547);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[299], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[300], 40962);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[301], 5377);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[302], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[303], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[304], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[305], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[306], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[307], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[308], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[309], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[310], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[311], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[312], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[313], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[314], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[315], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[316], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[317], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[318], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[319], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[320], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[321], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[322], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[323], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[324], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[325], 9221);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[326], 1717977701);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[327], 1717986917);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[328], 572662308);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[329], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[330], 572662305);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[331], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[332], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[333], 9217);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[334], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[335], 572662305);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[336], 858993458);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[337], 286331152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[338], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[339], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[340], 858980912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[341], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[342], 858993459);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[343], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[344], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[345], 286331153);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[346], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[347], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[348], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[349], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[350], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[351], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[352], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[353], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[354], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[355], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[356], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[357], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[358], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[359], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[360], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[361], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[362], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[363], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[364], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[365], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[366], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[367], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[368], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[369], 40960);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[370], 2863270568);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[371], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[372], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[373], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[374], 2863311530);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[375], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[376], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[377], 40962);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[378], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[379], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[380], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[381], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[382], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[383], 40960);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[384], 2863270568);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[385], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[386], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[387], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[388], 2863311530);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[389], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[390], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[391], 5381);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[392], 1431650389);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[393], 1145324613);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[394], 286331156);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[395], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[396], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[397], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[398], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[399], 5377);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[400], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[401], 1145324613);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[402], 286331152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[403], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[404], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[405], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[406], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[407], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[408], 286331153);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[409], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[410], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[411], 1431655765);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[412], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[413], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[414], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[415], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[416], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[417], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[418], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[419], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[420], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[421], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[422], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[423], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[424], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[425], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[426], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[427], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[428], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[429], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[430], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[431], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[432], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[433], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[434], 3);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[435], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[436], 1717986917);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[437], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[438], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[439], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[440], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[441], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[442], 1717986916);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[443], 572662305);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[444], 5);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[445], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[446], 1145324613);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[447], 1);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[448], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[449], 858980912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[450], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[451], 858993456);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[452], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[453], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[454], 286331154);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[455], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[456], 12546);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[457], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[458], 572662304);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[459], 858993458);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[460], 286331152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[461], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[462], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[463], 858980912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[464], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[465], 858993458);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[466], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[467], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[468], 286331152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[469], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[470], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[471], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[472], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[473], 286331156);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[474], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[475], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[476], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[477], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[478], 5376);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[479], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[480], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[481], 286331152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[482], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[483], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[484], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[485], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[486], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[487], 286331152);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[488], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[489], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[490], 1431655764);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[491], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[492], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[493], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[494], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[495], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[496], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[497], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[498], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[499], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[500], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[501], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[502], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[503], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[504], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[505], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[506], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[507], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[508], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[509], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[510], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[511], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[512], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[513], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[514], 46340);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[515], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[516], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[517], 12548);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[518], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[519], 40960);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[520], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[521], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[522], 46336);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[523], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[524], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[525], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[526], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[527], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[528], 46340);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[529], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[530], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[531], 12544);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[532], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[533], 40960);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[534], 5380);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[535], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[536], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[537], 4294920956);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[538], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[539], 858980916);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[540], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[541], 2863270568);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[542], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[543], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[544], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[545], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[546], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[547], 858980912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[548], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[549], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[550], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[551], 4294920956);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[552], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[553], 858980912);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[554], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[555], 2863270568);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[556], 1431650388);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[557], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[558], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[559], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[560], 1717986918);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[561], 572662308);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[562], 6);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[563], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[564], 1145324614);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[565], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[566], 9218);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[567], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[568], 1717986916);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[569], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[570], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[571], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[572], 9220);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[573], 1717977700);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[574], 1717986918);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[575], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[576], 4);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[577], 572662306);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[578], 1145324612);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[579], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[580], 12545);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[581], 858980913);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[582], 572662307);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[583], 858993456);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[584], 2);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[585], 572662307);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[586], 286331154);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[587], 0);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[588], 12547);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[589], 262144);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[590], 1048576);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[591], 4294443005);
}


TEST_F(and_01, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xa___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[592], 4294836214);
}

