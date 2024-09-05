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
        RISCV riscV(ArchTest::srai_01::generateCode().getCode(), scalarInitData);
        
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

TEST_F(srai_01, opcode__srai___op1_x31__dest_x25__op1val__0x9___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[4], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x10__op1val_0x5___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[5], 2);
}


TEST_F(srai_01, opcode__srai___op1_x8__dest_x28__op1val__0x1000001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[6], -16777217);
}


TEST_F(srai_01, opcode__srai___op1_x17__dest_x5__op1val_0x100000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[7], 1048576);
}


TEST_F(srai_01, opcode__srai___op1_x23__dest_x27__op1val__0x20001___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[8], -1);
}


TEST_F(srai_01, opcode__srai___op1_x13__dest_x20__op1val_0x1___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(srai_01, opcode__srai___op1_x22__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(srai_01, opcode__srai___op1_x7__dest_x30__op1val__0x80000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[11], -2147483648);
}


TEST_F(srai_01, opcode__srai___op1_x18__dest_x14__op1val_0x0___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(srai_01, opcode__srai___op1_x3__dest_x19__op1val_0x7fffffff___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(srai_01, opcode__srai___op1_x25__dest_x29__op1val_0x2___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[14], 0);
}


TEST_F(srai_01, opcode__srai___op1_x30__dest_x3__op1val_0x8___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(srai_01, opcode__srai___op1_x2__dest_x22__op1val_0x10___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[16], 0);
}


TEST_F(srai_01, opcode__srai___op1_x12__dest_x2__op1val_0x20___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(srai_01, opcode__srai___op1_x1__dest_x12__op1val_0x40___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[18], 0);
}


TEST_F(srai_01, opcode__srai___op1_x20__dest_x24__op1val_0x80___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[19], 0);
}


TEST_F(srai_01, opcode__srai___op1_x11__dest_x0__op1val_0x100___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(srai_01, opcode__srai___op1_x26__dest_x8__op1val_0x200___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[21], 1);
}


TEST_F(srai_01, opcode__srai___op1_x9__dest_x17__op1val_0x400___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[22], 0);
}


TEST_F(srai_01, opcode__srai___op1_x16__dest_x23__op1val_0x800___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[23], 0);
}


TEST_F(srai_01, opcode__srai___op1_x14__dest_x13__op1val_0x1000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[24], 128);
}


TEST_F(srai_01, opcode__srai___op1_x21__dest_x7__op1val_0x2000___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[25], 4096);
}


TEST_F(srai_01, opcode__srai___op1_x5__dest_x18__op1val_0x4000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[26], 512);
}


TEST_F(srai_01, opcode__srai___op1_x0__dest_x26__op1val_0x0___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[27], 0);
}


TEST_F(srai_01, opcode__srai___op1_x24__dest_x4__op1val_0x10000___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[28], 8);
}


TEST_F(srai_01, opcode__srai___op1_x15__dest_x9__op1val_0x20000___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[29], 0);
}


TEST_F(srai_01, opcode__srai___op1_x19__dest_x6__op1val_0x40000___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[30], 0);
}


TEST_F(srai_01, opcode__srai___op1_x28__dest_x21__op1val_0x80000___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[31], 0);
}


TEST_F(srai_01, opcode__srai___op1_x27__dest_x31__op1val_0x200000___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[32], 0);
}


TEST_F(srai_01, opcode__srai___op1_x6__dest_x1__op1val_0x400000___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[33], 16);
}


TEST_F(srai_01, opcode__srai___op1_x4__dest_x15__op1val_0x800000___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[34], 4096);
}


TEST_F(srai_01, opcode__srai___op1_x29__dest_x16__op1val_0x1000000___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[35], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x2000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[36], 33554432);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x4000000___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[37], 1048576);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x8000000___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[38], 1048576);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x10000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[39], 268435456);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x20000000___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[40], 131072);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x40000000___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[41], 4194304);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x2___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[42], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x3___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[43], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x5___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[44], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x11___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[45], -17);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x21___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[46], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x41___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[47], -5);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x81___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[48], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x101___immval_0xb) {
  ASSERT_EQ(getScalarWordData()[49], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x201___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[50], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x401___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[51], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x801___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[52], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x1001___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[53], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x2001___immval_0x1d) {
  ASSERT_EQ(getScalarWordData()[54], -1);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x4001___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[55], -2049);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x8001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[56], -32769);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x10001___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[57], -9);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x40001___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[58], -3);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x80001___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[59], -1025);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x100001___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[60], -524289);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x200001___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[61], -17);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x400001___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[62], -33);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x800001___immval_0xe) {
  ASSERT_EQ(getScalarWordData()[63], -513);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x2000001___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[64], -2097153);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x4000001___immval_0x12) {
  ASSERT_EQ(getScalarWordData()[65], -257);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x8000001___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[66], -16385);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x10000001___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[67], -33554433);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x20000001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[68], -4194305);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x40000001___immval_0x13) {
  ASSERT_EQ(getScalarWordData()[69], -2049);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x55555555___immval_0x15) {
  ASSERT_EQ(getScalarWordData()[70], 682);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x55555556___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[71], -43691);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x3___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[72], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[73], 858993459);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x66666667___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[74], 1677721);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0xb503___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[75], -23170);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[76], 46341);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[77], 357913941);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x66666666___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[78], 12);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0xb504___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[79], -12);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[80], 46340);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[81], 22369621);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x33333332___immval_0x1e) {
  ASSERT_EQ(getScalarWordData()[82], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x66666665___immval_0x1f) {
  ASSERT_EQ(getScalarWordData()[83], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0xb503___immval_0x17) {
  ASSERT_EQ(getScalarWordData()[84], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x55555556___immval_0x1b) {
  ASSERT_EQ(getScalarWordData()[85], 10);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val__0x55555555___immval_0xa) {
  ASSERT_EQ(getScalarWordData()[86], -1398102);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[87], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x33333334___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[88], 209715);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x100___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[89], 0);
}


TEST_F(srai_01, opcode__srai___op1_x10__dest_x11__op1val_0x8000___immval_0x11) {
  ASSERT_EQ(getScalarWordData()[90], 0);
}

