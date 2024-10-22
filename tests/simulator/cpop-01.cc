// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/cpop-01.h"

namespace
{
class cpop_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::cpop_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::cpop_01::generateCode(true), scalarInitData);
        
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

std::vector<uint8_t> cpop_01::s_ScalarData;
}


// Generate google tests

TEST_F(cpop_01, opcode__cpop___op1_x31__dest_x31__op1val_0x55555555_) {
  ASSERT_EQ(getScalarWordData()[5], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x29__dest_x30__op1val_0x7fffffff_) {
  ASSERT_EQ(getScalarWordData()[6], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x29__op1val_0xbfffffff_) {
  ASSERT_EQ(getScalarWordData()[7], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x27__dest_x28__op1val_0xdfffffff_) {
  ASSERT_EQ(getScalarWordData()[8], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x28__dest_x27__op1val_0xefffffff_) {
  ASSERT_EQ(getScalarWordData()[9], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x25__dest_x26__op1val_0xf7ffffff_) {
  ASSERT_EQ(getScalarWordData()[10], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x26__dest_x25__op1val_0xfbffffff_) {
  ASSERT_EQ(getScalarWordData()[11], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x23__dest_x24__op1val_0xfdffffff_) {
  ASSERT_EQ(getScalarWordData()[12], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x24__dest_x23__op1val_0xfeffffff_) {
  ASSERT_EQ(getScalarWordData()[13], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x21__dest_x22__op1val_0xff7fffff_) {
  ASSERT_EQ(getScalarWordData()[14], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x22__dest_x21__op1val_0xffbfffff_) {
  ASSERT_EQ(getScalarWordData()[15], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x19__dest_x20__op1val_0xffdfffff_) {
  ASSERT_EQ(getScalarWordData()[16], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x20__dest_x19__op1val_0xffefffff_) {
  ASSERT_EQ(getScalarWordData()[17], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x17__dest_x18__op1val_0xfff7ffff_) {
  ASSERT_EQ(getScalarWordData()[18], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x18__dest_x17__op1val_0xfffbffff_) {
  ASSERT_EQ(getScalarWordData()[19], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x15__dest_x16__op1val_0xfffdffff_) {
  ASSERT_EQ(getScalarWordData()[20], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x16__dest_x15__op1val_0xfffeffff_) {
  ASSERT_EQ(getScalarWordData()[21], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x13__dest_x14__op1val_0xffff7fff_) {
  ASSERT_EQ(getScalarWordData()[22], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x14__dest_x13__op1val_0xffffbfff_) {
  ASSERT_EQ(getScalarWordData()[23], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x11__dest_x12__op1val_0xffffdfff_) {
  ASSERT_EQ(getScalarWordData()[24], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x12__dest_x11__op1val_0xffffefff_) {
  ASSERT_EQ(getScalarWordData()[25], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x9__dest_x10__op1val_0xfffff7ff_) {
  ASSERT_EQ(getScalarWordData()[26], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x10__dest_x9__op1val_0xfffffbff_) {
  ASSERT_EQ(getScalarWordData()[27], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x7__dest_x8__op1val_0xfffffdff_) {
  ASSERT_EQ(getScalarWordData()[28], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x8__dest_x7__op1val_0xfffffeff_) {
  ASSERT_EQ(getScalarWordData()[29], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x5__dest_x6__op1val_0xffffff7f_) {
  ASSERT_EQ(getScalarWordData()[30], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x6__dest_x5__op1val_0xffffffbf_) {
  ASSERT_EQ(getScalarWordData()[31], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x3__dest_x4__op1val_0xffffffdf_) {
  ASSERT_EQ(getScalarWordData()[32], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x4__dest_x3__op1val_0xffffffef_) {
  ASSERT_EQ(getScalarWordData()[33], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x1__dest_x2__op1val_0xfffffff7_) {
  ASSERT_EQ(getScalarWordData()[34], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x2__dest_x1__op1val_0xfffffffb_) {
  ASSERT_EQ(getScalarWordData()[35], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x0__dest_x31__op1val_0x0_) {
  ASSERT_EQ(getScalarWordData()[36], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x31__dest_x0__op1val_0xfffffffe_) {
  ASSERT_EQ(getScalarWordData()[37], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x80000000_) {
  ASSERT_EQ(getScalarWordData()[38], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x40000000_) {
  ASSERT_EQ(getScalarWordData()[39], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x1_) {
  ASSERT_EQ(getScalarWordData()[40], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0xaaaaaaaa_) {
  ASSERT_EQ(getScalarWordData()[41], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x20000000_) {
  ASSERT_EQ(getScalarWordData()[42], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x10000000_) {
  ASSERT_EQ(getScalarWordData()[43], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x8000000_) {
  ASSERT_EQ(getScalarWordData()[44], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x4000000_) {
  ASSERT_EQ(getScalarWordData()[45], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x2000000_) {
  ASSERT_EQ(getScalarWordData()[46], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x1000000_) {
  ASSERT_EQ(getScalarWordData()[47], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x800000_) {
  ASSERT_EQ(getScalarWordData()[48], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x400000_) {
  ASSERT_EQ(getScalarWordData()[49], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x200000_) {
  ASSERT_EQ(getScalarWordData()[50], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x100000_) {
  ASSERT_EQ(getScalarWordData()[51], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x80000_) {
  ASSERT_EQ(getScalarWordData()[52], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x40000_) {
  ASSERT_EQ(getScalarWordData()[53], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x20000_) {
  ASSERT_EQ(getScalarWordData()[54], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x10000_) {
  ASSERT_EQ(getScalarWordData()[55], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x8000_) {
  ASSERT_EQ(getScalarWordData()[56], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x4000_) {
  ASSERT_EQ(getScalarWordData()[57], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x2000_) {
  ASSERT_EQ(getScalarWordData()[58], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x1000_) {
  ASSERT_EQ(getScalarWordData()[59], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x800_) {
  ASSERT_EQ(getScalarWordData()[60], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x400_) {
  ASSERT_EQ(getScalarWordData()[61], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x200_) {
  ASSERT_EQ(getScalarWordData()[62], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x100_) {
  ASSERT_EQ(getScalarWordData()[63], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x80_) {
  ASSERT_EQ(getScalarWordData()[64], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x40_) {
  ASSERT_EQ(getScalarWordData()[65], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x20_) {
  ASSERT_EQ(getScalarWordData()[66], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x10_) {
  ASSERT_EQ(getScalarWordData()[67], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x8_) {
  ASSERT_EQ(getScalarWordData()[68], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x4_) {
  ASSERT_EQ(getScalarWordData()[69], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0x2_) {
  ASSERT_EQ(getScalarWordData()[70], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0xfffffffd_) {
  ASSERT_EQ(getScalarWordData()[71], (uint32_t)0);
}


TEST_F(cpop_01, opcode__cpop___op1_x30__dest_x31__op1val_0xfffffffe_) {
  ASSERT_EQ(getScalarWordData()[72], (uint32_t)0);
}

