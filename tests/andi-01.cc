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
class andi_01Fixture : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = andi_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(andi_01::generateCode().getCode(), scalarInitData);
        
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

std::vector<uint8_t> andi_01Fixture::s_ScalarData;
}


// Generate google tests

TEST_F(andi_01Fixture, opcode__andi___op1_x22__dest_x10__op1val__0x2___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[4], 4294965248);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x25__dest_x25__op1val__0x1001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x16__dest_x17__op1val__0x2000001___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[6], 2047);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x2__dest_x8__op1val__0x20001___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[7], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x28__dest_x30__op1val__0x80000000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[8], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x4__dest_x19__op1val_0x0___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x2__op1val_0x7fffffff___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[10], 6);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x7__dest_x13__op1val_0x1___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[11], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x27__dest_x9__op1val_0x80___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[12], 128);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x17__dest_x3__op1val_0x7fffffff___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[13], 2147483604);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x0__dest_x26__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[14], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x23__dest_x21__op1val_0x66666666___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x6__dest_x14__op1val_0x0___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[16], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x5__dest_x22__op1val_0x100___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[17], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x8__dest_x29__op1val__0x5___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[18], 64);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x12__dest_x23__op1val_0x1___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[19], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x15__dest_x6__op1val__0x55555555___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[20], 512);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x29__dest_x11__op1val_0x0___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[21], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x20__dest_x1__op1val_0x66666667___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[22], 1717986918);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x31__dest_x5__op1val__0x100001___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[23], 4293918717);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x3__dest_x20__op1val_0x200___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[24], 512);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x9__dest_x7__op1val_0x4000___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[25], 16384);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x13__dest_x16__op1val_0x200000___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[26], 2097152);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x18__dest_x24__op1val__0x3___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[27], 4294967261);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x26__dest_x4__op1val__0x6___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[28], 4294967226);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x30__dest_x28__op1val_0x40___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[29], 64);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x19__dest_x0__op1val__0xb504___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[30], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x21__dest_x18__op1val__0x8001___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[31], 4294934015);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x1__dest_x12__op1val_0x40___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[32], 64);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x24__dest_x27__op1val__0x4001___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[33], 1365);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x11__dest_x31__op1val_0x40000000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[34], 1073741824);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x14__dest_x15__op1val_0x2___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[35], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[36], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x8___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[37], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x10___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[38], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x20___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[39], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x400___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[40], 1024);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x800___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[41], 2048);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x1000___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[42], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2000___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[43], 8192);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x8000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[44], 32768);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x10000___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[45], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x20000___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[46], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x40000___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[47], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x80000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[48], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x100000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[49], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x400000___immval__0x1) {
  ASSERT_EQ(getScalarWordData()[50], 4194304);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x800000___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[51], 8388608);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2000000___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[52], 33554432);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4000000___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[53], 67108864);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x8000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[54], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x10000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[55], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x20000000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[56], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x9___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[57], 4294967285);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x11___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[58], 1350);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x21___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[59], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x41___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[60], 9);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x81___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[61], 4294967165);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x101___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[62], 564);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x201___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[63], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x401___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[64], 64);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x801___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[65], 9);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x2001___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[66], 1639);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x10001___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[67], 2047);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x40001___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[68], 4294705107);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x80001___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[69], 512);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x200001___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[70], 4292870100);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x400001___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[71], 5);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x800001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[72], 7);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x1000001___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[73], 6);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x4000001___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[74], 8);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x8000001___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[75], 46);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x10000001___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[76], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x20000001___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[77], 3758096340);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x40000001___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[78], 1366);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[79], 1431655761);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[80], 128);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[81], 3);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[82], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[83], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[84], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[85], 3);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[86], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[87], 3);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[88], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[89], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[90], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[91], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[92], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[93], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[94], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[95], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[96], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[97], 3);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[98], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[99], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[100], 3);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[101], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[102], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[103], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[104], 1365);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[105], 1431654400);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[106], 5);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[107], 273);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[108], 1092);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[109], 1431655761);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[110], 5);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[111], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[112], 1364);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[113], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[114], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[115], 272);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[116], 1093);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[117], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[118], 1364);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[119], 1431654401);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[120], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[121], 276);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[122], 1093);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[123], 1431655764);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[124], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[125], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[126], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[127], 2863311530);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[128], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[129], 546);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[130], 546);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[131], 2863311490);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[132], 40);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[133], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[134], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[135], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[136], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[137], 546);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[138], 544);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[139], 40);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[140], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[141], 2863311530);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[142], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[143], 544);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[144], 546);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[145], 2863311488);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[146], 42);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[147], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[148], 5);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[149], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[150], 5);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[151], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[152], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[153], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[154], 5);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[155], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[156], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[157], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[158], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[159], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[160], 5);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[161], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[162], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[163], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[164], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[165], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[166], 5);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[167], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[168], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[169], 3);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[170], 273);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[171], 858993186);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[172], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[173], 819);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[174], 546);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[175], 858993427);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[176], 33);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[177], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[178], 272);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[179], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[180], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[181], 818);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[182], 545);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[183], 32);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[184], 274);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[185], 858993187);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[186], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[187], 816);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[188], 547);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[189], 858993424);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[190], 34);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[191], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[192], 1092);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[193], 1717985826);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[194], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[195], 546);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[196], 1638);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[197], 1717986882);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[198], 36);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[199], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[200], 1092);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[201], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[202], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[203], 546);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[204], 1636);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[205], 36);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[206], 1094);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[207], 1717985826);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[208], 6);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[209], 548);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[210], 1638);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[211], 1717986884);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[212], 38);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[213], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[214], 84);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[215], 4294920872);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[216], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[217], 560);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[218], 612);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[219], 4294920912);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[220], 44);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[221], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[222], 84);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[223], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[224], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[225], 560);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[226], 612);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[227], 44);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[228], 84);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[229], 4294920872);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[230], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[231], 564);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[232], 612);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[233], 4294920916);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[234], 44);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[235], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[236], 1284);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[237], 45056);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[238], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[239], 256);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[240], 1028);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[241], 46336);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[242], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[243], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[244], 1284);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[245], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[246], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[247], 256);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[248], 1028);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[249], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[250], 1284);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[251], 45056);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[252], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[253], 260);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[254], 1028);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[255], 46340);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[256], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[257], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[258], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[259], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[260], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[261], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[262], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[263], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[264], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[265], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[266], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[267], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[268], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[269], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[270], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[271], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[272], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[273], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[274], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[275], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[276], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[277], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[278], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[279], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[280], 1364);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[281], 1431654400);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[282], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[283], 272);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[284], 1092);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[285], 1431655760);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[286], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[287], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[288], 1364);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[289], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[290], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[291], 272);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[292], 1092);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[293], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[294], 1364);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[295], 1431654400);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[296], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[297], 276);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[298], 1092);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[299], 1431655764);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[300], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[301], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[302], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[303], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[304], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[305], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[306], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[307], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[308], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[309], 816);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[310], 548);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[311], 36);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[312], 276);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[313], 858993184);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[314], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[315], 820);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[316], 548);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[317], 858993428);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[318], 36);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[319], 3);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[320], 1093);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[321], 1717985826);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[322], 5);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[323], 547);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[324], 1638);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[325], 1717986883);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[326], 37);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[327], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[328], 1092);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[329], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[330], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[331], 546);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[332], 1637);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[333], 36);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[334], 1094);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[335], 1717985827);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[336], 6);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[337], 548);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[338], 1639);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[339], 1717986884);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[340], 38);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[341], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[342], 85);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[343], 4294920872);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[344], 5);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[345], 561);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[346], 612);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[347], 4294920913);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[348], 45);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[349], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[350], 84);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[351], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[352], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[353], 560);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[354], 613);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[355], 44);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[356], 84);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[357], 4294920873);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[358], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[359], 564);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[360], 613);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[361], 4294920916);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[362], 44);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[363], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[364], 1285);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[365], 45056);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[366], 5);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[367], 257);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[368], 1028);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[369], 46337);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[370], 5);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[371], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[372], 1284);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[373], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[374], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[375], 256);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[376], 1029);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[377], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[378], 1284);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[379], 45057);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[380], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[381], 260);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[382], 1029);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[383], 46340);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[384], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[385], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[386], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[387], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[388], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[389], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[390], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[391], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[392], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[393], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[394], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[395], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[396], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[397], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[398], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[399], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[400], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[401], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[402], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[403], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[404], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[405], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[406], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[407], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[408], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[409], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[410], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[411], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[412], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[413], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[414], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[415], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[416], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[417], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[418], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[419], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[420], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[421], 272);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[422], 858993186);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[423], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[424], 818);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[425], 546);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[426], 858993426);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[427], 32);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[428], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[429], 272);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[430], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[431], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[432], 818);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[433], 544);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[434], 32);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[435], 274);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[436], 858993186);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[437], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[438], 816);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[439], 546);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[440], 858993424);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[441], 34);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[442], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[443], 1093);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[444], 1717985824);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[445], 5);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[446], 545);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[447], 1636);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[448], 1717986881);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[449], 37);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[450], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[451], 1092);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[452], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[453], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[454], 544);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[455], 1637);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[456], 36);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[457], 1092);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[458], 1717985825);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[459], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[460], 548);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[461], 1637);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[462], 1717986884);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[463], 36);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[464], 3);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[465], 1281);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[466], 45058);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[467], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[468], 259);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[469], 1026);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[470], 46339);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[471], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[472], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[473], 1280);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[474], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[475], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[476], 258);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[477], 1025);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[478], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[479], 1282);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[480], 45059);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[481], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[482], 256);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[483], 1027);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[484], 46336);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[485], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[486], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[487], 1364);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[488], 1431654402);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[489], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[490], 274);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[491], 1094);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[492], 1431655762);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[493], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[494], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[495], 1364);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[496], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[497], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[498], 274);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[499], 1092);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[500], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[501], 1366);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[502], 1431654402);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[503], 6);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[504], 276);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[505], 1094);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[506], 1431655764);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[507], 6);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[508], 3);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[509], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[510], 2863311530);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[511], 1);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[512], 547);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[513], 546);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[514], 2863311491);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[515], 41);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[516], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[517], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[518], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[519], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[520], 546);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[521], 545);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[522], 40);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[523], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[524], 2863311531);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[525], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[526], 544);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[527], 547);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[528], 2863311488);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[529], 42);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[530], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[531], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[532], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[533], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[534], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[535], 6);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[536], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[537], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[538], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[539], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[540], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[541], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[542], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[543], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[544], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[545], 6);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[546], 2);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[547], 6);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[548], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[549], 6);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[550], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[551], 6);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[552], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[553], 276);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[554], 858993184);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[555], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[556], 816);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[557], 548);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[558], 858993424);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[559], 36);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[560], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[561], 276);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[562], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[563], 4);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val_0x1000000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[564], 0);
}


TEST_F(andi_01Fixture, opcode__andi___op1_x10__dest_x11__op1val__0xb504___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[565], 4294920956);
}

