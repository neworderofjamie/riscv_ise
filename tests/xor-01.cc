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
        RISCV riscV(ArchTest::xor_01::generateCode().getCode(), scalarInitData);
        
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
  ASSERT_EQ(getScalarWordData()[4], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x13__op2_x13__dest_x10__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(xor_01, opcode__xor___op1_x23__op2_x23__dest_x23__op1val__0x4001___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[6], 0);
}


TEST_F(xor_01, opcode__xor___op1_x28__op2_x14__dest_x28__op1val__0x41___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[7], 4294967223);
}


TEST_F(xor_01, opcode__xor___op1_x1__op2_x2__dest_x18__op1val__0x1___op2val__0x1) {
  ASSERT_EQ(getScalarWordData()[8], 0);
}


TEST_F(xor_01, opcode__xor___op1_x5__op2_x22__dest_x19__op1val_0x400000___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[9], 2151677952);
}


TEST_F(xor_01, opcode__xor___op1_x26__op2_x12__dest_x13__op1val__0x11___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[10], 4294967279);
}


TEST_F(xor_01, opcode__xor___op1_x12__op2_x11__dest_x4__op1val__0x55555556___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[11], 3579139413);
}


TEST_F(xor_01, opcode__xor___op1_x19__op2_x30__dest_x17__op1val_0x1___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(xor_01, opcode__xor___op1_x11__op2_x1__dest_x3__op1val__0x80000000___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[13], 1879048191);
}


TEST_F(xor_01, opcode__xor___op1_x24__op2_x29__dest_x8__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[14], 4294920956);
}


TEST_F(xor_01, opcode__xor___op1_x0__op2_x18__dest_x9__op1val_0x0___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[15], 4096);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x6__dest_x26__op1val_0x80000___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[16], 524290);
}


TEST_F(xor_01, opcode__xor___op1_x22__op2_x31__dest_x30__op1val__0x21___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[17], 4294967259);
}


TEST_F(xor_01, opcode__xor___op1_x8__op2_x0__dest_x16__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[18], 6);
}


TEST_F(xor_01, opcode__xor___op1_x16__op2_x27__dest_x15__op1val_0x5___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[19], 37);
}


TEST_F(xor_01, opcode__xor___op1_x3__op2_x26__dest_x31__op1val__0x200001___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[20], 4292870079);
}


TEST_F(xor_01, opcode__xor___op1_x9__op2_x25__dest_x14__op1val__0x8001___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[21], 4294934399);
}


TEST_F(xor_01, opcode__xor___op1_x30__op2_x10__dest_x6__op1val_0x55555556___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[22], 1431655510);
}


TEST_F(xor_01, opcode__xor___op1_x2__op2_x9__dest_x7__op1val__0x4001___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[23], 4294950399);
}


TEST_F(xor_01, opcode__xor___op1_x6__op2_x17__dest_x0__op1val_0x80___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[24], 0);
}


TEST_F(xor_01, opcode__xor___op1_x15__op2_x5__dest_x2__op1val__0x8001___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[25], 4294932479);
}


TEST_F(xor_01, opcode__xor___op1_x7__op2_x15__dest_x22__op1val_0x6___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[26], 8198);
}


TEST_F(xor_01, opcode__xor___op1_x14__op2_x16__dest_x20__op1val__0x9___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[27], 4294950903);
}


TEST_F(xor_01, opcode__xor___op1_x29__op2_x20__dest_x1__op1val_0x8000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[28], 0);
}


TEST_F(xor_01, opcode__xor___op1_x18__op2_x8__dest_x12__op1val_0x4___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[29], 65540);
}


TEST_F(xor_01, opcode__xor___op1_x21__op2_x28__dest_x5__op1val_0x33333332___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[30], 858862386);
}


TEST_F(xor_01, opcode__xor___op1_x17__op2_x4__dest_x11__op1val__0x4000001___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[31], 4227596287);
}


TEST_F(xor_01, opcode__xor___op1_x4__op2_x21__dest_x27__op1val_0x7___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[32], 524295);
}


TEST_F(xor_01, opcode__xor___op1_x31__op2_x3__dest_x29__op1val__0x1___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[33], 4293918719);
}


TEST_F(xor_01, opcode__xor___op1_x25__op2_x7__dest_x21__op1val_0xb505___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[34], 2143493);
}


TEST_F(xor_01, opcode__xor___op1_x20__op2_x19__dest_x25__op1val_0xb505___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[35], 4240645);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[36], 8388609);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[37], 4009754623);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[38], 33600773);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[39], 4219469823);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[40], 134221824);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[41], 4026531831);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[42], 3724541951);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[43], 3221094399);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[44], 33554433);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[45], 3221225469);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[46], 2576980381);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[47], 4261412855);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[48], 48);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[49], 4294967259);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[50], 4294967228);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[51], 4294836095);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[52], 268435712);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[53], 516);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[54], 1040);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[55], 48387);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[56], 4294963197);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x7___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[57], 8198);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[58], 16793600);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[59], 4294934524);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[60], 4294819839);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[61], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[62], 4026007551);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[63], 2146435071);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[64], 4292837375);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[65], 2143289343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[66], 3427585227);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x40000000___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[67], 1056964607);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[68], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[69], 4227858430);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[70], 2442762649);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[71], 537395200);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[72], 1073788163);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[73], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[74], 2829757098);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val__0x4) {
  ASSERT_EQ(getScalarWordData()[75], 4294967284);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[76], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[77], 96);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[78], 4294967199);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[79], 4290772735);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[80], 2863311018);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[81], 2863312554);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[82], 1717988965);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[83], 4293910527);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[84], 4294855421);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[85], 4293918711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[86], 4292853759);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[87], 1726375525);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[88], 2880088747);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[89], 71303168);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[90], 1565873492);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[91], 536871936);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[92], 46338);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[93], 4294967289);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[94], 4294920828);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[95], 4294962943);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[96], 4294966783);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[97], 268436480);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[98], 4261410815);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[99], 4097);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[100], 4294959099);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[101], 4294901757);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[102], 786432);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[103], 4293918687);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val__0xa) {
  ASSERT_EQ(getScalarWordData()[104], 4194313);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[105], 3301756108);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[106], 3741319167);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[107], 1342177280);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[108], 1717986913);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[109], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[110], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[111], 2863311529);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[112], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[113], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[114], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[115], 4294920959);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[116], 46343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[117], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[118], 1431655767);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[119], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[120], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[121], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[122], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[123], 46336);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[124], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[125], 2863311528);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[126], 5);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[127], 858993463);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[128], 1717986916);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[129], 4294920958);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[130], 46342);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[131], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[132], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[133], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[134], 1431655760);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[135], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[136], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[137], 2863275945);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[138], 1431691345);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[139], 1431655767);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[140], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[141], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[142], 1431655761);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[143], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[144], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[145], 1431691350);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[146], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[147], 4294967294);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[148], 1431655763);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[149], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[150], 2863275944);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[151], 1431691344);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[152], 2863311529);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[153], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[154], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[155], 2863311535);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[156], 2576980377);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[157], 3435973836);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[158], 1431691350);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[159], 2863275950);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[160], 2863311528);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[161], 4294967294);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[162], 2863311530);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[163], 2863311534);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[164], 2576980376);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[165], 3435973839);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[166], 2863275945);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[167], 4294967292);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[168], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[169], 2863311532);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[170], 2576980382);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[171], 3435973837);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[172], 1431691351);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[173], 2863275951);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[174], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[175], 1431655760);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[176], 2863311535);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[177], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[178], 858993462);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[179], 1717986915);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[180], 4294920953);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[181], 46337);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[182], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[183], 1431655761);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[184], 5);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[185], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[186], 858993463);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[187], 1717986912);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[188], 46342);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[189], 1431655763);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[190], 2863311534);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[191], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[192], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[193], 1717986914);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[194], 4294920952);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[195], 46336);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[196], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[197], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[198], 2576980377);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[199], 858993462);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[200], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[201], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[202], 3435952591);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[203], 859014711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[204], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[205], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[206], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[207], 858993463);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[208], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[209], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[210], 859014704);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[211], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[212], 2576980376);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[213], 858993461);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[214], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[215], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[216], 3435952590);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[217], 859014710);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[218], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[219], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[220], 3435973836);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[221], 1717986915);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[222], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[223], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[224], 2576952474);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[225], 1718014818);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[226], 1717986916);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[227], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[228], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[229], 1717986914);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[230], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[231], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[232], 1718014821);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[233], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[234], 3435973837);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[235], 1717986912);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[236], 1431655762);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[237], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[238], 2576952475);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[239], 1718014819);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[240], 4294920959);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[241], 2863275945);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[242], 1431691350);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[243], 4294920953);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[244], 3435952591);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[245], 2576952474);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[246], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[247], 4294967288);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[248], 4294920958);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[249], 2863275944);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[250], 4294920956);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[251], 4294920952);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[252], 3435952590);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[253], 2576952473);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[254], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[255], 2863275946);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[256], 1431691351);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[257], 4294920954);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[258], 3435952584);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[259], 2576952475);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[260], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[261], 4294967289);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[262], 46343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[263], 1431691345);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[264], 2863275950);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[265], 46337);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[266], 859014711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[267], 1718014818);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[268], 4294967288);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[269], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[270], 46342);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[271], 1431691344);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[272], 46340);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[273], 46336);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[274], 859014710);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[275], 1718014817);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[276], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[277], 1431691346);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[278], 2863275951);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[279], 46338);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[280], 859014704);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[281], 1718014819);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[282], 4294967289);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[283], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[284], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[285], 1431655767);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[286], 2863311528);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[287], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[288], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[289], 1717986916);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[290], 4294920958);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[291], 46342);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[292], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[293], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[294], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[295], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[296], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[297], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[298], 46337);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[299], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[300], 2863311529);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[301], 4);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[302], 858993462);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[303], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[304], 4294920959);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[305], 46343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[306], 1431655767);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[307], 4294967294);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[308], 1431655761);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[309], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[310], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[311], 2863275944);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[312], 1431691344);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[313], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[314], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[315], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[316], 1431655760);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[317], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[318], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[319], 1431691351);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[320], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[321], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[322], 1431655762);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[323], 1717986912);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[324], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[325], 2863275945);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[326], 1431691345);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[327], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[328], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[329], 2863311530);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[330], 5);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[331], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[332], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[333], 46340);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[334], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[335], 2863311533);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[336], 2576980383);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[337], 3435973836);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[338], 1431691350);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[339], 2863275950);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[340], 5);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[341], 1431655763);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[342], 2863311532);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[343], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[344], 858993461);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[345], 1717986912);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[346], 4294920954);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[347], 46338);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[348], 4);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[349], 1431655762);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[350], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[351], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[352], 858993460);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[353], 1717986915);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[354], 46341);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[355], 1431655760);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[356], 2863311533);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[357], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[358], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[359], 1717986913);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[360], 4294920955);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[361], 46339);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[362], 858993463);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[363], 1717986913);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[364], 2576980382);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[365], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[366], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[367], 1431655762);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[368], 3435952584);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[369], 859014704);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[370], 858993462);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[371], 1717986912);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[372], 858993460);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[373], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[374], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[375], 1431655761);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[376], 859014711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[377], 1717986914);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[378], 2576980383);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[379], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[380], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[381], 1431655763);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[382], 3435952585);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[383], 859014705);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[384], 1717986916);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[385], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[386], 3435973837);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[387], 1717986914);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[388], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[389], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[390], 2576952475);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[391], 1718014819);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[392], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[393], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[394], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[395], 1717986915);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[396], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[397], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[398], 1718014820);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[399], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[400], 3435973836);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[401], 1717986913);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[402], 1431655763);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[403], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[404], 2576952474);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[405], 1718014818);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[406], 4294920958);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[407], 2863275944);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[408], 1431691351);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[409], 4294920952);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[410], 3435952590);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[411], 2576952475);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[412], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[413], 4294967289);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[414], 4294920959);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[415], 2863275945);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[416], 4294920957);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[417], 4294920953);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[418], 3435952591);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[419], 2576952472);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[420], 4294967294);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[421], 2863275947);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[422], 1431691350);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[423], 4294920955);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[424], 3435952585);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[425], 2576952474);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[426], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[427], 4294967288);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[428], 46342);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[429], 1431691344);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[430], 2863275951);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[431], 46336);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[432], 859014710);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[433], 1718014819);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[434], 4294967289);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[435], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[436], 46343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[437], 1431691345);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[438], 46341);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[439], 46337);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[440], 859014711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[441], 1718014816);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[442], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[443], 1431691347);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[444], 2863275950);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[445], 46339);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[446], 859014705);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[447], 1718014818);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[448], 4294967288);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[449], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[450], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[451], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[452], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[453], 4);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[454], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[455], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[456], 46339);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[457], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[458], 2863311531);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[459], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[460], 858993460);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[461], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[462], 4294920957);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[463], 46341);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[464], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[465], 1431655761);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[466], 2863311534);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[467], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[468], 858993463);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[469], 1717986914);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[470], 4294920952);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[471], 46336);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[472], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[473], 1431655760);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[474], 4);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[475], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[476], 858993462);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[477], 1717986913);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[478], 46343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[479], 1431655762);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[480], 2863311535);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[481], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[482], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[483], 1717986915);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[484], 4294920953);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[485], 46337);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[486], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[487], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[488], 2576980376);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[489], 858993463);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[490], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[491], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[492], 3435952590);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[493], 859014710);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[494], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[495], 1717986918);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[496], 858993458);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[497], 858993462);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[498], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[499], 1431655767);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[500], 859014705);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[501], 1717986916);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[502], 2576980377);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[503], 858993460);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[504], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[505], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[506], 3435952591);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[507], 859014711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[508], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[509], 3435973839);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[510], 1717986912);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[511], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[512], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[513], 2576952473);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[514], 1718014817);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[515], 1717986919);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[516], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[517], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[518], 1717986913);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[519], 1431655767);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[520], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[521], 1718014822);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[522], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[523], 3435973838);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[524], 1717986915);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[525], 1431655761);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[526], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[527], 2576952472);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[528], 1718014816);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[529], 46336);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[530], 1431691350);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[531], 2863275945);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[532], 46342);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[533], 859014704);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[534], 1718014821);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[535], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[536], 7);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[537], 46337);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[538], 1431691351);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[539], 46339);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[540], 46343);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[541], 859014705);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[542], 1718014822);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[543], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[544], 1431691349);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[545], 2863275944);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[546], 46341);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[547], 859014711);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[548], 1718014820);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[549], 4294967294);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[550], 6);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[551], 1431655765);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[552], 3);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[553], 4294967292);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[554], 1431655763);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[555], 1717986917);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[556], 858993456);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[557], 2863275946);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[558], 1431691346);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[559], 1431655764);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[560], 2);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[561], 1431655766);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[562], 1431655762);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[563], 1717986916);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[564], 858993459);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[565], 1431691349);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[566], 0);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[567], 4294967293);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[568], 1431655760);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[569], 1717986914);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[570], 858993457);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[571], 2863275947);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[572], 1431691347);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[573], 2863311528);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[574], 4294967294);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[575], 1);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[576], 2863311534);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[577], 2576980376);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[578], 3435973837);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[579], 1431691351);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[580], 2863275951);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[581], 2863311529);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[582], 4294967295);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[583], 2863311531);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[584], 2863311535);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[585], 2576980377);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[586], 3435973838);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[587], 2863275944);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[588], 4294967293);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[589], 81920);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[590], 2147479551);
}


TEST_F(xor_01, opcode__xor___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[591], 1152);
}

