// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

// RISC-V architecture test includes
#include "riscv_arch_test/xori-01.h"

namespace
{
class xori_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::xori_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::xori_01::generateCode(false));
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

std::vector<uint8_t> xori_01::s_ScalarData;
}


// Generate google tests

TEST_F(xori_01, opcode__xori___op1_x12__dest_x13__op1val__0x20001___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[5], 133119);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x10__op1val_0x7fffffff___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[6], 2147485013);
}


TEST_F(xori_01, opcode__xori___op1_x8__dest_x4__op1val__0x40000001___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[7], 1073742080);
}


TEST_F(xori_01, opcode__xori___op1_x6__dest_x7__op1val__0x20000001___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[8], 3758096381);
}


TEST_F(xori_01, opcode__xori___op1_x9__dest_x14__op1val__0x10000001___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[9], 4026531833);
}


TEST_F(xori_01, opcode__xori___op1_x7__dest_x3__op1val__0x8000001___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[10], 134217771);
}


TEST_F(xori_01, opcode__xori___op1_x11__dest_x15__op1val__0x4000001___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[11], 4227858415);
}


TEST_F(xori_01, opcode__xori___op1_x13__dest_x1__op1val__0x2000001___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[12], 4261412044);
}


TEST_F(xori_01, opcode__xori___op1_x15__dest_x11__op1val__0x1000001___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[13], 4278190074);
}


TEST_F(xori_01, opcode__xori___op1_x4__dest_x8__op1val__0x800001___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[14], 8388672);
}


TEST_F(xori_01, opcode__xori___op1_x14__dest_x9__op1val__0x400001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[15], 4290772984);
}


TEST_F(xori_01, opcode__xori___op1_x1__dest_x2__op1val__0x200001___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[16], 2097161);
}


TEST_F(xori_01, opcode__xori___op1_x3__dest_x5__op1val__0x100001___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[17], 4293917082);
}


TEST_F(xori_01, opcode__xori___op1_x5__dest_x12__op1val__0x80001___immval__0x4) {
  ASSERT_EQ(getScalarWordData()[18], 524291);
}


TEST_F(xori_01, opcode__xori___op1_x2__dest_x6__op1val__0x40001___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[19], 4294705105);
}


TEST_F(xori_01, opcode__xori___op1_x0__dest_x1__op1val_0x0___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[20], 512);
}


TEST_F(xori_01, opcode__xori___op1_x12__dest_x0__op1val__0x8001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[21], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x4001___immval__0x1) {
  ASSERT_EQ(getScalarWordData()[22], 16384);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x2001___immval__0x4) {
  ASSERT_EQ(getScalarWordData()[23], 8195);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x1001___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[24], 4294963153);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x801___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[25], 4294963883);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x401___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[26], 4294966265);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x201___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[27], 517);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x101___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[28], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x81___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[29], 1152);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x41___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[30], 4294967230);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x21___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[31], 4294967260);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x11___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[32], 18);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x9___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[33], 4294965922);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[34], 4294965935);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[35], 4294965931);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[36], 4294967250);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[37], 1717985689);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x1001___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[38], 4608);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x20001___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[39], 131200);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x100001___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[40], 1048608);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x2___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[41], 17);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x100001___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[42], 1048584);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x2001___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[43], 8196);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x40000___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[44], 4294705150);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x80000000___immval__0x8) {
  ASSERT_EQ(getScalarWordData()[45], 2147483640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x40000000___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[46], 1073741888);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x20000000___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[47], 536870913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x10000000___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[48], 268435462);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x8000000___immval_0x3ff) {
  ASSERT_EQ(getScalarWordData()[49], 134218751);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4000000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[50], 67108869);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2000000___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[51], 4261412819);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x1000000___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[52], 16777219);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x800000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[53], 8388654);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x400000___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[54], 4290772989);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x200000___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[55], 4292870100);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x100000___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[56], 1049940);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x80000___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[57], 4294442495);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x20000___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[58], 4294836222);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x10000___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[59], 65539);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x8000___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[60], 34815);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4000___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[61], 4294950847);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2000___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[62], 8199);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x1000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[63], 4100);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x800___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[64], 3072);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x400___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[65], 615);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x200___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[66], 516);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x100___immval__0x8) {
  ASSERT_EQ(getScalarWordData()[67], 4294967032);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x80___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[68], 4294967166);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x40___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[69], 64);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x20___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[70], 32);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x10___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[71], 21);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x8___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[72], 11);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[73], 4294967255);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[74], 1366);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x1___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[75], 4294967278);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3fffffff___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[76], 1073741567);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[77], 2863311402);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x1___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[78], 33);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x7___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[79], 4294967281);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[80], 46379);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[81], 4294920913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[82], 45922);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[83], 46641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[84], 46339);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[85], 4294922158);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[86], 45139);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[87], 46337);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[88], 46377);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[89], 46341);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[90], 45920);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[91], 46647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[92], 45137);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[93], 46343);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[94], 46376);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[95], 4294920918);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[96], 45923);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[97], 46646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[98], 46336);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[99], 4294922159);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[100], 45136);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[101], 46342);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[102], 4294920915);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[103], 46377);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[104], 4294921370);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[105], 4294920649);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[106], 4294920955);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[107], 45142);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[108], 4294922155);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[109], 4294920953);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[110], 4294920913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[111], 4294920957);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[112], 4294921368);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[113], 4294920655);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[114], 4294922153);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[115], 4294920959);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[116], 4294920912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[117], 46382);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[118], 4294921371);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[119], 4294920654);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[120], 4294920952);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[121], 45143);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[122], 4294922152);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[123], 4294920958);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[124], 1717986889);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[125], 2576980403);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[126], 1717985280);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[127], 1717986643);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[128], 1717986913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[129], 2576981196);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[130], 1717986097);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[131], 1717986915);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[132], 1717986891);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[133], 1717986919);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[134], 1717985282);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[135], 1717986645);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[136], 1717986099);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[137], 1717986917);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[138], 1717986890);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[139], 2576980404);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[140], 1717985281);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[141], 1717986644);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[142], 1717986914);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[143], 2576981197);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[144], 1717986098);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[145], 1717986916);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[146], 858993434);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[147], 3435973856);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[148], 858994003);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[149], 858992640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[150], 858993458);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[151], 3435973023);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[152], 858994274);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[153], 858993456);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[154], 858993432);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[155], 858993460);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[156], 858994001);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[157], 858992646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[158], 858994272);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[159], 858993462);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[160], 858993433);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[161], 3435973863);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[162], 858994002);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[163], 858992647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[164], 858993457);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[165], 3435973022);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[166], 858994273);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[167], 858993463);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[168], 40);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[169], 4294967250);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[170], 1633);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[171], 818);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[172], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[173], 4294965933);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[174], 1360);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[175], 2);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[176], 42);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[177], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[178], 1635);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[179], 820);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[180], 1362);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[181], 4);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[182], 43);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[183], 4294967253);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[184], 1632);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[185], 821);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[186], 3);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[187], 4294965932);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[188], 1363);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[189], 5);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[190], 2863311493);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[191], 1431655807);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[192], 2863312076);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[193], 2863311263);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[194], 2863311533);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[195], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[196], 2863312893);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[197], 2863311535);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[198], 2863311495);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[199], 2863311531);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[200], 2863312078);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[201], 2863311257);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[202], 2863312895);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[203], 2863311529);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[204], 2863311494);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[205], 1431655800);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[206], 2863312077);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[207], 2863311256);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[208], 2863311534);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[209], 1431654401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[210], 2863312894);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[211], 2863311528);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[212], 1431655800);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[213], 2863311490);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[214], 1431655217);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[215], 1431656034);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[216], 1431655760);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[217], 2863312893);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[218], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[219], 1431655762);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[220], 1431655802);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[221], 1431655766);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[222], 1431655219);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[223], 1431656036);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[224], 1431654402);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[225], 1431655764);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[226], 1431655803);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[227], 2863311493);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[228], 1431655216);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[229], 1431656037);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[230], 1431655763);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[231], 2863312892);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[232], 1431654403);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[233], 1431655765);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[234], 42);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[235], 4294967248);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[236], 1635);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[237], 816);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[238], 2);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[239], 4294965935);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[240], 1362);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[241], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[242], 40);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[243], 4);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[244], 1633);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[245], 822);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[246], 1360);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[247], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[248], 41);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[249], 1634);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[250], 823);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[251], 1);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[252], 4294965934);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[253], 1361);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[254], 7);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[255], 46381);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[256], 4294920919);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[257], 45924);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[258], 46647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[259], 46341);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[260], 4294922152);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[261], 45141);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[262], 46343);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[263], 46383);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[264], 46339);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[265], 45926);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[266], 46641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[267], 45143);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[268], 46337);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[269], 46382);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[270], 4294920912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[271], 45925);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[272], 46640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[273], 46342);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[274], 4294922153);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[275], 45142);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[276], 46336);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[277], 46);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[278], 4294967252);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[279], 1639);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[280], 820);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[281], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[282], 4294965931);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[283], 1366);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[284], 4);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[285], 44);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[286], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[287], 1637);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[288], 818);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[289], 1364);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[290], 2);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[291], 45);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[292], 4294967251);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[293], 1638);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[294], 819);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[295], 5);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[296], 4294965930);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[297], 1365);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[298], 3);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[299], 1717986891);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[300], 2576980401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[301], 1717985282);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[302], 1717986641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[303], 1717986915);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[304], 2576981198);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[305], 1717986099);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[306], 1717986913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[307], 1717986889);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[308], 1431654401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[309], 2863312892);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[310], 2863311534);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[311], 2863311494);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[312], 2863311530);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[313], 2863312079);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[314], 2863311256);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[315], 2863312894);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[316], 2863311528);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[317], 2863311495);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[318], 1431655801);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[319], 2863312076);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[320], 2863311257);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[321], 2863311535);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[322], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[323], 2863312895);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[324], 2863311529);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[325], 1431655803);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[326], 2863311489);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[327], 1431655218);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[328], 1431656033);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[329], 1431655763);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[330], 2863312894);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[331], 1431654403);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[332], 1431655761);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[333], 1431655801);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[334], 1431655765);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[335], 1431655216);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[336], 1431656039);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[337], 1431654401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[338], 1431655767);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[339], 1431655800);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[340], 2863311494);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[341], 1431655219);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[342], 1431656038);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[343], 1431655760);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[344], 2863312895);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[345], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[346], 1431655766);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[347], 45);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[348], 4294967255);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[349], 1636);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[350], 823);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[351], 5);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[352], 4294965928);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[353], 1365);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[354], 7);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[355], 47);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[356], 3);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[357], 1638);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[358], 817);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[359], 1367);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[360], 1);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[361], 46);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[362], 4294967248);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[363], 1637);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[364], 816);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[365], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[366], 4294965929);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[367], 1366);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[368], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[369], 1717986917);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[370], 1717985280);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[371], 1717986647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[372], 1717986097);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[373], 1717986919);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[374], 1717986888);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[375], 2576980406);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[376], 1717985283);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[377], 1717986646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[378], 1717986912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[379], 2576981199);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[380], 1717986096);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[381], 1717986918);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[382], 858993436);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[383], 3435973862);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[384], 858994005);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[385], 858992646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[386], 858993460);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[387], 3435973017);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[388], 858994276);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[389], 858993462);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[390], 858993438);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[391], 858993458);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[392], 858994007);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[393], 858992640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[394], 858994278);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[395], 858993456);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[396], 858993439);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[397], 3435973857);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[398], 858994004);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[399], 858992641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[400], 858993463);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[401], 3435973016);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[402], 858994279);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[403], 858993457);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[404], 1431655802);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[405], 2863311488);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[406], 1431655219);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[407], 1431656032);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[408], 1431655762);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[409], 2863312895);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[410], 1431654402);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[411], 1431655760);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[412], 1431655800);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[413], 1431655764);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[414], 1431655217);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[415], 1431656038);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[416], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[417], 1431655766);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[418], 1431655801);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[419], 2863311495);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[420], 1431655218);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[421], 1431656039);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[422], 1431655761);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[423], 2863312894);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[424], 1431654401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[425], 1431655767);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[426], 44);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[427], 4294967254);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[428], 1637);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[429], 822);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[430], 4);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[431], 4294965929);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[432], 1364);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[433], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[434], 46);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[435], 2);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[436], 1639);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[437], 816);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[438], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[439], 47);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[440], 4294967249);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[441], 1636);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[442], 817);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[443], 7);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[444], 4294965928);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[445], 1367);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[446], 1);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[447], 46378);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[448], 4294920912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[449], 45923);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[450], 46640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[451], 46338);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[452], 4294922159);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[453], 45138);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[454], 46336);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[455], 46376);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[456], 46340);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[457], 45921);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[458], 46646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[459], 45136);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[460], 46342);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[461], 46377);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[462], 4294920919);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[463], 45922);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[464], 46647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[465], 46337);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[466], 4294922158);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[467], 45137);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[468], 46343);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[469], 4294920914);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[470], 46376);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[471], 4294921371);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[472], 4294920648);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[473], 4294920954);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[474], 45143);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[475], 4294922154);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[476], 4294920952);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[477], 4294920912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[478], 4294920956);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[479], 4294921369);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[480], 4294920654);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[481], 4294922152);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[482], 4294920958);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[483], 4294920913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[484], 46383);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[485], 4294921370);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[486], 4294920655);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[487], 4294920953);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[488], 45142);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[489], 4294922153);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[490], 4294920959);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[491], 1717986888);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[492], 2576980402);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[493], 1717985281);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[494], 1717986642);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[495], 1717986912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[496], 2576981197);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[497], 1717986096);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[498], 1717986914);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[499], 1717986890);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[500], 1717986918);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[501], 1717985283);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[502], 1717986644);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[503], 1717986098);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[504], 1717986916);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[505], 1717986891);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[506], 2576980405);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[507], 1717985280);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[508], 1717986645);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[509], 1717986915);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[510], 2576981196);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[511], 1717986099);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[512], 1717986917);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[513], 858993437);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[514], 3435973863);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[515], 858994004);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[516], 858992647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[517], 858993461);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[518], 3435973016);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[519], 858994277);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[520], 858993463);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[521], 858993439);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[522], 858993459);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[523], 858994006);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[524], 858992641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[525], 858994279);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[526], 858993457);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[527], 858993438);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[528], 3435973856);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[529], 858994005);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[530], 858992640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[531], 858993462);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[532], 3435973017);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[533], 858994278);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[534], 858993456);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[535], 43);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[536], 4294967249);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[537], 1634);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[538], 817);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[539], 3);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[540], 4294965934);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[541], 1363);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[542], 1);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[543], 41);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[544], 5);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[545], 1632);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[546], 823);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[547], 1361);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[548], 7);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[549], 40);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[550], 4294967254);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[551], 1635);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[552], 822);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[553], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[554], 4294965935);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[555], 1360);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[556], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[557], 2863311492);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[558], 1431655806);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[559], 2863312077);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[560], 2863311262);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[561], 2863311532);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x10001___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[562], 4294901247);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x8001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[563], 4294934527);
}

