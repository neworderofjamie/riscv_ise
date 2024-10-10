// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

// RISC-V architecture test includes
#include "riscv_arch_test/sltiu-01.h"

namespace
{
class sltiu_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::sltiu_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::sltiu_01::generateCode(false));
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

std::vector<uint8_t> sltiu_01::s_ScalarData;
}


// Generate google tests

TEST_F(sltiu_01, opcode__sltiu___op1_x7__dest_x2__op1val_0x9___immval_0xfff) {
  ASSERT_EQ(getScalarWordData()[5], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x3__dest_x3__op1val_0x7fffffff___immval_0xeff) {
  ASSERT_EQ(getScalarWordData()[6], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x4__op1val_0xbfffffff___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[7], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x4__dest_x6__op1val_0xdfffffff___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[8], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x6__dest_x13__op1val_0xefffffff___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x1__dest_x7__op1val_0xf7ffffff___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x2__dest_x8__op1val_0xfbffffff___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[11], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x0__dest_x1__op1val_0x0___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[12], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x5__dest_x12__op1val_0xfeffffff___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x11__dest_x9__op1val_0xff7fffff___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[14], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x12__dest_x10__op1val_0xffbfffff___immval_0xffd) {
  ASSERT_EQ(getScalarWordData()[15], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x9__dest_x14__op1val_0xffdfffff___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[16], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x14__dest_x15__op1val_0xffefffff___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x15__dest_x0__op1val_0xfff7ffff___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[18], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x13__dest_x11__op1val_0xfffbffff___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[19], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x8__dest_x5__op1val_0xfffdffff___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffeffff___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[21], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff7fff___immval_0xfff) {
  ASSERT_EQ(getScalarWordData()[22], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffbfff___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[23], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffdfff___immval_0xff7) {
  ASSERT_EQ(getScalarWordData()[24], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffefff___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[25], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffff7ff___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[26], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffbff___immval_0xbff) {
  ASSERT_EQ(getScalarWordData()[27], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffdff___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[28], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffeff___immval_0xdff) {
  ASSERT_EQ(getScalarWordData()[29], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffff7f___immval_0xffe) {
  ASSERT_EQ(getScalarWordData()[30], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffffbf___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[31], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffffdf___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[32], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffffef___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[33], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffff7___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[34], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffffb___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[35], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffffd___immval_0xdff) {
  ASSERT_EQ(getScalarWordData()[36], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffffe___immval_0xfef) {
  ASSERT_EQ(getScalarWordData()[37], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffffe___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[38], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffffdf___immval_0xf7f) {
  ASSERT_EQ(getScalarWordData()[39], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0xfbf) {
  ASSERT_EQ(getScalarWordData()[40], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffbffff___immval_0xfdf) {
  ASSERT_EQ(getScalarWordData()[41], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x80000___immval_0xffb) {
  ASSERT_EQ(getScalarWordData()[42], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x80000000___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[43], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x40000000___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[44], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x20000000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[45], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000000___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[46], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x8000000___immval_0xeff) {
  ASSERT_EQ(getScalarWordData()[47], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4000000___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[48], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2000000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[49], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1000000___immval_0x800) {
  ASSERT_EQ(getScalarWordData()[50], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x800000___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[51], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x400000___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[52], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x200000___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[53], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x100000___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[54], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x40000___immval_0xff7) {
  ASSERT_EQ(getScalarWordData()[55], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x20000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[56], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[57], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x8000___immval_0xffb) {
  ASSERT_EQ(getScalarWordData()[58], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[59], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2000___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[60], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1000___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[61], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x800___immval_0xfbf) {
  ASSERT_EQ(getScalarWordData()[62], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x400___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[63], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x200___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[64], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x100___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[65], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x80___immval_0x800) {
  ASSERT_EQ(getScalarWordData()[66], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x40___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[67], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x20___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[68], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10___immval_0xfef) {
  ASSERT_EQ(getScalarWordData()[69], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x8___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[70], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[71], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0xbff) {
  ASSERT_EQ(getScalarWordData()[72], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[73], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[74], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xdfffffff___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[75], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x12___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[76], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[77], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[78], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[79], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[80], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[81], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[82], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[83], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[84], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[85], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[86], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[87], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[88], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[89], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[90], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[91], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[92], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[93], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[94], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[95], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[96], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[97], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[98], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[99], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[100], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[101], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[102], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[103], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[104], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[105], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[106], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[107], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[108], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[109], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[110], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[111], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[112], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[113], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[114], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[115], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[116], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[117], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[118], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[119], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[120], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[121], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[122], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[123], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[124], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[125], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[126], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[127], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[128], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[129], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[130], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[131], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[132], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[133], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[134], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[135], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[136], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[137], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[138], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[139], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[140], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[141], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[142], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[143], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[144], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[145], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[146], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[147], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[148], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[149], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[150], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[151], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[152], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[153], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[154], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[155], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[156], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[157], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[158], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[159], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[160], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[161], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[162], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[163], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[164], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[165], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[166], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[167], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[168], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[169], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[170], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[171], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[172], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[173], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[174], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[175], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[176], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[177], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[178], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[179], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[180], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[181], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[182], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[183], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[184], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[185], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[186], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[187], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[188], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[189], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[190], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[191], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[192], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[193], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[194], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[195], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[196], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[197], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[198], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[199], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[200], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[201], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[202], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[203], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[204], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[205], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[206], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[207], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[208], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[209], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[210], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[211], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[212], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[213], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[214], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[215], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[216], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[217], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[218], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[219], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[220], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[221], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[222], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[223], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[224], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[225], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[226], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[227], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[228], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[229], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[230], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[231], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[232], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[233], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[234], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[235], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[236], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[237], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[238], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[239], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[240], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[241], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[242], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[243], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[244], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[245], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[246], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[247], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[248], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[249], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[250], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[251], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[252], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[253], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[254], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[255], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[256], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[257], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[258], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[259], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[260], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[261], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[262], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[263], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[264], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[265], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[266], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[267], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[268], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[269], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[270], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[271], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[272], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[273], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[274], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[275], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[276], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[277], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[278], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[279], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[280], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[281], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[282], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[283], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[284], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[285], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[286], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[287], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[288], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[289], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[290], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[291], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[292], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[293], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[294], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[295], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[296], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[297], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[298], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[299], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[300], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[301], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[302], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[303], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[304], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[305], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[306], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[307], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[308], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[309], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[310], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[311], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[312], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[313], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[314], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[315], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[316], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[317], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[318], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[319], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[320], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[321], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[322], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[323], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[324], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[325], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[326], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[327], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[328], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[329], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[330], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[331], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[332], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[333], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[334], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[335], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[336], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[337], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[338], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[339], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[340], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[341], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[342], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[343], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[344], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[345], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[346], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[347], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[348], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[349], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[350], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[351], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[352], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[353], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[354], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[355], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[356], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[357], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[358], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[359], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[360], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[361], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[362], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[363], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[364], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[365], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[366], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[367], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[368], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[369], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[370], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[371], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[372], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[373], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[374], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[375], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[376], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[377], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[378], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[379], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[380], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[381], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[382], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[383], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[384], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[385], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[386], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[387], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[388], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[389], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[390], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[391], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[392], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[393], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[394], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[395], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[396], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[397], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[398], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[399], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[400], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[401], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[402], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[403], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[404], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[405], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[406], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[407], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[408], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[409], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[410], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[411], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[412], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[413], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[414], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[415], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[416], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[417], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[418], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[419], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[420], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[421], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[422], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[423], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[424], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[425], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[426], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[427], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[428], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[429], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[430], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[431], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[432], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[433], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[434], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[435], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[436], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[437], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[438], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[439], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[440], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[441], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[442], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[443], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[444], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[445], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[446], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[447], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[448], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[449], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[450], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[451], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[452], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[453], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[454], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[455], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[456], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[457], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[458], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[459], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[460], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[461], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[462], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[463], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[464], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[465], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[466], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[467], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[468], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[469], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[470], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[471], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[472], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[473], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[474], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[475], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[476], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[477], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[478], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[479], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[480], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[481], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[482], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[483], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[484], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[485], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[486], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[487], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[488], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[489], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[490], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[491], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[492], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[493], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[494], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[495], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[496], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[497], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[498], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[499], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[500], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[501], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[502], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[503], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[504], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[505], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[506], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[507], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[508], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[509], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[510], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[511], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[512], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[513], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[514], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[515], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[516], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[517], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[518], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[519], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[520], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[521], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[522], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[523], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[524], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[525], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[526], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[527], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[528], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[529], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[530], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[531], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[532], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[533], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[534], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[535], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[536], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[537], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[538], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[539], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[540], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[541], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[542], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[543], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[544], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[545], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[546], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[547], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[548], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[549], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[550], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[551], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[552], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[553], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[554], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[555], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[556], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[557], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[558], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[559], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[560], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[561], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[562], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[563], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[564], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[565], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[566], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[567], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[568], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[569], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[570], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[571], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[572], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[573], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[574], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[575], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[576], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[577], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[578], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[579], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[580], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[581], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[582], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[583], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[584], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[585], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[586], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[587], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[588], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[589], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[590], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[591], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[592], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[593], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[594], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[595], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[596], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[597], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[598], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[599], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[600], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[601], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[602], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[603], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[604], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[605], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[606], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[607], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[608], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[609], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[610], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[611], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[612], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[613], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[614], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[615], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[616], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[617], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[618], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[619], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[620], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[621], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[622], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[623], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[624], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[625], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[626], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[627], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[628], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[629], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[630], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[631], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[632], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[633], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[634], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[635], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[636], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[637], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[638], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[639], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[640], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[641], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[642], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[643], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[644], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[645], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[646], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[647], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[648], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[649], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[650], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[651], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffffff___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[652], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[653], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[654], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[655], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[656], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[657], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[658], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[659], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[660], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[661], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[662], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[663], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[664], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[665], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[666], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[667], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[668], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[669], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[670], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[671], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[672], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[673], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[674], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[675], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[676], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[677], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[678], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[679], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[680], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[681], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[682], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[683], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[684], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[685], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[686], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[687], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[688], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[689], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[690], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[691], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[692], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[693], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[694], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[695], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[696], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[697], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[698], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[699], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfdffffff___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[700], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfff7ffff___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[701], 0);
}

