// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/lui-01.h"

namespace
{
class lui_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::lui_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::lui_01::generateCode().getCode(), scalarInitData);
        
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

std::vector<uint8_t> lui_01::s_ScalarData;
}


// Generate google tests

TEST_F(lui_01, opcode__lui___dest_x8__immval_0x0) {
  ASSERT_EQ(getScalarWordData()[4], 0);
}


TEST_F(lui_01, opcode__lui___dest_x5__immval_0x5) {
  ASSERT_EQ(getScalarWordData()[5], 20480);
}


TEST_F(lui_01, opcode__lui___dest_x19__immval_0xfffff) {
  ASSERT_EQ(getScalarWordData()[6], -4096);
}


TEST_F(lui_01, opcode__lui___dest_x13__immval_0x3) {
  ASSERT_EQ(getScalarWordData()[7], 12288);
}


TEST_F(lui_01, opcode__lui___dest_x0__immval_0x55555) {
  ASSERT_EQ(getScalarWordData()[8], 0);
}


TEST_F(lui_01, opcode__lui___dest_x4__immval_0xaaaaa) {
  ASSERT_EQ(getScalarWordData()[9], -1431658496);
}


TEST_F(lui_01, opcode__lui___dest_x22__immval_0x33333) {
  ASSERT_EQ(getScalarWordData()[10], 858992640);
}


TEST_F(lui_01, opcode__lui___dest_x26__immval_0x66666) {
  ASSERT_EQ(getScalarWordData()[11], 1717985280);
}


TEST_F(lui_01, opcode__lui___dest_x29__immval_0x2d4) {
  ASSERT_EQ(getScalarWordData()[12], 2965504);
}


TEST_F(lui_01, opcode__lui___dest_x3__immval_0x3ff) {
  ASSERT_EQ(getScalarWordData()[13], 4190208);
}


TEST_F(lui_01, opcode__lui___dest_x30__immval_0x2) {
  ASSERT_EQ(getScalarWordData()[14], 8192);
}


TEST_F(lui_01, opcode__lui___dest_x16__immval_0x55554) {
  ASSERT_EQ(getScalarWordData()[15], 1431650304);
}


TEST_F(lui_01, opcode__lui___dest_x2__immval_0xaaaa9) {
  ASSERT_EQ(getScalarWordData()[16], -1431662592);
}


TEST_F(lui_01, opcode__lui___dest_x24__immval_0x4) {
  ASSERT_EQ(getScalarWordData()[17], 16384);
}


TEST_F(lui_01, opcode__lui___dest_x27__immval_0x33332) {
  ASSERT_EQ(getScalarWordData()[18], 858988544);
}


TEST_F(lui_01, opcode__lui___dest_x20__immval_0x66665) {
  ASSERT_EQ(getScalarWordData()[19], 1717981184);
}


TEST_F(lui_01, opcode__lui___dest_x28__immval_0x2d3) {
  ASSERT_EQ(getScalarWordData()[20], 2961408);
}


TEST_F(lui_01, opcode__lui___dest_x6__immval_0x3fe) {
  ASSERT_EQ(getScalarWordData()[21], 4186112);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x55556) {
  ASSERT_EQ(getScalarWordData()[22], 1431658496);
}


TEST_F(lui_01, opcode__lui___dest_x12__immval_0xaaaab) {
  ASSERT_EQ(getScalarWordData()[23], -1431654400);
}


TEST_F(lui_01, opcode__lui___dest_x15__immval_0x6) {
  ASSERT_EQ(getScalarWordData()[24], 24576);
}


TEST_F(lui_01, opcode__lui___dest_x23__immval_0x33334) {
  ASSERT_EQ(getScalarWordData()[25], 858996736);
}


TEST_F(lui_01, opcode__lui___dest_x1__immval_0x66667) {
  ASSERT_EQ(getScalarWordData()[26], 1717989376);
}


TEST_F(lui_01, opcode__lui___dest_x18__immval_0x2d5) {
  ASSERT_EQ(getScalarWordData()[27], 2969600);
}


TEST_F(lui_01, opcode__lui___dest_x11__immval_0x1) {
  ASSERT_EQ(getScalarWordData()[28], 4096);
}


TEST_F(lui_01, opcode__lui___dest_x21__immval_0x400) {
  ASSERT_EQ(getScalarWordData()[29], 4194304);
}


TEST_F(lui_01, opcode__lui___dest_x17__immval_0x8) {
  ASSERT_EQ(getScalarWordData()[30], 32768);
}


TEST_F(lui_01, opcode__lui___dest_x25__immval_0x10) {
  ASSERT_EQ(getScalarWordData()[31], 65536);
}


TEST_F(lui_01, opcode__lui___dest_x7__immval_0x20) {
  ASSERT_EQ(getScalarWordData()[32], 131072);
}


TEST_F(lui_01, opcode__lui___dest_x14__immval_0x40) {
  ASSERT_EQ(getScalarWordData()[33], 262144);
}


TEST_F(lui_01, opcode__lui___dest_x9__immval_0x80) {
  ASSERT_EQ(getScalarWordData()[34], 524288);
}


TEST_F(lui_01, opcode__lui___dest_x31__immval_0xeffff) {
  ASSERT_EQ(getScalarWordData()[35], -268439552);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xdffff) {
  ASSERT_EQ(getScalarWordData()[36], -536875008);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xbffff) {
  ASSERT_EQ(getScalarWordData()[37], -1073745920);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x7ffff) {
  ASSERT_EQ(getScalarWordData()[38], 2147479552);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x100) {
  ASSERT_EQ(getScalarWordData()[39], 1048576);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x200) {
  ASSERT_EQ(getScalarWordData()[40], 2097152);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x800) {
  ASSERT_EQ(getScalarWordData()[41], 8388608);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x1000) {
  ASSERT_EQ(getScalarWordData()[42], 16777216);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x2000) {
  ASSERT_EQ(getScalarWordData()[43], 33554432);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x4000) {
  ASSERT_EQ(getScalarWordData()[44], 67108864);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x8000) {
  ASSERT_EQ(getScalarWordData()[45], 134217728);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x10000) {
  ASSERT_EQ(getScalarWordData()[46], 268435456);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x20000) {
  ASSERT_EQ(getScalarWordData()[47], 536870912);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x40000) {
  ASSERT_EQ(getScalarWordData()[48], 1073741824);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x80000) {
  ASSERT_EQ(getScalarWordData()[49], -2147483648);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xffffe) {
  ASSERT_EQ(getScalarWordData()[50], -8192);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xffffd) {
  ASSERT_EQ(getScalarWordData()[51], -12288);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xffffb) {
  ASSERT_EQ(getScalarWordData()[52], -20480);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xffff7) {
  ASSERT_EQ(getScalarWordData()[53], -36864);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xfffef) {
  ASSERT_EQ(getScalarWordData()[54], -69632);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xfffdf) {
  ASSERT_EQ(getScalarWordData()[55], -135168);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xfffbf) {
  ASSERT_EQ(getScalarWordData()[56], -266240);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xfff7f) {
  ASSERT_EQ(getScalarWordData()[57], -528384);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xffeff) {
  ASSERT_EQ(getScalarWordData()[58], -1052672);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xffdff) {
  ASSERT_EQ(getScalarWordData()[59], -2101248);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xffbff) {
  ASSERT_EQ(getScalarWordData()[60], -4198400);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xff7ff) {
  ASSERT_EQ(getScalarWordData()[61], -8392704);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xfefff) {
  ASSERT_EQ(getScalarWordData()[62], -16781312);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xfdfff) {
  ASSERT_EQ(getScalarWordData()[63], -33558528);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xfbfff) {
  ASSERT_EQ(getScalarWordData()[64], -67112960);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0xf7fff) {
  ASSERT_EQ(getScalarWordData()[65], -134221824);
}


TEST_F(lui_01, opcode__lui___dest_x10__immval_0x55555) {
  ASSERT_EQ(getScalarWordData()[66], 1431654400);
}

