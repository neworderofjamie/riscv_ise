// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

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

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::sltiu_01::generateCode(true), scalarInitData);
        
        if(!riscV.run()) {
            throw std::runtime_error("Error running test code");
        }
        
        // Copy output data
        s_ScalarData = riscV.getScalarDataMemory().getData();
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

TEST_F(sltiu_01, opcode__sltiu___op1_x23__dest_x28__op1val_0x400___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[5], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x2__dest_x2__op1val_0x800___immval_0xfff) {
  ASSERT_EQ(getScalarWordData()[6], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x3__dest_x25__op1val_0x4___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[7], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x19__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[8], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x14__dest_x15__op1val_0xffffffff___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[9], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x13__dest_x4__op1val_0x1___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[10], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x26__dest_x3__op1val_0xd___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[11], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x20__dest_x29__op1val_0xaaaaaaaa___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[12], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x27__dest_x16__op1val_0x7fffffff___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[13], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x17__dest_x20__op1val_0xfeffffff___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[14], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x31__dest_x8__op1val_0x800___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[15], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x24__dest_x23__op1val_0xc___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[16], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x25__dest_x26__op1val_0x55555555___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[17], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x22__dest_x6__op1val_0x80000___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[18], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x12__dest_x5__op1val_0xfffffff7___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[19], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x9__dest_x1__op1val_0x80000000___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[20], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x28__dest_x10__op1val_0xfffbffff___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[21], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x21__dest_x31__op1val_0x0___immval_0x800) {
  ASSERT_EQ(getScalarWordData()[22], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x0__dest_x21__op1val_0x0___immval_0xffe) {
  ASSERT_EQ(getScalarWordData()[23], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x11__dest_x14__op1val_0x12___immval_0xffd) {
  ASSERT_EQ(getScalarWordData()[24], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x30__dest_x13__op1val_0x6___immval_0xffb) {
  ASSERT_EQ(getScalarWordData()[25], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x29__dest_x17__op1val_0x40000000___immval_0xff7) {
  ASSERT_EQ(getScalarWordData()[26], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x8__dest_x24__op1val_0xffffffbf___immval_0xfef) {
  ASSERT_EQ(getScalarWordData()[27], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x1__dest_x27__op1val_0x5___immval_0xfdf) {
  ASSERT_EQ(getScalarWordData()[28], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x7__dest_x30__op1val_0x3___immval_0xfbf) {
  ASSERT_EQ(getScalarWordData()[29], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x5__dest_x22__op1val_0x40000___immval_0xf7f) {
  ASSERT_EQ(getScalarWordData()[30], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x15__dest_x0__op1val_0xe___immval_0xeff) {
  ASSERT_EQ(getScalarWordData()[31], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x6__dest_x7__op1val_0x3___immval_0xdff) {
  ASSERT_EQ(getScalarWordData()[32], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x18__dest_x12__op1val_0x7fffffff___immval_0xbff) {
  ASSERT_EQ(getScalarWordData()[33], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x16__dest_x18__op1val_0x1000000___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[34], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x4__dest_x9__op1val_0xfbffffff___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[35], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x19__op1val_0xfffff7ff___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[36], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[37], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x8___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[38], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x20___immval_0x800) {
  ASSERT_EQ(getScalarWordData()[39], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x40___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[40], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x80___immval_0xff7) {
  ASSERT_EQ(getScalarWordData()[41], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x100___immval_0xfdf) {
  ASSERT_EQ(getScalarWordData()[42], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x200___immval_0xff7) {
  ASSERT_EQ(getScalarWordData()[43], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[44], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[45], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4000___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[46], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x8000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[47], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[48], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x20000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[49], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x100000___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[50], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x200000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[51], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x400000___immval_0xfdf) {
  ASSERT_EQ(getScalarWordData()[52], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x800000___immval_0x800) {
  ASSERT_EQ(getScalarWordData()[53], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2000000___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[54], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4000000___immval_0x800) {
  ASSERT_EQ(getScalarWordData()[55], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x8000000___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[56], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000000___immval_0xfbf) {
  ASSERT_EQ(getScalarWordData()[57], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x20000000___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[58], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffffe___immval_0xfef) {
  ASSERT_EQ(getScalarWordData()[59], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffffd___immval_0xfff) {
  ASSERT_EQ(getScalarWordData()[60], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffffb___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[61], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffffef___immval_0xfff) {
  ASSERT_EQ(getScalarWordData()[62], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffffdf___immval_0xeff) {
  ASSERT_EQ(getScalarWordData()[63], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffff7f___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[64], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffeff___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[65], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffdff___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[66], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffbff___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[67], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffefff___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[68], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffdfff___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[69], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffbfff___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[70], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff7fff___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[71], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffeffff___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[72], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffdffff___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[73], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfff7ffff___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[74], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffefffff___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[75], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffdfffff___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[76], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffbfffff___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[77], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xff7fffff___immval_0xffe) {
  ASSERT_EQ(getScalarWordData()[78], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfdffffff___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[79], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xf7ffffff___immval_0x800) {
  ASSERT_EQ(getScalarWordData()[80], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xefffffff___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[81], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xdfffffff___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[82], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xbfffffff___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[83], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[84], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[85], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[86], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[87], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[88], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[89], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[90], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[91], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[92], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[93], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[94], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[95], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[96], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[97], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[98], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[99], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[100], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[101], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[102], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[103], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[104], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[105], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[106], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[107], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[108], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[109], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[110], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[111], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[112], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[113], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[114], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[115], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[116], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[117], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[118], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[119], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[120], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[121], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[122], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[123], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[124], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[125], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[126], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[127], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[128], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[129], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[130], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[131], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[132], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[133], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[134], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[135], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[136], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[137], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[138], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[139], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[140], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[141], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[142], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[143], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[144], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[145], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[146], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[147], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[148], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[149], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[150], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[151], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[152], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[153], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[154], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[155], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[156], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[157], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[158], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[159], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[160], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[161], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[162], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[163], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[164], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[165], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[166], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[167], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[168], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[169], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[170], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[171], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[172], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[173], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[174], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[175], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[176], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[177], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[178], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[179], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[180], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[181], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[182], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[183], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[184], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[185], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[186], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[187], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[188], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[189], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[190], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[191], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[192], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[193], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[194], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[195], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[196], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[197], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[198], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[199], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[200], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[201], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[202], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[203], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[204], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[205], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[206], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[207], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[208], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[209], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[210], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[211], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[212], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[213], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[214], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[215], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[216], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[217], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[218], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[219], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[220], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[221], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[222], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[223], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[224], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[225], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[226], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[227], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[228], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[229], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[230], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[231], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[232], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[233], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[234], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[235], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[236], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[237], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[238], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[239], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[240], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[241], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[242], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[243], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[244], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[245], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[246], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[247], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[248], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[249], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[250], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[251], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[252], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[253], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[254], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[255], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[256], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[257], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[258], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[259], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[260], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[261], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[262], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[263], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[264], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[265], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[266], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[267], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[268], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[269], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[270], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[271], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[272], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[273], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[274], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[275], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[276], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[277], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[278], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[279], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[280], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[281], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[282], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[283], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[284], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[285], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[286], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[287], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[288], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[289], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[290], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[291], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[292], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[293], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[294], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[295], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[296], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[297], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[298], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[299], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[300], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[301], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[302], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[303], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[304], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[305], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[306], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[307], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[308], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[309], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[310], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[311], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[312], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[313], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[314], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[315], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[316], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[317], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[318], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[319], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[320], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[321], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[322], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[323], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[324], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[325], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[326], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[327], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[328], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[329], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[330], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[331], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[332], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[333], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[334], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[335], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[336], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[337], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[338], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[339], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[340], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[341], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[342], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[343], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[344], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[345], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[346], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[347], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[348], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[349], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[350], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[351], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[352], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[353], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[354], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[355], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[356], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[357], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[358], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[359], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[360], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[361], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[362], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[363], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[364], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[365], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[366], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[367], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[368], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[369], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[370], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[371], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[372], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[373], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[374], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[375], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[376], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[377], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[378], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[379], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[380], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[381], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[382], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[383], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[384], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[385], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[386], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[387], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[388], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[389], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[390], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[391], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[392], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[393], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[394], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[395], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[396], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[397], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[398], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[399], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[400], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[401], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[402], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[403], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[404], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[405], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[406], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[407], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[408], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[409], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[410], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[411], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[412], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[413], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[414], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[415], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[416], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[417], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[418], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[419], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[420], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[421], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[422], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[423], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[424], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[425], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[426], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[427], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[428], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[429], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[430], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[431], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[432], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[433], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[434], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[435], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[436], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[437], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[438], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[439], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[440], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[441], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[442], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[443], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[444], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[445], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[446], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[447], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[448], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[449], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[450], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[451], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[452], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[453], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[454], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[455], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[456], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[457], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[458], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[459], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[460], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[461], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[462], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[463], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[464], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[465], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[466], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[467], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[468], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[469], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[470], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[471], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[472], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[473], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[474], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[475], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[476], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[477], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[478], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[479], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[480], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[481], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[482], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[483], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[484], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[485], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[486], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[487], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[488], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[489], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[490], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[491], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[492], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[493], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[494], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[495], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[496], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[497], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[498], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[499], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[500], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[501], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[502], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[503], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[504], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[505], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[506], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[507], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[508], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[509], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[510], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[511], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[512], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[513], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[514], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[515], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[516], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[517], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[518], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[519], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[520], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[521], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[522], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[523], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[524], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[525], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[526], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[527], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[528], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[529], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[530], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[531], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[532], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[533], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[534], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[535], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[536], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[537], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[538], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[539], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[540], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[541], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[542], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[543], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[544], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[545], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[546], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[547], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[548], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[549], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[550], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[551], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[552], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[553], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[554], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[555], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[556], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[557], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[558], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[559], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[560], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[561], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[562], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[563], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[564], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[565], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[566], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[567], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[568], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[569], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[570], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[571], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[572], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[573], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[574], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[575], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[576], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[577], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[578], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[579], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[580], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[581], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[582], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[583], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[584], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[585], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[586], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[587], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[588], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[589], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[590], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[591], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[592], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[593], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[594], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[595], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[596], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[597], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[598], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[599], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[600], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[601], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[602], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[603], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[604], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[605], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[606], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[607], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[608], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[609], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[610], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[611], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[612], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[613], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[614], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[615], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[616], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[617], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[618], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[619], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[620], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[621], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[622], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[623], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[624], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[625], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[626], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[627], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[628], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[629], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[630], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[631], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[632], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[633], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[634], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[635], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[636], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[637], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[638], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[639], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[640], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[641], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[642], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[643], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[644], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[645], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[646], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[647], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[648], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[649], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[650], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[651], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[652], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[653], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[654], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[655], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[656], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[657], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[658], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[659], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[660], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[661], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[662], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[663], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[664], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[665], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[666], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[667], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[668], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[669], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[670], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[671], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[672], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[673], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[674], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[675], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[676], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[677], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[678], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[679], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[680], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[681], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[682], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[683], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[684], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[685], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[686], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[687], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[688], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[689], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[690], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[691], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[692], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[693], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[694], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[695], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[696], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[697], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[698], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[699], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[700], (uint32_t)1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[701], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[702], (uint32_t)0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10___immval_0xffe) {
  ASSERT_EQ(getScalarWordData()[703], (uint32_t)1);
}

