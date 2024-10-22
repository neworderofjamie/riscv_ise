// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/srl-01.h"

namespace
{
class srl_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::srl_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::srl_01::generateCode(true), scalarInitData);
        
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

std::vector<uint8_t> srl_01::s_ScalarData;
}


// Generate google tests

TEST_F(srl_01, opcode__srl___op1_x26__op2_x11__dest_x11__op1val__0x400001___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[5], 130943);
}


TEST_F(srl_01, opcode__srl___op1_x31__op2_x31__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[6], 341);
}


TEST_F(srl_01, opcode__srl___op1_x7__op2_x7__dest_x7__op1val__0x1___op2val__0x1) {
  ASSERT_EQ(getScalarWordData()[7], 1);
}


TEST_F(srl_01, opcode__srl___op1_x18__op2_x12__dest_x18__op1val_0x100___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[8], 256);
}


TEST_F(srl_01, opcode__srl___op1_x14__op2_x3__dest_x8__op1val_0x9___op2val_0x9) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(srl_01, opcode__srl___op1_x21__op2_x22__dest_x20__op1val__0x80000000___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[10], 524288);
}


TEST_F(srl_01, opcode__srl___op1_x4__op2_x17__dest_x30__op1val_0x0___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[11], 0);
}


TEST_F(srl_01, opcode__srl___op1_x1__op2_x4__dest_x6__op1val_0x7fffffff___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[12], 1);
}


TEST_F(srl_01, opcode__srl___op1_x0__op2_x21__dest_x15__op1val_0x0___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(srl_01, opcode__srl___op1_x28__op2_x23__dest_x5__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[14], 0);
}


TEST_F(srl_01, opcode__srl___op1_x9__op2_x30__dest_x4__op1val_0x4___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(srl_01, opcode__srl___op1_x13__op2_x29__dest_x10__op1val_0x8___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[16], 2);
}


TEST_F(srl_01, opcode__srl___op1_x2__op2_x16__dest_x25__op1val_0x10___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(srl_01, opcode__srl___op1_x11__op2_x28__dest_x19__op1val_0x20___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[18], 0);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x6__dest_x23__op1val_0x40___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[19], 0);
}


TEST_F(srl_01, opcode__srl___op1_x3__op2_x1__dest_x0__op1val_0x80___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(srl_01, opcode__srl___op1_x6__op2_x8__dest_x14__op1val_0x200___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[21], 0);
}


TEST_F(srl_01, opcode__srl___op1_x20__op2_x27__dest_x9__op1val_0x400___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[22], 0);
}


TEST_F(srl_01, opcode__srl___op1_x30__op2_x18__dest_x16__op1val_0x800___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[23], 0);
}


TEST_F(srl_01, opcode__srl___op1_x22__op2_x10__dest_x24__op1val_0x1000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[24], 16);
}


TEST_F(srl_01, opcode__srl___op1_x15__op2_x24__dest_x2__op1val_0x2000___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[25], 2);
}


TEST_F(srl_01, opcode__srl___op1_x17__op2_x9__dest_x21__op1val_0x4000___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[26], 128);
}


TEST_F(srl_01, opcode__srl___op1_x12__op2_x20__dest_x27__op1val_0x8000___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[27], 0);
}


TEST_F(srl_01, opcode__srl___op1_x27__op2_x5__dest_x26__op1val_0x10000___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[28], 8192);
}


TEST_F(srl_01, opcode__srl___op1_x16__op2_x13__dest_x17__op1val_0x20000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[29], 512);
}


TEST_F(srl_01, opcode__srl___op1_x23__op2_x15__dest_x28__op1val_0x40000___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[30], 4);
}


TEST_F(srl_01, opcode__srl___op1_x8__op2_x26__dest_x1__op1val_0x80000___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[31], 16384);
}


TEST_F(srl_01, opcode__srl___op1_x24__op2_x2__dest_x29__op1val_0x100000___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[32], 0);
}


TEST_F(srl_01, opcode__srl___op1_x29__op2_x14__dest_x31__op1val_0x200000___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[33], 32);
}


TEST_F(srl_01, opcode__srl___op1_x5__op2_x19__dest_x22__op1val_0x400000___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[34], 65536);
}


TEST_F(srl_01, opcode__srl___op1_x19__op2_x0__dest_x3__op1val_0x800000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[35], 8388608);
}


TEST_F(srl_01, opcode__srl___op1_x25__op2_x4__dest_x8__op1val_0x1000000___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[36], 0);
}


TEST_F(srl_01, opcode__srl___op1_x6__op2_x25__dest_x30__op1val_0x2000000___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[37], 0);
}


TEST_F(srl_01, opcode__srl___op1_x12__op2_x9__dest_x13__op1val_0x4000000___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[38], 0);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[39], 524288);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[40], 65536);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[41], 4);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[42], 16777216);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[43], 2097151);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[44], 8191);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[45], 1073741822);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[46], 8191);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[47], 511);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[48], 131071);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[49], 262143);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[50], 67108861);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[51], 262143);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[52], 524287);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[53], 31);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[54], 31);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[55], 524287);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[56], 1048573);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[57], 65535);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[58], 3);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[59], 16383);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[60], 1);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[61], 131063);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[62], 31);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[63], 67092479);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[64], 16351);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[65], 32639);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[66], 1040383);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[67], 503);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[68], 2080374783);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val_0x9) {
  ASSERT_EQ(getScalarWordData()[69], 7864319);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[70], 458751);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[71], 12582911);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[72], 699050);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[73], 5);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[74], 0);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[75], 0);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[76], 838860);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[77], 3276);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[78], 0);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[79], 0);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[80], 2031);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[81], 1048564);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[82], 0);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[83], 682);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[84], 262015);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[85], 858993458);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[86], 214748364);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[87], 23169);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[88], 1398101);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[89], 6);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[90], 26843545);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[91], 67108139);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[92], 5592405);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val__0x1___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[93], 4294967295);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[94], 0);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[95], 2);
}


TEST_F(srl_01, opcode__srl___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[96], 4194304);
}

