// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

// RISC-V architecture test includes
#include "riscv_arch_test/addi-01.h"

namespace
{
class addi_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::addi_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::addi_01::generateCode(false));
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

std::vector<uint8_t> addi_01::s_ScalarData;
}


// Generate google tests

TEST_F(addi_01, opcode__addi___op1_x6__dest_x5__op1val_0x20000000___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[5], 536868864);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x10__op1val_0x7fffffff___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[6], 2147485286);
}


TEST_F(addi_01, opcode__addi___op1_x14__dest_x0__op1val__0x40000001___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[7], 0);
}


TEST_F(addi_01, opcode__addi___op1_x2__dest_x11__op1val__0x20000001___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[8], 3758097749);
}


TEST_F(addi_01, opcode__addi___op1_x15__dest_x6__op1val__0x10000001___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[9], 4026531822);
}


TEST_F(addi_01, opcode__addi___op1_x13__dest_x1__op1val__0x8000001___immval__0x4) {
  ASSERT_EQ(getScalarWordData()[10], 4160749563);
}


TEST_F(addi_01, opcode__addi___op1_x5__dest_x3__op1val__0x4000001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[11], 4227858431);
}


TEST_F(addi_01, opcode__addi___op1_x0__dest_x15__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(addi_01, opcode__addi___op1_x12__dest_x7__op1val__0x1000001___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[13], 4278190083);
}


TEST_F(addi_01, opcode__addi___op1_x1__dest_x8__op1val__0x800001___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[14], 4286578751);
}


TEST_F(addi_01, opcode__addi___op1_x11__dest_x4__op1val__0x400001___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[15], 4290772986);
}


TEST_F(addi_01, opcode__addi___op1_x7__dest_x13__op1val__0x200001___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[16], 4292871508);
}


TEST_F(addi_01, opcode__addi___op1_x3__dest_x14__op1val__0x100001___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[17], 4293918702);
}


TEST_F(addi_01, opcode__addi___op1_x4__dest_x2__op1val__0x80001___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[18], 4294442997);
}


TEST_F(addi_01, opcode__addi___op1_x8__dest_x9__op1val__0x40001___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[19], 4294704638);
}


TEST_F(addi_01, opcode__addi___op1_x9__dest_x12__op1val__0x20001___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[20], 4294836226);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x10001___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[21], 4294901764);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x8001___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[22], 4294934533);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x4001___immval__0x8) {
  ASSERT_EQ(getScalarWordData()[23], 4294950903);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x2001___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[24], 4294959097);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x1001___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[25], 4294963166);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x801___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[26], 4294965759);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x401___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[27], 4294966265);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x201___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[28], 4294966750);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x101___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[29], 4294967041);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x81___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[30], 4294967212);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x41___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[31], 4294967275);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x21___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[32], 4294967265);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x11___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[33], 4294967285);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x9___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[34], 4294967282);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[35], 39);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[36], 4294967249);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x2___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[37], 4294967284);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[38], 48386);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x400___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[39], 4294967295);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x1001___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[40], 4294962942);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x8000000___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[41], 134217599);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[42], 4294967235);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x41___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[43], 4294967222);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[44], 3);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x5___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[45], 4294967289);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x80000000___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[46], 2147485014);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x40000000___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[47], 1073740800);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x10000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[48], 268435456);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4000000___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[49], 67108854);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2000000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[50], 33554478);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x1000000___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[51], 16778855);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x800000___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[52], 8388543);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x400000___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[53], 4194047);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x200000___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[54], 2098517);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x100000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[55], 1048576);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x80000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[56], 524293);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x40000___immval_0x3ff) {
  ASSERT_EQ(getScalarWordData()[57], 263167);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x20000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[58], 131072);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x10000___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[59], 66048);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x8000___immval__0x7) {
  ASSERT_EQ(getScalarWordData()[60], 32761);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4000___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[61], 17203);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2000___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[62], 9557);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x1000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[63], 2730);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x800___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[64], 3685);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x200___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[65], 447);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x100___immval_0x3ff) {
  ASSERT_EQ(getScalarWordData()[66], 1279);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x80___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[67], 1766);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x40___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[68], 883);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x20___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[69], 2079);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x10___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[70], 528);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x8___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[71], 8);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[72], 4294967295);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x1___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[73], 257);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x41___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[74], 959);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[75], 128);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[76], 32);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x8000___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[77], 32784);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x8___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[78], 0);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[79], 1431655766);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[80], 46387);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[81], 46297);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[82], 47980);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[83], 47161);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[84], 46347);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[85], 44976);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[86], 47707);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[87], 46345);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[88], 46385);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[89], 46341);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[90], 47978);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[91], 47159);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[92], 47705);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[93], 46343);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[94], 46386);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[95], 46296);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[96], 47979);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[97], 47160);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[98], 46346);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[99], 44975);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[100], 47706);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[101], 46344);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[102], 4294921003);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[103], 4294920913);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[104], 4294922596);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[105], 4294921777);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[106], 4294920963);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[107], 4294919592);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[108], 4294922323);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[109], 4294920961);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[110], 4294921001);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[111], 4294920957);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[112], 4294922594);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[113], 4294921775);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[114], 4294922321);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[115], 4294920959);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[116], 4294921002);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[117], 4294920912);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[118], 4294922595);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[119], 4294921776);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[120], 4294920962);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[121], 4294919591);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[122], 4294922322);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[123], 4294920960);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[124], 1717986965);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[125], 1717986875);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[126], 1717988558);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[127], 1717987739);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[128], 1717986925);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[129], 1717985554);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[130], 1717988285);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[131], 1717986923);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[132], 1717986963);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[133], 1717986919);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[134], 1717988556);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[135], 1717987737);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[136], 1717988283);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[137], 1717986921);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[138], 1717986964);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[139], 1717986874);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[140], 1717988557);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[141], 1717987738);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[142], 1717986924);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[143], 1717985553);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[144], 1717988284);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[145], 1717986922);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[146], 858993506);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[147], 858993416);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[148], 858995099);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[149], 858994280);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[150], 858993466);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[151], 858992095);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[152], 858994826);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[153], 858993464);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[154], 858993504);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[155], 858993460);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[156], 858995097);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[157], 858994278);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[158], 858994824);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[159], 858993462);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[160], 858993505);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[161], 858993415);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[162], 858995098);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[163], 858994279);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[164], 858993465);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[165], 858992094);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[166], 858994825);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[167], 858993463);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[168], 52);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[169], 4294967258);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[170], 1645);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[171], 826);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[172], 12);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[173], 4294965937);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[174], 1372);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[175], 10);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[176], 50);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[177], 6);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[178], 1643);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[179], 824);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[180], 1370);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[181], 8);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[182], 51);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[183], 4294967257);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[184], 1644);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[185], 825);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[186], 11);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[187], 4294965936);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[188], 1371);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[189], 9);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[190], 2863311577);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[191], 2863311487);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[192], 2863313170);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[193], 2863312351);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[194], 2863311537);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[195], 2863310166);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[196], 2863312897);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[197], 2863311535);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[198], 2863311575);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[199], 2863311531);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[200], 2863313168);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[201], 2863312349);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[202], 2863312895);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[203], 2863311533);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[204], 2863311576);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[205], 2863311486);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[206], 2863313169);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[207], 2863312350);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[208], 2863311536);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[209], 2863310165);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[210], 2863312896);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[211], 2863311534);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[212], 1431655812);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[213], 1431655722);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[214], 1431657405);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[215], 1431656586);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[216], 1431655772);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[217], 1431654401);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[218], 1431657132);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[219], 1431655770);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[220], 1431655810);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[221], 1431655766);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[222], 1431657403);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[223], 1431656584);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[224], 1431657130);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[225], 1431655768);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[226], 1431655811);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[227], 1431655721);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[228], 1431657404);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[229], 1431656585);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[230], 1431655771);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[231], 1431654400);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[232], 1431657131);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[233], 1431655769);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[234], 50);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[235], 4294967256);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[236], 1643);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[237], 824);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[238], 10);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[239], 4294965935);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[240], 1370);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[241], 8);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[242], 48);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[243], 4);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[244], 1641);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[245], 822);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[246], 1368);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[247], 6);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[248], 49);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[249], 4294967255);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[250], 1642);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[251], 823);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[252], 9);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[253], 4294965934);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[254], 1369);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[255], 7);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[256], 46385);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[257], 46295);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[258], 47978);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[259], 47159);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[260], 46345);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[261], 44974);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[262], 47705);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[263], 46343);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[264], 46383);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[265], 46339);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[266], 47976);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[267], 47157);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[268], 47703);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[269], 46341);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[270], 46384);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[271], 46294);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[272], 47977);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[273], 47158);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[274], 46344);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[275], 44973);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[276], 47704);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[277], 46342);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[278], 46);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[279], 4294967252);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[280], 1639);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[281], 820);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[282], 6);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[283], 4294965931);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[284], 1366);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[285], 4);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[286], 44);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[287], 0);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[288], 1637);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[289], 818);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[290], 1364);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[291], 2);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[292], 45);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[293], 4294967251);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[294], 1638);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[295], 819);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[296], 5);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[297], 4294965930);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[298], 1365);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[299], 3);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[300], 1717986963);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[301], 1717986873);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[302], 1717988556);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[303], 1717987737);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[304], 1717986923);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[305], 1717985552);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[306], 1717988283);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[307], 1717986921);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[308], 1717986961);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[309], 1717986917);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[310], 2863310165);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[311], 2863312896);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[312], 2863311534);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[313], 2863311574);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[314], 2863311530);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[315], 2863313167);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[316], 2863312348);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[317], 2863312894);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[318], 2863311532);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[319], 2863311575);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[320], 2863311485);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[321], 2863313168);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[322], 2863312349);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[323], 2863311535);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[324], 2863310164);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[325], 2863312895);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[326], 2863311533);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[327], 1431655811);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[328], 1431655721);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[329], 1431657404);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[330], 1431656585);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[331], 1431655771);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[332], 1431654400);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[333], 1431657131);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[334], 1431655769);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[335], 1431655809);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[336], 1431655765);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[337], 1431657402);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[338], 1431656583);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[339], 1431657129);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[340], 1431655767);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[341], 1431655810);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[342], 1431655720);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[343], 1431657403);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[344], 1431656584);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[345], 1431655770);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[346], 1431654399);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[347], 1431657130);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[348], 1431655768);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[349], 49);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[350], 4294967255);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[351], 1642);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[352], 823);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[353], 9);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[354], 4294965934);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[355], 1369);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[356], 7);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[357], 47);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[358], 3);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[359], 1640);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[360], 821);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[361], 1367);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[362], 5);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[363], 48);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[364], 4294967254);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[365], 1641);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[366], 822);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[367], 8);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[368], 4294965933);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[369], 1368);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[370], 6);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[371], 1717988554);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[372], 1717987735);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[373], 1717988281);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[374], 1717986919);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[375], 1717986962);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[376], 1717986872);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[377], 1717988555);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[378], 1717987736);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[379], 1717986922);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[380], 1717985551);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[381], 1717988282);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[382], 1717986920);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[383], 858993504);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[384], 858993414);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[385], 858995097);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[386], 858994278);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[387], 858993464);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[388], 858992093);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[389], 858994824);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[390], 858993462);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[391], 858993502);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[392], 858993458);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[393], 858995095);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[394], 858994276);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[395], 858994822);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[396], 858993460);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[397], 858993503);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[398], 858993413);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[399], 858995096);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[400], 858994277);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[401], 858993463);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[402], 858992092);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[403], 858994823);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[404], 858993461);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[405], 1431655810);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[406], 1431655720);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[407], 1431657403);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[408], 1431656584);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[409], 1431655770);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[410], 1431654399);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[411], 1431657130);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[412], 1431655768);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[413], 1431655808);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[414], 1431655764);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[415], 1431657401);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[416], 1431656582);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[417], 1431657128);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[418], 1431655766);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[419], 1431655809);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[420], 1431655719);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[421], 1431657402);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[422], 1431656583);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[423], 1431655769);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[424], 1431654398);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[425], 1431657129);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[426], 1431655767);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[427], 48);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[428], 4294967254);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[429], 1641);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[430], 822);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[431], 8);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[432], 4294965933);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[433], 1368);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[434], 6);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[435], 46);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[436], 2);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[437], 1639);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[438], 820);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[439], 1366);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[440], 4);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[441], 47);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[442], 4294967253);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[443], 1640);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[444], 821);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[445], 7);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[446], 4294965932);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[447], 1367);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[448], 5);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[449], 46386);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[450], 46296);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[451], 47979);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[452], 47160);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[453], 46346);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[454], 44975);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[455], 47706);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[456], 46344);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[457], 46384);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[458], 46340);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[459], 47977);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[460], 47158);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[461], 47704);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[462], 46342);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[463], 46385);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[464], 46295);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[465], 47978);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[466], 47159);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[467], 46345);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[468], 44974);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[469], 47705);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[470], 46343);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[471], 4294921002);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[472], 4294920912);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[473], 4294922595);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[474], 4294921776);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[475], 4294920962);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[476], 4294919591);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[477], 4294922322);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[478], 4294920960);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[479], 4294921000);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[480], 4294920956);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[481], 4294922593);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[482], 4294921774);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[483], 4294922320);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[484], 4294920958);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[485], 4294921001);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[486], 4294920911);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[487], 4294922594);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[488], 4294921775);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[489], 4294920961);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[490], 4294919590);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[491], 4294922321);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[492], 4294920959);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[493], 1717986964);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[494], 1717986874);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[495], 1717988557);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[496], 1717987738);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[497], 1717986924);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[498], 1717985553);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[499], 1717988284);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[500], 1717986922);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[501], 1717986962);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[502], 1717986918);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[503], 1717988555);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[504], 1717987736);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[505], 1717988282);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[506], 1717986920);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[507], 1717986963);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[508], 1717986873);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[509], 1717988556);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[510], 1717987737);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[511], 1717986923);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[512], 1717985552);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[513], 1717988283);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[514], 1717986921);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[515], 858993505);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[516], 858993415);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[517], 858995098);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[518], 858994279);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[519], 858993465);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[520], 858992094);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[521], 858994825);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[522], 858993463);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[523], 858993503);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[524], 858993459);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[525], 858995096);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[526], 858994277);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[527], 858994823);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[528], 858993461);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[529], 858993504);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[530], 858993414);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[531], 858995097);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[532], 858994278);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[533], 858993464);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[534], 858992093);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[535], 858994824);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[536], 858993462);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[537], 51);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[538], 4294967257);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[539], 1644);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[540], 825);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[541], 11);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[542], 4294965936);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[543], 1371);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[544], 9);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[545], 49);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[546], 5);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[547], 1642);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[548], 823);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[549], 1369);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[550], 7);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[551], 50);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[552], 4294967256);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[553], 1643);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[554], 824);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[555], 10);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[556], 4294965935);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[557], 1370);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[558], 8);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[559], 2863311576);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[560], 2863311486);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[561], 2863313169);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[562], 2863312350);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[563], 2863311536);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x40000001___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[564], 3221225535);
}


TEST_F(addi_01, opcode__addi___op1_x10__dest_x11__op1val__0x2000001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[565], 4261412863);
}

