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

TEST_F(andi_01, opcode__andi___op1_x14__dest_x11__op1val_0x8___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(andi_01, opcode__andi___op1_x2__dest_x2__op1val_0x7fffffff___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[6], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x5__op1val__0x40000001___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[7], 4);
}


TEST_F(andi_01, opcode__andi___op1_x9__dest_x14__op1val__0x20000001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[8], 7);
}


TEST_F(andi_01, opcode__andi___op1_x12__dest_x3__op1val__0x10000001___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[9], 64);
}


TEST_F(andi_01, opcode__andi___op1_x4__dest_x7__op1val__0x8000001___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[10], 4160749563);
}


TEST_F(andi_01, opcode__andi___op1_x6__dest_x9__op1val__0x4000001___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[11], 818);
}


TEST_F(andi_01, opcode__andi___op1_x7__dest_x15__op1val__0x2000001___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[12], 6);
}


TEST_F(andi_01, opcode__andi___op1_x15__dest_x0__op1val__0x1000001___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(andi_01, opcode__andi___op1_x3__dest_x6__op1val__0x800001___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[14], 2);
}


TEST_F(andi_01, opcode__andi___op1_x8__dest_x10__op1val__0x400001___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[15], 16);
}


TEST_F(andi_01, opcode__andi___op1_x5__dest_x4__op1val__0x200001___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[16], 4292870079);
}


TEST_F(andi_01, opcode__andi___op1_x13__dest_x1__op1val__0x100001___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[17], 6);
}


TEST_F(andi_01, opcode__andi___op1_x1__dest_x8__op1val__0x80001___immval__0x4) {
  ASSERT_EQ(getScalarWordData()[18], 4294443004);
}


TEST_F(andi_01, opcode__andi___op1_x11__dest_x12__op1val__0x40001___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[19], 2047);
}


TEST_F(andi_01, opcode__andi___op1_x0__dest_x13__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[20], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x10001___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[21], 820);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x8001___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[22], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x4001___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[23], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x2001___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[24], 512);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x1001___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[25], 4294961834);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x801___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[26], 8);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x401___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[27], 818);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x201___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[28], 128);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x101___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[29], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x81___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[30], 45);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x41___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[31], 4294967187);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x21___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[32], 4294967251);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x11___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[33], 4294965930);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x9___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[34], 820);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[35], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[36], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x2___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[37], 4294966270);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[38], 1717986405);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2000000___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[39], 33554432);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x10000___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[40], 65536);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x8___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[41], 8);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[42], 1431655750);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x80000___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[43], 524288);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x1___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[44], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[45], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x80000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[46], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x40000000___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[47], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x20000000___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[48], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x10000000___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[49], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x8000000___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[50], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4000000___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[51], 67108864);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x1000000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[52], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x800000___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[53], 8388608);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x400000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[54], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x200000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[55], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x100000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[56], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x40000___immval__0x7) {
  ASSERT_EQ(getScalarWordData()[57], 262144);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x20000___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[58], 131072);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x8000___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[59], 32768);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4000___immval__0x7) {
  ASSERT_EQ(getScalarWordData()[60], 16384);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2000___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[61], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x1000___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[62], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x800___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[63], 2048);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x400___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[64], 1024);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x200___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[65], 512);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x100___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[66], 256);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x80___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[67], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x40___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[68], 64);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x20___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[69], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x10___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[70], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[71], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[72], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[73], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[74], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[75], 46340);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[76], 1029);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[77], 260);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[78], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[79], 45057);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[80], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[81], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[82], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[83], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[84], 1029);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[85], 256);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[86], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[87], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[88], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[89], 46337);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[90], 1028);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[91], 257);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[92], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[93], 45056);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[94], 1285);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[95], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[96], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[97], 4294920916);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[98], 613);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[99], 564);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[100], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[101], 4294920873);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[102], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[103], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[104], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[105], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[106], 613);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[107], 560);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[108], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[109], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[110], 45);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[111], 4294920913);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[112], 612);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[113], 561);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[114], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[115], 4294920872);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[116], 85);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[117], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[118], 38);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[119], 1717986884);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[120], 1639);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[121], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[122], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[123], 1717985827);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[124], 1094);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[125], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[126], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[127], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[128], 1637);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[129], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[130], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[131], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[132], 37);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[133], 1717986883);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[134], 1638);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[135], 547);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[136], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[137], 1717985826);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[138], 1093);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[139], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[140], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[141], 858993428);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[142], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[143], 820);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[144], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[145], 858993184);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[146], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[147], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[148], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[149], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[150], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[151], 816);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[152], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[153], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[154], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[155], 858993424);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[156], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[157], 816);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[158], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[159], 858993184);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[160], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[161], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[162], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[163], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[164], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[165], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[166], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[167], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[168], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[169], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[170], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[171], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[172], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[173], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[174], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[175], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[176], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[177], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[178], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[179], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[180], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[181], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[182], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[183], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[184], 42);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[185], 2863311488);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[186], 547);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[187], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[188], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[189], 2863311531);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[190], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[191], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[192], 40);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[193], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[194], 545);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[195], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[196], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[197], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[198], 41);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[199], 2863311491);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[200], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[201], 547);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[202], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[203], 2863311530);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[204], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[205], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[206], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[207], 1431655764);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[208], 1094);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[209], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[210], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[211], 1431654402);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[212], 1366);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[213], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[214], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[215], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[216], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[217], 274);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[218], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[219], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[220], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[221], 1431655762);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[222], 1094);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[223], 274);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[224], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[225], 1431654402);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[226], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[227], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[228], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[229], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[230], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[231], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[232], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[233], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[234], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[235], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[236], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[237], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[238], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[239], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[240], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[241], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[242], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[243], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[244], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[245], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[246], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[247], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[248], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[249], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[250], 46336);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[251], 1027);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[252], 256);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[253], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[254], 45059);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[255], 1282);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[256], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[257], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[258], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[259], 1025);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[260], 258);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[261], 1280);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[262], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[263], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[264], 46339);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[265], 1026);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[266], 259);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[267], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[268], 45058);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[269], 1281);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[270], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[271], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[272], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[273], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[274], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[275], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[276], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[277], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[278], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[279], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[280], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[281], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[282], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[283], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[284], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[285], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[286], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[287], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[288], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[289], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[290], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[291], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[292], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[293], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[294], 1717986884);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[295], 1637);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[296], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[297], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[298], 1717985825);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[299], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[300], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[301], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[302], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[303], 1637);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[304], 2863311530);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[305], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[306], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[307], 40);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[308], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[309], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[310], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[311], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[312], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[313], 40);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[314], 2863311490);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[315], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[316], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[317], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[318], 2863311530);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[319], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[320], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[321], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[322], 1431655764);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[323], 1093);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[324], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[325], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[326], 1431654401);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[327], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[328], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[329], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[330], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[331], 1093);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[332], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[333], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[334], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[335], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[336], 1431655761);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[337], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[338], 273);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[339], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[340], 1431654400);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[341], 1365);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[342], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[343], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[344], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[345], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[346], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[347], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[348], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[349], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[350], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[351], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[352], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[353], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[354], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[355], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[356], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[357], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[358], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[359], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[360], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[361], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[362], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[363], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[364], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[365], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[366], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[367], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[368], 37);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[369], 1717986881);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[370], 1636);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[371], 545);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[372], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[373], 1717985824);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[374], 1093);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[375], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[376], 34);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[377], 858993424);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[378], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[379], 816);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[380], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[381], 858993186);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[382], 274);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[383], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[384], 32);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[385], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[386], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[387], 818);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[388], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[389], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[390], 32);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[391], 858993426);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[392], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[393], 818);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[394], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[395], 858993186);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[396], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[397], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[398], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[399], 1431655764);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[400], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[401], 276);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[402], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[403], 1431654400);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[404], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[405], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[406], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[407], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[408], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[409], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[410], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[411], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[412], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[413], 1431655760);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[414], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[415], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[416], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[417], 1431654400);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[418], 1364);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[419], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[420], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[421], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[422], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[423], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[424], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[425], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[426], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[427], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[428], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[429], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[430], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[431], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[432], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[433], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[434], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[435], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[436], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[437], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[438], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[439], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[440], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[441], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[442], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[443], 46340);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[444], 1028);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[445], 260);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[446], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[447], 45056);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[448], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[449], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[450], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[451], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[452], 1028);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[453], 256);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[454], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[455], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[456], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[457], 46336);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[458], 1028);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[459], 256);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[460], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[461], 45056);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[462], 1284);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[463], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[464], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[465], 4294920916);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[466], 612);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[467], 564);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[468], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[469], 4294920872);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[470], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[471], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[472], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[473], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[474], 612);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[475], 560);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[476], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[477], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[478], 44);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[479], 4294920912);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[480], 612);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[481], 560);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[482], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[483], 4294920872);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[484], 84);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[485], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[486], 38);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[487], 1717986884);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[488], 1638);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[489], 548);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[490], 6);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[491], 1717985826);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[492], 1094);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[493], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[494], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[495], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[496], 1636);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[497], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[498], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[499], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[500], 36);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[501], 1717986882);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[502], 1638);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[503], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[504], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[505], 1717985826);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[506], 1092);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[507], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[508], 34);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[509], 858993424);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[510], 547);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[511], 816);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[512], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[513], 858993187);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[514], 274);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[515], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[516], 32);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[517], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[518], 545);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[519], 818);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[520], 272);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[521], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[522], 33);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[523], 858993427);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[524], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[525], 819);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[526], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[527], 858993186);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[528], 273);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[529], 3);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[530], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[531], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[532], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[533], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[534], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[535], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[536], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[537], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[538], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[539], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[540], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[541], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[542], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[543], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[544], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[545], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[546], 4);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[547], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[548], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[549], 0);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[550], 5);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[551], 1);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[552], 42);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[553], 2863311488);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[554], 546);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[555], 544);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[556], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x1000001___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[557], 2);
}


TEST_F(andi_01, opcode__andi___op1_x10__dest_x11__op1val__0x20001___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[558], 1365);
}

