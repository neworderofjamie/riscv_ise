// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/slti-01.h"

namespace
{
class slti_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::slti_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::slti_01::generateCode().getCode(), scalarInitData);
        
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

std::vector<uint8_t> slti_01::s_ScalarData;
}


// Generate google tests

TEST_F(slti_01, opcode__slti___op1_x25__dest_x12__op1val__0x81___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[4], 0);
}


TEST_F(slti_01, opcode__slti___op1_x5__dest_x5__op1val__0x1001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[5], 1);
}


TEST_F(slti_01, opcode__slti___op1_x4__dest_x28__op1val__0x40000000___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[6], 1);
}


TEST_F(slti_01, opcode__slti___op1_x31__dest_x15__op1val__0x11___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[7], 1);
}


TEST_F(slti_01, opcode__slti___op1_x1__dest_x13__op1val__0x80000000___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[8], 1);
}


TEST_F(slti_01, opcode__slti___op1_x15__dest_x1__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[9], 1);
}


TEST_F(slti_01, opcode__slti___op1_x16__dest_x9__op1val_0x7fffffff___immval__0x8) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(slti_01, opcode__slti___op1_x11__dest_x31__op1val_0x1___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[11], 0);
}


TEST_F(slti_01, opcode__slti___op1_x14__dest_x27__op1val_0x10___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(slti_01, opcode__slti___op1_x12__dest_x26__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(slti_01, opcode__slti___op1_x17__dest_x4__op1val_0x3fffffff___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[14], 0);
}


TEST_F(slti_01, opcode__slti___op1_x18__dest_x10__op1val__0x2001___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[15], 1);
}


TEST_F(slti_01, opcode__slti___op1_x27__dest_x21__op1val_0x3___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[16], 1);
}


TEST_F(slti_01, opcode__slti___op1_x3__dest_x8__op1val_0x55555554___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(slti_01, opcode__slti___op1_x7__dest_x0__op1val_0x55555554___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[18], 0);
}


TEST_F(slti_01, opcode__slti___op1_x22__dest_x24__op1val__0x55555556___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[19], 1);
}


TEST_F(slti_01, opcode__slti___op1_x24__dest_x18__op1val_0x4000___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(slti_01, opcode__slti___op1_x6__dest_x25__op1val__0x401___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[21], 1);
}


TEST_F(slti_01, opcode__slti___op1_x21__dest_x23__op1val_0x66666667___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[22], 0);
}


TEST_F(slti_01, opcode__slti___op1_x0__dest_x7__op1val_0x0___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[23], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x20__op1val__0xb504___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[24], 1);
}


TEST_F(slti_01, opcode__slti___op1_x26__dest_x16__op1val_0x3fffffff___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[25], 0);
}


TEST_F(slti_01, opcode__slti___op1_x20__dest_x29__op1val__0x80001___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[26], 1);
}


TEST_F(slti_01, opcode__slti___op1_x13__dest_x3__op1val__0x10000001___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[27], 1);
}


TEST_F(slti_01, opcode__slti___op1_x29__dest_x14__op1val_0x800000___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[28], 0);
}


TEST_F(slti_01, opcode__slti___op1_x23__dest_x2__op1val__0x10000001___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[29], 1);
}


TEST_F(slti_01, opcode__slti___op1_x2__dest_x22__op1val_0x0___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[30], 0);
}


TEST_F(slti_01, opcode__slti___op1_x30__dest_x19__op1val__0x40001___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[31], 1);
}


TEST_F(slti_01, opcode__slti___op1_x19__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[32], 1);
}


TEST_F(slti_01, opcode__slti___op1_x8__dest_x30__op1val__0x40000000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[33], 1);
}


TEST_F(slti_01, opcode__slti___op1_x9__dest_x17__op1val_0x2___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[34], 1);
}


TEST_F(slti_01, opcode__slti___op1_x28__dest_x6__op1val_0x8___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[35], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x20___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[36], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x40___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[37], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x80___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[38], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x100___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[39], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x200___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[40], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x400___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[41], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x800___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[42], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x1000___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[43], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[44], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x8000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[45], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x10000___immval__0x1) {
  ASSERT_EQ(getScalarWordData()[46], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x20000___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[47], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x40000___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[48], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x80000___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[49], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x100000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[50], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x200000___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[51], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x400000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[52], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x1000000___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[53], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2000000___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[54], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4000000___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[55], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x8000000___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[56], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x10000000___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[57], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x20000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[58], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x40000000___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[59], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x2___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[60], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[61], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x5___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[62], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x9___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[63], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x21___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[64], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x41___immval__0x4) {
  ASSERT_EQ(getScalarWordData()[65], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x101___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[66], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x201___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[67], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x801___immval__0x8) {
  ASSERT_EQ(getScalarWordData()[68], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x4001___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[69], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x8001___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[70], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x10001___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[71], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x20001___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[72], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x200001___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[73], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x400001___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[74], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x800001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[75], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x1000001___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[76], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x2000001___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[77], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x4000001___immval__0x4) {
  ASSERT_EQ(getScalarWordData()[78], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x8000001___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[79], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x20000001___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[80], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x40000001___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[81], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[82], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[83], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[84], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[85], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[86], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[87], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[88], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[89], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[90], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[91], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[92], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[93], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[94], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[95], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[96], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[97], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[98], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[99], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[100], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[101], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[102], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[103], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[104], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[105], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[106], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[107], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[108], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[109], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[110], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[111], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[112], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[113], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[114], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[115], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[116], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[117], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[118], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[119], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[120], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[121], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[122], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[123], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[124], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[125], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[126], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[127], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[128], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[129], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[130], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[131], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[132], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[133], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[134], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[135], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[136], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[137], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[138], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[139], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[140], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[141], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[142], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[143], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[144], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[145], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[146], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[147], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[148], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[149], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[150], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[151], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[152], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[153], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[154], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[155], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[156], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[157], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[158], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[159], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[160], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[161], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[162], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[163], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[164], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[165], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[166], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[167], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[168], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[169], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[170], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[171], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[172], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[173], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[174], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[175], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[176], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[177], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[178], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[179], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[180], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[181], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[182], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[183], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[184], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[185], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[186], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[187], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[188], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[189], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[190], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[191], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[192], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[193], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[194], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[195], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[196], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[197], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[198], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[199], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[200], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[201], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[202], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[203], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[204], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[205], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[206], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[207], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[208], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[209], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[210], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[211], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[212], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[213], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[214], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[215], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[216], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[217], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[218], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[219], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[220], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[221], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[222], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[223], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[224], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[225], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[226], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[227], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[228], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[229], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[230], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[231], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[232], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[233], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[234], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[235], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[236], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[237], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[238], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[239], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[240], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[241], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[242], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[243], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[244], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[245], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[246], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[247], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[248], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[249], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[250], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[251], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[252], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[253], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[254], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[255], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[256], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[257], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[258], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[259], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[260], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[261], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[262], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[263], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[264], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[265], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[266], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[267], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[268], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[269], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[270], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[271], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[272], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[273], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[274], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[275], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[276], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[277], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[278], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[279], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[280], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[281], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[282], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[283], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[284], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[285], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[286], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[287], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[288], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[289], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[290], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[291], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[292], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[293], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[294], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[295], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[296], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[297], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[298], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[299], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[300], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[301], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[302], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[303], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[304], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[305], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[306], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[307], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[308], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[309], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[310], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[311], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[312], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[313], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[314], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[315], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[316], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[317], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[318], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[319], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[320], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[321], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[322], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[323], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[324], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[325], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[326], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[327], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[328], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[329], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[330], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[331], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[332], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[333], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[334], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[335], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[336], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[337], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[338], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[339], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[340], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[341], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[342], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[343], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[344], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[345], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[346], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[347], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[348], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[349], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[350], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[351], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[352], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[353], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[354], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[355], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[356], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[357], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[358], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[359], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[360], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[361], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[362], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[363], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[364], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[365], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[366], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[367], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[368], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[369], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[370], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[371], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[372], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[373], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[374], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[375], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[376], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[377], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[378], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[379], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[380], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[381], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[382], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[383], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[384], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[385], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[386], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[387], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[388], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[389], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[390], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[391], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[392], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[393], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[394], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[395], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[396], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[397], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[398], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[399], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[400], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[401], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[402], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[403], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[404], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[405], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[406], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[407], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[408], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[409], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[410], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[411], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[412], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[413], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[414], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[415], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[416], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[417], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[418], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[419], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[420], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[421], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[422], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[423], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[424], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[425], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[426], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[427], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[428], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[429], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[430], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[431], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[432], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[433], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[434], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[435], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[436], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[437], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[438], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[439], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[440], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[441], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[442], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[443], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[444], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[445], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[446], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[447], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[448], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[449], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[450], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[451], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[452], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[453], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[454], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[455], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[456], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[457], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[458], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[459], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[460], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[461], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[462], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[463], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[464], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[465], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[466], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[467], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[468], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[469], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[470], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[471], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[472], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[473], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[474], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[475], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[476], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[477], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[478], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[479], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[480], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[481], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[482], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[483], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[484], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[485], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[486], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[487], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[488], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[489], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[490], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[491], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[492], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[493], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[494], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[495], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[496], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[497], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[498], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[499], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[500], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[501], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[502], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[503], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[504], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[505], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[506], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[507], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[508], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[509], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[510], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[511], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[512], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[513], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[514], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[515], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[516], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[517], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[518], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[519], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[520], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[521], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[522], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[523], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[524], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[525], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[526], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[527], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[528], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[529], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[530], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[531], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[532], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[533], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[534], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[535], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[536], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[537], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[538], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[539], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[540], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[541], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[542], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[543], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[544], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[545], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[546], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[547], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[548], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[549], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[550], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[551], 1);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[552], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[553], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[554], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[555], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[556], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[557], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[558], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[559], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[560], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[561], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[562], 0);
}


TEST_F(slti_01, opcode__slti___op1_x10__dest_x11__op1val__0x100001___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[563], 1);
}

