// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/slt-01.h"

namespace
{
class slt_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::slt_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::slt_01::generateCode(true), scalarInitData);
        
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

std::vector<uint8_t> slt_01::s_ScalarData;
}


// Generate google tests

TEST_F(slt_01, opcode__slt___op1_x5__op2_x10__dest_x7__op1val__0x81___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(slt_01, opcode__slt___op1_x0__op2_x6__dest_x0__op1val_0x0___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[6], 0);
}


TEST_F(slt_01, opcode__slt___op1_x9__op2_x8__dest_x8__op1val_0x200___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[7], 0);
}


TEST_F(slt_01, opcode__slt___op1_x4__op2_x4__dest_x11__op1val__0x1001___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[8], 0);
}


TEST_F(slt_01, opcode__slt___op1_x3__op2_x3__dest_x3__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(slt_01, opcode__slt___op1_x1__op2_x0__dest_x4__op1val__0x10001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[10], 1);
}


TEST_F(slt_01, opcode__slt___op1_x6__op2_x2__dest_x5__op1val__0x2000001___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[11], 0);
}


TEST_F(slt_01, opcode__slt___op1_x7__op2_x11__dest_x13__op1val__0x2___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(slt_01, opcode__slt___op1_x15__op2_x7__dest_x12__op1val_0x1000___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(slt_01, opcode__slt___op1_x13__op2_x5__dest_x2__op1val__0x10000001___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[14], 1);
}


TEST_F(slt_01, opcode__slt___op1_x2__op2_x9__dest_x15__op1val__0x55555556___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[15], 1);
}


TEST_F(slt_01, opcode__slt___op1_x8__op2_x15__dest_x14__op1val__0x201___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[16], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x13__dest_x9__op1val_0x80000___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(slt_01, opcode__slt___op1_x12__op2_x14__dest_x6__op1val_0x0___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[18], 0);
}


TEST_F(slt_01, opcode__slt___op1_x14__op2_x1__dest_x10__op1val__0x801___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[19], 0);
}


TEST_F(slt_01, opcode__slt___op1_x11__op2_x12__dest_x1__op1val_0x0___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[21], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[22], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[23], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[24], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[25], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[26], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[27], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[28], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[29], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[30], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[31], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[32], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[33], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[34], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x9___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[35], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[36], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x7fffffff___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[37], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[38], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[39], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[40], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[41], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[42], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[43], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[44], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[45], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[46], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[47], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[48], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val__0x4) {
  ASSERT_EQ(getScalarWordData()[49], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[50], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[51], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[52], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[53], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[54], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val__0x6) {
  ASSERT_EQ(getScalarWordData()[55], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[56], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[57], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[58], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[59], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[60], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[61], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[62], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x7___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[63], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[64], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[65], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[66], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[67], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[68], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[69], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[70], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[71], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[72], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[73], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[74], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3fffffff___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[75], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[76], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[77], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[78], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[79], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[80], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[81], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[82], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[83], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[84], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[85], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[86], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[87], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[88], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[89], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[90], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[91], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[92], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val__0x40000000) {
  ASSERT_EQ(getScalarWordData()[93], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[94], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[95], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[96], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[97], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[98], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[99], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[100], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[101], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[102], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[103], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[104], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[105], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[106], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[107], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[108], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[109], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[110], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[111], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[112], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[113], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[114], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[115], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[116], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[117], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[118], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[119], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[120], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[121], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[122], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[123], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[124], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[125], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[126], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[127], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[128], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[129], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[130], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[131], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[132], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[133], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[134], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[135], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[136], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[137], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[138], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[139], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[140], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[141], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[142], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[143], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[144], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[145], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[146], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[147], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[148], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[149], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[150], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[151], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[152], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[153], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[154], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[155], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[156], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[157], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[158], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[159], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[160], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[161], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[162], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[163], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[164], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[165], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[166], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[167], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[168], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[169], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[170], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[171], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[172], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[173], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[174], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[175], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[176], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[177], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[178], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[179], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[180], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[181], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[182], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[183], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[184], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[185], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[186], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[187], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[188], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[189], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[190], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[191], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[192], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[193], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[194], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[195], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[196], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[197], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[198], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[199], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[200], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[201], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[202], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[203], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[204], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[205], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[206], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[207], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[208], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[209], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[210], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[211], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[212], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[213], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[214], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[215], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[216], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[217], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[218], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[219], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[220], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[221], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[222], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[223], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[224], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[225], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[226], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[227], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[228], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[229], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[230], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[231], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[232], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[233], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[234], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[235], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[236], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[237], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[238], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[239], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[240], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[241], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[242], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[243], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[244], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[245], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[246], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[247], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[248], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[249], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[250], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[251], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[252], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[253], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[254], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[255], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[256], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[257], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[258], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[259], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[260], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[261], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[262], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[263], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[264], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[265], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[266], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[267], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[268], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[269], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[270], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[271], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[272], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[273], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[274], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[275], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[276], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[277], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[278], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[279], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[280], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[281], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[282], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[283], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[284], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[285], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[286], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[287], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[288], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[289], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[290], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[291], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[292], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[293], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[294], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[295], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[296], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[297], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[298], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[299], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[300], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[301], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[302], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[303], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[304], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[305], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[306], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[307], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[308], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[309], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[310], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[311], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[312], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[313], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[314], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[315], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[316], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[317], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[318], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[319], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[320], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[321], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[322], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[323], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[324], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[325], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[326], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[327], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[328], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[329], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[330], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[331], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[332], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[333], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[334], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[335], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[336], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[337], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[338], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[339], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[340], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[341], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[342], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[343], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[344], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[345], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[346], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[347], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[348], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[349], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[350], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[351], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[352], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[353], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[354], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[355], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[356], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[357], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[358], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[359], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[360], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[361], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[362], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[363], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[364], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[365], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[366], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[367], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[368], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[369], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[370], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[371], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[372], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[373], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[374], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[375], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[376], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[377], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[378], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[379], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[380], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[381], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[382], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[383], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[384], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[385], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[386], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[387], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[388], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[389], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[390], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[391], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[392], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[393], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[394], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[395], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[396], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[397], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[398], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[399], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[400], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[401], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[402], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[403], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[404], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[405], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[406], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[407], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[408], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[409], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[410], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[411], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[412], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[413], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[414], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[415], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[416], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[417], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[418], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[419], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[420], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[421], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[422], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[423], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[424], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[425], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[426], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[427], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[428], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[429], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[430], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[431], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[432], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[433], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[434], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[435], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[436], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[437], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[438], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[439], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[440], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[441], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[442], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[443], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[444], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[445], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[446], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[447], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[448], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[449], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[450], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[451], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[452], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[453], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[454], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[455], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[456], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[457], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[458], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[459], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[460], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[461], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[462], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[463], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[464], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[465], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[466], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[467], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[468], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[469], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[470], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[471], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[472], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[473], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[474], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[475], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[476], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[477], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[478], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[479], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[480], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[481], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[482], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[483], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[484], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[485], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[486], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[487], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[488], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[489], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[490], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[491], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[492], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[493], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[494], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[495], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[496], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[497], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[498], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[499], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[500], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[501], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[502], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[503], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[504], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[505], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[506], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[507], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[508], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[509], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[510], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[511], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[512], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[513], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[514], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[515], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[516], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[517], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[518], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[519], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[520], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[521], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[522], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[523], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[524], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[525], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[526], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[527], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[528], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[529], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[530], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[531], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[532], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[533], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[534], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[535], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[536], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[537], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[538], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[539], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[540], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[541], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[542], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[543], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[544], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[545], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[546], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[547], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[548], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[549], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[550], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[551], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[552], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[553], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[554], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[555], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[556], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[557], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[558], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[559], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[560], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[561], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[562], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[563], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[564], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[565], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[566], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[567], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[568], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[569], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[570], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[571], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[572], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[573], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[574], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[575], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[576], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[577], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[578], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[579], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[580], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[581], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[582], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[583], 0);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[584], 1);
}


TEST_F(slt_01, opcode__slt___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[585], 0);
}

