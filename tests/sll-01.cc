// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/sll-01.h"

namespace
{
class sll_01Fixture : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = sll_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(sll_01::generateCode().getCode(), scalarInitData);
        
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

std::vector<uint8_t> sll_01Fixture::s_ScalarData;
}


// Generate google tests

TEST_F(sll_01Fixture, opcode__sll___op1_x16__op2_x28__dest_x28__op1val__0x81___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[4], 4294835200);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x21__op2_x21__dest_x0__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x18__op2_x18__dest_x18__op1val__0x8001___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[6], 2147483648);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x5__op2_x13__dest_x5__op1val_0x7___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[7], 7);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x22__op2_x12__dest_x23__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[8], 384);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x19__op2_x0__dest_x6__op1val__0x80000000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[9], 2147483648);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x25__op2_x24__dest_x13__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x12__op2_x26__dest_x16__op1val_0x7fffffff___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[11], 4292870144);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x6__op2_x14__dest_x20__op1val_0x1___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[12], 16);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x14__op2_x1__dest_x22__op1val_0x2___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x29__op2_x7__dest_x21__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[14], 16);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x31__op2_x10__dest_x4__op1val_0x8___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x17__op2_x20__dest_x7__op1val_0x10___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[16], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x20__op2_x11__dest_x12__op1val_0x20___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[17], 268435456);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x11__op2_x22__dest_x3__op1val_0x40___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[18], 8388608);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x0__op2_x30__dest_x24__op1val_0x0___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[19], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x3__op2_x31__dest_x8__op1val_0x100___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x27__op2_x17__dest_x10__op1val_0x200___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[21], 512);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x19__dest_x11__op1val_0x400___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[22], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x8__op2_x27__dest_x1__op1val_0x800___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[23], 131072);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x28__op2_x9__dest_x25__op1val_0x1000___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[24], 268435456);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x2__op2_x15__dest_x27__op1val_0x2000___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[25], 67108864);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x24__op2_x29__dest_x26__op1val_0x4000___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[26], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x1__op2_x23__dest_x9__op1val_0x8000___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[27], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x23__op2_x16__dest_x14__op1val_0x10000___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[28], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x4__op2_x3__dest_x17__op1val_0x20000___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[29], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x7__op2_x4__dest_x15__op1val_0x40000___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[30], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x9__op2_x2__dest_x29__op1val_0x80000___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[31], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x15__op2_x8__dest_x31__op1val_0x100000___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[32], 2147483648);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x26__op2_x6__dest_x30__op1val_0x200000___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[33], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x30__op2_x5__dest_x19__op1val_0x400000___op2val_0x9) {
  ASSERT_EQ(getScalarWordData()[34], 2147483648);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x13__op2_x25__dest_x2__op1val_0x800000___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[35], 134217728);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[36], 67108864);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[37], 536870912);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[38], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[39], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[40], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[41], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[42], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[43], 3221225472);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[44], 1073741824);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[45], 4284481536);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[46], 4290248704);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[47], 4294949888);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[48], 4294958848);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[49], 4294963136);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[50], 4294965240);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[51], 4294835968);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[52], 4294965246);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[53], 3221225472);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[54], 4290771968);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[55], 4026499072);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[56], 4292870144);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[57], 4293918704);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[58], 4294836224);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[59], 4294705151);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[60], 4227858424);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[61], 4278190079);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[62], 4227858430);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[63], 4294443008);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[64], 3221225472);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[65], 4294443008);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[66], 4294705152);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[67], 4294967264);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[68], 1431654400);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[69], 2863311520);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[70], 402653184);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[71], 858993459);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[72], 858992640);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[73], 4293484416);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[74], 671088640);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[75], 3892314112);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[76], 3036938240);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[77], 2863311528);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[78], 4286578680);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[79], 3758096384);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[80], 3435973832);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[81], 2576980372);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[82], 94902272);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[83], 2863311532);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[84], 1431656448);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[85], 4294836224);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[86], 858993472);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[87], 4278190072);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[88], 4292870144);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[89], 40960);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[90], 4294934527);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[91], 0);
}


TEST_F(sll_01Fixture, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[92], 8388608);
}

