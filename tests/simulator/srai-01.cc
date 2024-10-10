// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/srai-01.h"

namespace
{
class srai_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::srai_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::srai_01::generateCode(true), scalarInitData);
        
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

std::vector<uint8_t> srai_01::s_ScalarData;
}


// Generate google tests

TEST_F(srai_01, opcode__srai___op1_x0__dest_x2__op1val_0x0___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(srai_01, opcode__srai___op1_x14__dest_x14__op1val_0x7fffffff___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[6], 524287);
}


TEST_F(srai_01, opcode__srai___op1_x3__dest_x11__op1val__0x40000001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[7], -8388609);
}


TEST_F(srai_01, opcode__srai___op1_x15__dest_x4__op1val__0x10000001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[8], -268435457);
}


TEST_F(srai_01, opcode__srai___op1_x7__dest_x10__op1val__0x8000001___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[9], -65);
}


TEST_F(srai_01, opcode__srai___op1_x8__dest_x6__op1val__0x4000001___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[10], -65537);
}


TEST_F(srai_01, opcode__srai___op1_x9__dest_x13__op1val__0x2000001___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[11], -8388609);
}


TEST_F(srai_01, opcode__srai___op1_x5__dest_x0__op1val__0x1000001___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x8__op1val__0x800001___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[13], -1);
}


TEST_F(srai_01, opcode__srai___op1_x13__dest_x5__op1val__0x400001___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[14], -8193);
}


TEST_F(srai_01, opcode__srai___op1_x4__dest_x12__op1val__0x200001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[15], -16385);
}


TEST_F(srai_01, opcode__srai___op1_x12__dest_x7__op1val__0x100001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[16], -8193);
}


TEST_F(srai_01, opcode__srai___op1_x2__dest_x15__op1val__0x80001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[17], -4097);
}


TEST_F(srai_01, opcode__srai___op1_x1__dest_x9__op1val__0x40001___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[18], -1025);
}


TEST_F(srai_01, opcode__srai___op1_x6__dest_x1__op1val__0x20001___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[19], -1);
}


TEST_F(srai_01, opcode__srai___op1_x11__dest_x3__op1val__0x10001___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[20], -4097);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x8001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[21], -257);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x4001___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[22], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x2001___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[23], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x1001___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[24], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x801___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[25], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x401___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[26], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x201___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[27], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x101___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[28], -129);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x81___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[29], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x41___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[30], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x21___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[31], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x11___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[32], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x9___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[33], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x5___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[34], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x3___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[35], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x2___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[36], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x1000___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[37], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x55555556___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[38], -11);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x0___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[39], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x80000000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[40], -536870912);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x40000000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[41], 268435456);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x20000000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[42], 33554432);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x10000000___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[43], 16384);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x8000000___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[44], 64);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x4000000___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[45], 16384);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x2000000___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[46], 65536);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x1000000___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[47], 1024);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x800000___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[48], 4096);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x400000___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[49], 32);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x200000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[50], 131072);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x100000___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[51], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x80000___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[52], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x40000___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[53], 2);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x20000___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[54], 256);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x10000___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[55], 1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x8000___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[56], 32);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x4000___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[57], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x2000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[58], 256);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x800___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[59], 32);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x400___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[60], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x200___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[61], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x100___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[62], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x80___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[63], 32);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x40___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[64], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x20___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[65], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x10___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[66], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x8___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[67], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x4___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[68], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x2___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[69], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x1___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[70], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0xb505___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[71], 22);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0xb503___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[72], -2);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x66666667___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[73], 6710886);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x33333334___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[74], 209715);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x6___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[75], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x55555555___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[76], -699051);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x3___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[77], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x55555555___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[78], 1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[79], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[80], 2);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[81], 357913941);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0xb503___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[82], 23169);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x66666665___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[83], 26214);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x33333332___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[84], 3276);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x55555554___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[85], 10);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0xb504___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[86], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0xb504___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[87], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x66666666___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[88], 104857);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x33333333___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[89], 3276);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x20000001___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[90], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x1000001___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[91], -65);
}

