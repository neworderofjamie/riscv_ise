// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/bgeu-01.h"

namespace
{
class bgeu_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::bgeu_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::bgeu_01::generateCode().getCode(), scalarInitData);
        
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

std::vector<uint8_t> bgeu_01::s_ScalarData;
}


// Generate google tests

TEST_F(bgeu_01, opcode__bgeu__op1_x2__op2_x2__op1val_0x8__op2val_0x8__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[4], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x31__op2_x3__op1val_0xffffffbf__op2val_0xffffffbf__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[5], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x21__op2_x25__op1val_0xffff__op2val_0xffff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[6], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x27__op2_x13__op1val_0xfffbffff__op2val_0xffefffff__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[7], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x14__op2_x29__op1val_0xaaaaaaab__op2val_0x4__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[8], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x7__op2_x16__op1val_0x7__op2val_0xb504__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[9], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x11__op2_x19__op1val_0xaaaaaaa9__op2val_0x1__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[10], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x21__op1val_0xaaaaaaaa__op2val_0x2__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[11], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x0__op2_x20__op1val_0x0__op2val_0x8__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[12], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x3__op2_x27__op1val_0xffff__op2val_0x10__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[13], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x25__op2_x28__op1val_0x6__op2val_0x20__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[14], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x30__op2_x0__op1val_0xfffff7ff__op2val_0x0__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[15], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x29__op2_x23__op1val_0xd__op2val_0x80__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[16], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x12__op2_x8__op1val_0xffffff7f__op2val_0x100__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[17], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x22__op2_x7__op1val_0xb504__op2val_0x200__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[18], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x24__op2_x26__op1val_0x4000__op2val_0x400__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[19], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x6__op2_x22__op1val_0x80000__op2val_0x800__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[20], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x4__op2_x18__op1val_0x8__op2val_0x1000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[21], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x1__op2_x30__op1val_0xb505__op2val_0x2000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[22], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x5__op2_x12__op1val_0xaaaaaaaa__op2val_0x4000__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[23], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x16__op2_x1__op1val_0x2000000__op2val_0x8000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[24], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x26__op2_x31__op1val_0xfffff7ff__op2val_0x10000__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[25], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x28__op2_x10__op1val_0xbfffffff__op2val_0x20000__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[26], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x9__op2_x24__op1val_0xfdffffff__op2val_0x40000__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[27], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x15__op2_x5__op1val_0x0__op2val_0x80000__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[28], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x18__op2_x14__op1val_0x55555556__op2val_0x100000__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[29], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x17__op2_x15__op1val_0x1__op2val_0x200000__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[30], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x20__op2_x17__op1val_0xfffff7ff__op2val_0x400000__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[31], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x23__op2_x11__op1val_0x4000__op2val_0x800000__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[32], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x13__op2_x4__op1val_0xfffffbff__op2val_0x1000000__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[33], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x19__op2_x6__op1val_0x80__op2val_0x2000000__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[34], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x8__op2_x9__op1val_0x3__op2val_0x4000000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[35], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff7fff__op2val_0x8000000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[36], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x10000000__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[37], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xdfffffff__op2val_0x20000000__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[38], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x40000000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[39], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x80000000__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[40], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x800000__op2val_0xfffffffe__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[41], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffff7ff__op2val_0xfffffffd__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[42], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0xfffffffb__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[43], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x8000000__op2val_0xfffffff7__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[44], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xffffffef__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[45], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0xffffffdf__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[46], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x12__op2val_0xffffff7f__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[47], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0xfffffeff__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[48], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4000000__op2val_0xfffffdff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[49], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0xfffff7ff__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[50], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xbfffffff__op2val_0xffffefff__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[51], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffffdff__op2val_0xffffdfff__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[52], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x8__op2val_0xffffbfff__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[53], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffffeff__op2val_0xffff7fff__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[54], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0xfffeffff__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[55], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffffdfff__op2val_0xfffdffff__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[56], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2000000__op2val_0xfffbffff__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[57], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffbffff__op2val_0xfff7ffff__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[58], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4000000__op2val_0xffdfffff__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[59], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffffdfff__op2val_0xffbfffff__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[60], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1000000__op2val_0xff7fffff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[61], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffffefff__op2val_0xfeffffff__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[62], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0xfdffffff__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[63], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xfbffffff__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[64], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffffbfff__op2val_0xf7ffffff__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[65], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4000__op2val_0xefffffff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[66], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0xdfffffff__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[67], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0xbfffffff__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[68], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x8000000__op2val_0x7fffffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[69], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x200000__op2val_0x55555555__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[70], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffffbff__op2val_0xaaaaaaaa__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[71], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10__op2val_0x20__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[72], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x20__op2val_0xfffffeff__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[73], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x40__op2val_0xf7ffffff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[74], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x100__op2val_0xfffffff7__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[75], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x200__op2val_0x400000__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[76], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x400__op2val_0xffffbfff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[77], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x800__op2val_0xaaaaaaaa__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[78], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1000__op2val_0x1000__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[79], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2000__op2val_0x1__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[80], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x8000__op2val_0xfffff7ff__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[81], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[82], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x20000__op2val_0x20000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[83], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x40000__op2val_0x8000000__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[84], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x100000__op2val_0x9__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[85], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x400000__op2val_0x8000__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[86], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000000__op2val_0x55555555__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[87], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x20000000__op2val_0x80__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[88], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x40000000__op2val_0xffffffbf__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[89], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x80000000__op2val_0x4000000__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[90], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffffffd__op2val_0xfffffffd__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[91], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffffffb__op2val_0x40__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[92], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffffff7__op2val_0xaaaaaaaa__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[93], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffffffef__op2val_0x0__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[94], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffffffdf__op2val_0x1__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[95], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffeffff__op2val_0xffffbfff__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[96], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffdffff__op2val_0xfffe__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[97], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfff7ffff__op2val_0xfffbffff__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[98], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffefffff__op2val_0x2__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[99], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffdfffff__op2val_0xfffdffff__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[100], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffbfffff__op2val_0xa__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[101], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xff7fffff__op2val_0xffffffef__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[102], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfeffffff__op2val_0x100__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[103], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfbffffff__op2val_0xffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[104], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xf7ffffff__op2val_0xaaaaaaaa__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[105], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xefffffff__op2val_0x12__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[106], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x7fffffff__op2val_0x6__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[107], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x3__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[108], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x55555555__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[109], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0xaaaaaaaa__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[110], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x5__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[111], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x33333333__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[112], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x66666666__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[113], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0xb504__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[114], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x0__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[115], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0xffff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[116], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x2__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[117], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x55555554__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[118], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0xaaaaaaa9__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[119], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x4__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[120], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x33333332__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[121], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x66666665__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[122], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0xb503__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[123], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0xfffe__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[124], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x55555556__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[125], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0xaaaaaaab__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[126], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x6__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[127], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x33333334__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[128], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x66666667__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[129], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0xb505__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[130], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x1__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[131], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x3__op2val_0x10000__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[132], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x3__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[133], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[134], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xaaaaaaaa__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[135], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x5__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[136], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x33333333__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[137], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x66666666__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[138], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xb504__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[139], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x0__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[140], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xffff__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[141], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x2__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[142], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x55555554__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[143], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xaaaaaaa9__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[144], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x4__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[145], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x33333332__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[146], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x66666665__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[147], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xb503__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[148], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xfffe__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[149], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x55555556__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[150], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xaaaaaaab__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[151], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x6__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[152], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x33333334__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[153], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x66666667__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[154], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xb505__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[155], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x1__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[156], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x10000__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[157], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x3__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[158], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[159], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xaaaaaaaa__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[160], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x5__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[161], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x33333333__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[162], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x66666666__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[163], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xb504__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[164], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x0__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[165], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xffff__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[166], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x55555554__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[167], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xaaaaaaa9__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[168], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x4__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[169], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x33333332__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[170], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x66666665__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[171], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xb503__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[172], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xfffe__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[173], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x55555556__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[174], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xaaaaaaab__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[175], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x6__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[176], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x33333334__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[177], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x66666667__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[178], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xb505__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[179], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x1__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[180], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x10000__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[181], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x3__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[182], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x55555555__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[183], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0xaaaaaaaa__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[184], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x5__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[185], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x33333333__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[186], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x66666666__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[187], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0xb504__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[188], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x0__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[189], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0xffff__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[190], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x2__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[191], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x55555554__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[192], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0xaaaaaaa9__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[193], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x4__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[194], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x33333332__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[195], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x66666665__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[196], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0xb503__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[197], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0xfffe__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[198], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[199], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0xaaaaaaab__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[200], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x6__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[201], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x33333334__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[202], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x66666667__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[203], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0xb505__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[204], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x1__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[205], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x5__op2val_0x10000__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[206], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x3__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[207], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x55555555__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[208], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xaaaaaaaa__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[209], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x5__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[210], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x33333333__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[211], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x66666666__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[212], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xb504__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[213], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x0__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[214], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xffff__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[215], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x2__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[216], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x55555554__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[217], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xaaaaaaa9__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[218], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x4__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[219], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x33333332__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[220], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x66666665__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[221], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xb503__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[222], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xfffe__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[223], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x55555556__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[224], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xaaaaaaab__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[225], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x6__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[226], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x33333334__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[227], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x66666667__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[228], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xb505__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[229], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x1__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[230], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x10000__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[231], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x3__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[232], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x55555555__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[233], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xaaaaaaaa__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[234], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x5__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[235], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x33333333__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[236], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x66666666__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[237], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xb504__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[238], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x0__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[239], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xffff__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[240], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x2__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[241], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x55555554__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[242], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xaaaaaaa9__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[243], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x4__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[244], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x33333332__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[245], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x66666665__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[246], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xb503__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[247], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xfffe__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[248], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[249], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xaaaaaaab__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[250], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x6__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[251], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x33333334__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[252], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x66666667__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[253], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xb505__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[254], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x1__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[255], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x10000__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[256], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x3__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[257], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x55555555__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[258], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0xaaaaaaaa__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[259], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x5__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[260], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x33333333__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[261], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x66666666__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[262], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[263], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x0__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[264], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0xffff__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[265], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x2__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[266], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x55555554__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[267], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0xaaaaaaa9__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[268], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x4__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[269], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x33333332__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[270], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x66666665__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[271], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0xb503__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[272], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0xfffe__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[273], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x55555556__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[274], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0xaaaaaaab__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[275], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x6__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[276], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x33333334__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[277], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x66666667__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[278], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0xb505__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[279], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x1__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[280], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb504__op2val_0x10000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[281], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x3__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[282], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x55555555__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[283], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0xaaaaaaaa__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[284], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x5__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[285], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x33333333__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[286], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x66666666__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[287], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0xb504__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[288], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x0__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[289], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0xffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[290], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x2__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[291], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x55555554__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[292], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0xaaaaaaa9__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[293], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x4__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[294], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[295], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x66666665__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[296], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0xb503__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[297], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0xfffe__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[298], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x55555556__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[299], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0xaaaaaaab__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[300], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x6__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[301], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x33333334__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[302], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x66666667__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[303], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0xb505__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[304], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x1__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[305], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x0__op2val_0x10000__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[306], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x3__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[307], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x55555555__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[308], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0xaaaaaaaa__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[309], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x5__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[310], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x33333333__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[311], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x66666666__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[312], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0xb504__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[313], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x0__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[314], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x2__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[315], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x55555554__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[316], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0xaaaaaaa9__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[317], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x4__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[318], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x33333332__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[319], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x66666665__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[320], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0xb503__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[321], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0xfffe__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[322], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x55555556__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[323], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0xaaaaaaab__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[324], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x6__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[325], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x33333334__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[326], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x66666667__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[327], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0xb505__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[328], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x1__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[329], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xffff__op2val_0x10000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[330], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x3__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[331], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x55555555__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[332], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0xaaaaaaaa__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[333], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x5__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[334], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x33333333__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[335], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x66666666__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[336], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0xb504__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[337], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x0__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[338], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0xffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[339], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x2__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[340], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x55555554__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[341], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0xaaaaaaa9__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[342], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x4__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[343], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x33333332__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[344], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x66666665__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[345], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0xb503__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[346], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0xfffe__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[347], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[348], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0xaaaaaaab__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[349], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x6__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[350], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x33333334__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[351], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x66666667__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[352], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0xb505__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[353], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x1__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[354], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x2__op2val_0x10000__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[355], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x3__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[356], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x55555555__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[357], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xaaaaaaaa__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[358], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x5__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[359], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x33333333__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[360], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x66666666__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[361], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xb504__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[362], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x0__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[363], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xffff__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[364], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x2__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[365], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x55555554__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[366], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xaaaaaaa9__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[367], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x4__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[368], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[369], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x66666665__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[370], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xb503__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[371], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xfffe__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[372], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[373], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xaaaaaaab__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[374], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x6__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[375], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x33333334__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[376], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x66666667__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[377], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xb505__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[378], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x1__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[379], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x10000__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[380], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x3__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[381], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x55555555__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[382], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xaaaaaaaa__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[383], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x5__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[384], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x33333333__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[385], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x66666666__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[386], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xb504__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[387], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x0__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[388], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[389], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x2__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[390], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x55555554__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[391], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xaaaaaaa9__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[392], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x4__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[393], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x33333332__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[394], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x66666665__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[395], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xb503__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[396], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xfffe__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[397], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x55555556__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[398], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xaaaaaaab__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[399], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x6__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[400], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x33333334__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[401], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x66666667__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[402], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xb505__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[403], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x10000__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[404], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x3__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[405], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[406], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0xaaaaaaaa__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[407], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x5__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[408], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x33333333__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[409], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x66666666__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[410], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[411], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x0__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[412], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0xffff__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[413], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x2__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[414], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x55555554__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[415], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0xaaaaaaa9__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[416], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x4__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[417], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x33333332__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[418], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x66666665__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[419], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0xb503__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[420], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0xfffe__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[421], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x55555556__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[422], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0xaaaaaaab__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[423], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x6__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[424], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x33333334__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[425], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x66666667__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[426], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0xb505__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[427], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x1__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[428], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x4__op2val_0x10000__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[429], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x3__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[430], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x55555555__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[431], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xaaaaaaaa__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[432], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x5__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[433], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x33333333__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[434], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x66666666__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[435], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xb504__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[436], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x0__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[437], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[438], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x2__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[439], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x55555554__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[440], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xaaaaaaa9__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[441], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x4__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[442], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x33333332__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[443], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x66666665__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[444], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xb503__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[445], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xfffe__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[446], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[447], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xaaaaaaab__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[448], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x6__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[449], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x33333334__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[450], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x66666667__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[451], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xb505__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[452], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x1__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[453], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x10000__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[454], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x3__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[455], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[456], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xaaaaaaaa__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[457], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x5__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[458], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x33333333__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[459], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x66666666__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[460], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[461], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x0__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[462], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xffff__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[463], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x2__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[464], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x55555554__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[465], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xaaaaaaa9__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[466], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x4__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[467], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x33333332__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[468], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x66666665__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[469], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xb503__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[470], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xfffe__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[471], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[472], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x1__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[473], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x10000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[474], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x3__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[475], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x55555555__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[476], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0xaaaaaaaa__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[477], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x5__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[478], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x33333333__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[479], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x66666666__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[480], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0xb504__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[481], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x0__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[482], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0xffff__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[483], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x2__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[484], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x55555554__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[485], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0xaaaaaaa9__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[486], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x4__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[487], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x33333332__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[488], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x66666665__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[489], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0xb503__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[490], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0xfffe__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[491], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x55555556__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[492], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0xaaaaaaab__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[493], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x6__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[494], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x33333334__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[495], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x66666667__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[496], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0xb505__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[497], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x1__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[498], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb503__op2val_0x10000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[499], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x3__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[500], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x55555555__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[501], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xaaaaaaaa__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[502], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x5__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[503], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x33333333__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[504], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x66666666__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[505], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xb504__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[506], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x0__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[507], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xffff__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[508], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x2__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[509], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x55555554__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[510], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xaaaaaaa9__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[511], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x4__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[512], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x33333332__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[513], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x66666665__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[514], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xb503__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[515], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xfffe__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[516], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x55555556__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[517], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xaaaaaaab__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[518], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x6__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[519], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x33333334__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[520], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x66666667__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[521], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xb505__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[522], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x1__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[523], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x10000__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[524], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x3__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[525], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x55555555__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[526], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xaaaaaaaa__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[527], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x5__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[528], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x33333333__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[529], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x66666666__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[530], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xb504__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[531], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x0__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[532], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xffff__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[533], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x2__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[534], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x55555554__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[535], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xaaaaaaa9__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[536], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x4__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[537], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x33333332__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[538], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x66666665__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[539], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xb503__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[540], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xfffe__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[541], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x55555556__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[542], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xaaaaaaab__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[543], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x6__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[544], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x33333334__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[545], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x66666667__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[546], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xb505__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[547], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x1__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[548], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x10000__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[549], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x3__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[550], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x55555555__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[551], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xaaaaaaaa__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[552], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x5__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[553], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x33333333__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[554], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x66666666__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[555], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xb504__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[556], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x0__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[557], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[558], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x2__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[559], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x55555554__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[560], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xaaaaaaa9__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[561], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x33333332__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[562], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x66666665__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[563], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xb503__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[564], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xfffe__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[565], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x55555556__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[566], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xaaaaaaab__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[567], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x6__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[568], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x33333334__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[569], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x66666667__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[570], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xb505__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[571], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x1__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[572], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x10000__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[573], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x3__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[574], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x55555555__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[575], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0xaaaaaaaa__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[576], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x5__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[577], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x33333333__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[578], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x66666666__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[579], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0xb504__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[580], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x0__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[581], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0xffff__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[582], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x2__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[583], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x55555554__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[584], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0xaaaaaaa9__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[585], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x4__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[586], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x33333332__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[587], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x66666665__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[588], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0xb503__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[589], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0xfffe__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[590], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x55555556__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[591], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0xaaaaaaab__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[592], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x6__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[593], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x33333334__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[594], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x66666667__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[595], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0xb505__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[596], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x1__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[597], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x6__op2val_0x10000__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[598], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x3__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[599], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[600], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xaaaaaaab__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[601], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xaaaaaaaa__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[602], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x5__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[603], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x33333333__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[604], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x66666666__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[605], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xb504__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[606], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x0__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[607], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xffff__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[608], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x2__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[609], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x55555554__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[610], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xaaaaaaa9__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[611], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x4__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[612], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x33333332__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[613], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x66666665__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[614], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xb503__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[615], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xfffe__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[616], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[617], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xaaaaaaab__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[618], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x6__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[619], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x33333334__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[620], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x66666667__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[621], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xb505__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[622], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x1__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[623], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x10000__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[624], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x3__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[625], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x55555555__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[626], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xaaaaaaaa__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[627], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x5__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[628], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x33333333__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[629], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x66666666__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[630], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xb504__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[631], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x0__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[632], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xffff__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[633], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x2__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[634], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x55555554__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[635], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xaaaaaaa9__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[636], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x4__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[637], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[638], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x66666665__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[639], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xb503__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[640], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xfffe__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[641], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x55555556__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[642], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xaaaaaaab__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[643], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x6__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[644], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x33333334__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[645], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x66666667__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[646], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xb505__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[647], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x1__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[648], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x10000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[649], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x3__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[650], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x55555555__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[651], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0xaaaaaaaa__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[652], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x5__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[653], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x33333333__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[654], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x66666666__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[655], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0xb504__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[656], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x0__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[657], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0xffff__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[658], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x2__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[659], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x55555554__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[660], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0xaaaaaaa9__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[661], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x4__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[662], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x33333332__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[663], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x66666665__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[664], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0xb503__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[665], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0xfffe__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[666], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x55555556__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[667], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0xaaaaaaab__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[668], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x6__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[669], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x33333334__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[670], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x66666667__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[671], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0xb505__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[672], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x1__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[673], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xb505__op2val_0x10000__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[674], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x3__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[675], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x55555555__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[676], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0xaaaaaaaa__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[677], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x5__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[678], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x33333333__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[679], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x66666666__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[680], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0xb504__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[681], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x0__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[682], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0xffff__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[683], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x2__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[684], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x55555554__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[685], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0xaaaaaaa9__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[686], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x4__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[687], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[688], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x66666665__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[689], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0xb503__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[690], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0xfffe__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[691], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x55555556__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[692], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0xaaaaaaab__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[693], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x6__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[694], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x33333334__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[695], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x66666667__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[696], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0xb505__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[697], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x1__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[698], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x1__op2val_0x10000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[699], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x3__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[700], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0xaaaaaaaa__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[701], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x5__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[702], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x33333333__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[703], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x66666666__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[704], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0xb504__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[705], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x0__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[706], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0xffff__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[707], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x2__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[708], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x55555554__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[709], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0xaaaaaaa9__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[710], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x4__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[711], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x33333332__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[712], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x66666665__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[713], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0xb503__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[714], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0xfffe__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[715], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x55555556__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[716], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0xaaaaaaab__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[717], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x6__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[718], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x33333334__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[719], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x66666667__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[720], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0xb505__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[721], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x1__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[722], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x10000__op2val_0x10000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[723], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x6__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[724], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x33333334__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[725], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x66666667__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[726], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xb505__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[727], 3);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0x8__op2val_0xfffffbff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[728], 1);
}


TEST_F(bgeu_01, opcode__bgeu__op1_x10__op2_x11__op1val_0xfffffffe__op2val_0x8__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[729], 1);
}

