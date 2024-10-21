// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/andi-01.h"

namespace
{
class andi_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::andi_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::andi_01::generateCode(true), scalarInitData);
        
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

std::vector<uint8_t> andi_01::s_ScalarData;
}


// Generate google tests

TEST_F(andi_01, opcode__andi___op1_x22__dest_x10__op1val__0x2___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[5], 4294965248);
}


TEST_F(andi_01, opcode__andi___op1_x25__dest_x25__op1val__0x1001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[6], 0);
}


TEST_F(andi_01, opcode__andi___op1_x16__dest_x17__op1val__0x2000001___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[7], 2047);
}


TEST_F(andi_01, opcode__andi___op1_x2__dest_x8__op1val__0x20001___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[8], 1);
}


TEST_F(andi_01, opcode__andi___op1_x28__dest_x30__op1val__0x80000000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(andi_01, opcode__andi___op1_x4__dest_x19__op1val_0x0___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x2__op1val_0x7fffffff___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[11], 6);
}


TEST_F(andi_01, opcode__andi___op1_x7__dest_x13__op1val_0x1___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(andi_01, opcode__andi___op1_x27__dest_x9__op1val_0x80___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[13], 128);
}


TEST_F(andi_01, opcode__andi___op1_x17__dest_x3__op1val_0x7fffffff___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[14], 2147483604);
}


TEST_F(andi_01, opcode__andi___op1_x0__dest_x26__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(andi_01, opcode__andi___op1_x23__dest_x21__op1val_0x66666666___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[16], 0);
}


TEST_F(andi_01, opcode__andi___op1_x6__dest_x14__op1val_0x0___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(andi_01, opcode__andi___op1_x5__dest_x22__op1val_0x100___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[18], 0);
}


TEST_F(andi_01, opcode__andi___op1_x8__dest_x29__op1val__0x5___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[19], 64);
}


TEST_F(andi_01, opcode__andi___op1_x12__dest_x23__op1val_0x1___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(andi_01, opcode__andi___op1_x15__dest_x6__op1val__0x55555555___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[21], 512);
}


TEST_F(andi_01, opcode__andi___op1_x29__dest_x11__op1val_0x0___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[22], 0);
}


TEST_F(andi_01, opcode__andi___op1_x20__dest_x1__op1val_0x66666667___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[23], 1717986918);
}


TEST_F(andi_01, opcode__andi___op1_x31__dest_x5__op1val__0x100001___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[24], 4293918717);
}


TEST_F(andi_01, opcode__andi___op1_x3__dest_x20__op1val_0x200___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[25], 512);
}


TEST_F(andi_01, opcode__andi___op1_x9__dest_x7__op1val_0x4000___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[26], 16384);
}


TEST_F(andi_01, opcode__andi___op1_x13__dest_x16__op1val_0x200000___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[27], 2097152);
}


TEST_F(andi_01, opcode__andi___op1_x18__dest_x24__op1val__0x3___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[28], 4294967261);
}


TEST_F(andi_01, opcode__andi___op1_x26__dest_x4__op1val__0x6___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[29], 4294967226);
}


TEST_F(andi_01, opcode__andi___op1_x30__dest_x28__op1val_0x40___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[30], 64);
}


TEST_F(andi_01, opcode__andi___op1_x19__dest_x0__op1val__0xb504___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[31], 0);
}


TEST_F(andi_01, opcode__andi___op1_x21__dest_x18__op1val__0x8001___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[32], 4294934015);
}


TEST_F(andi_01, opcode__andi___op1_x1__dest_x12__op1val_0x40___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[33], 64);
}


TEST_F(andi_01, opcode__andi___op1_x24__dest_x27__op1val__0x4001___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[34], 1365);
}


TEST_F(andi_01, opcode__andi___op1_x11__dest_x31__op1val_0x40000000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[35], 1073741824);
}


TEST_F(andi_01, opcode__andi___op1_x14__dest_x15__op1val_0x2___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[36], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[37], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x8___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[38], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x10___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[39], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x20___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[40], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x400___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[41], 1024);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x800___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[42], 2048);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x1000___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[43], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2000___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[44], 8192);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x8000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[45], 32768);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x10000___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[46], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x20000___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[47], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x40000___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[48], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x80000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[49], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x100000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[50], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x400000___immval__0x1) {
  ASSERT_EQ(getScalarWordData()[51], 4194304);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x800000___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[52], 8388608);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2000000___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[53], 33554432);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4000000___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[54], 67108864);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x8000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[55], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x10000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[56], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x20000000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[57], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x9___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[58], 4294967285);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x11___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[59], 1350);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x21___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[60], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x41___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[61], 9);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x81___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[62], 4294967165);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x101___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[63], 564);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x201___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[64], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x401___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[65], 64);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x801___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[66], 9);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x2001___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[67], 1639);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x10001___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[68], 2047);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x40001___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[69], 4294705107);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x80001___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[70], 512);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x200001___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[71], 4292870100);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x400001___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[72], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x800001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[73], 7);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x1000001___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[74], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x4000001___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[75], 8);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x8000001___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[76], 46);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x10000001___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[77], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x20000001___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[78], 3758096340);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x40000001___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[79], 1366);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[80], 1431655761);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[81], 128);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[82], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[83], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[84], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[85], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[86], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[87], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[88], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[89], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[90], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[91], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[92], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[93], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[94], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[95], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[96], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[97], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[98], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[99], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[100], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[101], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[102], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[103], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[104], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[105], 1365);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[106], 1431654400);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[107], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[108], 273);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[109], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[110], 1431655761);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[111], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[112], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[113], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[114], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[115], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[116], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[117], 1093);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[118], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[119], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[120], 1431654401);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[121], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[122], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[123], 1093);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[124], 1431655764);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[125], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[126], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[127], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[128], 2863311530);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[129], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[130], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[131], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[132], 2863311490);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[133], 40);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[134], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[135], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[136], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[137], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[138], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[139], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[140], 40);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[141], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[142], 2863311530);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[143], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[144], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[145], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[146], 2863311488);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[147], 42);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[148], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[149], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[150], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[151], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[152], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[153], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[154], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[155], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[156], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[157], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[158], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[159], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[160], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[161], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[162], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[163], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[164], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[165], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[166], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[167], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[168], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[169], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[170], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[171], 273);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[172], 858993186);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[173], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[174], 819);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[175], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[176], 858993427);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[177], 33);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[178], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[179], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[180], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[181], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[182], 818);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[183], 545);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[184], 32);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[185], 274);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[186], 858993187);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[187], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[188], 816);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[189], 547);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[190], 858993424);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[191], 34);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[192], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[193], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[194], 1717985826);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[195], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[196], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[197], 1638);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[198], 1717986882);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[199], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[200], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[201], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[202], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[203], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[204], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[205], 1636);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[206], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[207], 1094);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[208], 1717985826);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[209], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[210], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[211], 1638);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[212], 1717986884);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[213], 38);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[214], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[215], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[216], 4294920872);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[217], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[218], 560);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[219], 612);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[220], 4294920912);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[221], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[222], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[223], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[224], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[225], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[226], 560);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[227], 612);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[228], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[229], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[230], 4294920872);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[231], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[232], 564);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[233], 612);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[234], 4294920916);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[235], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[236], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[237], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[238], 45056);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[239], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[240], 256);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[241], 1028);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[242], 46336);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[243], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[244], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[245], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[246], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[247], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[248], 256);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[249], 1028);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[250], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[251], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[252], 45056);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[253], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[254], 260);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[255], 1028);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[256], 46340);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[257], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[258], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[259], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[260], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[261], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[262], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[263], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[264], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[265], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[266], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[267], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[268], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[269], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[270], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[271], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[272], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[273], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[274], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[275], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[276], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[277], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[278], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[279], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[280], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[281], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[282], 1431654400);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[283], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[284], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[285], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[286], 1431655760);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[287], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[288], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[289], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[290], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[291], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[292], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[293], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[294], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[295], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[296], 1431654400);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[297], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[298], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[299], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[300], 1431655764);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[301], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[302], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[303], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[304], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[305], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[306], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[307], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[308], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[309], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[310], 816);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[311], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[312], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[313], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[314], 858993184);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[315], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[316], 820);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[317], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[318], 858993428);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[319], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[320], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[321], 1093);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[322], 1717985826);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[323], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[324], 547);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[325], 1638);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[326], 1717986883);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[327], 37);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[328], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[329], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[330], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[331], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[332], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[333], 1637);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[334], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[335], 1094);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[336], 1717985827);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[337], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[338], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[339], 1639);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[340], 1717986884);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[341], 38);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[342], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[343], 85);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[344], 4294920872);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[345], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[346], 561);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[347], 612);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[348], 4294920913);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[349], 45);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[350], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[351], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[352], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[353], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[354], 560);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[355], 613);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[356], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[357], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[358], 4294920873);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[359], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[360], 564);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[361], 613);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[362], 4294920916);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[363], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[364], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[365], 1285);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[366], 45056);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[367], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[368], 257);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[369], 1028);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[370], 46337);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[371], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[372], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[373], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[374], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[375], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[376], 256);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[377], 1029);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[378], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[379], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[380], 45057);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[381], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[382], 260);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[383], 1029);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[384], 46340);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[385], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[386], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[387], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[388], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[389], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[390], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[391], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[392], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[393], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[394], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[395], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[396], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[397], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[398], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[399], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[400], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[401], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[402], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[403], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[404], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[405], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[406], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[407], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[408], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[409], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[410], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[411], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[412], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[413], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[414], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[415], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[416], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[417], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[418], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[419], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[420], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[421], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[422], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[423], 858993186);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[424], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[425], 818);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[426], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[427], 858993426);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[428], 32);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[429], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[430], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[431], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[432], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[433], 818);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[434], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[435], 32);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[436], 274);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[437], 858993186);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[438], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[439], 816);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[440], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[441], 858993424);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[442], 34);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[443], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[444], 1093);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[445], 1717985824);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[446], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[447], 545);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[448], 1636);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[449], 1717986881);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[450], 37);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[451], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[452], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[453], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[454], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[455], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[456], 1637);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[457], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[458], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[459], 1717985825);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[460], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[461], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[462], 1637);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[463], 1717986884);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[464], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[465], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[466], 1281);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[467], 45058);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[468], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[469], 259);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[470], 1026);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[471], 46339);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[472], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[473], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[474], 1280);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[475], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[476], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[477], 258);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[478], 1025);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[479], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[480], 1282);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[481], 45059);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[482], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[483], 256);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[484], 1027);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[485], 46336);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[486], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[487], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[488], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[489], 1431654402);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[490], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[491], 274);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[492], 1094);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[493], 1431655762);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[494], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[495], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[496], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[497], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[498], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[499], 274);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[500], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[501], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[502], 1366);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[503], 1431654402);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[504], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[505], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[506], 1094);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[507], 1431655764);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[508], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[509], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[510], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[511], 2863311530);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[512], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[513], 547);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[514], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[515], 2863311491);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[516], 41);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[517], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[518], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[519], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[520], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[521], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[522], 545);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[523], 40);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[524], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[525], 2863311531);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[526], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[527], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[528], 547);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[529], 2863311488);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[530], 42);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[531], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[532], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[533], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[534], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[535], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[536], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[537], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[538], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[539], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[540], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[541], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[542], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[543], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[544], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[545], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[546], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[547], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[548], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[549], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[550], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[551], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[552], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[553], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[554], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[555], 858993184);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[556], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[557], 816);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[558], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[559], 858993424);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[560], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[561], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[562], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[563], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[564], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x1000000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[565], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[566], 4294920956);
}

