// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/sltu-01.h"

namespace
{
class sltu_01Fixture : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = sltu_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(sltu_01::generateCode().getCode(), scalarInitData);
        
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

std::vector<uint8_t> sltu_01Fixture::s_ScalarData;
}


// Generate google tests

TEST_F(sltu_01Fixture, opcode__sltu___op1_x0__op2_x31__dest_x31__op1val_0x0___op2val_0xfffffffe) {
  ASSERT_EQ(getScalarWordData()[4], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x19__op2_x19__dest_x5__op1val_0x100000___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x25__op2_x25__dest_x25__op1val_0x40000000___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[6], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x14__op2_x24__dest_x14__op1val_0xfffffffe___op2val_0xffffffff) {
  ASSERT_EQ(getScalarWordData()[7], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x17__op2_x13__dest_x12__op1val_0x1___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[8], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x26__op2_x18__dest_x24__op1val_0x0___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[9], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x5__op2_x14__dest_x19__op1val_0xffffffff___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x3__op2_x22__dest_x0__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[11], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x23__op2_x29__dest_x20__op1val_0xf7ffffff___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x4__op2_x6__dest_x10__op1val_0x11___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x12__op2_x17__dest_x1__op1val_0x7fffffff___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[14], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x30__op2_x8__dest_x6__op1val_0x2000000___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x21__op2_x16__dest_x3__op1val_0xfffff7ff___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[16], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x29__op2_x26__dest_x17__op1val_0x400___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x18__op2_x10__dest_x28__op1val_0xd___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[18], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x2__op2_x28__dest_x11__op1val_0x4___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[19], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x8__op2_x30__dest_x29__op1val_0xffffffbf___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x22__op2_x11__dest_x7__op1val_0x80___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[21], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x15__op2_x4__dest_x9__op1val_0x200000___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[22], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x28__op2_x12__dest_x13__op1val_0x80___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[23], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x31__op2_x20__dest_x26__op1val_0x800000___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[24], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x27__op2_x5__dest_x16__op1val_0x8000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[25], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x16__op2_x0__dest_x2__op1val_0xdfffffff___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[26], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x11__op2_x9__dest_x23__op1val_0x3___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[27], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x20__op2_x21__dest_x4__op1val_0x80000000___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[28], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x13__op2_x7__dest_x27__op1val_0x20000000___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[29], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x7__op2_x1__dest_x18__op1val_0x8___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[30], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x1__op2_x27__dest_x22__op1val_0xffefffff___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[31], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x9__op2_x23__dest_x21__op1val_0xaaaaaaaa___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[32], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x6__op2_x2__dest_x30__op1val_0x66666667___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[33], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x3__dest_x15__op1val_0xfbffffff___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[34], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x24__op2_x15__dest_x8__op1val_0x100___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[35], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffffdfff___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[36], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[37], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfeffffff___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[38], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[39], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0x80000000) {
  ASSERT_EQ(getScalarWordData()[40], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xfffffffd) {
  ASSERT_EQ(getScalarWordData()[41], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val_0xfffffffb) {
  ASSERT_EQ(getScalarWordData()[42], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xfffffff7) {
  ASSERT_EQ(getScalarWordData()[43], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xffffffef) {
  ASSERT_EQ(getScalarWordData()[44], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb___op2val_0xffffffdf) {
  ASSERT_EQ(getScalarWordData()[45], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xa___op2val_0xffffffbf) {
  ASSERT_EQ(getScalarWordData()[46], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val_0xffffff7f) {
  ASSERT_EQ(getScalarWordData()[47], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffffff7___op2val_0xfffffeff) {
  ASSERT_EQ(getScalarWordData()[48], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val_0xfffffdff) {
  ASSERT_EQ(getScalarWordData()[49], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffffff7___op2val_0xfffffbff) {
  ASSERT_EQ(getScalarWordData()[50], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val_0xfffff7ff) {
  ASSERT_EQ(getScalarWordData()[51], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfff7ffff___op2val_0xffffefff) {
  ASSERT_EQ(getScalarWordData()[52], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xffffdfff) {
  ASSERT_EQ(getScalarWordData()[53], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0xffffbfff) {
  ASSERT_EQ(getScalarWordData()[54], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffdffff___op2val_0xffff7fff) {
  ASSERT_EQ(getScalarWordData()[55], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xa___op2val_0xfffeffff) {
  ASSERT_EQ(getScalarWordData()[56], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfff7ffff___op2val_0xfffdffff) {
  ASSERT_EQ(getScalarWordData()[57], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val_0xfffbffff) {
  ASSERT_EQ(getScalarWordData()[58], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val_0xfff7ffff) {
  ASSERT_EQ(getScalarWordData()[59], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val_0xffefffff) {
  ASSERT_EQ(getScalarWordData()[60], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xffdfffff) {
  ASSERT_EQ(getScalarWordData()[61], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xffbfffff) {
  ASSERT_EQ(getScalarWordData()[62], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0xff7fffff) {
  ASSERT_EQ(getScalarWordData()[63], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xfeffffff) {
  ASSERT_EQ(getScalarWordData()[64], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val_0xfdffffff) {
  ASSERT_EQ(getScalarWordData()[65], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xfbffffff) {
  ASSERT_EQ(getScalarWordData()[66], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xf7ffffff) {
  ASSERT_EQ(getScalarWordData()[67], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffdfffff___op2val_0xefffffff) {
  ASSERT_EQ(getScalarWordData()[68], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xdfffffff) {
  ASSERT_EQ(getScalarWordData()[69], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val_0xbfffffff) {
  ASSERT_EQ(getScalarWordData()[70], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[71], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffbfffff___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[72], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[73], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[74], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[75], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[76], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[77], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[78], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[79], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[80], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[81], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[82], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[83], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0xfffdffff) {
  ASSERT_EQ(getScalarWordData()[84], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffffffd___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[85], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffffffb___op2val_0xffffff7f) {
  ASSERT_EQ(getScalarWordData()[86], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffffffef___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[87], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffffffdf___op2val_0xfffbffff) {
  ASSERT_EQ(getScalarWordData()[88], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffffff7f___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[89], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffffeff___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[90], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffffdff___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[91], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffffbff___op2val_0xf7ffffff) {
  ASSERT_EQ(getScalarWordData()[92], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffffefff___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[93], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffffbfff___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[94], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff7fff___op2val_0xfffeffff) {
  ASSERT_EQ(getScalarWordData()[95], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffeffff___op2val_0xfffff7ff) {
  ASSERT_EQ(getScalarWordData()[96], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffbffff___op2val_0xfff7ffff) {
  ASSERT_EQ(getScalarWordData()[97], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xff7fffff___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[98], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfdffffff___op2val_0xbfffffff) {
  ASSERT_EQ(getScalarWordData()[99], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xefffffff___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[100], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xbfffffff___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[101], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[102], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[103], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[104], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[105], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[106], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[107], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[108], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[109], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[110], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[111], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[112], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[113], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[114], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[115], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[116], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[117], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[118], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[119], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[120], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[121], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[122], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[123], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[124], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[125], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[126], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[127], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[128], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[129], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[130], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[131], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[132], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[133], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[134], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[135], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[136], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[137], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[138], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[139], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[140], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[141], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[142], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[143], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[144], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[145], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[146], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[147], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[148], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[149], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[150], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[151], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[152], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[153], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[154], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[155], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[156], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[157], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[158], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[159], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[160], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[161], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[162], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[163], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[164], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[165], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[166], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[167], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[168], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[169], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[170], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[171], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[172], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[173], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[174], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[175], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaaa___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[176], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[177], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[178], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[179], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[180], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[181], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[182], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[183], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[184], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[185], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[186], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[187], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[188], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[189], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[190], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[191], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[192], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[193], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[194], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[195], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[196], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[197], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[198], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[199], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[200], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[201], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[202], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[203], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[204], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[205], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[206], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[207], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[208], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[209], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[210], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[211], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[212], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[213], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[214], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[215], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[216], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[217], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[218], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[219], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[220], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[221], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[222], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[223], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[224], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[225], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[226], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[227], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[228], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[229], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[230], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[231], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[232], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[233], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[234], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[235], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[236], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[237], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[238], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[239], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[240], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[241], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[242], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[243], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[244], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[245], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[246], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[247], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[248], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[249], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[250], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[251], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[252], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[253], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[254], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[255], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[256], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[257], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[258], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[259], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[260], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[261], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[262], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[263], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[264], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[265], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[266], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[267], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[268], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[269], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[270], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[271], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[272], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[273], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[274], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[275], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[276], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[277], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[278], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[279], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[280], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[281], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[282], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[283], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[284], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[285], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[286], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[287], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[288], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[289], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[290], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[291], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[292], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[293], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[294], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[295], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[296], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[297], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[298], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[299], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[300], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[301], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[302], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[303], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[304], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[305], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[306], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[307], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[308], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[309], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[310], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[311], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[312], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[313], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[314], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[315], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[316], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[317], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[318], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[319], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[320], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[321], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[322], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[323], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[324], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[325], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xffff___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[326], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[327], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[328], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[329], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[330], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[331], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[332], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[333], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[334], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[335], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[336], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[337], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[338], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[339], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[340], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[341], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[342], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[343], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[344], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[345], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[346], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[347], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[348], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[349], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[350], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[351], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[352], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[353], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[354], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[355], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[356], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[357], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[358], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[359], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[360], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[361], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[362], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[363], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[364], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[365], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[366], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[367], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[368], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[369], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[370], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[371], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[372], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[373], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[374], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[375], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[376], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[377], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[378], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[379], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[380], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[381], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[382], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[383], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[384], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[385], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[386], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[387], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[388], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[389], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[390], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[391], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[392], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[393], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[394], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[395], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[396], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[397], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[398], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaa9___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[399], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[400], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[401], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[402], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[403], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[404], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[405], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[406], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[407], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[408], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[409], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[410], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[411], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[412], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[413], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[414], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[415], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[416], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[417], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[418], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[419], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[420], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[421], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[422], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[423], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[424], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[425], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[426], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[427], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[428], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[429], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[430], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[431], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[432], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[433], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[434], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[435], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[436], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[437], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[438], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[439], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[440], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[441], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[442], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[443], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[444], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[445], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[446], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[447], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[448], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[449], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[450], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[451], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[452], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[453], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[454], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[455], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[456], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[457], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[458], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[459], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[460], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[461], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[462], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[463], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[464], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[465], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[466], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[467], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[468], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[469], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[470], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[471], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[472], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[473], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[474], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[475], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[476], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[477], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[478], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[479], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[480], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[481], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[482], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[483], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[484], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[485], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[486], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[487], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[488], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[489], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[490], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[491], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[492], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[493], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[494], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[495], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[496], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[497], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[498], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[499], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[500], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[501], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[502], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[503], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[504], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[505], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[506], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[507], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[508], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[509], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[510], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[511], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[512], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[513], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[514], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[515], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[516], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[517], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xfffe___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[518], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[519], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[520], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[521], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[522], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[523], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[524], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[525], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[526], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[527], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[528], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[529], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[530], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[531], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[532], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[533], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[534], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[535], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[536], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[537], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[538], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[539], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[540], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[541], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[542], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[543], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[544], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[545], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[546], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[547], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[548], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[549], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[550], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[551], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[552], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[553], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[554], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[555], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[556], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[557], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[558], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[559], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[560], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[561], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[562], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[563], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[564], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[565], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[566], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[567], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xaaaaaaab___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[568], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[569], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[570], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[571], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[572], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[573], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[574], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[575], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[576], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[577], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[578], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[579], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[580], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[581], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[582], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[583], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[584], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[585], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[586], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[587], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[588], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[589], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[590], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[591], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[592], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[593], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[594], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[595], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[596], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[597], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[598], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[599], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[600], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[601], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[602], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[603], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[604], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[605], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[606], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[607], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[608], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[609], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[610], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[611], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[612], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[613], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[614], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[615], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[616], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[617], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[618], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[619], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[620], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[621], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[622], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[623], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[624], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[625], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[626], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[627], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[628], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[629], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[630], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[631], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[632], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[633], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[634], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[635], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[636], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[637], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[638], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[639], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[640], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[641], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[642], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[643], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[644], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[645], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[646], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[647], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[648], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[649], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[650], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[651], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[652], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[653], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[654], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[655], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[656], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[657], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[658], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[659], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[660], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[661], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[662], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[663], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[664], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[665], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[666], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[667], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[668], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[669], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[670], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[671], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[672], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[673], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[674], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[675], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[676], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[677], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[678], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[679], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[680], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[681], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[682], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[683], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[684], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[685], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[686], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[687], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[688], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[689], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[690], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[691], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[692], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[693], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[694], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[695], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0xaaaaaaaa) {
  ASSERT_EQ(getScalarWordData()[696], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[697], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[698], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[699], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[700], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0xffff) {
  ASSERT_EQ(getScalarWordData()[701], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[702], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[703], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0xaaaaaaa9) {
  ASSERT_EQ(getScalarWordData()[704], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[705], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[706], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[707], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[708], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0xfffe) {
  ASSERT_EQ(getScalarWordData()[709], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[710], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0xaaaaaaab) {
  ASSERT_EQ(getScalarWordData()[711], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[712], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[713], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[714], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[715], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[716], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[717], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[718], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[719], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[720], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[721], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val_0xfffffffe) {
  ASSERT_EQ(getScalarWordData()[722], 1);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[723], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[724], 0);
}


TEST_F(sltu_01Fixture, opcode__sltu___op1_x10__op2_x11__dest_x12__op1val_0xdfffffff___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[725], 0);
}

