// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/ori-01.h"

namespace
{
class ori_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::ori_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::ori_01::generateCode().getCode(), scalarInitData);
        
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

std::vector<uint8_t> ori_01::s_ScalarData;
}


// Generate google tests

TEST_F(ori_01, opcode__ori___op1_x5__dest_x22__op1val__0x201___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[4], 4294966783);
}


TEST_F(ori_01, opcode__ori___op1_x27__dest_x27__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(ori_01, opcode__ori___op1_x17__dest_x8__op1val_0x33333334___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[6], 858994687);
}


TEST_F(ori_01, opcode__ori___op1_x20__dest_x1__op1val__0xb504___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[7], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x12__dest_x19__op1val__0x80000000___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[8], 2147483693);
}


TEST_F(ori_01, opcode__ori___op1_x8__dest_x3__op1val_0x7fffffff___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[9], 2147483647);
}


TEST_F(ori_01, opcode__ori___op1_x28__dest_x26__op1val_0x1___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[10], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x16__dest_x23__op1val_0x7___immval__0x7) {
  ASSERT_EQ(getScalarWordData()[11], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x25__dest_x31__op1val_0x40000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[12], 262146);
}


TEST_F(ori_01, opcode__ori___op1_x23__dest_x11__op1val_0x20000000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[13], 536870916);
}


TEST_F(ori_01, opcode__ori___op1_x14__dest_x17__op1val__0x201___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[14], 4294966783);
}


TEST_F(ori_01, opcode__ori___op1_x31__dest_x7__op1val_0x2___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[15], 18);
}


TEST_F(ori_01, opcode__ori___op1_x21__dest_x4__op1val_0x8000___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[16], 32800);
}


TEST_F(ori_01, opcode__ori___op1_x15__dest_x5__op1val_0x800___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[17], 2112);
}


TEST_F(ori_01, opcode__ori___op1_x30__dest_x25__op1val__0x40001___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[18], 4294705151);
}


TEST_F(ori_01, opcode__ori___op1_x11__dest_x30__op1val__0x5___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[19], 4294967291);
}


TEST_F(ori_01, opcode__ori___op1_x4__dest_x10__op1val__0x80001___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[20], 4294443007);
}


TEST_F(ori_01, opcode__ori___op1_x13__dest_x0__op1val__0x40000001___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[21], 0);
}


TEST_F(ori_01, opcode__ori___op1_x26__dest_x6__op1val__0x21___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[22], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x19__dest_x18__op1val_0xb503___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[23], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x7__dest_x21__op1val_0x10000000___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[24], 4294967291);
}


TEST_F(ori_01, opcode__ori___op1_x24__dest_x16__op1val__0x80001___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[25], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x0__dest_x9__op1val_0x0___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[26], 4294967279);
}


TEST_F(ori_01, opcode__ori___op1_x3__dest_x28__op1val_0x2000___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[27], 4294967263);
}


TEST_F(ori_01, opcode__ori___op1_x1__dest_x15__op1val_0x55555556___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[28], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x18__dest_x2__op1val__0xb504___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[29], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x22__dest_x20__op1val__0x55555556___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[30], 4294967039);
}


TEST_F(ori_01, opcode__ori___op1_x6__dest_x13__op1val_0x1000000___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[31], 4294966783);
}


TEST_F(ori_01, opcode__ori___op1_x29__dest_x14__op1val_0x20000000___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[32], 4294966271);
}


TEST_F(ori_01, opcode__ori___op1_x2__dest_x12__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[33], 4294966014);
}


TEST_F(ori_01, opcode__ori___op1_x9__dest_x24__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[34], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x29__op1val_0x8___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[35], 14);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x10___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[36], 528);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x20___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[37], 44);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x40___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[38], 67);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x80___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[39], 160);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x100___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[40], 4294967231);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x200___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[41], 4294967290);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x400___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[42], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x1000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[43], 4101);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4000___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[44], 18021);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x10000___immval_0x9) {
  ASSERT_EQ(getScalarWordData()[45], 65545);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x20000___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[46], 131584);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x80000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[47], 524288);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x100000___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[48], 1049088);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x200000___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[49], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x400000___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[50], 4294966783);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x800000___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[51], 8388736);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2000000___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[52], 4294967263);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4000000___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[53], 67108880);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x8000000___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[54], 134219092);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x40000000___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[55], 4294967290);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x2___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[56], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x3___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[57], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x9___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[58], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x11___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[59], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x41___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[60], 4294967231);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x81___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[61], 4294967167);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x101___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[62], 4294967039);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x401___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[63], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x801___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[64], 4294965247);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x1001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[65], 4294963199);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x2001___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[66], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x8001___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[67], 4294934527);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x10001___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[68], 4294901759);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x20001___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[69], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x100001___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[70], 4293918719);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x200001___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[71], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x400001___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[72], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x800001___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[73], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x1000001___immval_0x3ff) {
  ASSERT_EQ(getScalarWordData()[74], 4278190079);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x2000001___immval__0x8) {
  ASSERT_EQ(getScalarWordData()[75], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x4000001___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[76], 4227858431);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x8000001___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[77], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x10000001___immval__0x7) {
  ASSERT_EQ(getScalarWordData()[78], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x20000001___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[79], 3758096383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[80], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[81], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[82], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[83], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[84], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[85], 819);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[86], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[87], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[88], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[89], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[90], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[91], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[92], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[93], 819);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[94], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[95], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[96], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[97], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[98], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[99], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[100], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[101], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[102], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[103], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[104], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[105], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[106], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[107], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[108], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[109], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[110], 1431655805);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[111], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[112], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[113], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[114], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[115], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[116], 1431656309);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[117], 1431655805);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[118], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[119], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[120], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[121], 1431656309);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[122], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[123], 4294967253);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[124], 1431655807);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[125], 2863311531);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[126], 2863312895);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[127], 4294965930);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[128], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[129], 2863311803);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[130], 2863312622);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[131], 4294967291);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[132], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[133], 2863311530);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[134], 2863312894);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[135], 2863311530);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[136], 2863311534);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[137], 2863311802);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[138], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[139], 2863311534);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[140], 2863312894);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[141], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[142], 2863311534);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[143], 2863311806);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[144], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[145], 4294967294);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[146], 2863311534);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[147], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[148], 1365);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[149], 4294965935);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[150], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[151], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[152], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[153], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[154], 45);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[155], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[156], 1365);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[157], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[158], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[159], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[160], 1637);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[161], 45);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[162], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[163], 4294965935);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[164], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[165], 821);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[166], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[167], 4294967253);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[168], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[169], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[170], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[171], 4294966203);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[172], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[173], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[174], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[175], 4294967283);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[176], 858993471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[177], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[178], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[179], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[180], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[181], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[182], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[183], 858993471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[184], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[185], 4294966203);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[186], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[187], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[188], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[189], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[190], 858993471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[191], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[192], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[193], 4294967022);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[194], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[195], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[196], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[197], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[198], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[199], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[200], 1717987190);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[201], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[202], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[203], 1717987190);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[204], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[205], 1717986926);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[206], 1717987190);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[207], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[208], 1717986918);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[209], 1717987190);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[210], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[211], 4294967286);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[212], 1717986926);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[213], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[214], 4294922237);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[215], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[216], 4294921215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[217], 4294921982);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[218], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[219], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[220], 4294920958);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[221], 4294922236);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[222], 4294920956);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[223], 4294920956);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[224], 4294921214);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[225], 4294921981);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[226], 4294920956);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[227], 4294922238);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[228], 4294966015);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[229], 4294920958);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[230], 4294921212);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[231], 4294921983);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[232], 4294967292);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[233], 4294920958);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[234], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[235], 46421);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[236], 4294967214);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[237], 46341);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[238], 46903);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[239], 46950);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[240], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[241], 46381);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[242], 46342);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[243], 46420);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[244], 46340);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[245], 46340);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[246], 46902);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[247], 46949);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[248], 46380);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[249], 46422);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[250], 4294967215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[251], 46342);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[252], 46900);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[253], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[254], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[255], 46382);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[256], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[257], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[258], 4294965930);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[259], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[260], 819);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[261], 1638);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[262], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[263], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[264], 2);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[265], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[266], 2);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[267], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[268], 818);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[269], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[270], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[271], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[272], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[273], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[274], 822);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[275], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[276], 4294967254);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[277], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[278], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[279], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[280], 4294967294);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[281], 1431655765);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[282], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[283], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[284], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[285], 1431655805);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[286], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[287], 1431655764);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[288], 1431655764);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[289], 1431655764);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[290], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[291], 1431656309);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[292], 1431655804);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[293], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[294], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[295], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[296], 1431656308);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[297], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[298], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[299], 1431655806);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[300], 3);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[301], 1365);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[302], 4294965930);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[303], 5);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[304], 819);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[305], 1638);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[306], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[307], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[308], 858994549);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[309], 858993468);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[310], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[311], 4294966207);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[312], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[313], 858993460);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[314], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[315], 4294967284);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[316], 858993470);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[317], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[318], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[319], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[320], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[321], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[322], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[323], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[324], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[325], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[326], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[327], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[328], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[329], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[330], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[331], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[332], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[333], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[334], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[335], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[336], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[337], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[338], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[339], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[340], 4294922237);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[341], 4294966015);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[342], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[343], 4294921215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[344], 4294921983);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[345], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[346], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[347], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[348], 4294922237);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[349], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[350], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[351], 4294921215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[352], 4294921981);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[353], 4294920957);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[354], 4294922239);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[355], 4294966015);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[356], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[357], 4294921213);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[358], 4294921983);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[359], 4294967293);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[360], 4294920959);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[361], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[362], 46421);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[363], 4294967215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[364], 46341);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[365], 46903);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[366], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[367], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[368], 46381);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[369], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[370], 46421);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[371], 46341);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[372], 46341);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[373], 46903);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[374], 46949);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[375], 46381);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[376], 46423);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[377], 4294967215);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[378], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[379], 46901);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[380], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[381], 4294967253);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[382], 46383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[383], 45);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[384], 2);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[385], 1364);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[386], 4);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[387], 818);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[388], 1637);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[389], 44);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[390], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[391], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[392], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[393], 820);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[394], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[395], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[396], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[397], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[398], 1365);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[399], 4294965934);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[400], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[401], 1638);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[402], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[403], 45);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[404], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[405], 1364);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[406], 4);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[407], 4);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[408], 822);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[409], 1637);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[410], 44);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[411], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[412], 4294965935);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[413], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[414], 820);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[415], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[416], 4294967252);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[417], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[418], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[419], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[420], 4294966202);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[421], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[422], 858993459);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[423], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[424], 4294967283);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[425], 858993471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[426], 858993458);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[427], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[428], 858993458);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[429], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[430], 858993458);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[431], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[432], 858993470);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[433], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[434], 4294966203);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[435], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[436], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[437], 858994551);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[438], 4294967286);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[439], 858993470);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[440], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[441], 1717987189);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[442], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[443], 1717986917);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[444], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[445], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[446], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[447], 1717986925);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[448], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[449], 1717987189);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[450], 1717986917);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[451], 1717986917);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[452], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[453], 1717986917);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[454], 1717986925);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[455], 1717987191);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[456], 4294967023);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[457], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[458], 1717987189);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[459], 1717986919);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[460], 4294967285);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[461], 1717986927);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[462], 46339);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[463], 46423);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[464], 4294967211);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[465], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[466], 46899);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[467], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[468], 4294967251);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[469], 46383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[470], 46339);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[471], 46423);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[472], 46339);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[473], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[474], 46899);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[475], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[476], 46383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[477], 46423);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[478], 4294967211);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[479], 46343);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[480], 46903);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[481], 46951);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[482], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[483], 46383);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[484], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[485], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[486], 4294967294);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[487], 1431655767);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[488], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[489], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[490], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[491], 1431655807);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[492], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[493], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[494], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[495], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[496], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[497], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[498], 1431655806);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[499], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[500], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[501], 1431655766);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[502], 1431656310);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[503], 1431656311);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[504], 4294967254);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[505], 1431655806);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[506], 2863311531);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[507], 2863312895);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[508], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[509], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[510], 2863311803);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[511], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[512], 4294967291);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[513], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[514], 2863311531);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[515], 2863312895);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[516], 2863311531);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[517], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[518], 2863311803);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[519], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[520], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[521], 2863312895);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[522], 4294965931);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[523], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[524], 2863311807);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[525], 2863312623);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[526], 4294967295);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[527], 2863311535);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[528], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[529], 1367);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[530], 4294965934);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[531], 7);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[532], 823);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[533], 1638);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[534], 4294967255);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[535], 47);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[536], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[537], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[538], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[539], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[540], 822);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[541], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[542], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[543], 1366);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[544], 4294965935);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[545], 6);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[546], 822);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[547], 1639);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[548], 4294967254);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[549], 46);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[550], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[551], 858994549);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[552], 4294966206);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[553], 858993461);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[554], 858993463);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[555], 858994550);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[556], 4294967287);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[557], 858993469);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[558], 858993462);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[559], 858994548);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[560], 858993460);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[561], 858993460);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x40000001___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[562], 3221225471);
}


TEST_F(ori_01, opcode__ori___op1_x10__dest_x11__op1val__0x4001___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[563], 4294967295);
}

