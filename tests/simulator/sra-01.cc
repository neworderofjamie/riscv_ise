// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/sra-01.h"

namespace
{
class sra_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::sra_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::sra_01::generateCode(true), scalarInitData);
        
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

std::vector<uint8_t> sra_01::s_ScalarData;
}


// Generate google tests

TEST_F(sra_01, opcode__sra___op1_x16__op2_x27__dest_x27__op1val__0x80000000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[5], -8388608);
}


TEST_F(sra_01, opcode__sra___op1_x12__op2_x12__dest_x16__op1val_0x2000000___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[6], 33554432);
}


TEST_F(sra_01, opcode__sra___op1_x1__op2_x1__dest_x1__op1val__0x801___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[7], -1);
}


TEST_F(sra_01, opcode__sra___op1_x13__op2_x19__dest_x13__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[8], 858993459);
}


TEST_F(sra_01, opcode__sra___op1_x28__op2_x2__dest_x8__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(sra_01, opcode__sra___op1_x26__op2_x31__dest_x19__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(sra_01, opcode__sra___op1_x14__op2_x28__dest_x29__op1val_0x7fffffff___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[11], 8191);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x26__dest_x12__op1val_0x1___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(sra_01, opcode__sra___op1_x30__op2_x16__dest_x15__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(sra_01, opcode__sra___op1_x24__op2_x0__dest_x6__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[14], 4);
}


TEST_F(sra_01, opcode__sra___op1_x21__op2_x15__dest_x3__op1val_0x8___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(sra_01, opcode__sra___op1_x15__op2_x30__dest_x10__op1val_0x10___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[16], 16);
}


TEST_F(sra_01, opcode__sra___op1_x18__op2_x4__dest_x22__op1val_0x20___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[17], 4);
}


TEST_F(sra_01, opcode__sra___op1_x19__op2_x17__dest_x11__op1val_0x40___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[18], 0);
}


TEST_F(sra_01, opcode__sra___op1_x5__op2_x14__dest_x0__op1val_0x80___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[19], 0);
}


TEST_F(sra_01, opcode__sra___op1_x6__op2_x24__dest_x5__op1val_0x100___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[20], 16);
}


TEST_F(sra_01, opcode__sra___op1_x7__op2_x18__dest_x31__op1val_0x200___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[21], 0);
}


TEST_F(sra_01, opcode__sra___op1_x31__op2_x21__dest_x30__op1val_0x400___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[22], 0);
}


TEST_F(sra_01, opcode__sra___op1_x4__op2_x11__dest_x2__op1val_0x800___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[23], 0);
}


TEST_F(sra_01, opcode__sra___op1_x22__op2_x25__dest_x9__op1val_0x1000___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[24], 1024);
}


TEST_F(sra_01, opcode__sra___op1_x23__op2_x13__dest_x14__op1val_0x2000___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[25], 0);
}


TEST_F(sra_01, opcode__sra___op1_x8__op2_x29__dest_x28__op1val_0x4000___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[26], 16);
}


TEST_F(sra_01, opcode__sra___op1_x17__op2_x8__dest_x26__op1val_0x8000___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[27], 0);
}


TEST_F(sra_01, opcode__sra___op1_x29__op2_x3__dest_x23__op1val_0x10000___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[28], 1);
}


TEST_F(sra_01, opcode__sra___op1_x9__op2_x5__dest_x7__op1val_0x20000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[29], 131072);
}


TEST_F(sra_01, opcode__sra___op1_x20__op2_x7__dest_x24__op1val_0x40000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[30], 1024);
}


TEST_F(sra_01, opcode__sra___op1_x25__op2_x10__dest_x17__op1val_0x80000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[31], 2048);
}


TEST_F(sra_01, opcode__sra___op1_x27__op2_x22__dest_x20__op1val_0x100000___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[32], 0);
}


TEST_F(sra_01, opcode__sra___op1_x3__op2_x6__dest_x25__op1val_0x200000___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[33], 1024);
}


TEST_F(sra_01, opcode__sra___op1_x0__op2_x23__dest_x4__op1val_0x0___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[34], 0);
}


TEST_F(sra_01, opcode__sra___op1_x2__op2_x20__dest_x18__op1val_0x800000___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[35], 0);
}


TEST_F(sra_01, opcode__sra___op1_x11__op2_x9__dest_x21__op1val_0x1000000___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[36], 8192);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[37], 16384);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[38], 8388608);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[39], 131072);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[40], 524288);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[41], 32768);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[42], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[43], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[44], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[45], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[46], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[47], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[48], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[49], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[50], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[51], -257);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[52], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[53], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[54], -33);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[55], -2);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[56], -9);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[57], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[58], -129);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[59], -129);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[60], -524289);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[61], -17);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[62], -4097);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[63], -1);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[64], -32769);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[65], -131073);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[66], -16777217);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[67], -4194305);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[68], -33554433);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[69], 699050);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[70], -2731);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[71], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[72], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[73], 3276);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[74], -23170);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[75], 181);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[76], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[77], 2);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[78], 349525);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[79], -9);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[80], 6710886);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[81], -524289);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[82], 858993458);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[83], 204);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[84], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[85], 44739242);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[86], -715827883);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[87], -17);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[88], 1717986919);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[89], -6);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[90], 128);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[91], -2049);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[92], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[93], 0);
}


TEST_F(sra_01, opcode__sra___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[94], 16384);
}

