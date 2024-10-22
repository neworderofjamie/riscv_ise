// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/xor-01.h"

namespace
{
class xor_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::xor_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::xor_01::generateCode(true), scalarInitData);
        
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

std::vector<uint8_t> xor_01::s_ScalarData;
}


// Generate google tests

TEST_F(xor_01, opcode__xor___op1_x27__op2_x24__dest_x24__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[5], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x13__op2_x13__dest_x10__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[6], 0);
}


TEST_F(xor_01, opcode__xor___op1_x23__op2_x23__dest_x23__op1val__0x4001___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[7], 0);
}


TEST_F(xor_01, opcode__xor___op1_x28__op2_x14__dest_x28__op1val__0x41___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[8], 4294967223);
}


TEST_F(xor_01, opcode__xor___op1_x1__op2_x2__dest_x18__op1val__0x1___op2val__0x1) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(xor_01, opcode__xor___op1_x5__op2_x22__dest_x19__op1val_0x400000___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[10], 2151677952);
}


TEST_F(xor_01, opcode__xor___op1_x26__op2_x12__dest_x13__op1val__0x11___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[11], 4294967279);
}


TEST_F(xor_01, opcode__xor___op1_x12__op2_x11__dest_x4__op1val__0x55555556___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[12], 3579139413);
}


TEST_F(xor_01, opcode__xor___op1_x19__op2_x30__dest_x17__op1val_0x1___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(xor_01, opcode__xor___op1_x11__op2_x1__dest_x3__op1val__0x80000000___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[14], 1879048191);
}


TEST_F(xor_01, opcode__xor___op1_x24__op2_x29__dest_x8__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[15], 4294920956);
}


TEST_F(xor_01, opcode__xor___op1_x0__op2_x18__dest_x9__op1val_0x0___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[16], 4096);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x6__dest_x26__op1val_0x80000___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[17], 524290);
}


TEST_F(xor_01, opcode__xor___op1_x22__op2_x31__dest_x30__op1val__0x21___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[18], 4294967259);
}


TEST_F(xor_01, opcode__xor___op1_x8__op2_x0__dest_x16__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[19], 6);
}


TEST_F(xor_01, opcode__xor___op1_x16__op2_x27__dest_x15__op1val_0x5___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[20], 37);
}


TEST_F(xor_01, opcode__xor___op1_x3__op2_x26__dest_x31__op1val__0x200001___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[21], 4292870079);
}


TEST_F(xor_01, opcode__xor___op1_x9__op2_x25__dest_x14__op1val__0x8001___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[22], 4294934399);
}


TEST_F(xor_01, opcode__xor___op1_x30__op2_x10__dest_x6__op1val_0x55555556___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[23], 1431655510);
}


TEST_F(xor_01, opcode__xor___op1_x2__op2_x9__dest_x7__op1val__0x4001___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[24], 4294950399);
}


TEST_F(xor_01, opcode__xor___op1_x6__op2_x17__dest_x0__op1val_0x80___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[25], 0);
}


TEST_F(xor_01, opcode__xor___op1_x15__op2_x5__dest_x2__op1val__0x8001___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[26], 4294932479);
}


TEST_F(xor_01, opcode__xor___op1_x7__op2_x15__dest_x22__op1val_0x6___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[27], 8198);
}


TEST_F(xor_01, opcode__xor___op1_x14__op2_x16__dest_x20__op1val__0x9___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[28], 4294950903);
}


TEST_F(xor_01, opcode__xor___op1_x29__op2_x20__dest_x1__op1val_0x8000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[29], 0);
}


TEST_F(xor_01, opcode__xor___op1_x18__op2_x8__dest_x12__op1val_0x4___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[30], 65540);
}


TEST_F(xor_01, opcode__xor___op1_x21__op2_x28__dest_x5__op1val_0x33333332___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[31], 858862386);
}


TEST_F(xor_01, opcode__xor___op1_x17__op2_x4__dest_x11__op1val__0x4000001___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[32], 4227596287);
}


TEST_F(xor_01, opcode__xor___op1_x4__op2_x21__dest_x27__op1val_0x7___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[33], 524295);
}


TEST_F(xor_01, opcode__xor___op1_x31__op2_x3__dest_x29__op1val__0x1___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[34], 4293918719);
}


TEST_F(xor_01, opcode__xor___op1_x25__op2_x7__dest_x21__op1val_0xb505___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[35], 2143493);
}


TEST_F(xor_01, opcode__xor___op1_x20__op2_x19__dest_x25__op1val_0xb505___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[36], 4240645);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[37], 8388609);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[38], 4009754623);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[39], 33600773);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[40], 4219469823);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[41], 134221824);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[42], 4026531831);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[43], 3724541951);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[44], 3221094399);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[45], 33554433);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[46], 3221225469);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[47], 2576980381);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[48], 4261412855);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[49], 48);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[50], 4294967259);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[51], 4294967228);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[52], 4294836095);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[53], 268435712);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[54], 516);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[55], 1040);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[56], 48387);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[57], 4294963197);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x7___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[58], 8198);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[59], 16793600);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[60], 4294934524);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[61], 4294819839);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[62], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[63], 4026007551);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[64], 2146435071);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[65], 4292837375);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[66], 2143289343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[67], 3427585227);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x40000000___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[68], 1056964607);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[69], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[70], 4227858430);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[71], 2442762649);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[72], 537395200);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[73], 1073788163);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[74], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[75], 2829757098);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val__0x4) {
  ASSERT_EQ(getScalarWordData()[76], 4294967284);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[77], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[78], 96);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[79], 4294967199);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[80], 4290772735);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[81], 2863311018);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[82], 2863312554);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[83], 1717988965);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[84], 4293910527);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[85], 4294855421);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[86], 4293918711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[87], 4292853759);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[88], 1726375525);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[89], 2880088747);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[90], 71303168);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[91], 1565873492);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[92], 536871936);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[93], 46338);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[94], 4294967289);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[95], 4294920828);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[96], 4294962943);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[97], 4294966783);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[98], 268436480);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[99], 4261410815);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[100], 4097);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[101], 4294959099);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[102], 4294901757);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[103], 786432);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[104], 4293918687);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val__0xa) {
  ASSERT_EQ(getScalarWordData()[105], 4194313);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[106], 3301756108);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[107], 3741319167);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[108], 1342177280);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[109], 1717986913);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[110], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[111], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[112], 2863311529);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[113], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[114], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[115], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[116], 4294920959);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[117], 46343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[118], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[119], 1431655767);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[120], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[121], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[122], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[123], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[124], 46336);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[125], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[126], 2863311528);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[127], 5);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[128], 858993463);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[129], 1717986916);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[130], 4294920958);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[131], 46342);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[132], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[133], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[134], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[135], 1431655760);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[136], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[137], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[138], 2863275945);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[139], 1431691345);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[140], 1431655767);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[141], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[142], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[143], 1431655761);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[144], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[145], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[146], 1431691350);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[147], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[148], 4294967294);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[149], 1431655763);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[150], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[151], 2863275944);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[152], 1431691344);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[153], 2863311529);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[154], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[155], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[156], 2863311535);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[157], 2576980377);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[158], 3435973836);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[159], 1431691350);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[160], 2863275950);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[161], 2863311528);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[162], 4294967294);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[163], 2863311530);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[164], 2863311534);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[165], 2576980376);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[166], 3435973839);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[167], 2863275945);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[168], 4294967292);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[169], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[170], 2863311532);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[171], 2576980382);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[172], 3435973837);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[173], 1431691351);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[174], 2863275951);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[175], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[176], 1431655760);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[177], 2863311535);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[178], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[179], 858993462);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[180], 1717986915);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[181], 4294920953);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[182], 46337);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[183], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[184], 1431655761);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[185], 5);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[186], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[187], 858993463);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[188], 1717986912);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[189], 46342);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[190], 1431655763);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[191], 2863311534);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[192], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[193], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[194], 1717986914);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[195], 4294920952);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[196], 46336);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[197], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[198], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[199], 2576980377);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[200], 858993462);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[201], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[202], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[203], 3435952591);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[204], 859014711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[205], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[206], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[207], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[208], 858993463);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[209], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[210], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[211], 859014704);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[212], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[213], 2576980376);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[214], 858993461);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[215], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[216], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[217], 3435952590);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[218], 859014710);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[219], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[220], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[221], 3435973836);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[222], 1717986915);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[223], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[224], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[225], 2576952474);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[226], 1718014818);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[227], 1717986916);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[228], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[229], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[230], 1717986914);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[231], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[232], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[233], 1718014821);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[234], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[235], 3435973837);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[236], 1717986912);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[237], 1431655762);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[238], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[239], 2576952475);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[240], 1718014819);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[241], 4294920959);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[242], 2863275945);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[243], 1431691350);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[244], 4294920953);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[245], 3435952591);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[246], 2576952474);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[247], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[248], 4294967288);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[249], 4294920958);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[250], 2863275944);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[251], 4294920956);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[252], 4294920952);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[253], 3435952590);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[254], 2576952473);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[255], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[256], 2863275946);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[257], 1431691351);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[258], 4294920954);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[259], 3435952584);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[260], 2576952475);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[261], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[262], 4294967289);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[263], 46343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[264], 1431691345);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[265], 2863275950);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[266], 46337);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[267], 859014711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[268], 1718014818);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[269], 4294967288);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[270], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[271], 46342);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[272], 1431691344);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[273], 46340);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[274], 46336);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[275], 859014710);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[276], 1718014817);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[277], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[278], 1431691346);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[279], 2863275951);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[280], 46338);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[281], 859014704);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[282], 1718014819);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[283], 4294967289);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[284], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[285], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[286], 1431655767);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[287], 2863311528);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[288], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[289], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[290], 1717986916);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[291], 4294920958);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[292], 46342);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[293], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[294], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[295], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[296], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[297], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[298], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[299], 46337);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[300], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[301], 2863311529);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[302], 4);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[303], 858993462);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[304], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[305], 4294920959);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[306], 46343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[307], 1431655767);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[308], 4294967294);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[309], 1431655761);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[310], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[311], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[312], 2863275944);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[313], 1431691344);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[314], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[315], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[316], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[317], 1431655760);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[318], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[319], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[320], 1431691351);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[321], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[322], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[323], 1431655762);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[324], 1717986912);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[325], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[326], 2863275945);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[327], 1431691345);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[328], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[329], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[330], 2863311530);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[331], 5);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[332], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[333], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[334], 46340);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[335], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[336], 2863311533);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[337], 2576980383);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[338], 3435973836);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[339], 1431691350);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[340], 2863275950);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[341], 5);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[342], 1431655763);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[343], 2863311532);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[344], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[345], 858993461);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[346], 1717986912);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[347], 4294920954);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[348], 46338);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[349], 4);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[350], 1431655762);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[351], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[352], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[353], 858993460);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[354], 1717986915);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[355], 46341);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[356], 1431655760);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[357], 2863311533);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[358], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[359], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[360], 1717986913);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[361], 4294920955);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[362], 46339);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[363], 858993463);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[364], 1717986913);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[365], 2576980382);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[366], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[367], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[368], 1431655762);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[369], 3435952584);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[370], 859014704);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[371], 858993462);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[372], 1717986912);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[373], 858993460);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[374], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[375], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[376], 1431655761);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[377], 859014711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[378], 1717986914);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[379], 2576980383);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[380], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[381], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[382], 1431655763);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[383], 3435952585);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[384], 859014705);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[385], 1717986916);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[386], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[387], 3435973837);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[388], 1717986914);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[389], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[390], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[391], 2576952475);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[392], 1718014819);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[393], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[394], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[395], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[396], 1717986915);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[397], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[398], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[399], 1718014820);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[400], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[401], 3435973836);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[402], 1717986913);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[403], 1431655763);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[404], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[405], 2576952474);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[406], 1718014818);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[407], 4294920958);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[408], 2863275944);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[409], 1431691351);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[410], 4294920952);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[411], 3435952590);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[412], 2576952475);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[413], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[414], 4294967289);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[415], 4294920959);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[416], 2863275945);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[417], 4294920957);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[418], 4294920953);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[419], 3435952591);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[420], 2576952472);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[421], 4294967294);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[422], 2863275947);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[423], 1431691350);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[424], 4294920955);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[425], 3435952585);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[426], 2576952474);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[427], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[428], 4294967288);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[429], 46342);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[430], 1431691344);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[431], 2863275951);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[432], 46336);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[433], 859014710);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[434], 1718014819);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[435], 4294967289);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[436], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[437], 46343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[438], 1431691345);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[439], 46341);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[440], 46337);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[441], 859014711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[442], 1718014816);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[443], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[444], 1431691347);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[445], 2863275950);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[446], 46339);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[447], 859014705);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[448], 1718014818);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[449], 4294967288);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[450], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[451], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[452], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[453], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[454], 4);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[455], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[456], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[457], 46339);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[458], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[459], 2863311531);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[460], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[461], 858993460);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[462], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[463], 4294920957);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[464], 46341);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[465], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[466], 1431655761);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[467], 2863311534);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[468], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[469], 858993463);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[470], 1717986914);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[471], 4294920952);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[472], 46336);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[473], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[474], 1431655760);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[475], 4);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[476], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[477], 858993462);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[478], 1717986913);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[479], 46343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[480], 1431655762);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[481], 2863311535);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[482], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[483], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[484], 1717986915);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[485], 4294920953);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[486], 46337);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[487], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[488], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[489], 2576980376);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[490], 858993463);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[491], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[492], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[493], 3435952590);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[494], 859014710);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[495], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[496], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[497], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[498], 858993462);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[499], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[500], 1431655767);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[501], 859014705);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[502], 1717986916);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[503], 2576980377);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[504], 858993460);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[505], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[506], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[507], 3435952591);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[508], 859014711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[509], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[510], 3435973839);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[511], 1717986912);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[512], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[513], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[514], 2576952473);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[515], 1718014817);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[516], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[517], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[518], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[519], 1717986913);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[520], 1431655767);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[521], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[522], 1718014822);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[523], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[524], 3435973838);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[525], 1717986915);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[526], 1431655761);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[527], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[528], 2576952472);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[529], 1718014816);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[530], 46336);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[531], 1431691350);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[532], 2863275945);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[533], 46342);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[534], 859014704);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[535], 1718014821);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[536], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[537], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[538], 46337);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[539], 1431691351);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[540], 46339);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[541], 46343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[542], 859014705);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[543], 1718014822);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[544], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[545], 1431691349);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[546], 2863275944);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[547], 46341);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[548], 859014711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[549], 1718014820);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[550], 4294967294);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[551], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[552], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[553], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[554], 4294967292);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[555], 1431655763);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[556], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[557], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[558], 2863275946);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[559], 1431691346);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[560], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[561], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[562], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[563], 1431655762);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[564], 1717986916);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[565], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[566], 1431691349);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[567], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[568], 4294967293);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[569], 1431655760);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[570], 1717986914);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[571], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[572], 2863275947);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[573], 1431691347);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[574], 2863311528);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[575], 4294967294);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[576], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[577], 2863311534);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[578], 2576980376);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[579], 3435973837);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[580], 1431691351);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[581], 2863275951);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[582], 2863311529);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[583], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[584], 2863311531);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[585], 2863311535);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[586], 2576980377);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[587], 3435973838);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[588], 2863275944);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[589], 4294967293);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[590], 81920);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[591], 2147479551);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[592], 1152);
}

