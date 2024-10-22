// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/add-01.h"

namespace
{
class add_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::add_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::add_01::generateCode(true), scalarInitData);
        
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

std::vector<uint8_t> add_01::s_ScalarData;
}


// Generate google tests

TEST_F(add_01, opcode__add___op1_x4__op2_x24__dest_x24__op1val_0x7fffffff___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[5], 2147483648);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x10__dest_x28__op1val_0x20000___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[6], 262144);
}


TEST_F(add_01, opcode__add___op1_x21__op2_x21__dest_x21__op1val__0x1000001___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[7], 4261412862);
}


TEST_F(add_01, opcode__add___op1_x22__op2_x31__dest_x22__op1val__0x2___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[8], 262142);
}


TEST_F(add_01, opcode__add___op1_x12__op2_x6__dest_x11__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[9], 2863311532);
}


TEST_F(add_01, opcode__add___op1_x29__op2_x13__dest_x10__op1val_0x2___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[10], 2147483650);
}


TEST_F(add_01, opcode__add___op1_x31__op2_x5__dest_x26__op1val__0x11___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[11], 4294967279);
}


TEST_F(add_01, opcode__add___op1_x2__op2_x1__dest_x7__op1val_0x66666666___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[12], 3865470565);
}


TEST_F(add_01, opcode__add___op1_x8__op2_x25__dest_x14__op1val__0x80000000___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[13], 715827882);
}


TEST_F(add_01, opcode__add___op1_x13__op2_x8__dest_x1__op1val_0x0___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[14], 4261412863);
}


TEST_F(add_01, opcode__add___op1_x28__op2_x9__dest_x0__op1val_0x1___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(add_01, opcode__add___op1_x14__op2_x4__dest_x20__op1val_0x7___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[16], 9);
}


TEST_F(add_01, opcode__add___op1_x7__op2_x19__dest_x16__op1val_0x8___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[17], 12);
}


TEST_F(add_01, opcode__add___op1_x23__op2_x29__dest_x8__op1val_0x800___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[18], 2056);
}


TEST_F(add_01, opcode__add___op1_x5__op2_x27__dest_x13__op1val_0x0___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[19], 16);
}


TEST_F(add_01, opcode__add___op1_x25__op2_x20__dest_x27__op1val_0x55555556___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[20], 1431655798);
}


TEST_F(add_01, opcode__add___op1_x15__op2_x26__dest_x17__op1val__0x11___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[21], 47);
}


TEST_F(add_01, opcode__add___op1_x17__op2_x2__dest_x29__op1val__0x5___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[22], 123);
}


TEST_F(add_01, opcode__add___op1_x24__op2_x17__dest_x4__op1val_0x20___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[23], 288);
}


TEST_F(add_01, opcode__add___op1_x16__op2_x11__dest_x2__op1val_0x40000000___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[24], 1073742336);
}


TEST_F(add_01, opcode__add___op1_x0__op2_x16__dest_x6__op1val_0x0___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[25], 1024);
}


TEST_F(add_01, opcode__add___op1_x11__op2_x3__dest_x31__op1val__0x55555555___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[26], 2863313579);
}


TEST_F(add_01, opcode__add___op1_x18__op2_x0__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[27], 4);
}


TEST_F(add_01, opcode__add___op1_x30__op2_x7__dest_x9__op1val__0x2000001___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[28], 4261421055);
}


TEST_F(add_01, opcode__add___op1_x3__op2_x30__dest_x5__op1val_0xb503___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[29], 62723);
}


TEST_F(add_01, opcode__add___op1_x27__op2_x28__dest_x18__op1val__0xb503___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[30], 4294953725);
}


TEST_F(add_01, opcode__add___op1_x9__op2_x14__dest_x15__op1val__0x201___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[31], 65023);
}


TEST_F(add_01, opcode__add___op1_x26__op2_x15__dest_x3__op1val_0x40___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[32], 131136);
}


TEST_F(add_01, opcode__add___op1_x1__op2_x18__dest_x23__op1val_0x3___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[33], 524291);
}


TEST_F(add_01, opcode__add___op1_x6__op2_x12__dest_x30__op1val__0x10001___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[34], 983039);
}


TEST_F(add_01, opcode__add___op1_x20__op2_x22__dest_x19__op1val__0x400001___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[35], 4292870143);
}


TEST_F(add_01, opcode__add___op1_x19__op2_x23__dest_x25__op1val_0x400000___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[36], 8388608);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[37], 16730877);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[38], 33423359);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[39], 67108864);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[40], 134184959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[41], 1342177280);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[42], 536854527);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[43], 1073807360);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[44], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[45], 4294967260);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[46], 4294967258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[47], 4294836214);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[48], 2031);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[49], 536870879);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[50], 2147483582);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[51], 4286578558);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[52], 4294920443);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[53], 4294962174);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[54], 4294965251);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[55], 4294916860);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[56], 38147);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[57], 2147467262);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[58], 4294934524);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[59], 4294948098);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[60], 2863180457);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[61], 16515071);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[62], 2146959358);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[63], 4293920767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[64], 4292870151);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[65], 4290781183);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[66], 4286578654);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[67], 4276092926);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[68], 4227858433);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[69], 1297438036);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[70], 4026662911);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[71], 3758227455);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x7___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[72], 3221225478);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[73], 1432704341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[74], 858993475);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[75], 858993586);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0xa) {
  ASSERT_EQ(getScalarWordData()[76], 246);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[77], 4294921469);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[78], 1431656789);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[79], 4101);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[80], 14335);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[81], 4278222847);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[82], 17039360);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[83], 557056);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[84], 270532608);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[85], 8387583);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[86], 1465210197);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[87], 67141632);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[88], 993211186);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val__0x40000000) {
  ASSERT_EQ(getScalarWordData()[89], 3489660928);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[90], 4294442998);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[91], 2147483583);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[92], 2863311401);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[93], 4294967042);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val__0x8) {
  ASSERT_EQ(getScalarWordData()[94], 4294966263);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[95], 4294965250);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[96], 4294959105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[97], 4286316542);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[98], 4294489347);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val__0x40000000) {
  ASSERT_EQ(getScalarWordData()[99], 3220176895);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[100], 1650878052);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[101], 3892314110);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[102], 4026533887);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[103], 3758227455);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[104], 3221225342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[105], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[106], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[107], 2863311533);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[108], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[109], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[110], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[111], 4294920959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[112], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[113], 5);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[114], 1431655767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[115], 3);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[116], 7);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[117], 858993461);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[118], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[119], 46342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[120], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[121], 2863311534);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[122], 9);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[123], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[124], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[125], 4294920960);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[126], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[127], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[128], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[129], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[130], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[131], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[132], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[133], 1431609425);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[134], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[135], 1431655767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[136], 2863311529);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[137], 1431655765);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[138], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[139], 2290649223);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[140], 3149642682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[141], 1431702104);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[142], 2863311531);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[143], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[144], 1431655771);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[145], 2290649225);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[146], 3149642684);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[147], 1431609426);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[148], 1431702106);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[149], 2863311533);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[150], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[151], 1431655764);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[152], 2863311535);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[153], 3722304989);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[154], 286331152);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[155], 2863265190);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[156], 2863357870);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[157], 2863311532);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[158], 4294967294);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[159], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[160], 2863311534);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[161], 3722304988);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[162], 286331151);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[163], 2863357869);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[164], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[165], 1431655765);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[166], 2863311536);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[167], 3722304990);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[168], 286331153);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[169], 2863265191);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[170], 2863357871);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[171], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[172], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[173], 2863311535);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[174], 10);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[175], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[176], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[177], 4294920961);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[178], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[179], 7);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[180], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[181], 5);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[182], 9);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[183], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[184], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[185], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[186], 1431655771);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[187], 2863311536);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[188], 11);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[189], 858993465);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[190], 1717986924);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[191], 4294920962);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[192], 46346);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[193], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[194], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[195], 3722304989);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[196], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[197], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[198], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[199], 858947119);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[200], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[201], 858993461);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[202], 2290649223);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[203], 858993459);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[204], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[205], 1717986917);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[206], 2576980376);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[207], 859039798);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[208], 2290649225);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[209], 3722304990);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[210], 858993465);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[211], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[212], 2576980378);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[213], 858947120);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[214], 859039800);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[215], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[216], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[217], 286331152);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[218], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[219], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[220], 3435973836);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[221], 1717940578);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[222], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[223], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[224], 3149642682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[225], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[226], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[227], 2576980376);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[228], 3435973835);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[229], 1718033257);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[230], 3149642684);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[231], 286331153);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[232], 1717986924);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[233], 2576980378);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[234], 3435973837);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[235], 1717940579);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[236], 1718033259);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[237], 4294920959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[238], 1431609425);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[239], 2863265190);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[240], 4294920961);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[241], 858947119);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[242], 1717940578);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[243], 4294874616);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[244], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[245], 4294920958);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[246], 1431609424);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[247], 4294920956);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[248], 4294920960);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[249], 858947118);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[250], 1717940577);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[251], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[252], 1431609426);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[253], 2863265191);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[254], 4294920962);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[255], 858947120);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[256], 1717940579);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[257], 4294874617);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[258], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[259], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[260], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[261], 2863357870);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[262], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[263], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[264], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[265], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[266], 92680);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[267], 46342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[268], 1431702104);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[269], 46340);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[270], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[271], 859039798);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[272], 1718033257);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[273], 92679);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[274], 1431702106);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[275], 2863357871);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[276], 46346);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[277], 859039800);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[278], 1718033259);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[279], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[280], 92681);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[281], 5);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[282], 1431655767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[283], 2863311532);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[284], 7);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[285], 858993461);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[286], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[287], 4294920958);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[288], 46342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[289], 4);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[290], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[291], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[292], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[293], 858993460);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[294], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[295], 46341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[296], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[297], 2863311533);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[298], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[299], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[300], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[301], 4294920959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[302], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[303], 1431655767);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[304], 2863311529);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[305], 4294967294);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[306], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[307], 2290649223);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[308], 3149642682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[309], 1431609424);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[310], 1431702104);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[311], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[312], 2863311528);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[313], 1431655764);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[314], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[315], 2290649222);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[316], 3149642681);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[317], 1431702103);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[318], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[319], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[320], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[321], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[322], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[323], 1431609425);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[324], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[325], 3);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[326], 1431655765);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[327], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[328], 5);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[329], 858993459);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[330], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[331], 4294920956);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[332], 46340);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[333], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[334], 2863311537);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[335], 3722304991);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[336], 286331154);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[337], 2863265192);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[338], 2863357872);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[339], 9);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[340], 1431655771);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[341], 2863311536);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[342], 11);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[343], 858993465);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[344], 1717986924);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[345], 4294920962);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[346], 46346);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[347], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[348], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[349], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[350], 10);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[351], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[352], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[353], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[354], 1431655772);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[355], 2863311537);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[356], 12);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[357], 858993466);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[358], 1717986925);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[359], 4294920963);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[360], 46347);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[361], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[362], 2290649225);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[363], 3722304990);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[364], 858993465);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[365], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[366], 2576980378);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[367], 858947120);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[368], 859039800);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[369], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[370], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[371], 858993460);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[372], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[373], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[374], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[375], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[376], 2290649226);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[377], 3722304991);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[378], 858993466);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[379], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[380], 2576980379);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[381], 858947121);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[382], 859039801);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[383], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[384], 3149642684);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[385], 286331153);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[386], 1717986924);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[387], 2576980378);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[388], 3435973837);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[389], 1717940579);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[390], 1718033259);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[391], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[392], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[393], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[394], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[395], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[396], 3435973836);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[397], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[398], 3149642685);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[399], 286331154);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[400], 1717986925);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[401], 2576980379);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[402], 3435973838);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[403], 1717940580);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[404], 1718033260);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[405], 4294920960);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[406], 1431609426);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[407], 2863265191);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[408], 4294920962);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[409], 858947120);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[410], 1717940579);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[411], 4294874617);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[412], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[413], 4294920959);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[414], 1431609425);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[415], 4294920957);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[416], 4294920961);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[417], 858947119);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[418], 1717940578);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[419], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[420], 1431609427);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[421], 2863265192);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[422], 4294920963);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[423], 858947121);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[424], 1717940580);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[425], 4294874618);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[426], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[427], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[428], 1431702106);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[429], 2863357871);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[430], 46346);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[431], 859039800);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[432], 1718033259);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[433], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[434], 92681);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[435], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[436], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[437], 46341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[438], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[439], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[440], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[441], 92680);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[442], 1431702107);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[443], 2863357872);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[444], 46347);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[445], 859039801);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[446], 1718033260);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[447], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[448], 92682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[449], 2);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[450], 1431655764);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[451], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[452], 4);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[453], 858993458);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[454], 1717986917);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[455], 46339);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[456], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[457], 2863311531);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[458], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[459], 858993460);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[460], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[461], 4294920957);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[462], 46341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[463], 7);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[464], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[465], 2863311534);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[466], 9);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[467], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[468], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[469], 4294920960);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[470], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[471], 6);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[472], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[473], 4);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[474], 8);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[475], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[476], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[477], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[478], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[479], 2863311535);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[480], 10);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[481], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[482], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[483], 4294920961);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[484], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[485], 858993461);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[486], 2290649223);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[487], 3722304988);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[488], 858993463);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[489], 1717986917);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[490], 2576980376);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[491], 858947118);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[492], 859039798);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[493], 858993460);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[494], 2290649222);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[495], 858993458);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[496], 858993462);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[497], 1717986916);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[498], 2576980375);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[499], 859039797);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[500], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[501], 3722304989);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[502], 858993464);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[503], 1717986918);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[504], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[505], 858947119);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[506], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[507], 1717986920);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[508], 3149642682);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[509], 286331151);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[510], 1717986922);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[511], 2576980376);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[512], 3435973835);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[513], 1717940577);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[514], 1718033257);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[515], 1717986919);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[516], 3149642681);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[517], 1717986917);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[518], 1717986921);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[519], 2576980375);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[520], 3435973834);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[521], 1718033256);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[522], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[523], 286331152);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[524], 1717986923);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[525], 2576980377);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[526], 3435973836);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[527], 1717940578);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[528], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[529], 46342);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[530], 1431702104);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[531], 2863357869);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[532], 46344);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[533], 859039798);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[534], 1718033257);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[535], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[536], 92679);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[537], 46341);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[538], 1431702103);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[539], 46339);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[540], 46343);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[541], 859039797);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[542], 1718033256);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[543], 92678);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[544], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[545], 2863357870);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[546], 46345);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[547], 859039799);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[548], 1718033258);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[549], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[550], 92680);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[551], 1431655769);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[552], 2863311531);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[553], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[554], 1431655771);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[555], 2290649225);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[556], 3149642684);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[557], 1431609426);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[558], 1431702106);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[559], 1431655768);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[560], 2863311530);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[561], 1431655766);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[562], 1431655770);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[563], 2290649224);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[564], 3149642683);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[565], 1431702105);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[566], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[567], 1431655772);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[568], 2290649226);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[569], 3149642685);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[570], 1431609427);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[571], 1431702107);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[572], 2863311534);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[573], 0);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[574], 1431655765);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[575], 2863311536);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[576], 3722304990);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[577], 286331153);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[578], 2863265191);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[579], 2863357871);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[580], 2863311533);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[581], 4294967295);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[582], 2863311531);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[583], 2863311535);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[584], 3722304989);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[585], 286331152);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[586], 2863357870);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[587], 1);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[588], 130815);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val__0x7) {
  ASSERT_EQ(getScalarWordData()[589], 4278190072);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[590], 8388609);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[591], 4294921980);
}


TEST_F(add_01, opcode__add___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[592], 4100);
}

