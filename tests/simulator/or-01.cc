// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/or-01.h"

namespace
{
class or_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::or_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::or_01::generateCode(true), scalarInitData);
        
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

std::vector<uint8_t> or_01::s_ScalarData;
}


// Generate google tests

TEST_F(or_01, opcode__or___op1_x8__op2_x26__dest_x26__op1val_0x100000___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[5], 1048592);
}


TEST_F(or_01, opcode__or___op1_x6__op2_x6__dest_x17__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[6], 2);
}


TEST_F(or_01, opcode__or___op1_x31__op2_x31__dest_x31__op1val__0x10000001___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[7], 4026531839);
}


TEST_F(or_01, opcode__or___op1_x27__op2_x29__dest_x27__op1val__0x801___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[8], 4294965247);
}


TEST_F(or_01, opcode__or___op1_x30__op2_x19__dest_x18__op1val__0x100001___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[9], 4293918719);
}


TEST_F(or_01, opcode__or___op1_x21__op2_x14__dest_x9__op1val_0x20000___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[10], 2147614720);
}


TEST_F(or_01, opcode__or___op1_x26__op2_x24__dest_x4__op1val__0x2001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[11], 4294959103);
}


TEST_F(or_01, opcode__or___op1_x9__op2_x8__dest_x30__op1val_0x0___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[12], 2147483647);
}


TEST_F(or_01, opcode__or___op1_x23__op2_x7__dest_x8__op1val__0x800001___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[13], 4286578687);
}


TEST_F(or_01, opcode__or___op1_x12__op2_x0__dest_x22__op1val__0x80000000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[14], 2147483648);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x30__dest_x28__op1val_0x7fffffff___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[15], 2147483647);
}


TEST_F(or_01, opcode__or___op1_x18__op2_x21__dest_x16__op1val_0x1___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[16], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x14__op2_x17__dest_x12__op1val_0x1000___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[17], 4098);
}


TEST_F(or_01, opcode__or___op1_x19__op2_x16__dest_x15__op1val__0x800001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[18], 4286578687);
}


TEST_F(or_01, opcode__or___op1_x4__op2_x2__dest_x7__op1val__0x401___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[19], 4294966271);
}


TEST_F(or_01, opcode__or___op1_x2__op2_x22__dest_x11__op1val_0x7fffffff___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[20], 2147483647);
}


TEST_F(or_01, opcode__or___op1_x28__op2_x15__dest_x25__op1val__0x201___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[21], 4294966783);
}


TEST_F(or_01, opcode__or___op1_x25__op2_x1__dest_x6__op1val_0xb504___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[22], 46340);
}


TEST_F(or_01, opcode__or___op1_x17__op2_x10__dest_x20__op1val_0x4___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[23], 516);
}


TEST_F(or_01, opcode__or___op1_x20__op2_x23__dest_x5__op1val__0x100001___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[24], 4293918719);
}


TEST_F(or_01, opcode__or___op1_x11__op2_x12__dest_x2__op1val_0x0___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[25], 2048);
}


TEST_F(or_01, opcode__or___op1_x13__op2_x20__dest_x1__op1val__0x4000001___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[26], 4227858431);
}


TEST_F(or_01, opcode__or___op1_x15__op2_x27__dest_x3__op1val_0x4___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[27], 8196);
}


TEST_F(or_01, opcode__or___op1_x22__op2_x4__dest_x21__op1val__0x10001___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[28], 4294901759);
}


TEST_F(or_01, opcode__or___op1_x7__op2_x13__dest_x19__op1val_0x80000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[29], 557056);
}


TEST_F(or_01, opcode__or___op1_x24__op2_x5__dest_x14__op1val__0x4___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[30], 4294967292);
}


TEST_F(or_01, opcode__or___op1_x16__op2_x18__dest_x10__op1val_0x55555555___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[31], 1431786837);
}


TEST_F(or_01, opcode__or___op1_x1__op2_x9__dest_x23__op1val__0xb504___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[32], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x3__op2_x11__dest_x29__op1val_0x4___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[33], 524292);
}


TEST_F(or_01, opcode__or___op1_x29__op2_x28__dest_x13__op1val__0x1001___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[34], 4294963199);
}


TEST_F(or_01, opcode__or___op1_x5__op2_x3__dest_x0__op1val_0x33333332___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[35], 0);
}


TEST_F(or_01, opcode__or___op1_x0__op2_x25__dest_x24__op1val_0x0___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[36], 8388608);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[37], 16777280);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[38], 4294967287);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[39], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[40], 150994944);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x6___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[41], 4294967290);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[42], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[43], 1082130432);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[44], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[45], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[46], 4294967291);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[47], 4294967287);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[48], 4294967279);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[49], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[50], 4294967231);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[51], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[52], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[53], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[54], 4294963199);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[55], 4294959103);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[56], 4294950911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[57], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[58], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[59], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[60], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[61], 4294443007);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[62], 4292870143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[63], 4290772991);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[64], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[65], 4278190079);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[66], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[67], 4227858431);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[68], 4160749567);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x40000000___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[69], 4026531839);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[70], 3758096383);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[71], 3221225471);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[72], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[73], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val__0x7) {
  ASSERT_EQ(getScalarWordData()[74], 4294967289);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val__0x7) {
  ASSERT_EQ(getScalarWordData()[75], 4294967289);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[76], 4227858431);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[77], 2863312554);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[78], 4292870143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[79], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val__0x6) {
  ASSERT_EQ(getScalarWordData()[80], 4294967290);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[81], 34816);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[82], 4278190079);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[83], 2097152);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[84], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val_0x3fffffff) {
  ASSERT_EQ(getScalarWordData()[85], 1073741823);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[86], 4294443007);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[87], 134217760);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[88], 4294959103);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[89], 1073741832);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[90], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val__0x8) {
  ASSERT_EQ(getScalarWordData()[91], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[92], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[93], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[94], 4294967231);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[95], 4294967167);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[96], 4294950911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[97], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[98], 4294705151);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[99], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[100], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[101], 4278190079);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[102], 4261412863);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[103], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[104], 3221225471);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[105], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[106], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[107], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[108], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[109], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[110], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[111], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[112], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[113], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[114], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[115], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[116], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[117], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[118], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[119], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[120], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[121], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[122], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[123], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[124], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[125], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[126], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[127], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[128], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[129], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[130], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[131], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[132], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[133], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[134], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[135], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[136], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[137], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[138], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[139], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[140], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[141], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[142], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[143], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[144], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[145], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[146], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[147], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[148], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[149], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[150], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[151], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[152], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[153], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[154], 4008636142);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[155], 4294961918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[156], 2863316910);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[157], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[158], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[159], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[160], 2863311534);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[161], 3149642682);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[162], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[163], 2863316907);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[164], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[165], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[166], 2863311534);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[167], 3149642686);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[168], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[169], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[170], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[171], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[172], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[173], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[174], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[175], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[176], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[177], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[178], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[179], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[180], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[181], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[182], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[183], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[184], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[185], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[186], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[187], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[188], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[189], 858993461);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[190], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[191], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[192], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[193], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[194], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[195], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[196], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[197], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[198], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[199], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[200], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[201], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[202], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[203], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[204], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[205], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[206], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[207], 859027251);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[208], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[209], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[210], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[211], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[212], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[213], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[214], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[215], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[216], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[217], 4008636142);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[218], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[219], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[220], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[221], 4294930174);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[222], 1718024038);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[223], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[224], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[225], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[226], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[227], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[228], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[229], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[230], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[231], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[232], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[233], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[234], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[235], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[236], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[237], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[238], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[239], 4294961918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[240], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[241], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[242], 4294930174);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[243], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[244], 4294967292);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[245], 4294920958);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[246], 4294926332);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[247], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[248], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[249], 4294933502);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[250], 4294930173);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[251], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[252], 4294926334);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[253], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[254], 4294920958);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[255], 4294933500);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[256], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[257], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[258], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[259], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[260], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[261], 2863316910);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[262], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[263], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[264], 1718024038);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[265], 4294967292);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[266], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[267], 46342);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[268], 1431696724);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[269], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[270], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[271], 859027254);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[272], 1718024037);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[273], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[274], 1431696726);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[275], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[276], 46342);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[277], 859027252);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[278], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[279], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[280], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[281], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[282], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[283], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[284], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[285], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[286], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[287], 4294920958);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[288], 46342);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[289], 2);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[290], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[291], 2);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[292], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[293], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[294], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[295], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[296], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[297], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[298], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[299], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[300], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[301], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[302], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[303], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[304], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[305], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[306], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[307], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[308], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[309], 4294926332);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[310], 1431696724);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[311], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[312], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[313], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[314], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[315], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[316], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[317], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[318], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[319], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[320], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[321], 2004318068);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[322], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[323], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[324], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[325], 3);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[326], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[327], 2863311530);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[328], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[329], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[330], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[331], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[332], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[333], 2);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[334], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[335], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[336], 3149642687);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[337], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[338], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[339], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[340], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[341], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[342], 2863311534);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[343], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[344], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[345], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[346], 4294920958);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[347], 46342);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[348], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[349], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[350], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[351], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[352], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[353], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[354], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[355], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[356], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[357], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[358], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[359], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[360], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[361], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[362], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[363], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[364], 3149642686);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[365], 858993461);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[366], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[367], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[368], 4294933500);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[369], 859027252);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[370], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[371], 2004318068);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[372], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[373], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[374], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[375], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[376], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[377], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[378], 3149642687);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[379], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[380], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[381], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[382], 4294933501);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[383], 859027253);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[384], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[385], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[386], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[387], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[388], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[389], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[390], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[391], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[392], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[393], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[394], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[395], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[396], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[397], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[398], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[399], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[400], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[401], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[402], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[403], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[404], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[405], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[406], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[407], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[408], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[409], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[410], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[411], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[412], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[413], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[414], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[415], 4294926333);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[416], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[417], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[418], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[419], 4294930173);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[420], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[421], 4294926335);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[422], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[423], 4294920959);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[424], 4294933501);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[425], 4294930175);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[426], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[427], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[428], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[429], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[430], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[431], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[432], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[433], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[434], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[435], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[436], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[437], 1431696725);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[438], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[439], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[440], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[441], 1718024037);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[442], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[443], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[444], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[445], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[446], 859027253);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[447], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[448], 4294967293);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[449], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[450], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[451], 0);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[452], 4);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[453], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[454], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[455], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[456], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[457], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[458], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[459], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[460], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[461], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[462], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[463], 7);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[464], 1431655765);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[465], 2863311534);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[466], 5);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[467], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[468], 1717986918);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[469], 4294920956);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[470], 46340);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[471], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[472], 1431655764);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[473], 4);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[474], 4);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[475], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[476], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[477], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[478], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[479], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[480], 6);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[481], 858993460);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[482], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[483], 4294920957);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[484], 46341);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[485], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[486], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[487], 3149642682);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[488], 858993463);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[489], 858993459);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[490], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[491], 4294933502);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[492], 859027254);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[493], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[494], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[495], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[496], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[497], 858993458);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[498], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[499], 859027251);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[500], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[501], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[502], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[503], 858993462);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[504], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[505], 4294933503);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[506], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[507], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[508], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[509], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[510], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[511], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[512], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[513], 4294930173);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[514], 1718024037);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[515], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[516], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[517], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[518], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[519], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[520], 1717986917);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[521], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[522], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[523], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[524], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[525], 2004318069);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[526], 1717986919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[527], 4294930173);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[528], 1718024037);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[529], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[530], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[531], 2863316907);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[532], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[533], 859027251);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[534], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[535], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[536], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[537], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[538], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[539], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[540], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[541], 859027251);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[542], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[543], 46339);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[544], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[545], 2863316907);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[546], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[547], 859027255);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[548], 1718024039);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[549], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[550], 46343);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[551], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[552], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[553], 4294967294);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[554], 1431655767);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[555], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[556], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[557], 4294926334);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[558], 1431696726);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[559], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[560], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[561], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[562], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[563], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[564], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[565], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[566], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[567], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[568], 1431655766);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[569], 2004318070);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[570], 2004318071);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[571], 4294926335);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[572], 1431696727);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[573], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[574], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[575], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[576], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[577], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[578], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[579], 4294961919);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[580], 2863316911);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[581], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[582], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[583], 2863311531);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[584], 2863311535);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[585], 3149642683);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[586], 4008636143);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[587], 2863316907);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[588], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[589], 4294965247);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[590], 4294967295);
}


TEST_F(or_01, opcode__or___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[591], 858993458);
}

