// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/bltu-01.h"

namespace
{
class bltu_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::bltu_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::bltu_01::generateCode().getCode(), scalarInitData);
        
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

std::vector<uint8_t> bltu_01::s_ScalarData;
}


// Generate google tests

TEST_F(bltu_01, opcode__bltu__op1_x17__op2_x17__op1val_0x20000000__op2val_0x20000000__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[4], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x8__op2_x20__op1val_0xff7fffff__op2val_0xff7fffff__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[5], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x21__op2_x7__op1val_0x7fffffff__op2val_0x7fffffff__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[6], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x20__op2_x21__op1val_0xfdffffff__op2val_0xaaaaaaab__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[7], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x26__op2_x19__op1val_0xfffffffe__op2val_0xffffffff__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[8], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x25__op2_x0__op1val_0x40000000__op2val_0x0__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[9], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x29__op2_x18__op1val_0x40000__op2val_0x1__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[10], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x0__op2_x13__op1val_0x0__op2val_0x2__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[11], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x30__op2_x1__op1val_0xaaaaaaab__op2val_0x4__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[12], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x16__op2_x8__op1val_0x80__op2val_0x8__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[13], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x7__op2_x30__op1val_0x40000__op2val_0x10__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[14], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x9__op2_x3__op1val_0x0__op2val_0x20__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[15], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x14__op2_x9__op1val_0x11__op2val_0x40__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[16], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x6__op2_x29__op1val_0xbfffffff__op2val_0x80__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[17], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x1__op2_x5__op1val_0x7__op2val_0x100__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[18], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x31__op2_x28__op1val_0xf__op2val_0x200__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[19], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x18__op2_x22__op1val_0x4000000__op2val_0x400__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[20], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x4__op1val_0x66666665__op2val_0x800__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[21], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x4__op2_x31__op1val_0xefffffff__op2val_0x1000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[22], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x15__op2_x25__op1val_0xffffffef__op2val_0x2000__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[23], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x5__op2_x16__op1val_0x0__op2val_0x4000__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[24], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x12__op2_x26__op1val_0x0__op2val_0x10000__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[25], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x22__op2_x24__op1val_0x20__op2val_0x20000__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[26], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x2__op2_x27__op1val_0x7__op2val_0x40000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[27], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x27__op2_x15__op1val_0xe__op2val_0x80000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[28], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x11__op2_x10__op1val_0x800000__op2val_0x100000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[29], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x23__op2_x2__op1val_0x40__op2val_0x200000__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[30], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x19__op2_x12__op1val_0x100__op2val_0x400000__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[31], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x24__op2_x11__op1val_0xfffffffd__op2val_0x800000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[32], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x3__op2_x14__op1val_0x40000000__op2val_0x1000000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[33], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x28__op2_x6__op1val_0xffdfffff__op2val_0x2000000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[34], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x13__op2_x23__op1val_0x66666665__op2val_0x4000000__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[35], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xbfffffff__op2val_0x8000000__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[36], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xefffffff__op2val_0x10000000__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[37], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4000__op2val_0x20000000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[38], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffeffff__op2val_0x40000000__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[39], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffdffff__op2val_0x80000000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[40], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0xfffffffe__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[41], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xc__op2val_0xfffffffd__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[42], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0xfffffffb__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[43], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xfffffff7__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[44], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xffffffef__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[45], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffffffef__op2val_0xffffffdf__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[46], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffdfffff__op2val_0xffffffbf__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[47], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xf__op2val_0xffffff7f__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[48], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xf__op2val_0xfffffeff__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[49], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x800000__op2val_0xfffffdff__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[50], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb__op2val_0xfffffbff__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[51], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x100000__op2val_0xfffff7ff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[52], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xffffefff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[53], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xffffdfff__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[54], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xdfffffff__op2val_0xffffbfff__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[55], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0xffff7fff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[56], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2000__op2val_0xfffeffff__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[57], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4000__op2val_0xfffdffff__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[58], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0xfffbffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[59], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0xfff7ffff__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[60], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0xffefffff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[61], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0xffdfffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[62], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffffff7f__op2val_0xffbfffff__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[63], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x40000__op2val_0xfeffffff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[64], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0xfdffffff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[65], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x40000000__op2val_0xf7ffffff__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[66], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffffff7__op2val_0xefffffff__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[67], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffffffef__op2val_0xdfffffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[68], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0xbfffffff__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[69], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x55555555__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[70], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10__op2val_0xaaaaaaaa__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[71], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x8__op2val_0x3__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[72], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x200__op2val_0xd__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[73], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x400__op2val_0xffbfffff__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[74], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x800__op2val_0x5__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[75], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1000__op2val_0x40000000__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[76], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x8000__op2val_0xfffbffff__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[77], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x20000__op2val_0x800000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[78], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x80000__op2val_0xffffefff__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[79], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x200000__op2val_0x4000000__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[80], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x400000__op2val_0x40000__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[81], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1000000__op2val_0xdfffffff__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[82], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2000000__op2val_0x4000__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[83], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x8000000__op2val_0x20000__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[84], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000000__op2val_0xfffffbff__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[85], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x80000000__op2val_0xfdffffff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[86], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffffffb__op2val_0x55555554__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[87], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffffffdf__op2val_0xffff7fff__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[88], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffffffbf__op2val_0x2__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[89], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffffeff__op2val_0xaaaaaaaa__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[90], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffffdff__op2val_0x400__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[91], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffffbff__op2val_0x100000__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[92], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffff7ff__op2val_0x1000__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[93], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffffefff__op2val_0x3__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[94], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffffdfff__op2val_0xff7fffff__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[95], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffffbfff__op2val_0x3__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[96], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff7fff__op2val_0x9__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[97], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffbffff__op2val_0x800__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[98], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfff7ffff__op2val_0xffffffff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[99], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffefffff__op2val_0x10000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[100], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffbfffff__op2val_0x0__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[101], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfeffffff__op2val_0x80__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[102], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfbffffff__op2val_0x3__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[103], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xf7ffffff__op2val_0xffffbfff__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[104], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xdfffffff__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[105], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x3__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[106], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x55555555__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[107], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0xaaaaaaaa__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[108], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x5__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[109], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x33333333__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[110], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x66666666__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[111], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[112], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x0__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[113], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0xffff__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[114], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x2__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[115], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x55555554__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[116], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0xaaaaaaa9__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[117], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x4__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[118], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x33333332__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[119], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x66666665__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[120], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0xb503__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[121], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0xfffe__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[122], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x55555556__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[123], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0xaaaaaaab__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[124], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x6__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[125], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x33333334__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[126], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x66666667__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[127], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0xb505__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[128], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x1__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[129], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x3__op2val_0x10000__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[130], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x3__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[131], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x55555555__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[132], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xaaaaaaaa__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[133], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x5__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[134], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x33333333__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[135], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x66666666__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[136], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xb504__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[137], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x0__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[138], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xffff__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[139], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x2__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[140], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x55555554__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[141], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xaaaaaaa9__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[142], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x4__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[143], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x33333332__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[144], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x66666665__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[145], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xb503__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[146], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xfffe__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[147], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x55555556__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[148], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xaaaaaaab__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[149], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x6__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[150], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x33333334__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[151], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x66666667__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[152], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0xb505__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[153], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x1__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[154], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555555__op2val_0x10000__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[155], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x3__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[156], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x55555555__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[157], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xaaaaaaaa__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[158], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x5__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[159], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x33333333__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[160], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x66666666__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[161], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xb504__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[162], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x0__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[163], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xffff__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[164], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x2__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[165], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x55555554__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[166], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xaaaaaaa9__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[167], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x4__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[168], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x33333332__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[169], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x66666665__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[170], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xb503__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[171], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xfffe__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[172], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x55555556__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[173], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xaaaaaaab__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[174], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x6__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[175], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x33333334__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[176], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x66666667__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[177], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0xb505__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[178], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x1__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[179], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaaa__op2val_0x10000__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[180], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x3__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[181], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[182], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0xaaaaaaaa__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[183], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x5__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[184], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x33333333__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[185], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x66666666__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[186], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[187], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x0__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[188], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0xffff__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[189], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x2__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[190], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x55555554__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[191], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0xaaaaaaa9__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[192], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x4__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[193], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x33333332__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[194], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x66666665__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[195], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0xb503__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[196], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0xfffe__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[197], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x55555556__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[198], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0xaaaaaaab__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[199], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x6__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[200], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x33333334__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[201], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x66666667__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[202], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0xb505__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[203], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x1__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[204], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x5__op2val_0x10000__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[205], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x3__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[206], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x55555555__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[207], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xaaaaaaaa__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[208], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x5__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[209], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x33333333__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[210], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x66666666__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[211], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xb504__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[212], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x0__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[213], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xffff__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[214], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x55555554__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[215], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xaaaaaaa9__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[216], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x4__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[217], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x33333332__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[218], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x66666665__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[219], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xb503__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[220], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xfffe__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[221], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x55555556__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[222], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xaaaaaaab__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[223], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x6__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[224], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x33333334__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[225], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x66666667__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[226], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0xb505__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[227], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x1__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[228], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x10000__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[229], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x3__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[230], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x55555555__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[231], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xaaaaaaaa__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[232], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x5__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[233], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x33333333__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[234], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x66666666__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[235], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xb504__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[236], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x0__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[237], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xffff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[238], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x2__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[239], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x55555554__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[240], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xaaaaaaa9__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[241], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x4__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[242], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[243], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x66666665__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[244], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xb503__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[245], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xfffe__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[246], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x55555556__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[247], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xaaaaaaab__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[248], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x6__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[249], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x33333334__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[250], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x66666667__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[251], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0xb505__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[252], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x1__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[253], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666666__op2val_0x10000__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[254], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x3__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[255], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x55555555__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[256], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0xaaaaaaaa__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[257], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x5__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[258], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x33333333__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[259], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x66666666__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[260], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0xb504__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[261], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x0__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[262], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0xffff__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[263], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x2__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[264], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x55555554__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[265], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0xaaaaaaa9__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[266], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x4__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[267], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x33333332__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[268], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x66666665__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[269], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0xb503__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[270], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0xfffe__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[271], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x55555556__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[272], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0xaaaaaaab__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[273], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x6__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[274], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x33333334__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[275], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x66666667__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[276], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0xb505__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[277], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x1__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[278], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb504__op2val_0x10000__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[279], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x3__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[280], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x55555555__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[281], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0xaaaaaaaa__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[282], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x5__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[283], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x33333333__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[284], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x66666666__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[285], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0xb504__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[286], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x0__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[287], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0xffff__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[288], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x2__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[289], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x55555554__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[290], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0xaaaaaaa9__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[291], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x4__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[292], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[293], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x66666665__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[294], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0xb503__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[295], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0xfffe__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[296], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x55555556__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[297], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0xaaaaaaab__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[298], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x6__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[299], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x33333334__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[300], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x66666667__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[301], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0xb505__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[302], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x0__op2val_0x1__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[303], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x3__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[304], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x55555555__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[305], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0xaaaaaaaa__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[306], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x5__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[307], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x33333333__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[308], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x66666666__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[309], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0xb504__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[310], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x0__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[311], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0xffff__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[312], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x2__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[313], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x55555554__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[314], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0xaaaaaaa9__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[315], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x4__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[316], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x33333332__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[317], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x66666665__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[318], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0xb503__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[319], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0xfffe__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[320], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x55555556__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[321], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0xaaaaaaab__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[322], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x6__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[323], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x33333334__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[324], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x66666667__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[325], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0xb505__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[326], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x1__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[327], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xffff__op2val_0x10000__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[328], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x3__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[329], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x55555555__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[330], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0xaaaaaaaa__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[331], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x5__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[332], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x33333333__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[333], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x66666666__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[334], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0xb504__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[335], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x0__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[336], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0xffff__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[337], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x2__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[338], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x55555554__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[339], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0xaaaaaaa9__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[340], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x4__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[341], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[342], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x66666665__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[343], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0xb503__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[344], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0xfffe__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[345], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[346], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0xaaaaaaab__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[347], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x6__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[348], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x33333334__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[349], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x66666667__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[350], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0xb505__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[351], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x1__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[352], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x2__op2val_0x10000__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[353], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x3__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[354], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x55555555__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[355], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xaaaaaaaa__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[356], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x5__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[357], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x33333333__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[358], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x66666666__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[359], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xb504__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[360], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x0__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[361], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xffff__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[362], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x2__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[363], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x55555554__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[364], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xaaaaaaa9__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[365], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x4__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[366], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x33333332__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[367], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x66666665__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[368], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xb503__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[369], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xfffe__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[370], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x55555556__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[371], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xaaaaaaab__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[372], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x6__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[373], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x33333334__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[374], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x66666667__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[375], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0xb505__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[376], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x1__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[377], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555554__op2val_0x10000__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[378], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x3__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[379], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x55555555__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[380], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xaaaaaaaa__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[381], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x5__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[382], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x33333333__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[383], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x66666666__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[384], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xb504__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[385], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x0__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[386], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xffff__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[387], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x2__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[388], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x55555554__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[389], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xaaaaaaa9__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[390], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x4__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[391], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[392], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x66666665__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[393], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xb503__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[394], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xfffe__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[395], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[396], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xaaaaaaab__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[397], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x6__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[398], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x33333334__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[399], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x66666667__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[400], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0xb505__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[401], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x1__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[402], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaa9__op2val_0x10000__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[403], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x3__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[404], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x55555555__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[405], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0xaaaaaaaa__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[406], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x5__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[407], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x33333333__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[408], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x66666666__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[409], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0xb504__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[410], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x0__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[411], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0xffff__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[412], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x2__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[413], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x55555554__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[414], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0xaaaaaaa9__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[415], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x4__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[416], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[417], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x66666665__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[418], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0xb503__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[419], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0xfffe__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[420], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x55555556__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[421], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0xaaaaaaab__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[422], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x6__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[423], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x33333334__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[424], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x66666667__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[425], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0xb505__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[426], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x1__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[427], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x4__op2val_0x10000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[428], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x3__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[429], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x55555555__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[430], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xaaaaaaaa__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[431], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x5__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[432], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x33333333__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[433], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x66666666__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[434], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xb504__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[435], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x0__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[436], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xffff__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[437], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x2__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[438], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x55555554__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[439], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xaaaaaaa9__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[440], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x4__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[441], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x33333332__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[442], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x66666665__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[443], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xb503__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[444], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xfffe__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[445], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x55555556__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[446], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xaaaaaaab__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[447], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x6__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[448], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x33333334__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[449], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x66666667__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[450], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0xb505__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[451], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x1__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[452], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333332__op2val_0x10000__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[453], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x3__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[454], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x55555555__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[455], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xaaaaaaaa__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[456], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x5__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[457], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x33333333__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[458], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x66666666__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[459], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xb504__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[460], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x0__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[461], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xffff__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[462], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x2__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[463], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x55555554__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[464], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xaaaaaaa9__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[465], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x4__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[466], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x33333332__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[467], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x66666665__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[468], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xb503__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[469], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xfffe__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[470], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x55555556__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[471], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x1__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[472], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x10000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[473], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x3__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[474], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x55555555__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[475], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0xaaaaaaaa__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[476], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x5__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[477], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x33333333__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[478], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x66666666__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[479], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0xb504__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[480], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x0__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[481], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0xffff__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[482], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x2__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[483], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x55555554__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[484], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0xaaaaaaa9__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[485], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x4__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[486], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x33333332__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[487], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x66666665__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[488], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0xb503__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[489], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0xfffe__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[490], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x55555556__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[491], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0xaaaaaaab__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[492], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x6__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[493], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x33333334__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[494], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x66666667__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[495], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0xb505__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[496], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x1__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[497], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb503__op2val_0x10000__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[498], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x3__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[499], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x55555555__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[500], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xaaaaaaaa__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[501], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x5__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[502], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x33333333__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[503], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x66666666__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[504], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xb504__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[505], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x0__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[506], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xffff__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[507], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x2__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[508], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x55555554__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[509], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xaaaaaaa9__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[510], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x4__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[511], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x33333332__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[512], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x66666665__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[513], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xb503__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[514], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xfffe__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[515], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x55555556__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[516], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xaaaaaaab__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[517], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x6__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[518], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x33333334__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[519], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x66666667__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[520], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0xb505__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[521], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x1__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[522], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xfffe__op2val_0x10000__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[523], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x3__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[524], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x55555555__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[525], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xaaaaaaaa__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[526], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x5__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[527], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x33333333__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[528], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x66666666__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[529], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xb504__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[530], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x0__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[531], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[532], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x2__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[533], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x55555554__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[534], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xaaaaaaa9__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[535], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x4__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[536], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x33333332__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[537], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x66666665__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[538], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xb503__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[539], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xfffe__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[540], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x55555556__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[541], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xaaaaaaab__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[542], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x6__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[543], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x33333334__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[544], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x66666667__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[545], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0xb505__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[546], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x1__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[547], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x55555556__op2val_0x10000__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[548], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x3__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[549], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x55555555__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[550], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xaaaaaaaa__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[551], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x5__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[552], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x33333333__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[553], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x66666666__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[554], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xb504__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[555], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x0__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[556], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xffff__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[557], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x2__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[558], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x55555554__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[559], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xaaaaaaa9__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[560], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x33333332__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[561], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x66666665__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[562], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xb503__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[563], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xfffe__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[564], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x55555556__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[565], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xaaaaaaab__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[566], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x6__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[567], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x33333334__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[568], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x66666667__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[569], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0xb505__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[570], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x1__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[571], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xaaaaaaab__op2val_0x10000__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[572], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x3__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[573], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x55555555__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[574], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0xaaaaaaaa__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[575], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x5__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[576], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x33333333__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[577], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x66666666__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[578], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0xb504__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[579], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x0__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[580], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0xffff__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[581], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x2__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[582], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x55555554__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[583], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0xaaaaaaa9__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[584], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x4__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[585], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[586], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x66666665__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[587], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0xb503__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[588], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0xfffe__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[589], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x55555556__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[590], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0xaaaaaaab__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[591], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x6__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[592], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x33333334__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[593], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x66666667__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[594], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0xb505__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[595], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x1__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[596], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x6__op2val_0x10000__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[597], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x3__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[598], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xaaaaaaaa__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[599], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xaaaaaaab__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[600], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x5__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[601], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x33333333__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[602], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x66666666__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[603], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xb504__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[604], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x0__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[605], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[606], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x2__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[607], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x55555554__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[608], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xaaaaaaa9__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[609], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x4__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[610], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x33333332__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[611], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x66666665__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[612], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xb503__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[613], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xfffe__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[614], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x55555556__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[615], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xaaaaaaab__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[616], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x6__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[617], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x33333334__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[618], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x66666667__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[619], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0xb505__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[620], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x1__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[621], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333334__op2val_0x10000__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[622], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x3__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[623], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x55555555__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[624], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xaaaaaaaa__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[625], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x5__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[626], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x33333333__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[627], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x66666666__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[628], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xb504__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[629], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x0__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[630], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xffff__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[631], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x2__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[632], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x55555554__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[633], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xaaaaaaa9__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[634], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x4__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[635], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x33333332__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[636], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x66666665__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[637], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xb503__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[638], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xfffe__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[639], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x55555556__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[640], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xaaaaaaab__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[641], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x6__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[642], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x33333334__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[643], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x66666667__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[644], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0xb505__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[645], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x1__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[646], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666667__op2val_0x10000__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[647], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x3__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[648], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x55555555__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[649], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0xaaaaaaaa__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[650], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x5__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[651], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x33333333__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[652], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x66666666__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[653], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0xb504__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[654], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x0__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[655], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0xffff__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[656], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x2__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[657], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x55555554__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[658], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0xaaaaaaa9__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[659], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x4__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[660], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x33333332__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[661], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x66666665__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[662], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0xb503__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[663], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0xfffe__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[664], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x55555556__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[665], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0xaaaaaaab__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[666], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x6__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[667], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x33333334__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[668], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x66666667__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[669], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0xb505__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[670], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x1__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[671], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0xb505__op2val_0x10000__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[672], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x3__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[673], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x55555555__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[674], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0xaaaaaaaa__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[675], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x5__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[676], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x33333333__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[677], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x66666666__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[678], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0xb504__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[679], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x0__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[680], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0xffff__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[681], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x2__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[682], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x55555554__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[683], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0xaaaaaaa9__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[684], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x4__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[685], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x33333332__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[686], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x66666665__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[687], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0xb503__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[688], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0xfffe__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[689], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x55555556__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[690], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0xaaaaaaab__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[691], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x6__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[692], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x33333334__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[693], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x66666667__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[694], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0xb505__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[695], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x1__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[696], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x1__op2val_0x10000__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[697], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x3__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[698], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x55555555__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[699], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0xaaaaaaaa__immval_0x8__align_0) {
  ASSERT_EQ(getScalarWordData()[700], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x5__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[701], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x33333333__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[702], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x66666666__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[703], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0xb504__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[704], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x0__immval_0x80__align_0) {
  ASSERT_EQ(getScalarWordData()[705], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0xffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[706], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x2__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[707], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x55555554__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[708], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0xaaaaaaa9__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[709], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x4__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[710], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x33333332__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[711], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x66666665__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[712], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0xb503__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[713], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0xfffe__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[714], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x55555556__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[715], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0xaaaaaaab__immval_0x6__align_0) {
  ASSERT_EQ(getScalarWordData()[716], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x6__immval_0x100__align_0) {
  ASSERT_EQ(getScalarWordData()[717], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x33333334__immval_0xa__align_0) {
  ASSERT_EQ(getScalarWordData()[718], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x66666667__immval_0x20__align_0) {
  ASSERT_EQ(getScalarWordData()[719], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0xb505__immval_0x40__align_0) {
  ASSERT_EQ(getScalarWordData()[720], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x1__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[721], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x10000__op2val_0x10000__immval_0x200__align_0) {
  ASSERT_EQ(getScalarWordData()[722], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x6__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[723], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x33333334__immval_0x400__align_0) {
  ASSERT_EQ(getScalarWordData()[724], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0x66666667__immval_0x4__align_0) {
  ASSERT_EQ(getScalarWordData()[725], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x66666665__op2val_0xb505__immval_0x10__align_0) {
  ASSERT_EQ(getScalarWordData()[726], 3);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x20000000__op2val_0x8000__immval_0x556__align_0) {
  ASSERT_EQ(getScalarWordData()[727], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x40000000__op2val_0xfbffffff__immval_0x2__align_0) {
  ASSERT_EQ(getScalarWordData()[728], 1);
}


TEST_F(bltu_01, opcode__bltu__op1_x10__op2_x11__op1val_0x33333333__op2val_0x2__immval_0x0__align_0) {
  ASSERT_EQ(getScalarWordData()[729], 3);
}

