// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/ctz-01.h"

namespace
{
class ctz_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::ctz_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::ctz_01::generateCode(true), scalarInitData);
        
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

std::vector<uint8_t> ctz_01::s_ScalarData;
}


// Generate google tests

TEST_F(ctz_01, opcode__ctz___op1_x11__dest_x11__op1val_0x55555555_) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x6__dest_x9__op1val_0xaaaaaaaa_) {
  ASSERT_EQ(getScalarWordData()[6], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x8__dest_x5__op1val_0x1_) {
  ASSERT_EQ(getScalarWordData()[7], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x3__dest_x8__op1val_0x2_) {
  ASSERT_EQ(getScalarWordData()[8], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x7__dest_x1__op1val_0x4_) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x4__dest_x3__op1val_0x8_) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x5__dest_x7__op1val_0xfffffffe_) {
  ASSERT_EQ(getScalarWordData()[11], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x13__dest_x4__op1val_0xfffffffd_) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x9__dest_x6__op1val_0xfffffffb_) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x0__op1val_0xfffffff7_) {
  ASSERT_EQ(getScalarWordData()[14], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x0__dest_x13__op1val_0x0_) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x15__dest_x14__op1val_0xffffffdf_) {
  ASSERT_EQ(getScalarWordData()[16], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x12__dest_x10__op1val_0xffffffbf_) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x1__dest_x15__op1val_0xffffff7f_) {
  ASSERT_EQ(getScalarWordData()[18], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x2__dest_x12__op1val_0xfffffeff_) {
  ASSERT_EQ(getScalarWordData()[19], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x10__dest_x2__op1val_0xfffffdff_) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xfffffbff_) {
  ASSERT_EQ(getScalarWordData()[21], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xfffff7ff_) {
  ASSERT_EQ(getScalarWordData()[22], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xffffefff_) {
  ASSERT_EQ(getScalarWordData()[23], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xffffdfff_) {
  ASSERT_EQ(getScalarWordData()[24], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xffffbfff_) {
  ASSERT_EQ(getScalarWordData()[25], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xffff7fff_) {
  ASSERT_EQ(getScalarWordData()[26], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xfffeffff_) {
  ASSERT_EQ(getScalarWordData()[27], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xfffdffff_) {
  ASSERT_EQ(getScalarWordData()[28], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xfffbffff_) {
  ASSERT_EQ(getScalarWordData()[29], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xfff7ffff_) {
  ASSERT_EQ(getScalarWordData()[30], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xffefffff_) {
  ASSERT_EQ(getScalarWordData()[31], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xffdfffff_) {
  ASSERT_EQ(getScalarWordData()[32], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xffbfffff_) {
  ASSERT_EQ(getScalarWordData()[33], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xff7fffff_) {
  ASSERT_EQ(getScalarWordData()[34], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xfeffffff_) {
  ASSERT_EQ(getScalarWordData()[35], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xfdffffff_) {
  ASSERT_EQ(getScalarWordData()[36], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xfbffffff_) {
  ASSERT_EQ(getScalarWordData()[37], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xf7ffffff_) {
  ASSERT_EQ(getScalarWordData()[38], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xefffffff_) {
  ASSERT_EQ(getScalarWordData()[39], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x40000000_) {
  ASSERT_EQ(getScalarWordData()[40], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x80000000_) {
  ASSERT_EQ(getScalarWordData()[41], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xdfffffff_) {
  ASSERT_EQ(getScalarWordData()[42], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xbfffffff_) {
  ASSERT_EQ(getScalarWordData()[43], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x7fffffff_) {
  ASSERT_EQ(getScalarWordData()[44], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x10_) {
  ASSERT_EQ(getScalarWordData()[45], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x20_) {
  ASSERT_EQ(getScalarWordData()[46], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x40_) {
  ASSERT_EQ(getScalarWordData()[47], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x80_) {
  ASSERT_EQ(getScalarWordData()[48], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x100_) {
  ASSERT_EQ(getScalarWordData()[49], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x200_) {
  ASSERT_EQ(getScalarWordData()[50], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x400_) {
  ASSERT_EQ(getScalarWordData()[51], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x800_) {
  ASSERT_EQ(getScalarWordData()[52], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x1000_) {
  ASSERT_EQ(getScalarWordData()[53], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x2000_) {
  ASSERT_EQ(getScalarWordData()[54], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x4000_) {
  ASSERT_EQ(getScalarWordData()[55], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x8000_) {
  ASSERT_EQ(getScalarWordData()[56], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x10000_) {
  ASSERT_EQ(getScalarWordData()[57], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x20000_) {
  ASSERT_EQ(getScalarWordData()[58], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x40000_) {
  ASSERT_EQ(getScalarWordData()[59], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x80000_) {
  ASSERT_EQ(getScalarWordData()[60], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x100000_) {
  ASSERT_EQ(getScalarWordData()[61], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x200000_) {
  ASSERT_EQ(getScalarWordData()[62], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x400000_) {
  ASSERT_EQ(getScalarWordData()[63], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x800000_) {
  ASSERT_EQ(getScalarWordData()[64], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x1000000_) {
  ASSERT_EQ(getScalarWordData()[65], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x2000000_) {
  ASSERT_EQ(getScalarWordData()[66], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x4000000_) {
  ASSERT_EQ(getScalarWordData()[67], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x8000000_) {
  ASSERT_EQ(getScalarWordData()[68], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x10000000_) {
  ASSERT_EQ(getScalarWordData()[69], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0x20000000_) {
  ASSERT_EQ(getScalarWordData()[70], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xfffffff7_) {
  ASSERT_EQ(getScalarWordData()[71], 0);
}


TEST_F(ctz_01, opcode__ctz___op1_x14__dest_x15__op1val_0xffffffef_) {
  ASSERT_EQ(getScalarWordData()[72], 0);
}

